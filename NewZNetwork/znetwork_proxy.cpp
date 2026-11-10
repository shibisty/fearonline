// ============================================================================
//  ZNetwork.dll proxy/stub  --  SAFE VERSION
//  Patching only known offsets of global variables in GameClient.dll.
//  Blind byte-scan across the entire module memory removed -- too high risk
//  of accidentally overwriting unrelated data/code that matches by 4 bytes.
// ============================================================================

#include <windows.h>
#include <psapi.h>
#include <cstdio>
#include <share.h>
#include <cstdarg>
#include <cstring>
#include <cwchar>
#include <cstdint>
#include <map>
#include <string>
#include <exception>
#include <intrin.h>

#pragma comment(lib, "psapi.lib")

// The crash dumper intentionally probes arbitrary addresses under SEH. Keep
// those probes outside ASan instrumentation so ASan doesn't abort before the
// hardware access violation can be contained by __try/__except.
#if defined(NEWZ_ASAN_BUILD)
#define ASAN_NO_INSTRUMENT __declspec(no_sanitize_address)
#else
#define ASAN_NO_INSTRUMENT
#endif

// ============ Logging ============
// Path to the log file -- next to the project, for easy copying of contents
// without needing to scroll through DebugView. Overwritten (not appended)
// each time the game starts anew.
static const char* LOG_FILE_PATH = "C:\\fearonline\\znetwork_proxy_log.txt";
static FILE* g_LogFile = nullptr;
static bool g_LogFileInitTried = false;
static volatile LONG g_LogLinesSinceFlush = 0;
// Updated at dispatcher entry so an exception that happens after a command's
// own log entry was throttled still carries the last command in the crash log.
static char g_LastDispatchName[65] = "<none>";
static uint32_t g_LastDispatchParams[3] = { 0, 0, 0 };
static volatile LONG g_CallbackTraceSequence = 0;
static volatile LONG g_LastCallbackSequence = 0;
static volatile LONG g_LastCallbackReturnedSequence = 0;
static const char* volatile g_LastCallbackName = "<none>";
static void* volatile g_LastCallbackTarget = nullptr;
static const void* volatile g_LastCallbackData = nullptr;
// GAME_LOAD_COMPLETE answer to Send_GAME_START_PRE_COMPLETE, delivered from Tick()
// (see Send_GAME_START_PRE_COMPLETE below).
static volatile LONG g_GameLoadCompletePending = 0;
static volatile LONG g_GameLoadCompleteSent = 0;
static volatile DWORD g_LastTickMs = 0;   // Tick() heartbeat -- is the engine still ticking us?
static volatile LONG g_BeginRoundPending = 0;   // Send_HOST_GAME_BEGIN_ROUND -> HOST_GAME_BEGIN_ROUND answer
static volatile LONG g_BeginRoundCount = 0;

static void EnsureLogFileOpen() {
    if (g_LogFileInitTried) return;
    g_LogFileInitTried = true;
    // 2026-11-03: _fsopen with _SH_DENYWR instead of fopen_s -- fopen_s opens
    // "w" files in exclusive mode (_SH_DENYRW), which made the log
    // unreadable from outside while the game was sitting on the loading
    // screen ("Permission denied" even for a copy). Deny only writers.
    g_LogFile = _fsopen(LOG_FILE_PATH, "w", _SH_DENYWR);  // "w" = overwrite from scratch
    // Ordinary writes are buffered; Log() flushes in batches and crash
    // diagnostics force a flush when immediate persistence matters.
}

static void Log(const char* fmt, ...) {
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
    va_end(ap);
    // Keep diagnostics in the file only. OutputDebugStringA generates the
    // DBG_PRINTEXCEPTION_C path on this host; Windows Error Reporting recorded
    // that code as Engine.exe's fatal exception during the latest runs, which
    // obscures the real failure and may interact badly with our first-chance
    // exception handler. Define NEWZ_OUTPUT_DEBUG_STRING explicitly only when
    // a debugger trace is needed.
#if defined(NEWZ_OUTPUT_DEBUG_STRING)
    OutputDebugStringA(buf);
    OutputDebugStringA("\n");
#endif

    EnsureLogFileOpen();
    if (g_LogFile) {
        fputs(buf, g_LogFile);
        fputc('\n', g_LogFile);
        // 2026-11-03: flush EVERY line again. The 64-line batching (added in
        // October to avoid synchronous I/O on hot UI paths) cost us the
        // whole crash context in the first run of this build: the process
        // died hard and the last <64 lines -- including whatever the VEH
        // managed to write -- never reached the disk. A crash log that
        // stops before the crash is worth nothing; the I/O cost is
        // irrelevant for a diagnostic build.
        InterlockedExchange(&g_LogLinesSinceFlush, 0);
        fflush(g_LogFile);
    }
}

// Safe reading of one byte via __try/__except -- used both in the dispatcher hook
// (command name check) and in crash diagnostics. More reliable than obsolete
// IsBadReadPtr/IsBadStringPtr* (see comment later in the file).
ASAN_NO_INSTRUMENT static bool SafeReadByte(const uint8_t* p, uint8_t* out) {
    __try {
        *out = *p;
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

// Forward declarations -- real definitions are with the rest of the crash
// diagnostics helpers further down (they reuse SafeReadByte the same way),
// but HookedDispatcher (below, defined earlier in the file) needs them too,
// to log the command name string on every dispatch, not just on crash.
static void TryLogString(const char* label, const void* ptr);
// Same safe byte-by-byte read as TryLogString, but returns the string into a
// caller buffer instead of logging it directly -- used by HookedDispatcher to
// dedupe repeated command names (see there) before deciding whether to log.
static bool TryGetString(const void* ptr, char* outBuf, size_t outBufSize);
// The local-map fallback is defined with the room/game exports below. The
// UI command dispatcher is the only reliable place to catch RoomGameStart if
// the game's network wrapper does not reach Send_ROOM_GAME_START itself.
static void EnsureLocalMapStart(const char* source);
// Engine console-print capture (defined with the game-start code below);
// Tick() retries it until g_pLTClient exists.
static void TryInstallLTClientCPrintHook();
static void DeliverPendingGameLoadComplete();
static void DeliverPendingBeginRound();

// ============ Structures ============
#pragma pack(push, 1)

struct ServerData {
    uint32_t connectionStatus;
    uint32_t ping;
    uint32_t serverId;
    char     serverIp[64];
    uint16_t serverPort;
    uint32_t sessionKey;
    void*    pNetworkManager;
    void*    pUserSession;
};

struct ServerHostData {
    uint32_t hostMode;
    uint32_t hostPlayerId;
    char     hostIp[64];
    uint16_t hostPort;
    uint8_t  isRoomOwner;
};

struct LoginData {
    uint32_t resultCode;
    uint32_t userId;
    uint32_t sessionId;
    uint32_t serverTime;
    char     userName[64];
    uint32_t serverId;
    uint32_t channelId;
};

struct UserInfo {
    uint32_t userId;
    uint32_t level;
    uint32_t exp;
    uint32_t nextLevelExp;
    char     nickName[64];
    uint32_t clanId;
    uint32_t clanRank;
    uint32_t money;
    uint32_t cash;
};

struct LoginFailData {
    uint32_t errorCode;
    char     errorMsg[128];
};

struct GateServerInfo {
    uint32_t serverId;
    uint32_t serverType;
    char     serverName[64];
    char     serverIp[32];
    uint16_t serverPort;
    uint32_t playerCount;
    uint32_t maxPlayers;
};

struct GateChannelInfo {
    uint32_t channelId;
    char     channelName[64];
    uint32_t playerCount;
    uint32_t maxPlayers;
    uint32_t status;
};

struct GateLobbyInfo {
    uint32_t lobbyId;
    char     lobbyName[64];
    uint32_t playerCount;
    uint32_t maxPlayers;
};

struct LobbyRoomInfo {
    uint32_t roomId;
    char     roomName[64];
    uint32_t playerCount;
    uint32_t maxPlayers;
    uint32_t mode;
    uint32_t mapId;
    uint8_t  isPrivate;
    char     password[16];
};

// The client registers a callback for this (Set_On_LOBBY_ALL_MODE_MAP_Callback
// fires at login), but nothing in this proxy ever called it -- we only found
// out it mattered after "HOST GAME" kept saying "Map does not exist" (36001007)
// even once the local CSV/g_FakeRooms both agreed on mapId 518. The reference
// project's real ProudNet server (lobby.go: openMapList(), RMI 0x8D2B) sends
// this list right after answering the lobby-join request as a varint count
// followed by (uint16 mapIndex, uint8 isOpen) pairs ON THE WIRE.
//
// 2026-09-11: found ZNetwork.dll's OWN decompiled deserializer for this exact
// RMI (0x8d2b, internally named "NotifyOpenMapList", array element type
// `Common::Standard::Room::OpenMap`). Its per-element unpack code
// (FUN_10087c90 in decompiled_znetwork.c) does:
//     local_18 = Common::Standard::Room::OpenMap::vftable;   // offset 0: vtable ptr
//     local_14 = CONCAT13(..., *(undefined3*)(param_3 + 4)); // offset 4: uint16 mapIndex + uint8 isOpen
// i.e. the *native* object ZNetwork.dll builds (and presumably what it hands
// to whatever calls the registered callback) has a 4-byte field BEFORE
// mapIndex/isOpen, not just the two payload fields back-to-back at offset 0.
// That would explain why the first attempt (bare uint16+uint8 at offset 0,
// no leading 4 bytes) had no effect: whoever reads this data would read the
// leading 4 bytes of OUR struct as mapIndex/isOpen, not our real fields.
// This is a strong lead but still a guess about the callback boundary
// specifically -- we only confirmed the shape for ZNetwork's *internal*
// wire-deserialization object, not 100% for what it hands across the
// Set_On_LOBBY_ALL_MODE_MAP_Callback/Set_On_ROOM_OPEN_MAP_INFO_Callback
// boundary specifically (that would need finding GameClient.dll's own
// registered handler function, not yet located). Trying it because it's the
// best-supported theory so far and cheap to test. If "still no change",
// next step is finding that handler function via x32dbg or further
// decompile search, not another blind struct guess.
// 2026-09-11 REVERTED: the {uint32 header=0; uint16 mapIndex; uint8 isOpen}
// version below caused a reproducible crash right after login (log showed
// LOBBY_ALL_MODE_MAP/LOBBY_MODE_MAP/ROOM_EVENT_MAP_INFO all "returned"
// cleanly, then "LOGIN COMPLETE - FAKE SUCCESS", then nothing -- no further
// dispatch lines at all, meaning the crash happened just after login, most
// likely when GameClient.dll's UI code later reads back the stored array
// and dereferences offset 0 as a vtable pointer (matching the theory in the
// diary) -- except we filled it with 0, which is a guaranteed null-deref if
// that's really what happens. That's actually evidence FOR the theory (a
// wrong-but-plausible vtable slot crashes when used; a merely-differently-
// shaped-but-inert struct wouldn't crash this specifically, this cleanly,
// this late). But we don't have a real vtable address to put there, so
// reverting to the known-crash-free layout until that can be found (e.g. by
// locating GameClient.dll's actual registered handler function, not just
// ZNetwork.dll's internal wire deserializer). See diary "продолжение 11"
// and the follow-up entry for this crash.
struct LobbyModeMapInfo {
    uint32_t mapIndex;
    uint32_t isOpen;
};

// Simplified structure for ITEM_INFO
struct ItemInfo {
    uint32_t itemId;
    char     itemName[64];
    uint32_t price;
    uint32_t cashPrice;
    uint32_t category;
    uint32_t type;
    uint32_t level;
    uint32_t quantity;
};

#pragma pack(pop)

// ============ Global Data ============
// GetServerData()/GetServerHostData() return a pointer that the actual
// GameClient.dll code reads/writes far beyond our small structures
// (confirmed by disassembly -- FUN_10108da0 writes up to offset 0x358,
// and FUN_100741f0 reads the channel vector at offset 0x25e0-0x25e4 and
// the lobby vector at 0x25f0-0x25f4 from the GetServerData() result).
// Therefore, we allocate a large zeroed buffer with a margin for each structure,
// and overlay the small structure at its beginning -- the existing code
// reading/writing fields by name (g_ServerData.xxx) continues to work as is.
static uint8_t g_ServerDataBuffer[65536] = { 0 };

static ServerData& g_ServerData = *reinterpret_cast<ServerData*>(g_ServerDataBuffer);

// (2026-11-10: the room user slot writer moved to fo_world.inc -- FoWriteRoomUserRecord.)

static uint8_t g_ServerHostDataBuffer[65536] = { 0 };
static ServerHostData& g_ServerHostData = *reinterpret_cast<ServerHostData*>(g_ServerHostDataBuffer);

// ---------------------------------------------------------------------------
// Channel vector (FUN_100741f0, disassembled): begin/end pointers at
// offsets +0x25e0/+0x25e4 from GetServerData(), element size -- 44 bytes (0x2C):
//   +0x00 int    ChannelNum
//   +0x04 char*  ChannelName (pointer to string)
//   +0x1c uint   ChannelUserCount
//   +0x20 int    ChannelMaxUserCount
//   +0x24 int    ChannelPing
// The "ChannelType" offset in the decompilation is ambiguous (seems to duplicate
// ChannelNum) -- left as is, not patched separately.
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct ChannelRecord {
    int32_t channelNum;      // +0x00
    char*   channelName;     // +0x04
    uint8_t _pad[0x1c - 0x08]; // up to +0x1c
    uint32_t userCount;       // +0x1c
    int32_t  maxUserCount;    // +0x20
    int32_t  ping;            // +0x24
    uint8_t _pad2[0x2c - 0x28]; // up to full record size 0x2c
};
#pragma pack(pop)
static_assert(sizeof(ChannelRecord) == 0x2c, "ChannelRecord size mismatch");

static char g_FakeChannelName[32] = "Regular Channel";
static ChannelRecord g_FakeChannels44[1] = {
    { 0, g_FakeChannelName, {0}, 5, 20, 30, {0} }
};

// Lobby vector (same FUN_100741f0, second part): begin/end at
// +0x25f0/+0x25f4, step size 0x30 (48 bytes). The exact field layout inside
// the lobby record is NOT confirmed by disassembly to the same degree as the channel --
// we leave the vector empty (begin==end==0), which is safe: the code simply
// skips the loop if begin==0.

// Offline world model (fo_world.inc, included after the callback helpers).
static void FoWorldInitOnce();

static bool g_ServerDataInitialized = false;
static void InitFakeServerDataOnce() {
    if (g_ServerDataInitialized) return;  // don't touch the buffer again -- the engine may have already written to it
    g_ServerDataInitialized = true;
    g_ServerData.connectionStatus = 2;
    g_ServerData.ping = 30;
    g_ServerData.serverId = 1;
    strcpy_s(g_ServerData.serverIp, "127.0.0.1");
    g_ServerData.serverPort = 7777;
    g_ServerData.sessionKey = 0x12345678;
    g_ServerData.pNetworkManager = (void*)0xDEADBEEF;
    g_ServerData.pUserSession = (void*)0xCAFEBABE;

    // Channel vector -- begin/end at confirmed offsets 0x25e0/0x25e4
    *reinterpret_cast<ChannelRecord**>(g_ServerDataBuffer + 0x25e0) = &g_FakeChannels44[0];
    *reinterpret_cast<ChannelRecord**>(g_ServerDataBuffer + 0x25e4) = &g_FakeChannels44[0] + 1;

    // 2026-11-10: everything else that used to be written here by hand
    // (open-map vector +0x2690, "current room" block +0x374.., room user
    // slot +0x48C, profile fields +0x38.., clan name +0x1e4c) is now owned
    // by the offline world model -- FoWorldInitOnce() in fo_world.inc writes
    // the real layouts (wide strings, not narrow; the whole MapInfo map list;
    // profile/inventory/friends/mail/clan vectors).
    FoWorldInitOnce();

    // hostPort 27889 -- the "LithTech" port constant confirmed in the
    // fear-online-server reference project (proudnet/game_start.go,
    // lithTechPort). Everything else here stays neutral (not yet hosting)
    // until Send_ROOM_GAME_START() flips it -- see there for why.
    g_ServerHostData.hostMode = 0;
    g_ServerHostData.hostPlayerId = 0;
    strcpy_s(g_ServerHostData.hostIp, "127.0.0.1");
    g_ServerHostData.hostPort = 27889;
    g_ServerHostData.isRoomOwner = 0;
}

// (2026-11-10: the old LoginData/UserInfo/GateChannelInfo/LobbyRoomInfo/
// ItemInfo fake structs that used to be passed to the callbacks are gone --
// every callback is a no-argument cdecl notification that re-reads the sd
// buffer; the data now lives in the offline world model, fo_world.inc.)

// ============ PATCHING GLOBAL VARIABLES REMOVED ============
// Previously there was a table of "offset -> value" for writing directly into
// GameClient.dll memory. Disassembly analysis (FUN_100395f0) showed that this
// is methodologically wrong: the actual _global.g_... variables are not at
// fixed addresses as simple structure fields -- they are set ONLY through a
// virtual call of the form
//   (**(code**)(*obj + 0x24))("_global.g_name", &value)
// i.e., this is a dictionary/hash-map by string name inside a Scaleform object
// (GFx variable store), not a flat C-structure. The previous patching wrote
// over addresses calculated from the location of STRINGS WITH VARIABLE NAMES
// in the binary -- in fact, it did not affect the actual game values (and
// fortunately didn't break anything, as these strings are apparently not
// re-read after a one-time registration in FUN_1003b440-like initializers).
//
// The correct path -- which already works -- is to pass correctly filled
// structures (LoginData/UserInfo/GateChannelInfo/...) through real callbacks,
// and GameClient.dll itself internally calls the right Global-setters with
// the proper names/values based on these structures.

// ============ HOOK on Engine.exe command dispatcher (crash bypass) ============
// FUN_00486534 in Engine.exe (address is fixed -- ASLR is disabled for both
// Engine.exe and GameClient.dll) -- this is the COMMON dispatcher for UI commands.
//
// Full disassembled listing (obtained from a live Ghidra session)
// showed the exact cause of the crash:
//   004865FF   CALL dword ptr [EAX+0xa4]   -- actual command handler call
//                                              (e.g., real GameServerConnect logic)
//   00486605   PUSH EAX
//   00486606   CALL dword ptr [EDI]        -- THIS call corrupts EBP
//   ...
//   00486631   LEAVE                        -- crash here: EBP is already garbage
//
// IMPORTANT: the first call is, apparently, real, useful logic (the actual
// command handler). If we simply skip the WHOLE function (as we did before),
// we lose this useful work -- that's why the UI didn't respond to the
// connection attempt. If we NOP out only the second call with a static patch,
// it affects ALL commands going through this dispatcher path, not just
// GameServerConnect, which is risky.
//
// The correct solution: execute the ORIGINAL function COMPLETELY (via a
// trampoline), but wrap the call in our __try/__except -- thus we get all
// the legitimate side effects of the first call, and the crash in the epilogue
// (LEAVE) is simply "absorbed" by our SEH, without crashing the process.
// This works for ANY command if it happens to hit the same bug -- not just
// GameServerConnect -- without needing to check its name.
static const uintptr_t HOOK_TARGET_ADDR = 0x00486534;
static const size_t HOOK_PATCH_SIZE = 6;  // push ebp(1) + mov ebp,esp(2) + sub esp,0x1C(3) = 6 bytes, no instruction break

static uint8_t g_OriginalBytes[HOOK_PATCH_SIZE] = { 0 };
static uint8_t* g_Trampoline = nullptr;

typedef void (__stdcall *Dispatcher_t)(int*, uint32_t, uint32_t, uint32_t);
static Dispatcher_t g_OriginalDispatcher = nullptr;

// ============ Dispatcher fault recovery WITHOUT SEH unwinding (2026-11-03) ============
// The "Multiplayer.GameServerConnect" dispatch ends in `call [edi]` at
// Engine.exe 0x00486606 with a register clobbered by the deeply re-entrant
// login callbacks (every nested DISPATCH line in the log happens INSIDE that
// first handler call). Sometimes the fault is "nice" (EIP=0x00486631, ESP
// still inside the stack) and the __except below used to absorb it; this run
// it jumped to EIP=0 with ESP=0x0034004B, far outside the thread stack. Our
// VEH correctly declined to probe -- but the OS then tried to SEH-unwind that
// frame to reach our __except, could not, and raised 0xC0000029
// (STATUS_INVALID_UNWIND_TARGET) -> fast-fail -> the "hung" process.
//
// So we stop depending on unwinding at all. The original dispatcher is now
// entered through GuardedDispatcherCall (naked asm): it records its own
// ESP/EBP in a small per-depth table (the dispatcher re-enters itself from
// nested UI commands, so a single slot is not enough), pushes the four
// stdcall arguments and calls the trampoline. If ANY access violation is
// raised while a guarded call is active, the vectored handler simply writes
// the recorded ESP/EBP back into the thread context, points EIP at
// DispatcherResumeStub and returns EXCEPTION_CONTINUE_EXECUTION -- the CPU
// continues as if the original had just returned; nothing is unwound, so a
// trashed frame can no longer kill the process. DispatcherResumeStub is the
// exact epilogue of GuardedDispatcherCall (pop edi/esi/ebx/ebp; ret 16).
static const LONG kDispMaxDepth = 64;
static DWORD g_DispFrames[kDispMaxDepth * 2];   // [2*i] = esp, [2*i+1] = ebp
static volatile LONG g_DispDepth = 0;
static volatile LONG g_DispResumedCount = 0;
static HMODULE g_hThisModule = nullptr;   // set in DllMain; faults inside our own code are never 'resumed'

__declspec(naked) static void DispatcherResumeStub() {
    __asm {
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret 16
    }
}

__declspec(naked) static void __stdcall GuardedDispatcherCall(int* p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov eax, g_DispDepth
        cmp eax, 64                      ; kDispMaxDepth
        jae no_save
        mov ecx, eax
        shl ecx, 3                       ; 8 bytes per entry
        mov dword ptr g_DispFrames[ecx], esp
        mov dword ptr g_DispFrames[ecx + 4], ebp
    no_save:
        inc dword ptr g_DispDepth
        push dword ptr [ebp + 20]        ; p4
        push dword ptr [ebp + 16]        ; p3
        push dword ptr [ebp + 12]        ; p2
        push dword ptr [ebp + 8]         ; p1
        call dword ptr g_OriginalDispatcher   ; stdcall -> callee pops the 4 args
        dec dword ptr g_DispDepth
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret 16
    }
}

// Called from the vectored handler. Returns true if the context was
// redirected and the handler should return EXCEPTION_CONTINUE_EXECUTION.
static bool TryResumeGuardedDispatcher(EXCEPTION_POINTERS* ex) {
    const LONG depth = g_DispDepth;
    if (depth <= 0 || depth > kDispMaxDepth) return false;
    CONTEXT* c0 = ex->ContextRecord;
    // Never hijack the EXPECTED first-chance faults of our own SEH-guarded
    // memory probes (SafeReadByte/TryGetString/CopyConsoleLine) -- those are
    // handled by their own __except and happen all the time while a nested
    // dispatcher call is active.
    if (g_hThisModule) {
        MODULEINFO mi = {};
        if (GetModuleInformation(GetCurrentProcess(), g_hThisModule, &mi, sizeof(mi))) {
            const uintptr_t lo = (uintptr_t)mi.lpBaseOfDll, hi = lo + mi.SizeOfImage;
            if (c0->Eip >= lo && c0->Eip < hi) return false;
        }
    }
    // Only the signatures that the OS cannot unwind / the game cannot handle:
    //   - ESP outside the thread stack (frame trashed),
    //   - EIP in the dispatcher function itself (its LEAVE/RET epilogue), or
    //   - EIP in the null page (jumped through a clobbered pointer).
    // Everything else keeps the old behaviour (CONTINUE_SEARCH -> game's
    // own SEH, or our __except around the guarded call).
    const NT_TIB* tib = reinterpret_cast<const NT_TIB*>(NtCurrentTeb());
    const uintptr_t stackLo = (uintptr_t)tib->StackLimit, stackHi = (uintptr_t)tib->StackBase;
    const bool espBad   = (c0->Esp < stackLo || c0->Esp >= stackHi);
    const bool eipDisp  = (c0->Eip >= HOOK_TARGET_ADDR && c0->Eip < HOOK_TARGET_ADDR + 0x101);  // 0x00486534..0x00486634
    const bool eipNull  = (c0->Eip < 0x10000);
    if (!espBad && !eipDisp && !eipNull) return false;
    const LONG i = depth - 1;
    CONTEXT* c = ex->ContextRecord;
    const DWORD faultEip = c->Eip, faultEsp = c->Esp, faultEbp = c->Ebp;
    c->Esp = g_DispFrames[2 * i];
    c->Ebp = g_DispFrames[2 * i + 1];
    c->Eip = (DWORD)(uintptr_t)&DispatcherResumeStub;
    g_DispDepth = i;
    const LONG n = InterlockedIncrement(&g_DispResumedCount);
    if (n <= 20) {
        Log("[Proxy] HOOK: dispatcher fault (EIP=0x%08X ESP=0x%08X EBP=0x%08X, depth %ld, last cmd \"%s\") -> context repaired to ESP=0x%08X EBP=0x%08X, resuming past the dispatcher (occurrence %ld)",
            faultEip, faultEsp, faultEbp, depth, g_LastDispatchName, c->Esp, c->Ebp, n);
    }
    return true;
}

static void __stdcall HookedDispatcher(int* param_1, uint32_t param_2, uint32_t param_3, uint32_t param_4) {
    if (!g_OriginalDispatcher) return;

    // DIY debug tracing instead of an interactive x32dbg session: this
    // dispatcher is the COMMON path for every UI command (see the big
    // comment above), so logging every call through it -- not just the ones
    // that happen to crash afterwards -- is our own "logging breakpoint" on
    // every command the UI fires. In particular, this should let us see
    // whatever command name fires when HOST GAME is clicked and rejected
    // with "36001007 Map does not exist" (a validation that apparently
    // never crashes and never reaches any of our Send_* exports, so it was
    // completely invisible to us until now). param_2 is (per the crash-absorb
    // branch below, which already assumed this) a command-name string
    // pointer -- confirmed true in practice: "Global.OnConstruct",
    // "MultiplayerHost.OnConstruct", etc.
    //
    // First attempt at this (flat cap at 300 total calls) burned its whole
    // budget on a UI screen that repeatedly queries
    // "KeyBindings.GetButtonForCommand"/"GetButtonNameForCommand" in a loop
    // (one pair per bindable action) long before the user ever reached HOST
    // GAME -- so we never even saw it. Fix: dedupe PER COMMAND NAME instead
    // of a single global counter -- log the first few occurrences of each
    // distinct name in full, then go quiet for that name specifically (a
    // repeating command like the keybindings query stops flooding the log
    // after its first few hits, while a rare, one-off command fired only
    // once -- like whatever HOST GAME triggers -- always gets logged).
    // Small fixed-size table (no STL container here, to keep this __try/SEH
    // function as close to the codebase's existing low-level style as
    // possible) -- linear scan is fine at this size and call rate.
    struct DispatchNameCount { char name[65]; int count; };
    static DispatchNameCount seenNames[256];
    static int seenNamesCount = 0;
    static int unnamedLoggedCount = 0;  // param_2 didn't look like a string
    const int kLogPerNameLimit = 3;

    char nameBuf[65];
    bool haveName = TryGetString((const void*)(uintptr_t)param_2, nameBuf, sizeof(nameBuf));
    if (haveName) {
        strcpy_s(g_LastDispatchName, nameBuf);
    } else {
        strcpy_s(g_LastDispatchName, "<unreadable>");
    }
    g_LastDispatchParams[0] = param_2;
    g_LastDispatchParams[1] = param_3;
    g_LastDispatchParams[2] = param_4;

    if (haveName) {
        int idx = -1;
        for (int i = 0; i < seenNamesCount; i++) {
            if (strcmp(seenNames[i].name, nameBuf) == 0) { idx = i; break; }
        }
        if (idx < 0 && seenNamesCount < 256) {
            idx = seenNamesCount++;
            strcpy_s(seenNames[idx].name, nameBuf);
            seenNames[idx].count = 0;
        }
        if (idx >= 0) {
            seenNames[idx].count++;
            if (seenNames[idx].count <= kLogPerNameLimit) {
                Log("[Proxy] DISPATCH \"%s\" (occurrence %d for this name): param_1=0x%p param_3=0x%08X param_4=0x%08X",
                    nameBuf, seenNames[idx].count, (void*)param_1, param_3, param_4);
            } else if (seenNames[idx].count == kLogPerNameLimit + 1) {
                Log("[Proxy] DISPATCH \"%s\": further occurrences of this specific name silenced", nameBuf);
            }
        }
        // Table full (256 distinct names) -- fall through silently for any
        // further brand-new names; everything already tracked keeps working.
    } else if (unnamedLoggedCount < 20) {
        unnamedLoggedCount++;
        Log("[Proxy] DISPATCH (param_2 not a readable string, occurrence %d): param_1=0x%p param_2=0x%08X param_3=0x%08X param_4=0x%08X",
            unnamedLoggedCount, (void*)param_1, param_2, param_3, param_4);
    }

    // Execute the ORIGINAL function in full -- including the real command
    // handler call (the first call, which is what performs our login for
    // "Multiplayer.GameServerConnect"). Faults inside it are recovered by the
    // vectored handler through GuardedDispatcherCall's recorded frame -- see
    // the comment above that function. (This used to be a __try/__except;
    // that only works while the OS can still unwind the faulting frame.)
    const LONG depthAtEntry = g_DispDepth;
    __try {
        GuardedDispatcherCall(param_1, param_2, param_3, param_4);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        // Ordinary (unwindable) fault that nobody below us handled -- same
        // absorb-and-continue as before. The guarded call never reached its
        // `dec g_DispDepth`, so restore the depth we entered with.
        g_DispDepth = depthAtEntry;
        static int loggedCount = 0;
        if (loggedCount < 5) {
            loggedCount++;
            Log("[Proxy] HOOK: absorbed dispatcher fault via __except (occurrence %d, cmd \"%s\")",
                loggedCount, haveName ? nameBuf : "<unreadable>");
        }
    }

    // RoomCreate only prepares the lobby. The trace showed that starting the
    // world here queues a transition too early, before the user starts the
    // match; this then suppresses the real Start Game event. Only allow the
    // fallback after the actual RoomGameStart command.
    if (haveName && strcmp(nameBuf, "Multiplayer.RoomGameStart") == 0) {
        EnsureLocalMapStart("Multiplayer.RoomGameStart dispatcher fallback");
    }
}

static bool InstallDispatcherHook() {
    uint8_t* target = (uint8_t*)HOOK_TARGET_ADDR;

    // Save original bytes and create a trampoline: [original bytes] + [jmp back to original+6]
    memcpy(g_OriginalBytes, target, HOOK_PATCH_SIZE);

    g_Trampoline = (uint8_t*)VirtualAlloc(nullptr, HOOK_PATCH_SIZE + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!g_Trampoline) {
        Log("[Proxy] HOOK: VirtualAlloc for trampoline failed!");
        return false;
    }
    memcpy(g_Trampoline, g_OriginalBytes, HOOK_PATCH_SIZE);
    g_Trampoline[HOOK_PATCH_SIZE] = 0xE9;  // jmp rel32
    *(int32_t*)(g_Trampoline + HOOK_PATCH_SIZE + 1) =
        (int32_t)((target + HOOK_PATCH_SIZE) - (g_Trampoline + HOOK_PATCH_SIZE + 5));
    g_OriginalDispatcher = (Dispatcher_t)g_Trampoline;

    // Patch the original: jmp rel32 to our hook + 1 byte NOP (fills the 6th byte)
    DWORD oldProtect;
    if (!VirtualProtect(target, HOOK_PATCH_SIZE, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        Log("[Proxy] HOOK: VirtualProtect failed!");
        return false;
    }
    target[0] = 0xE9;  // jmp rel32
    *(int32_t*)(target + 1) = (int32_t)((uint8_t*)HookedDispatcher - (target + 5));
    target[5] = 0x90;  // NOP, fills the 6th byte
    VirtualProtect(target, HOOK_PATCH_SIZE, oldProtect, &oldProtect);
    FlushInstructionCache(GetCurrentProcess(), target, HOOK_PATCH_SIZE);

    Log("[Proxy] HOOK: installed on Engine.exe+0x486534 (command dispatcher, SEH-wrapped passthrough)");
    return true;
}

// ============ MAIN PATCHING FUNCTION ============
static void ApplyAllPatches() {
    Log("[Proxy] ========================================");
    Log("[Proxy] No raw-memory patches to apply (removed -- see comment above).");
    Log("[Proxy] Relying on properly-filled fake structures via real callbacks.");
    Log("[Proxy] ========================================");

    if (InstallDispatcherHook()) {
        Log("[Proxy] Dispatcher hook active -- GameServerConnect crash should be avoided.");
    } else {
        Log("[Proxy] !!! Dispatcher hook FAILED to install -- crash will still occur !!!");
    }
}

// ============ Callback Storage ============
typedef void* CallbackPtr;
typedef std::map<std::string, CallbackPtr> CallbackMap;
static CallbackMap g_Callbacks;

static void StoreCallback(const char* name, void* callback) {
    if (callback) {
        g_Callbacks[name] = callback;
        Log("[Proxy] StoreCallback: %s -> 0x%p", name, callback);
    }
}

static void* GetCallback(const char* name) {
    auto it = g_Callbacks.find(name);
    return (it != g_Callbacks.end()) ? it->second : nullptr;
}

// Emit one durable, correlated record immediately before each indirect call
// into GameClient.dll. The sequence and last-call fields are also included in
// exception logs so the failing callback is still identifiable if output
// around the fault is truncated.
static LONG LogCallbackBegin(const char* name, void* callback, const void* data,
                             uint32_t arg2, uint32_t arg3, uintptr_t caller) {
    const LONG sequence = InterlockedIncrement(&g_CallbackTraceSequence);
    char modulePath[MAX_PATH] = "<unmapped>";
    HMODULE module = nullptr;
    if (callback && GetModuleHandleExA(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCSTR>(callback), &module)) {
        const DWORD length = GetModuleFileNameA(module, modulePath, MAX_PATH);
        if (length == 0) strcpy_s(modulePath, "<path unavailable>");
    }

    g_LastCallbackName = name;
    g_LastCallbackTarget = callback;
    g_LastCallbackData = data;
    InterlockedExchange(&g_LastCallbackSequence, sequence);
    Log("[Proxy] CALLBACK #%ld PRE name=\"%s\" target=%p module=\"%s\" data=%p arg2=0x%08X arg3=0x%08X caller=0x%08X tid=%lu tick=%lu",
        sequence, name, callback, modulePath, data, arg2, arg3,
        (uint32_t)caller, GetCurrentThreadId(), GetTickCount());
    return sequence;
}

static void LogCallbackReturn(LONG sequence, const char* name) {
    InterlockedExchange(&g_LastCallbackReturnedSequence, sequence);
    Log("[Proxy] CALLBACK #%ld POST name=\"%s\" result=returned tid=%lu tick=%lu",
        sequence, name, GetCurrentThreadId(), GetTickCount());
}

static void LogLastCallbackContext() {
    const LONG sequence = InterlockedCompareExchange(&g_LastCallbackSequence, 0, 0);
    const LONG returned = InterlockedCompareExchange(&g_LastCallbackReturnedSequence, 0, 0);
    if (sequence == 0) {
        Log("[Proxy] Last callback: none");
        return;
    }
    Log("[Proxy] Last callback #%ld state=%s name=\"%s\" target=%p data=%p",
        sequence, (sequence == returned) ? "returned" : "entered-not-returned",
        g_LastCallbackName, g_LastCallbackTarget, g_LastCallbackData);
}

// Dump raw bytes of a structure we're passing to a callback -- allows
// comparison with what GameClient.dll actually expects, without guessing.
static void DumpStructBytes(const char* label, const void* data, size_t size) {
    if (!data) { Log("[Proxy]   %s = NULL", label); return; }
    const uint8_t* p = (const uint8_t*)data;
    size_t toShow = size > 0x60 ? 0x60 : size;  // no more than 96 bytes to avoid flooding the log
    char line[128];
    for (size_t i = 0; i < toShow; i += 16) {
        int pos = sprintf_s(line, "[Proxy]   %s+0x%02zX: ", label, i);
        for (size_t j = 0; j < 16 && i + j < toShow; j++) {
            pos += sprintf_s(line + pos, sizeof(line) - pos, "%02X ", p[i + j]);
        }
        Log("%s", line);
    }
    if (size > toShow) {
        Log("[Proxy]   %s: ... (%zu more bytes not shown)", label, size - toShow);
    }
}

// Like DumpStructBytes, but for pointers we did NOT allocate ourselves --
// e.g. an argument a Send_* export receives from the client, whose real
// size we don't actually know. Reads byte-by-byte through SafeReadByte
// (same helper the crash diagnostics use) so guessing too large a length
// just gets truncated at the first inaccessible page instead of crashing
// the process on an out-of-bounds read.
static void SafeDumpExternal(const char* label, const void* ptr, size_t maxLen) {
    if (!ptr) { Log("[Proxy]   %s = NULL", label); return; }
    const uint8_t* p = (const uint8_t*)ptr;
    char line[128];
    for (size_t i = 0; i < maxLen; i += 16) {
        int pos = sprintf_s(line, "[Proxy]   %s+0x%02zX: ", label, i);
        bool any = false;
        for (size_t j = 0; j < 16 && i + j < maxLen; j++) {
            uint8_t b;
            if (SafeReadByte(p + i + j, &b)) {
                pos += sprintf_s(line + pos, sizeof(line) - pos, "%02X ", b);
                any = true;
            } else {
                pos += sprintf_s(line + pos, sizeof(line) - pos, "?? ");
            }
        }
        Log("%s", line);
        if (!any) break;  // ran off the end of whatever was actually mapped
    }
}

template<typename T>
static void CallCallback(const char* name, T* data) {
    void* cb = GetCallback(name);
    if (cb) {
        Log("[Proxy] Calling %s", name);
        DumpStructBytes(name, data, sizeof(T));
        const LONG traceSequence = LogCallbackBegin(
            name, cb, data, 0, 0, (uintptr_t)_ReturnAddress());
        typedef void (__stdcall *CallbackFunc)(T*);
        ((CallbackFunc)cb)(data);
        LogCallbackReturn(traceSequence, name);
        Sleep(10);
    }
}

// Variant with two parameters -- the second as an INTEGER (not a pointer),
// e.g., resultCode/status. Used for callbacks that crash with a 1-parameter call
// by reading address 0x1 -- seems the second parameter is read as garbage from
// the stack and the code tries to dereference it. If it's actually an int --
// no dereference will occur, and the crash will disappear.
template<typename T>
static void CallCallback2(const char* name, T* data, int secondArg) {
    void* cb = GetCallback(name);
    if (cb) {
        Log("[Proxy] Calling %s (2-arg, second=%d)", name, secondArg);
        DumpStructBytes(name, data, sizeof(T));
        const LONG traceSequence = LogCallbackBegin(
            name, cb, data, (uint32_t)secondArg, 0, (uintptr_t)_ReturnAddress());
        typedef void (__stdcall *CallbackFunc)(T*, int);
        ((CallbackFunc)cb)(data, secondArg);
        LogCallbackReturn(traceSequence, name);
        Sleep(10);
    }
}

static void CallCallbackArray(const char* name, void* data, int count) {
    void* cb = GetCallback(name);
    if (cb) {
        Log("[Proxy] Calling %s (count=%d)", name, count);
        const LONG traceSequence = LogCallbackBegin(
            name, cb, data, (uint32_t)count, 0, (uintptr_t)_ReturnAddress());
        typedef void (__stdcall *CallbackFunc)(void*, int);
        ((CallbackFunc)cb)(data, count);
        LogCallbackReturn(traceSequence, name);
        Sleep(10);
    }
}

static void CallCallbackVoid(const char* name) {
    void* cb = GetCallback(name);
    if (cb) {
        Log("[Proxy] Calling %s", name);
        const LONG traceSequence = LogCallbackBegin(
            name, cb, nullptr, 0, 0, (uintptr_t)_ReturnAddress());
        typedef void (__stdcall *CallbackFunc)(void*);
        ((CallbackFunc)cb)(nullptr);
        LogCallbackReturn(traceSequence, name);
        Sleep(10);
    }
}

// These three GameClient callback entry points were inspected in the captured
// x86 image: their wrapper bodies consult the client's singleton managers and
// do not read a payload from the caller stack. They are cdecl/no-argument
// notifications, unlike CallCallbackVoid(), which passes one pointer.
static bool CallCallbackNotification(const char* name) {
    void* cb = GetCallback(name);
    if (!cb) {
        Log("[Proxy] Room notification %s is not registered", name);
        return false;
    }
    const LONG traceSequence = LogCallbackBegin(
        name, cb, nullptr, 0, 0, (uintptr_t)_ReturnAddress());
    bool returned = false;
    __try {
        typedef void (__cdecl *CallbackFunc)();
        ((CallbackFunc)cb)();
        returned = true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        Log("[Proxy] Room notification %s raised exception 0x%08lX; continuing guarded sequence",
            name, GetExceptionCode());
    }
    if (returned) LogCallbackReturn(traceSequence, name);
    return returned;
}

// GameClient's start callback signature is visible in the forwarding proxy
// used by fear-online-server: (wide map name, int16 name length, int intrude).
// Keep this separate from the generic callback helpers so a missing/incorrect
// payload cannot silently become a null pointer argument.
static void CallGameStartCallback(const char* name, const wchar_t* mapName,
                                  short mapNameLength, int intrude) {
    void* cb = GetCallback(name);
    if (!cb) {
        Log("[Proxy] Callback %s is not registered; skipping", name);
        return;
    }
    Log("[Proxy] Calling %s(map=%ls, length=%d, intrude=%d)",
        name, mapName, (int)mapNameLength, intrude);
    // The forwarding DLL confirms the three stack arguments, but does not
    // establish whether GameClient cleans the stack. Restore ESP ourselves so
    // either cdecl or stdcall cleanup is safe for this experimental callback.
    uint32_t savedEsp = 0;
    const int mapNameLengthArg = (int)mapNameLength;
    const LONG traceSequence = LogCallbackBegin(
        name, cb, mapName, (uint32_t)mapNameLength, (uint32_t)intrude,
        (uintptr_t)_ReturnAddress());
    __try {
        __asm {
            mov savedEsp, esp
            push intrude
            push mapNameLengthArg
            push mapName
            call cb
            mov esp, savedEsp
        }
        LogCallbackReturn(traceSequence, name);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        __asm { mov esp, savedEsp }
        Log("[Proxy] EXCEPTION in CALLBACK #%ld name=\"%s\"; exception absorbed",
            traceSequence, name);
    }
}

// One integer argument, ESP restored by us (callee convention unknown),
// SEH-wrapped -- for the host-side callbacks registered by the in-process
// GameServer.dll (e.g. HOST_GAME_USER_LOAD_COMPLETE(userIndex)).
static void CallCallbackInt1Safe(const char* name, int arg) {
    void* cb = GetCallback(name);
    if (!cb) { Log("[Proxy] Callback %s is not registered; skipping", name); return; }
    Log("[Proxy] Calling %s(%d)", name, arg);
    uint32_t savedEsp = 0;
    const LONG traceSequence = LogCallbackBegin(name, cb, nullptr, (uint32_t)arg, 0, (uintptr_t)_ReturnAddress());
    __try {
        __asm {
            mov savedEsp, esp
            push arg
            call cb
            mov esp, savedEsp
        }
        LogCallbackReturn(traceSequence, name);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        __asm { mov esp, savedEsp }
        Log("[Proxy] EXCEPTION in CALLBACK #%ld name=\"%s\"; exception absorbed", traceSequence, name);
    }
}

// ============ OFFLINE WORLD MODEL (2026-11-10) ============
// Profile, inventory/hangar, rooms/maps, chat, friends, mail, clan -- all
// written into the sd/hd buffers with the ORIGINAL DLL's layouts and
// answered from Tick() through a job queue. See fo_world.inc.
#include "fo_world.inc"

// ============ Main Exports ============
extern "C" __declspec(dllexport) void* GetServerData() {
    static int callCount = 0;
    InitFakeServerDataOnce();
    if (callCount < 2) {
        Log("[Proxy] GetServerData() -> 0x%p (65536-byte buffer)", &g_ServerData);
    } else if (callCount == 2) {
        Log("[Proxy] GetServerData() called again -- further calls silenced");
    }
    callCount++;
    return &g_ServerData;
}

extern "C" __declspec(dllexport) void* GetServerHostData() {
    static int callCount = 0;
    InitFakeServerDataOnce();
    if (callCount < 2) {
        Log("[Proxy] GetServerHostData() -> 0x%p (65536-byte buffer)", &g_ServerHostData);
    } else if (callCount == 2) {
        Log("[Proxy] GetServerHostData() called again -- further calls silenced");
    }
    callCount++;
    return &g_ServerHostData;
}

extern "C" __declspec(dllexport) void Tick() {
    static int callCount = 0;
    static bool loggedOnce = false;

    // 2026-11-03: mirror the engine console (all the "yc_"/MissionMgr/
    // ClientConnectionMgr trace prints) into our log -- see
    // TryInstallLTClientCPrintHook. Cheap no-op once installed.
    TryInstallLTClientCPrintHook();
    g_LastTickMs = GetTickCount();

    DeliverPendingGameLoadComplete();
    DeliverPendingBeginRound();

    // The real Tick() (see disassembly of ZNetwork_orig.dll) iterates over
    // several embedded singleton sub-managers and calls their own virtual Tick()
    // if they are not NULL:
    //   if (piVar1[0]   != 0) piVar1[0]->Tick();
    //   if (piVar1[0xda] != 0) piVar1[0xda]->Tick();
    //   if (piVar1[1]   != 0) piVar1[1]->Tick();
    // We don't have real manager objects (fully fake mode), but we can use
    // the regular Tick() call for the same purpose it serves in the original --
    // maintaining the invariants of the internal GetServerData()-buffer structures
    // in a valid state in case something (our own code or the engine itself)
    // temporarily zeros them out between calls.
    if (g_ServerDataInitialized) {
        // Channels: begin/end at +0x25e0/+0x25e4 must continue to point
        // to our single fake channel.
        *reinterpret_cast<ChannelRecord**>(g_ServerDataBuffer + 0x25e0) = &g_FakeChannels44[0];
        *reinterpret_cast<ChannelRecord**>(g_ServerDataBuffer + 0x25e4) = &g_FakeChannels44[0] + 1;
        // 2026-11-10: every other sd/hd structure (open maps, items, chat
        // deque, friends/mail/clan vectors, current room block, room user
        // slot) is re-asserted from the offline world model.
        FoReassertSd();
    }

    // Deliver the queued "server answers" (room entry, hangar, clan, chat...).
    FoRunQueuedJobs();

    if (!loggedOnce && callCount > 2) {
        Log("[Proxy] Tick() is being called regularly -- reasserting buffer invariants each tick");
        loggedOnce = true;
    }
    callCount++;
}

extern "C" __declspec(dllexport) DWORD init() {
    Log("[Proxy] init() -> 0");
    return 0;
}

extern "C" __declspec(dllexport) void DisConnect() {
    Log("[Proxy] DisConnect()");
}

// IS_HOST() flips to true once Send_ROOM_GAME_START() fires (see below). The
// fear-online-server reference project's proudnet/game_start.go shows the
// real architecture: ProudNet/ZNetwork only brokers the lobby -- actual
// gameplay ("running around the map") is a LithTech listen-server that one of
// the players' own machines hosts (see hostAddress() there, and the
// lithTechPort=27889 constant we reuse in g_ServerHostData below). For a
// single local test client, that host can only be us, so once a match starts
// we must report ourselves as the host or GameClient.dll has no reason to
// spin up its own listen server and load the map.
static bool g_IsHost = false;
extern "C" __declspec(dllexport) int IS_HOST() { return g_IsHost ? 1 : 0; }
extern "C" __declspec(dllexport) int IS_DEDI() { return 0; }
extern "C" __declspec(dllexport) int GetPing() { return 30; }

extern "C" __declspec(dllexport) void destoryed() {
    Log("[Proxy] destoryed()");
}

extern "C" __declspec(dllexport) int GetRoomCount() { return (int)g_FoLobbyRoomCount + (g_FoRoom.active ? 1 : 0); }

extern "C" __declspec(dllexport) void DediDisconnect() {
    Log("[Proxy] DediDisconnect()");
}

extern "C" __declspec(dllexport) void DediStartConnect_EX() {
    Log("[Proxy] DediStartConnect_EX()");
}

// ============ LoginServer_EX ============
// Real prototype (ORIGINAL ZNetwork.dll, plain `ret` = cdecl, all strings
// wide): LoginServer_EX(const wchar_t* ip, short port, const wchar_t* tag
// ("AE"/"NO"/"WI"), const wchar_t* uid, void* extra) -> bool. The old
// __stdcall declaration popped 20 bytes the caller then popped again.
extern "C" __declspec(dllexport) int LoginServer_EX(
    const wchar_t* ip, short port, const wchar_t* tag, const wchar_t* uid, void* extra
) {
    wchar_t wip[64], wtag[8], wuid[64];
    SafeWCopy(wip, 64, ip); SafeWCopy(wtag, 8, tag); SafeWCopy(wuid, 64, uid);
    Log("[Proxy] ========================================");
    Log("[Proxy] LOGIN START (offline)");
    Log("[Proxy] ========================================");
    Log("[Proxy] LoginServer_EX(ip=%ls, port=%d, tag=%ls, uid=%ls, extra=0x%p)", wip, (int)port, wtag, wuid, extra);
    InitFakeServerDataOnce();
    // The real server answers asynchronously (AnswerCreateCredentialSuccess ->
    // LOGIN_DATA, then the login chain). Deliver the whole chain from Tick().
    FoPost(JobLoginSequence, "LoginSequence");
    return 1;
}

// ============ XignCodeInit ============
extern "C" __declspec(dllexport) void XignCodeInit() {
    Log("[Proxy] XignCodeInit()");
}

// ============ Set_On_* functions ============
#define SET_CALLBACK(name) \
    extern "C" __declspec(dllexport) void Set_On_##name##_Callback(void* callback) { \
        StoreCallback(#name, callback); \
    }

#define GET_CALLBACK(name) \
    extern "C" __declspec(dllexport) void* Get_On_##name##_Callback() { \
        return GetCallback(#name); \
    }

#define CALLBACK_LIST \
    X(GAME_AI_DEATH) X(GAME_AI_KILL) X(GAME_BEGIN_ROUND) X(GAME_BOMB) \
    X(GAME_CHECK_BODY) X(GAME_END) X(GAME_GHOST) X(GAME_GHOST_KING) \
    X(GAME_HOST_CHANGE) X(GAME_INTRUDE) X(GAME_LEAVE) X(GAME_LOAD_COMPLETE) \
    X(GAME_OCCUPY) X(GAME_PVE_DATANET) X(GAME_PVE_DOLL) X(GAME_PVE_HELP) \
    X(GAME_RESPAWN) X(GAME_REWARD) X(GAME_ROUND_END) X(GAME_SCORE) \
    X(GAME_START) X(GAME_START_PRE) X(GAME_SUPER_DEFUSING_KIT) \
    X(GAME_TEAM_CHANGE) X(GAME_USER) X(GAME_USER_DEATH) X(GAME_USER_KILL) \
    X(GAME_USER_LEAVE) X(HOST_GAME_AI_DEATH) X(HOST_GAME_AI_KILL) \
    X(HOST_GAME_BEGIN_ROUND) X(HOST_GAME_BOMB) X(HOST_GAME_CHECK_BODY) \
    X(HOST_GAME_END) X(HOST_GAME_GHOST) X(HOST_GAME_GHOST_KING) \
    X(HOST_GAME_LEAVE) X(HOST_GAME_LOAD_COMPLETE) X(HOST_GAME_OCCUPY) \
    X(HOST_GAME_PVE_DATANET) X(HOST_GAME_PVE_DOLL) X(HOST_GAME_PVE_HELP) \
    X(HOST_GAME_PVE_RANK) X(HOST_GAME_RESPAWN) X(HOST_GAME_ROUND_END) \
    X(HOST_GAME_SCORE) X(HOST_GAME_START) X(HOST_GAME_SUPER_DEFUSING_KIT) \
    X(HOST_GAME_TEAM_CHANGE) X(HOST_GAME_USER) X(HOST_GAME_USER_DEATH) \
    X(HOST_GAME_USER_EQUIP) X(HOST_GAME_USER_ITEM) X(HOST_GAME_USER_KILL) \
    X(HOST_GAME_USER_LEAVE) X(HOST_GAME_USER_LOAD_COMPLETE) \
    X(HOST_GAME_WEAPON_CHANGE) X(HOST_GAME_ZOMBIE) \
    X(CAPSULE_BUY) X(CAPSULE_CLOSE) X(CAPSULE_NOTICE) X(CAPSULE_OPEN) \
    X(CHAT) X(CHELLENGE_CLEAR_FAIL) X(CHELLENGE_CLEAR_SUCCESS) \
    X(CHELLENGE_MEDAL_GET) X(CLAN_CHANGE_MARK) X(CLAN_CHECK_CLANNAME) \
    X(CLAN_CHECK_MARK) X(CLAN_CLOSE) X(CLAN_DELETE) X(CLAN_INFO) \
    X(CLAN_INVITE) X(CLAN_KICK_OUT) X(CLAN_MATCH_CANCEL) \
    X(CLAN_MATCH_START) X(CLAN_MEMBER_INFO) X(CLAN_OPEN) X(CLAN_OUT) \
    X(CLAN_REQUEST) X(CLAN_REQUEST_CANCEL) X(CLAN_REQUEST_INFO) \
    X(CLAN_REQUEST_USER) X(CLAN_SEARCH) \
    X(DAILY_MISSION_CLEAR_FAIL) X(DAILY_MISSION_CLEAR_SUCCESS) \
    X(DAILY_MISSION_INFO) X(DAILY_MISSION_NEW_INFO) \
    X(DAILY_MISSION_START_FAIL) X(DAILY_MISSION_START_SUCCESS) \
    X(DEDI_DISCONNECT) X(DEDI_LOGIN_DATA) X(EXPIRE_ITEM_INFO) \
    X(FRIEND_INVITE) X(FRIEND_LIST) X(FRIEND_REQUEST) X(FRIEND_REQUEST_LIST) \
    X(GATE_CHANNEL_INFO) X(GATE_CHANNEL_JOIN) X(GATE_LOBBY_INFO) \
    X(GATE_SERVER_INFO) X(GATE_SERVER_JOIN) X(GIFT_RECEIVE) \
    X(GIFT_RECEIVE_PAGE_INFO) X(GIFT_SEND_PAGE_INFO) X(GIFT_WRITE) \
    X(GM_BAN_USER_BY_USER) X(GM_KICK_USER_BY_USER) X(GM_QUIET_USER_BY_USER) \
    X(GM_WARN_MESSAGE_BY_USER) X(HANGAR_BREAK_WEAR) X(HANGAR_CLOSE) \
    X(HANGAR_CUSTOM_BREAK) X(HANGAR_CUSTOM_BUY) X(HANGAR_CUSTOM_EQUIP) \
    X(HANGAR_CUSTOM_SLOT) X(HANGAR_EQUIP_WEAR) X(HANGAR_ITEM_BUY) \
    X(HANGAR_ITEM_DELETE) X(HANGAR_ITEM_EQUIP) X(HANGAR_ITEM_EXPIRE) \
    X(HANGAR_ITEM_REPAIR) X(HANGAR_ITEM_RESELL) X(HANGAR_ITEM_USE) \
    X(HANGAR_JOIN) X(HANGAR_LEAVE) X(HANGAR_MONEY) X(HANGAR_NICKNAME_CHANGE) \
    X(HANGAR_OPEN) X(HANGAR_PARTS_SLOT) X(HANGAR_RECIPE_ITEM) \
    X(HANGAR_WEAPON_SLOT) X(HANGAR_WEAR_SLOT) X(ITEM_INFO) \
    X(LOBBY_ALL_MODE_MAP) X(LOBBY_CLAN_ROOM_INFO) X(LOBBY_INVITE) \
    X(LOBBY_JOIN) X(LOBBY_MODE_MAP) X(LOBBY_QUICK_JOIN_FAILED) \
    X(LOBBY_ROOM_INFO) X(LOBBY_USER_INFO) X(LOGIN_DATA) X(LOGIN_FAIL) \
    X(MY_ATTENDANCE) X(MY_NICKNAME_CREATE) X(MY_USER_INFO) \
    X(NICKNAME_IN_USE) X(NOTICE_CHAT) X(NOTIFY_ERROR) X(NOTIFY_ERROR_EXIT) \
    X(PERK_BREAK_SLOT) X(PERK_CLOSE) X(PERK_EQUIP_SLOT) \
    X(PERK_INCREASE_SLOT) X(PERK_OPEN) X(POST_CLOSE) X(POST_OPEN) \
    X(POST_RECEIVE_PAGE_INFO) X(POST_SEND_PAGE_INFO) X(POST_UNREAD) \
    X(POST_WRITE) X(PURCHASE_FINALIZE) X(RECIPE_CLOSE) X(RECIPE_ITEM) \
    X(RECIPE_OPEN) X(REPORT) X(REWARD_ITEM_INFO) X(ROOM_EVENT_MAP_INFO) \
    X(ROOM_INFO) X(ROOM_INVITE_USER_INFO) X(ROOM_JOIN) X(ROOM_MATCH_CANCEL) \
    X(ROOM_MATCH_INFO) X(ROOM_MATCH_JOIN) X(ROOM_OPEN_MAP_INFO) \
    X(ROOM_USER_INFO) X(SEARCH_USER_INFO) X(TUTORIAL_CLEAR) \
    X(TUTORIAL_CLOSE) X(TUTORIAL_OPEN)

#define X(name) SET_CALLBACK(name)
CALLBACK_LIST
#undef X

#define X(name) GET_CALLBACK(name)
CALLBACK_LIST
#undef X

// ============ Special Functions ============
extern "C" __declspec(dllexport) void Set_Command_Enum(void* cmd) {
    StoreCallback("Command_Enum", cmd);
}

extern "C" __declspec(dllexport) void Set_On_Command_Enum_Callback(void* callback) {
    StoreCallback("Command_Enum", callback);
}

extern "C" __declspec(dllexport) void* Get_On_Command_Enum_Callback() {
    return GetCallback("Command_Enum");
}

extern "C" __declspec(dllexport) void Set_OnIoConnectedCallback(void* callback) {
    StoreCallback("IoConnected", callback);
}

extern "C" __declspec(dllexport) void Set_OnIoDisonnectedCallback(void* callback) {
    StoreCallback("IoDisconnected", callback);
}

// ============ Send_* functions ============
#define SEND_STUB(name) \
    extern "C" __declspec(dllexport) int Send_##name() { \
        Log("[Proxy] Send_" #name "() -> 1"); \
        return 1; \
    }

#define SEND_STUB_1(name) \
    extern "C" __declspec(dllexport) int Send_##name(void* arg1) { \
        Log("[Proxy] Send_" #name "(0x%p) -> 1", arg1); \
        return 1; \
    }

extern "C" __declspec(dllexport) int Send_LOBBY_USER_INFO() {
    Log("[Proxy] Send_LOBBY_USER_INFO() -> lobby user list");
    FoPost(JobLobbyUsers, "LobbyUsers");
    return 1;
}

// True once the client asked for a room (create or join) in this session.
static bool g_RoomCreateRequested = false;
static bool g_MatchStartSent = false;

// ============ Direct call into GameClient.dll's "ConsoleRunWorld" console
// command implementation (2026-09-11, продолжение) ============
//
// Context: the entire Set_On_*_Callback mechanism for actually spinning up a
// listen server and loading a map turned out to be dead in this build --
// GAME_START/HOST_GAME_START/Set_OnIoConnectedCallback's handlers are all
// confirmed (via live Ghidra decompile, following each JMP thunk to a real
// function body) to be empty `{ return; }` stubs, and GetServerHostData()'s
// wrapper is never called anywhere in the whole decompile. netstat also
// confirms port 27889 never actually opens. So nothing we do through that
// callback path can ever trigger a map load.
//
// Instead: the engine ships a large, UI-hidden developer console command
// table, registered the classic LithTech way via a global command-line-
// argument/console object (`DAT_104ac068` in the decompile, also used to
// parse "+LoginServerIP"-style launch args). One registered command,
// "ConsoleRunWorld", is exactly what we need -- load a world directly, no
// networking involved. Ghidra-confirmed (real function bodies, not stubs):
//
//   LAB_10149460(argc, argv):
//     if (argc < 1 || argv == 0) print usage; else FUN_10149240(0);
//
//   FUN_10149240(param_1 /* passed as 0 by the caller above */):
//     reads a SECOND, IMPLICIT argument out of the EAX register at entry
//     (Ghidra shows this as `char *in_EAX` -- a calling-convention detail
//     the caller's decompile can't show, since Ghidra didn't recognize the
//     mixed register+stack argument passing here). If that string is
//     non-null/non-empty: logs "commandline runworld:\n %s", writes two
//     flag bytes into a global struct at DAT_104a8714+0x52a8/+0x52a9
//     (param_1 and a constant 1), calls four internal helpers with no
//     visible arguments (very likely: stash the world name, then queue/
//     trigger the actual load), builds a small event/message structure on
//     the stack and dispatches it through a function-pointer table at
//     DAT_104b79f8+4 (the classic LithTech "post an engine message" idiom).
//     Returns 1 on success, 0 (with a logged error) if the string was
//     missing.
//
// This confirms ConsoleRunWorld is real, working map-load logic -- not
// another dead stub -- so it remains the best lead for the user's actual
// goal (load a map for solo play, no real host/server needed).
//
// Testing this as a bare "+ConsoleRunWorld TAM_Killhouse" LAUNCH PARAMETER
// crashed (znetwork_proxy_log.txt cut off right after "LOGIN COMPLETE -
// FAKE SUCCESS", the same signature as every other hard crash in this
// project; Engine_Assert00.log showed only the two pre-existing benign
// asserts, so this looks like a genuine unhandled access violation, not an
// assert-and-continue). Two candidate explanations, not mutually
// exclusive:
//   1. TIMING: "+ConsoleRunWorld ..." is parsed and executed by
//      DAT_104ac068's own startup pass over the command line -- the SAME
//      pass that handles "+LoginServerIP"/"+LoginServerPort" -- meaning it
//      almost certainly runs at ENGINE STARTUP, long before our fake
//      LoginServer_EX/room flow has ever run. Whatever state FUN_10149240's
//      post-validation code pokes (the DAT_104a8714/DAT_104b79f8 globals)
//      may simply not be initialized yet at that point in a normal client
//      lifetime -- a very different situation from calling it once the
//      client is already sitting on our fake room's infinite loading
//      screen, well past login.
//   2. CALLING CONVENTION: if the real call site (inside LAB_10149460,
//      compiler-generated) loads argv[0]'s pointer into EAX right before
//      calling FUN_10149240(0), and the launch-parameter code path somehow
//      reaches this without EAX actually holding that pointer (e.g. a
//      difference in how launch-parameter argv differs from a normal
//      dispatched console command's argv), `*in_EAX` could dereference
//      garbage. This wouldn't apply to us calling FUN_10149240 ourselves,
//      since we control EAX directly at the call site below.
//
// Either way, calling FUN_10149240 ourselves -- well after our own fake
// login/room/GAME_START flow, wrapped in our own __try/__except -- sidesteps
// explanation 1 entirely (state should already be as initialized as our fake
// flow ever makes it) and gives us a SAFE experiment for explanation 2: if
// it crashes, our own SEH catches it (unlike the launch-parameter case,
// which crashes in engine startup code we have no hook around) and the
// process should survive with a logged diagnostic instead of a silent
// cutoff.
//
// 2026-09-11 CORRECTION: the first attempt used 0x10149240 as a literal
// runtime address, reasoning (wrongly) that "ASLR disabled" meant
// GameClient.dll always loads at Ghidra's assumed preferred base
// (0x10000000, inferred from the 0x10xxxxxx shape of every address in this
// decompile). That reasoning only actually held for Engine.exe's own base
// (an EXE, which the OS loader always tries to place at its own preferred
// base first and essentially always succeeds, barring ASLR). A DLL is
// different: even with ASLR off, if its preferred base happens to already
// be occupied by another module loaded earlier in the process (0x10000000
// is an extremely common default DLL base -- collisions are the rule, not
// the exception), the loader silently relocates it elsewhere and patches
// its own relocation table -- with no randomization involved, but also no
// guarantee it lands back at 0x10000000. The test confirmed exactly this:
// znetwork_proxy_log.txt's crash dump showed ExceptionAddress==0x10149240
// itself (not some address reached *from* it) -- i.e. the CPU faulted
// trying to execute the very first instruction at that literal address,
// meaning GameClient.dll is NOT actually sitting at 0x10000000 in this
// process's address space right now.
//
// Fix: resolve GameClient.dll's ACTUAL runtime base via GetModuleHandleA()
// at call time and rebase Ghidra's address onto it, exactly like the
// approach already flagged (but not yet needed) earlier in this project for
// anything living in GameClient.dll rather than Engine.exe.
static const uintptr_t GHIDRA_ASSUMED_GAMECLIENT_BASE = 0x10000000;
static const uintptr_t FUN_10149240_GHIDRA_ADDR = 0x10149240;

static char CallConsoleRunWorldDirect(const char* worldName) {
    HMODULE hGameClient = GetModuleHandleA("GameClient.dll");
    if (!hGameClient) {
        Log("[Proxy] CallConsoleRunWorldDirect: GetModuleHandleA(\"GameClient.dll\") failed (err=%lu) -- skipping call", GetLastError());
        return 0;
    }
    const uintptr_t realBase = (uintptr_t)hGameClient;
    const uintptr_t addr = realBase + (FUN_10149240_GHIDRA_ADDR - GHIDRA_ASSUMED_GAMECLIENT_BASE);
    char result = 0;
    uint32_t savedEsp = 0;
    Log("[Proxy] GameClient.dll runtime base=0x%p (Ghidra assumed 0x%p, delta=0x%08X)",
        (void*)realBase, (void*)GHIDRA_ASSUMED_GAMECLIENT_BASE,
        (unsigned)(realBase - GHIDRA_ASSUMED_GAMECLIENT_BASE));
    Log("[Proxy] Calling FUN_10149240(\"%s\") directly at rebased addr 0x%p (ConsoleRunWorld impl, EAX=worldname, arg=0)...",
        worldName, (void*)addr);
    __try {
        // The real calling convention (stdcall vs. cdecl) of FUN_10149240
        // isn't confirmed -- everything else internal to this build that we
        // HAVE been able to check turned out stdcall, but we save/restore
        // ESP ourselves around the call regardless, so a wrong guess there
        // corrupts nothing beyond this one function's local stack usage
        // instead of our own return address/frame.
        __asm {
            mov savedEsp, esp
            mov eax, worldName
            mov ecx, addr
            push 0
            call ecx
            mov esp, savedEsp
            mov result, al
        }
        Log("[Proxy] FUN_10149240(\"%s\") returned %d (no exception)", worldName, (int)result);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        Log("[Proxy] EXCEPTION inside FUN_10149240(\"%s\") -- absorbed by our own SEH, process should survive", worldName);
        result = 0;
    }
    return result;
}

// ============ Engine console-print capture (2026-11-03) ============
// GameClient.dll is full of developer trace prints ("yc_ ..." lines: the
// LoadControlMgr connection state machine, MissionMgr::FinishPreload,
// "Dedi IP : %s, Port : %d", "Client_On_GAME_START_Callback/ bIntude", error
// paths like "No valid maps found for this game mode") that all end up in
// ILTClient::CPrint -- the in-game console, which is disabled in this build,
// so nobody ever sees them. Every one of GameClient's print wrappers
// (FUN_10370420 narrow, FUN_10370480 wide, FUN_1037b120 "dedi,...") ends in
//     ecx = g_pLTClient (DAT_104ac068 / DAT_104bb82c); push str;
//     call [vtbl + 0x11c]          (thiscall, one arg, callee cleans)
// The engine's own "ErrorLog 1" console command exists but its print hook
// pointer is never read anywhere in this Engine.exe build (checked), so it
// would write an empty file. Instead we patch that ONE vtable slot of the
// ILTClient object (data patch, no code trampolines) and mirror every line
// into our log, then forward to the original. Installed lazily from Tick()
// once g_pLTClient is non-null. This gives us the narrative of what the
// client is doing after GAME_START without x32dbg.
static const uintptr_t G_PLTCLIENT_GHIDRA_ADDR = 0x104ac068;   // ILTClient* g_pLTClient in GameClient.dll .data
static const size_t   ILTCLIENT_CPRINT_VTBL_OFFSET = 0x11c;   // confirmed at the call sites above

typedef void (__fastcall *LTClientCPrint_t)(void* self, void* edxUnused, const char* msg);
static LTClientCPrint_t g_OrigLTClientCPrint = nullptr;
static volatile LONG g_CPrintHookState = 0;   // 0 = not tried/retry, 1 = installed, -1 = gave up
static volatile LONG g_CPrintLinesLogged = 0;
static const LONG kCPrintLogCap = 6000;        // safety cap so a chatty frame loop can't flood the log

// Console lines are often long (>64 chars), contain '\n' or non-ASCII
// (Korean/Russian text) -- TryGetString rejects all of that, which is why the
// first run logged "<unreadable string pointer 0x001AE...>" for perfectly
// good stack-resident strings. Dedicated copier: SEH-guarded byte reads, up
// to ~1000 bytes, control characters escaped, high bytes passed through.
ASAN_NO_INSTRUMENT static size_t CopyConsoleLine(const char* src, char* out, size_t outSize) {
    size_t n = 0;
    if (!src || outSize < 4) { if (outSize) out[0] = '\0'; return 0; }
    for (size_t i = 0; n + 3 < outSize; i++) {
        uint8_t c;
        if (!SafeReadByte(reinterpret_cast<const uint8_t*>(src) + i, &c)) { out[n++] = '?'; break; }
        if (c == 0) break;
        if (c == '\n') { if (i == 0) continue; out[n++] = '\\'; out[n++] = 'n'; }
        else if (c == '\r') { /* drop */ }
        else if (c == '\t') { out[n++] = ' '; }
        else if (c < 0x20) { out[n++] = '.'; }
        else out[n++] = (char)c;
    }
    // trim trailing "\n" escapes so every entry stays one clean log line
    while (n >= 2 && out[n - 2] == '\\' && out[n - 1] == 'n') n -= 2;
    out[n] = '\0';
    return n;
}

static void __fastcall HookedLTClientCPrint(void* self, void* edxUnused, const char* msg) {
    if (InterlockedIncrement(&g_CPrintLinesLogged) <= kCPrintLogCap) {
        char buf[1100];
        if (CopyConsoleLine(msg, buf, sizeof(buf)) > 0) {
            Log("[CPrint] %s", buf);
        } else {
            Log("[CPrint] <empty/unreadable string pointer 0x%p>", (const void*)msg);
        }
        if (g_CPrintLinesLogged == kCPrintLogCap) {
            Log("[CPrint] cap of %ld lines reached -- further console output not mirrored", kCPrintLogCap);
        }
    }
    if (g_OrigLTClientCPrint) g_OrigLTClientCPrint(self, edxUnused, msg);
}

static void TryInstallLTClientCPrintHook() {
    if (g_CPrintHookState != 0) return;
    static int attempts = 0;
    if (++attempts > 600) {   // ~10 s of ticks at 60 Hz; stop nagging
        g_CPrintHookState = -1;
        Log("[Proxy] CPrint hook: g_pLTClient never became available -- giving up");
        return;
    }
    HMODULE hGameClient = GetModuleHandleA("GameClient.dll");
    if (!hGameClient) return;
    const uintptr_t base = (uintptr_t)hGameClient;
    void** ppLTClient = reinterpret_cast<void**>(base + (G_PLTCLIENT_GHIDRA_ADDR - GHIDRA_ASSUMED_GAMECLIENT_BASE));
    void* pLTClient = nullptr;
    __try {
        pLTClient = *ppLTClient;
        if (!pLTClient) return;          // not initialised yet -- retry next tick
        void** vtbl = *reinterpret_cast<void***>(pLTClient);
        void** slot = vtbl + (ILTCLIENT_CPRINT_VTBL_OFFSET / sizeof(void*));
        void* orig = *slot;
        if (orig == reinterpret_cast<void*>(&HookedLTClientCPrint)) { g_CPrintHookState = 1; return; }
        DWORD oldProtect = 0;
        if (!VirtualProtect(slot, sizeof(void*), PAGE_READWRITE, &oldProtect)) {
            Log("[Proxy] CPrint hook: VirtualProtect failed (err=%lu) -- giving up", GetLastError());
            g_CPrintHookState = -1;
            return;
        }
        g_OrigLTClientCPrint = reinterpret_cast<LTClientCPrint_t>(orig);
        *slot = reinterpret_cast<void*>(&HookedLTClientCPrint);
        VirtualProtect(slot, sizeof(void*), oldProtect, &oldProtect);
        g_CPrintHookState = 1;
        Log("[Proxy] CPrint hook installed: g_pLTClient=0x%p vtbl=0x%p slot[0x%X]=0x%p (orig 0x%p) -- engine console output now mirrored as [CPrint]",
            pLTClient, (void*)vtbl, (unsigned)ILTCLIENT_CPRINT_VTBL_OFFSET, (void*)slot, orig);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        g_CPrintHookState = -1;
        Log("[Proxy] CPrint hook: exception while reading g_pLTClient/vtable -- giving up");
    }
}

// ============ Kick ClientConnectionMgr's session cleanup (2026-11-03) ============
// Live run of the host-branch build: GAME_START_PRE/GAME_START went through,
// "[1234] Client_On_Game_Start_PRE_Callback" printed, the UI tore down, our
// DisConnect()/destoryed() were called -- and then nothing: no "Dedi IP",
// no "Client_On_GAME_START_Callback/ bIntude", no state-machine prints.
// FUN_10151060 (the match starter) was never reached.
//
// Why: it is only called from ClientConnectionMgr::Update (FUN_100e8280)
// `case 1` of the CLEANUP state machine (this+0x2ef0: 4 = SessionCleanup ->
// 3 = DisconnectFromServer -> 1 = done -> if (shell+0x2e9c) FUN_10151060).
// That machine is kicked by RequestForcedDisconnect_YC (FUN_100e1470:
// MissionMgr+0x10 = 0; this+0x2ef0 = 4), which InterfaceMgr::EnterMenuState
// (FUN_10184620, called from the PRE handler via FUN_10166370) only issues
// when the engine says it is still connected to a server
// (ILTClient member fn at +0x1038 == IsConnected). In the real game the
// menu client IS connected (the lobby/"GameServerConnect" server), so the
// cleanup runs and ends in FUN_10151060. We have no server at all, so
// IsConnected()==false, nothing is requested, cleanup state stays 0 and
// the pending-start flag waits forever -- the loading screen.
//
// Fix: do exactly what RequestForcedDisconnect_YC does, once, right after
// GAME_START. The game's own Update then walks 4 -> 3 -> 1 over the next
// frames (its "DIS_LOG eCleanupState_DisconnectFromServer" print will show
// up in [CPrint]) and calls FUN_10151060 itself -- no function pointers,
// no calling-convention guesses, just two plain memory writes into objects
// whose addresses we resolve from GameClient.dll's runtime base:
//   g_pClientConnectionMgr = *(DAT_104a7904)   (set in FUN_100e8760)
//   g_pMissionMgr          = *(DAT_104b3890)   (set in FUN_101b0850)
static const uintptr_t G_PCLIENTCONNECTIONMGR_GHIDRA_ADDR = 0x104a7904;
static const uintptr_t G_PMISSIONMGR_GHIDRA_ADDR          = 0x104b3890;
static const size_t    CCM_CLEANUP_STATE_OFFSET            = 0x2ef0;   // FUN_100e8280: param_1[0xbbc]
static const size_t    MISSIONMGR_GAMEMODE_OFFSET          = 0x10;

static void KickClientConnectionCleanup() {
    HMODULE hGameClient = GetModuleHandleA("GameClient.dll");
    if (!hGameClient) { Log("[Proxy] KickClientConnectionCleanup: GameClient.dll not found"); return; }
    const uintptr_t base = (uintptr_t)hGameClient;
    __try {
        uint8_t* ccm = *reinterpret_cast<uint8_t**>(base + (G_PCLIENTCONNECTIONMGR_GHIDRA_ADDR - GHIDRA_ASSUMED_GAMECLIENT_BASE));
        uint8_t* mm  = *reinterpret_cast<uint8_t**>(base + (G_PMISSIONMGR_GHIDRA_ADDR - GHIDRA_ASSUMED_GAMECLIENT_BASE));
        if (!ccm) { Log("[Proxy] KickClientConnectionCleanup: g_pClientConnectionMgr is NULL -- cannot kick"); return; }
        const int32_t before = *reinterpret_cast<int32_t*>(ccm + CCM_CLEANUP_STATE_OFFSET);
        if (mm) *reinterpret_cast<uint8_t*>(mm + MISSIONMGR_GAMEMODE_OFFSET) = 0;   // as RequestForcedDisconnect_YC
        *reinterpret_cast<int32_t*>(ccm + CCM_CLEANUP_STATE_OFFSET) = 4;          // eCleanupState_SessionCleanup
        Log("[Proxy] KickClientConnectionCleanup: g_pClientConnectionMgr=0x%p cleanupState %d -> 4 (SessionCleanup), g_pMissionMgr=0x%p gameMode reset; expect DIS_LOG ... then 'Dedi IP' from FUN_10151060",
            (void*)ccm, before, (void*)mm);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        Log("[Proxy] KickClientConnectionCleanup: exception while touching ClientConnectionMgr/MissionMgr");
    }
}

// Send_ROOM_GAME_START(): the client's "Start" button. In the real
// architecture (see fear-online-server's beginMatch()/hostAddress()),
// ProudNet/ZNetwork never runs the match itself -- it just tells everyone in
// the room which machine is hosting a LithTech listen-server on port 27889
// (lithTechPort there, confirmed) and steps out of the way. For one local
// test client, that host can only be us, so this is the one place where
// flipping IS_HOST() to true actually matters: without it, GameClient.dll has
// no reason to spin up its own listen server and load a map at all.
extern "C" __declspec(dllexport) int Send_ROOM_GAME_START() {
    if (g_MatchStartSent) {
        Log("[Proxy] Send_ROOM_GAME_START() duplicate ignored; start callbacks already sent");
        return 1;
    }
    // Set this before invoking callbacks because they may synchronously
    // re-enter the command path through the engine dispatcher.
    g_MatchStartSent = true;
    InterlockedExchange(&g_GameLoadCompleteSent, 0);
    InterlockedExchange(&g_GameLoadCompletePending, 0);
    Log("[Proxy] ========================================");
    Log("[Proxy] Send_ROOM_GAME_START() -- switching this client to HOST mode");
    Log("[Proxy] ========================================");
    if (!g_RoomCreateRequested) {
        Log("[Proxy] WARNING: game start requested but Send_LOBBY_ROOM_CREATE_EX never ran -- continuing anyway");
    }

    InitFakeServerDataOnce();
    g_IsHost = true;
    g_ServerHostData.hostMode = 1;      // guess: 1 = actively hosting (was 0 = idle)
    g_ServerHostData.hostPlayerId = g_Me.userId;
    strcpy_s(g_ServerHostData.hostIp, "127.0.0.1");
    g_ServerHostData.hostPort = 27889;  // confirmed LithTech port (see comment above)
    g_ServerHostData.isRoomOwner = 1;

    // ------------------------------------------------------------------
    // 2026-11-03: full static analysis of the real match-start path (see
    // docs/reference_comparison_and_next_steps.md, entry of this date, for
    // the function-by-function trace). Summary of what GameClient.dll does
    // after GAME_START, with the real (second-registration) handlers resolved
    // through the logged StoreCallback pointers + GameClient.dll's runtime
    // base (0x0B610000 in the last run, NOT the Ghidra base 0x10000000):
    //
    //   GAME_START handler (ghidra 0x10341B90, stdcall, 3 args, ret 0xc):
    //       CGameClientShell::OnGameStart(wideStr, int16, byte)  [0x10146D80]
    //           stores wideStr (narrowed) at shell+0x535c (64 chars),
    //           int16 at shell+0x539c, byte at shell+0x539e
    //       then falls through into the GAME_START_PRE handler [0x10341B20]:
    //           FUN_101679d0: shell+0x2e9c = 1  ("match start pending")
    //                         (and clears the +runworld buffer, i.e. the
    //                          ConsoleRunWorld route is explicitly reset
    //                          here -- it is NOT part of this flow)
    //   Then the UI tears down (Menu.SetReturnMenu/OnDestruct cascade), our
    //   DisConnect()/destoryed() get called (ClientConnectionMgr session
    //   cleanup), and when the cleanup state machine reaches its final step
    //   (ClientConnectionMgr::Update, FUN_100e8280, case 1) it sees
    //   shell+0x2e9c and calls CGameClientShell's match starter
    //   FUN_10151060, which:
    //       - formats "%s:%d" from shell+0x535c / shell+0x539c and logs
    //         "Dedi IP : %s, Port : %d" -- i.e. the GAME_START arguments are
    //         the GAME HOST ADDRESS and PORT (matching fear-online-server's
    //         rmiNotifyGameStart {host, port, ...}), NOT a map name;
    //       - sd = GetServerData();
    //         if (sd[0xECC] == 0 && *(int*)sd == *(int*)(sd+0x450))
    //               -> I AM THE HOST: sets host port from shell+0x539c,
    //                  ClientConnectionMgr::SetupServerHost (m_Type=0 =
    //                  STARTGAME_HOST), MissionMgr::StartGame(1)
    //         else  -> JOIN AS CLIENT: SetupClient("host:port"),
    //                  MissionMgr::StartGame(0xc = eStartGameAsClient)
    //       - LoadControlMgr::RequestStartGame -> state Hello ->
    //         StartGameLobby_OnEnter -> ClientConnectionMgr::StartClientServer
    //         -> ILTClient::StartGame(&request) -> engine hosts GameServer.dll
    //         locally ("UDP: Hosting on ...") -> MID_START_GAME with the
    //         world from MapInfo[ServerData+0x440] -> world load.
    //
    //   Our buffer had +0x000 == 1 but +0x450 == 0, so every previous run
    //   took the JOIN branch and tried to connect to whatever string the
    //   GAME_START args happened to be ("TAM_Killhouse":13 last time) --
    //   that is the infinite loading screen. Nothing ever listened on 27889
    //   because this client never decided it was the host.
    //
    //   The ORIGINAL ZNetwork.dll's NotifyGameStart handler
    //   (decompiled_znetwork.c FUN_100a3750) writes, for "you are host":
    //       sd+0x450 = (uint)sd[0xEE8]   (host index = my index)
    //       sd+0xE84 = 1; sd[0x43D] = 1; sd[0xE80] = 1; sd[0xECC] = dedi
    //       flag (0); sd[0xECD] = 0; sd+0xE88 = 1; sd+0x34 = 4;
    //       sd+0x2700 = 0; sd[0x2704] = 0
    //   and then calls GAME_START(hostAddrW, (short)port, (byte)intrude).
    //   Replicate exactly that.
    // ------------------------------------------------------------------
    {
        uint8_t* sd = g_ServerDataBuffer;
        const int32_t myIndex = *reinterpret_cast<int32_t*>(sd + 0x000);  // 1 (set in InitFakeServerDataOnce/Tick)
        *reinterpret_cast<int32_t*>(sd + 0x450) = myIndex;               // game host index == me -> HOST branch
        *reinterpret_cast<uint8_t*>(sd + 0xEE8) = (uint8_t)myIndex;      // original: +0x450 is copied from this byte
        const int32_t mapIndex = g_FoRoom.active ? (int32_t)g_FoRoom.mapIndex : 518;
        *reinterpret_cast<int32_t*>(sd + 0x440) = mapIndex;              // map index copy read at match start
        *reinterpret_cast<int32_t*>(sd + 0x464) = mapIndex;
        InterlockedExchange(&g_FoMatchActive, 1);                        // stop FoReassertSd() from rewriting +0x450/+0x43D
        g_FoRoom.state = 1;
        *reinterpret_cast<int32_t*>(sd + 0xE84) = 1;
        *reinterpret_cast<uint8_t*>(sd + 0x43D) = 1;
        *reinterpret_cast<uint8_t*>(sd + 0xE80) = 1;
        *reinterpret_cast<uint8_t*>(sd + 0xECC) = 0;                     // NOT a dedicated-server match (required for HOST branch)
        *reinterpret_cast<uint8_t*>(sd + 0xECD) = 0;
        *reinterpret_cast<int32_t*>(sd + 0xE88) = 1;
        *reinterpret_cast<int32_t*>(sd + 0x034) = 4;
        *reinterpret_cast<int32_t*>(sd + 0x2700) = 0;
        *reinterpret_cast<uint8_t*>(sd + 0x2704) = 0;
        Log("[Proxy] ServerData game-start fields set: +0x000=%d +0x450=%d +0xEE8=%d +0xECC=0 +0x440=%d (%s)",
            myIndex, *reinterpret_cast<int32_t*>(sd + 0x450), (int)*reinterpret_cast<uint8_t*>(sd + 0xEE8),
            mapIndex, FoMapFind((uint32_t)mapIndex) ? FoMapFind((uint32_t)mapIndex)->mapName : "unknown map");
    }

    // GAME_START_PRE first (the real server sends rmiNotifyRoomGame before
    // rmiNotifyGameStart). Its handler (0x10341B20) takes no arguments and
    // returns with a plain `ret` -- CallCallbackNotification is the matching
    // cdecl/no-arg helper (CallCallbackVoid would push an argument that the
    // handler never pops).
    CallCallbackNotification("GAME_START_PRE");

    // GAME_START(hostAddressW, port, intrude): host address + port of the
    // match host. Since we ARE the host, the address is only echoed into the
    // "Dedi IP" log line; the PORT becomes the listen-server port
    // (shell+0x539c -> HostOptions+0x1ba -> StartGameRequest.m_HostInfo).
    static const wchar_t kHostAddress[] = L"127.0.0.1";
    CallGameStartCallback("GAME_START", kHostAddress, (short)27889, 0);
    // HOST_GAME_START's registered handler is a confirmed `ret` stub
    // (0x1034F330); harmless, kept for parity with the real sequence.
    CallGameStartCallback("HOST_GAME_START", kHostAddress, (short)27889, 0);

    // The real client only gets from here to the match starter
    // (FUN_10151060) through ClientConnectionMgr's session-cleanup state
    // machine, which nobody kicks when the engine isn't connected to a
    // server -- see KickClientConnectionCleanup().
    KickClientConnectionCleanup();

    // NOTE: CallConsoleRunWorldDirect() is intentionally no longer called.
    // The +runworld mechanism it drives is the single-player "run this world
    // file" path; GAME_START_PRE explicitly clears that buffer, and the
    // genuine multiplayer path above hosts the world through the engine's
    // StartGame(STARTGAME_HOST) instead.

    Log("[Proxy] Send_ROOM_GAME_START() -> 1 (IS_HOST now reports true, host=127.0.0.1:27889)");
    return 1;
}

static void EnsureLocalMapStart(const char* source) {
    if (!g_RoomCreateRequested) {
        Log("[Proxy] %s: room create export has not completed; skipping local start", source);
        return;
    }
    if (g_MatchStartSent) {
        Log("[Proxy] %s: Send_ROOM_GAME_START already handled by the game", source);
        return;
    }
    Log("[Proxy] %s: game did not call Send_ROOM_GAME_START; starting local map fallback", source);
    Send_ROOM_GAME_START();
}

// Stubs in the ORIGINAL DLL too (bare `ret`): LOBBY_ROOM_JOIN/CREATE/MAKE, ROOM_SETTING, ROOM_MAP, ROOM_MAP_CHANGE.
extern "C" __declspec(dllexport) int Send_LOBBY_ROOM_JOIN(int roomId) {
    Log("[Proxy] Send_LOBBY_ROOM_JOIN(%d) -> no-op (original stub; the real join is Send_LOBBY_ROOM_ENTER)", roomId);
    return 1;
}
extern "C" __declspec(dllexport) int Send_ROOM_SETTING() {
    Log("[Proxy] Send_ROOM_SETTING() -> no-op (original stub; the real one is Send_ROOM_DETAIL_SETTING_EX)");
    return 1;
}

extern "C" __declspec(dllexport) int Send_GAME_LOAD_PERCENT(int percent) {
    Log("[Proxy] Send_GAME_LOAD_PERCENT(%d%%) -> 1", percent);
    // 2026-11-03 live run: the client reached "ClientLoadingProcess_Complete",
    // reported Send_GAME_LOAD_PERCENT(100) twice, then sat at
    // "nLoadingPct: 100" for ~30 s and gave up with Send_GAME_LEAVE. It never
    // called Send_GAME_START_PRE_COMPLETE -- 100% load progress IS the
    // "I'm ready" signal the real server answers with NotifyGameLoadComplete.
    if (percent >= 100) {
        if (InterlockedCompareExchange(&g_GameLoadCompleteSent, 0, 0) == 0) {
            InterlockedExchange(&g_GameLoadCompletePending, 1);
            const DWORD sinceTick = GetTickCount() - g_LastTickMs;
            if (g_LastTickMs != 0 && sinceTick < 1000) {
                Log("[Proxy] load reached 100%% -> queueing load-complete sequence for next Tick (last Tick %lu ms ago)", sinceTick);
            } else {
                // Tick() is not being called any more at this point (the
                // lobby session was torn down) -- deliver inline instead.
                Log("[Proxy] load reached 100%% and Tick() is silent (%lu ms) -> delivering load-complete sequence inline", sinceTick);
                DeliverPendingGameLoadComplete();
            }
        }
    }
    return 1;
}

// 2026-11-03: GAME_LOAD_COMPLETE is NOT a passive "all loaded" notice -- its
// real handler (0x1033FD20, no arguments, plain ret) is what moves the
// LoadControlMgr state machine into state 0xb = eClientConnectionState_
// StartLoadWorld: it prints "##### Client_On_GAME_LOAD_COMPLETE_Callback",
// pokes the engine (g_pLTClient slot 0x14 -> ->vtbl[100]) when
// ServerData[0xECC]==0, calls LoadControlMgr::SetState(0xb) and applies the
// local players' loadouts. In the real protocol the client, once its local
// listen server is up and it has reached PreGameLobby, reports
// Send_GAME_START_PRE_COMPLETE (and Send_GAME_LOAD_PERCENT) to ZNetwork, the
// server answers NotifyGameLoadComplete, and ZNetwork_orig's handler
// (decompiled_znetwork.c FUN_100a5130) does `ServerData+0xE84 = 2` and fires
// the callback. So: when the client tells us it is ready, answer the same way
// -- but from Tick(), not re-entrantly from inside the export (same rule as
// the deferred room-entry sequence).

extern "C" __declspec(dllexport) int Send_GAME_START_PRE_COMPLETE() {
    Log("[Proxy] Send_GAME_START_PRE_COMPLETE() -> 1 (queueing GAME_LOAD_COMPLETE for next Tick)");
    InterlockedExchange(&g_GameLoadCompletePending, 1);
    return 1;
}

static void DeliverPendingGameLoadComplete() {
    if (InterlockedCompareExchange(&g_GameLoadCompletePending, 0, 0) == 0 ||
        InterlockedCompareExchange(&g_GameLoadCompleteSent, 0, 0) != 0) return;
    InterlockedExchange(&g_GameLoadCompletePending, 0);
    InterlockedExchange(&g_GameLoadCompleteSent, 1);
    // Replicate the ORIGINAL ZNetwork.dll for the host:
    //  FUN_100a50c0 (NotifyUserLoadComplete): in GetServerHostData() find the
    //    20-entry user table at +0x388 (0x60 bytes each, [+0] = user index),
    //    set its "loaded" byte (+0xD, i.e. hostdata+0x395 for entry 0), then
    //    HOST_GAME_USER_LOAD_COMPLETE(userIndex) -- the in-process
    //    GameServer.dll registered that one (StoreCallback -> 0x39xxxxxx).
    //  FUN_100a5130 (NotifyGameLoadComplete): ServerData+0xE84 = 2; for the
    //    host also hostdata+0x33c = 2 and HOST_GAME_LOAD_COMPLETE();
    //    then GAME_LOAD_COMPLETE() on the client side.
    uint8_t* hd = g_ServerHostDataBuffer;
    *reinterpret_cast<uint32_t*>(hd + 0x388) = 1;   // entry 0: my user index (== ServerData+0x000)
    *reinterpret_cast<uint8_t*>(hd + 0x395) = 1;    // entry 0: loaded
    *reinterpret_cast<int32_t*>(hd + 0x33c) = 2;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0xE84) = 2;   // as ZNetwork_orig FUN_100a5130
    Log("[Proxy] Delivering load-complete sequence from Tick(): HOST_GAME_USER_LOAD_COMPLETE(1), HOST_GAME_LOAD_COMPLETE(), GAME_LOAD_COMPLETE()");
    CallCallbackInt1Safe("HOST_GAME_USER_LOAD_COMPLETE", 1);
    CallCallbackNotification("HOST_GAME_LOAD_COMPLETE");
    CallCallbackNotification("GAME_LOAD_COMPLETE");
}

// 2026-11-03 live run #2: the world loaded, the client went PostLoad ->
// RegisterMatch -> Playing_OnEnter, and the in-process GameServer.dll
// immediately called Send_HOST_GAME_BEGIN_ROUND() -- "server asks the match
// service to start the round". The real service answers NotifyBeginRound;
// ZNetwork_orig's handler (decompiled_znetwork.c FUN_100a4c70) then, in
// GetServerHostData(): for every user entry (+0x388 + i*0x60) whose byte at
// +4 (0x38C) is set, state dword at +8 (0x390) = 3; hostdata+0x340 = round
// number; hostdata+0x33c = 3; and calls HOST_GAME_BEGIN_ROUND() (no args,
// registered by GameServer.dll). The client-side GAME_BEGIN_ROUND is a `ret`
// stub and is never invoked by the original. Without this answer the round
// never starts: HUD up, timer 00:00, no spawn, black screen.
extern "C" __declspec(dllexport) int Send_HOST_GAME_BEGIN_ROUND() {
    Log("[Proxy] Send_HOST_GAME_BEGIN_ROUND() -> 1 (queueing HOST_GAME_BEGIN_ROUND answer)");
    InterlockedExchange(&g_BeginRoundPending, 1);
    const DWORD sinceTick = GetTickCount() - g_LastTickMs;
    if (g_LastTickMs == 0 || sinceTick >= 1000) {
        Log("[Proxy] Tick() is silent (%lu ms) -> delivering begin-round inline", sinceTick);
        DeliverPendingBeginRound();
    }
    return 1;
}

static void DeliverPendingBeginRound() {
    if (InterlockedCompareExchange(&g_BeginRoundPending, 0, 0) == 0) return;
    InterlockedExchange(&g_BeginRoundPending, 0);
    const LONG round = InterlockedIncrement(&g_BeginRoundCount);
    uint8_t* hd = g_ServerHostDataBuffer;
    *reinterpret_cast<uint32_t*>(hd + 0x388) = 1;   // entry 0: my user index
    *reinterpret_cast<uint8_t*>(hd + 0x38C) = 1;    // entry 0: present
    *reinterpret_cast<uint32_t*>(hd + 0x390) = 3;   // entry 0: state = in round
    *reinterpret_cast<uint8_t*>(hd + 0x395) = 1;    // entry 0: loaded
    *reinterpret_cast<uint32_t*>(hd + 0x340) = (uint32_t)round;
    *reinterpret_cast<int32_t*>(hd + 0x33c) = 3;
    Log("[Proxy] Delivering HOST_GAME_BEGIN_ROUND (round %ld) -- hostdata+0x33c=3, +0x340=%ld, user0 state=3", round, round);
    CallCallbackNotification("HOST_GAME_BEGIN_ROUND");
}

#include "fo_exports.inc"

#define SEND_LIST \
    X(CAPSULE_BUY) X(CAPSULE_CLOSE) X(CAPSULE_OPEN) \
    X(CHAT_CLAN_MANAGER) X(CHELLENGE_CLEAR) X(CLAN_MATCH_CANCEL) \
    X(CLAN_MATCH_CREATE) X(CLAN_MATCH_START) X(DAILY_MISSION_CLEAR) \
    X(DAILY_MISSION_INFO) X(DAILY_MISSION_START) X(GAME_HIT_COUNT) \
    X(GAME_PING) X(GATE_CHANNEL_JOIN) X(GATE_LOBBY_INFO) \
    X(GATE_LOBBY_JOIN) X(GATE_SERVER_INFO) X(GATE_SERVER_JOIN) \
    X(GM_BAN_USER) X(GM_KICK_USER) X(GM_NOTICE_MESSAGE) \
    X(GM_QUIET_USER) X(GM_WARN_MESSAGE) X(HANGAR_BREAK_CUSTOM_SLOT) \
    X(HANGAR_BUY_CUSTOM_SLOT) X(HANGAR_EQUIP_CUSTOM_SLOT) \
    X(HANGAR_JOIN) X(HANGAR_LEAVE) X(HANGAR_NICKNAME_CHANGE) \
    X(HANGAR_PARTS_SLOT) X(HANGAR_RECIPE_ITEM) \
    X(HANGAR_WEAPON_CUSTOM_SLOT) X(HANGAR_WEAPON_SLOT) \
    X(HANGAR_WEAR_SLOT) X(HOST_DAILY_MISSION_CLEAR) X(HOST_GAME_BOMB) \
    X(HOST_GAME_CHECK_BODY) X(HOST_GAME_GHOST) \
    X(HOST_GAME_INSTANT_RESPAWN) X(HOST_GAME_LEAVE) \
    X(HOST_GAME_MUST_LOAD_COMPLETE) X(HOST_GAME_OCCUPY) \
    X(HOST_GAME_PING) X(HOST_GAME_PVE_DATANET) X(HOST_GAME_PVE_DOLL) \
    X(HOST_GAME_PVE_HELP) X(HOST_GAME_PVE_RANK) X(HOST_GAME_RESPAWN) \
    X(HOST_GAME_RESPAWN_COMPLETE) X(HOST_GAME_ROUND_END_PVE) \
    X(HOST_GAME_START_WATCH) X(HOST_GAME_SUPER_DEFUSING_KIT) \
    X(HOST_GAME_TIMEOUT) X(HOST_GAME_USER_DEATH) \
    X(HOST_GAME_USER_EQUIP) X(HOST_GAME_USER_ITEM) \
    X(HOST_GAME_USER_KILL) X(HOST_GAME_WEAPON_CHANGE) \
    X(HOST_GAME_ZOMBIE) X(LOBBY_CLAN_ROOM_INFO) \
    X(LOBBY_REFRESH_CLAN_ROOM_INFO) X(LOBBY_ROOM_CREATE) \
    X(LOBBY_ROOM_MAKE) X(PURCHASE_FINALIZE) X(PURCHASE_INIT) \
    X(ROOM_DETAIL_SETTING) X(ROOM_MAP) X(ROOM_MAP_CHANGE) \
    X(ROOM_MATCH_CANCEL) X(ROOM_MATCH_JOIN) X(ROOM_MATCH_START) \
    X(TUTORIAL_CLEAR) X(TUTORIAL_CLOSE) X(TUTORIAL_OPEN)

#define X(name) SEND_STUB(name)
SEND_LIST
#undef X

// ============ Crash Diagnostics (read-only, safe) ============
// IsBadReadPtr/IsBadStringPtrA/IsBadStringPtrW are officially deprecated and
// themselves use SEH internally -- when called from our own VEH handler (i.e.,
// already during exception handling), this can behave unpredictably (we observed
// a secondary crash inside kernel32.dll precisely on such an attempt). Instead --
// direct __try/__except around manual byte-by-byte reading: more reliable,
// without hidden reentrancy issues (the SafeReadByte function itself is declared
// earlier in the file, near Log()).

ASAN_NO_INSTRUMENT static bool TryGetString(const void* ptr, char* outBuf, size_t outBufSize) {
    if (!ptr || !outBuf || outBufSize == 0) return false;
    const uint8_t* s = (const uint8_t*)ptr;
    size_t len = 0;
    size_t maxLen = (outBufSize - 1 < 64) ? (outBufSize - 1) : 64;
    for (; len < maxLen; len++) {
        uint8_t c;
        if (!SafeReadByte(s + len, &c)) return false;  // page inaccessible -- not a string
        if (c == 0) break;
        if (c < 0x20 || c > 0x7E) return false;
        outBuf[len] = (char)c;
    }
    outBuf[len] = '\0';
    return len > 0;
}

ASAN_NO_INSTRUMENT static void TryLogString(const char* label, const void* ptr) {
    if (!ptr) return;
    const uint8_t* s = (const uint8_t*)ptr;
    char buf[65];
    size_t len = 0;
    bool printable = true;
    for (; len < 64; len++) {
        uint8_t c;
        if (!SafeReadByte(s + len, &c)) { printable = false; break; }  // page inaccessible -- not a string
        if (c == 0) break;
        if (c < 0x20 || c > 0x7E) { printable = false; break; }
        buf[len] = (char)c;
    }
    buf[len] = '\0';
    if (printable && len > 0) {
        Log("[Proxy]   %s -> ASCII string: \"%s\"", label, buf);
    }
}

ASAN_NO_INSTRUMENT static void TryLogWideString(const char* label, const void* ptr) {
    if (!ptr) return;
    const uint8_t* s = (const uint8_t*)ptr;
    wchar_t buf[65];
    size_t len = 0;
    bool printable = true;
    for (; len < 64; len++) {
        uint8_t lo, hi;
        if (!SafeReadByte(s + len * 2, &lo) || !SafeReadByte(s + len * 2 + 1, &hi)) { printable = false; break; }
        wchar_t c = (wchar_t)(lo | (hi << 8));
        if (c == 0) break;
        if (c < 0x20 || c > 0x7E) { printable = false; break; }
        buf[len] = c;
    }
    buf[len] = L'\0';
    if (printable && len > 0) {
        Log("[Proxy]   %s -> WIDE string: \"%ls\"", label, buf);
    }
}

ASAN_NO_INSTRUMENT static void DumpBufferRegion(const char* label, const uint8_t* buf, size_t offset, size_t len) {
    Log("[Proxy] --- %s (offset 0x%zX, %zu bytes) ---", label, offset, len);
    char line[128];
    for (size_t i = 0; i < len; i += 16) {
        int pos = sprintf_s(line, "[Proxy]   +0x%04zX: ", offset + i);
        for (size_t j = 0; j < 16 && i + j < len; j++) {
            pos += sprintf_s(line + pos, sizeof(line) - pos, "%02X ", buf[offset + i + j]);
        }
        Log("%s", line);
    }
}

ASAN_NO_INSTRUMENT static void DumpServerDataHotspots() {
    // All offsets we've already confirmed by disassembly as actually
    // read/written by the engine inside the GetServerData()-buffer:
    //   +0x018       -- field read by FUN_006383b0 (current crash, GameServerConnect)
    //   +0x110/0x114/0x118 -- begin/end/capacity vector from FUN_10108da0/FUN_10108bb0
    //   +0x25e0/0x25e4     -- begin/end channel vector (FUN_100741f0)
    //   +0x25f0/0x25f4     -- begin/end lobby vector (FUN_100741f0, second part)
    Log("[Proxy] ======== ServerData buffer hotspot dump ========");
    DumpBufferRegion("start of buffer (general front area)",      g_ServerDataBuffer, 0x00, 0x30);
    DumpBufferRegion("+0x110 vector (MY_USER_INFO area)",          g_ServerDataBuffer, 0x100, 0x30);
    DumpBufferRegion("+0x25e0 channel vector",                     g_ServerDataBuffer, 0x25d0, 0x30);
    DumpBufferRegion("+0x25f0 lobby vector",                       g_ServerDataBuffer, 0x25f0, 0x20);
    Log("[Proxy] ==================================================");
}

ASAN_NO_INSTRUMENT static void DumpCrashDiagnostics(EXCEPTION_POINTERS* ex) {
    CONTEXT* c = ex->ContextRecord;
    Log("[Proxy] --- Registers ---");
    Log("[Proxy]   EAX=0x%08X EBX=0x%08X ECX=0x%08X EDX=0x%08X",
        c->Eax, c->Ebx, c->Ecx, c->Edx);
    Log("[Proxy]   ESI=0x%08X EDI=0x%08X EBP=0x%08X ESP=0x%08X",
        c->Esi, c->Edi, c->Ebp, c->Esp);

    Log("[Proxy] --- Registers as strings (if printable) ---");
    struct { const char* name; DWORD val; } regs[] = {
        {"EAX", c->Eax}, {"EBX", c->Ebx}, {"ECX", c->Ecx}, {"EDX", c->Edx},
        {"ESI", c->Esi}, {"EDI", c->Edi},
    };
    for (auto& r : regs) {
        TryLogString(r.name, (const void*)r.val);
        TryLogWideString(r.name, (const void*)r.val);
    }

    Log("[Proxy] --- Scanning stack for printable string pointers (ESP..ESP+256) ---");
    {
        DWORD* stack = (DWORD*)c->Esp;
        for (int i = 0; i < 64; i++) {
            uint8_t probe;
            if (!SafeReadByte((const uint8_t*)(stack + i), &probe)) continue;  // page inaccessible
            char label[32];
            sprintf_s(label, "[ESP+%d]", i * 4);
            TryLogString(label, (const void*)stack[i]);
            TryLogWideString(label, (const void*)stack[i]);
        }
    }

    // ---------------------------------------------------------------------
    // Raw machine code bytes RIGHT AT THE CRASH ADDRESS (EIP) -- for exact
    // comparison with the disassembler/x32dbg. This gives 100% accurate
    // binding to the real instruction, without hypotheses about which
    // function is currently executing (we've already been wrong assuming
    // FUN_006383b0 -- the actual crash was always at 0x00486631, and we never
    // confirmed that FUN_006383b0 is even involved in this specific execution path).
    // ---------------------------------------------------------------------
    Log("[Proxy] --- Raw bytes at EIP (0x%08X, actual crash instruction) ---", c->Eip);
    {
        uint8_t probe;
        const uint8_t* start = (const uint8_t*)c->Eip - 16;
        if (!SafeReadByte(start, &probe)) start = (const uint8_t*)c->Eip;  // if earlier bytes inaccessible -- start at EIP
        char line[128];
        for (int row = 0; row < 3; row++) {
            const uint8_t* rowStart = start + row * 16;
            int pos = sprintf_s(line, "[Proxy]   0x%08X: ", (unsigned int)(uintptr_t)rowStart);
            bool any = false;
            for (int j = 0; j < 16; j++) {
                uint8_t b;
                if (SafeReadByte(rowStart + j, &b)) {
                    pos += sprintf_s(line + pos, sizeof(line) - pos, "%02X ", b);
                    any = true;
                } else {
                    pos += sprintf_s(line + pos, sizeof(line) - pos, "?? ");
                }
            }
            if (any) Log("%s", line);
        }
        Log("[Proxy]   (EIP is within 16 bytes of the start of this dump, i.e., at address 0x%08X)", c->Eip);
    }

    // ---------------------------------------------------------------------
    // EBX was IDENTICAL (0x00777EEC) in ALL observed crashes, regardless of
    // the crash address/login/password -- looks like a fixed pointer to a
    // global singleton (e.g., CGameClientShell). See what's actually there,
    // and what's at [EBX] and [EBX+0x18].
    // ---------------------------------------------------------------------
    Log("[Proxy] --- Memory pointed to by EBX (0x%08X) ---", c->Ebx);
    {
        uint8_t probe;
        if (SafeReadByte((const uint8_t*)c->Ebx, &probe)) {
            DumpBufferRegion("[EBX]", (const uint8_t*)c->Ebx, 0x00, 0x30);
            // Field +0x18 -- the one that causes the crash in FUN_006383b0
            uint32_t val18 = 0;
            bool ok = true;
            for (int i = 0; i < 4; i++) {
                uint8_t b;
                if (!SafeReadByte((const uint8_t*)c->Ebx + 0x18 + i, &b)) { ok = false; break; }
                val18 |= (uint32_t)b << (i * 8);
            }
            if (ok) {
                Log("[Proxy]   *[EBX+0x18] = 0x%08X  <-- field read by FUN_006383b0", val18);
                // If this is a non-zero pointer -- see what's there (this should
                // be that same "iVar3" from the decompilation, if the assumption is correct)
                uint8_t probe2;
                if (val18 != 0 && SafeReadByte((const uint8_t*)(uintptr_t)val18, &probe2)) {
                    DumpBufferRegion("*[EBX+0x18] (dereferenced)", (const uint8_t*)(uintptr_t)val18, 0x00, 0x30);
                }
            } else {
                Log("[Proxy]   *[EBX+0x18] inaccessible for reading");
            }
        } else {
            Log("[Proxy]   [EBX] inaccessible for reading (invalid pointer)");
        }
    }

    // Since we keep our own copy of the GetServerData()-buffer, and the engine
    // writes directly into it through the returned pointer -- let's see what's
    // actually there at the moment of the crash, without needing to guess from
    // the disassembly.
    DumpServerDataHotspots();

    // Explicit flush right here (not on every ordinary log line) --
    // guarantees that the crash dump definitely gets to disk, even if the
    // process then hangs/crashes again.
    if (g_LogFile) {
        fflush(g_LogFile);
    }
}

// ============ Interception of "silent" terminations (abort/invalid_parameter) ============
// SetUnhandledExceptionFilter only catches true structured exceptions
// (SEH, C0000005, etc.). Calls to abort()/_invalid_parameter_noinfo() in this
// codebase (we've already seen them in the decompilation) terminate the process
// "silently", without a structured exception -- separate CRT hooks are needed.
#include <csignal>
#include <cstdlib>

static void __cdecl OnAbortSignal(int) {
    Log("[Proxy] ========================================");
    Log("[Proxy] SIGABRT caught (abort() was called somewhere in GameClient.dll/Engine.exe)!");
    Log("[Proxy] ========================================");
    // Give the log enough time to reach DebugView before termination
    Sleep(200);
}

static void __cdecl OnInvalidParameter(const wchar_t* expr, const wchar_t* func,
                                        const wchar_t* file, unsigned int line, uintptr_t) {
    Log("[Proxy] ========================================");
    Log("[Proxy] CRT invalid parameter handler triggered!");
    Log("[Proxy]   expr: %ls", expr ? expr : L"(null)");
    Log("[Proxy]   func: %ls", func ? func : L"(null)");
    Log("[Proxy]   file: %ls line: %u", file ? file : L"(null)", line);
    Log("[Proxy] ========================================");
    Sleep(200);
}

// ============ DllMain ============
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        g_hThisModule = hModule;
        DisableThreadLibraryCalls(hModule);

        // IMPORTANT: open the log file IMMEDIATELY, synchronously, here -- not lazily
        // on the first Log(). If EnsureLogFileOpen() (with its fopen_s,
        // allocating memory via the CRT heap) were called for the FIRST time
        // already inside our VEH handler at the moment of a crash -- and the
        // crash occurred while the same thread already held the CRT heap lock
        // (e.g., was inside malloc/free) -- fopen_s could deadlock (non-reentrant
        // lock). Opening the file in advance completely removes this risk.
        EnsureLogFileOpen();

        signal(SIGABRT, OnAbortSignal);
        _set_invalid_parameter_handler(OnInvalidParameter);
        _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT); // don't show the standard "Debug/Abort/Retry" dialog, just log

        // IMPORTANT: the game itself wraps its per-frame tick/command dispatcher
        // in SEH (__try/__except -- we saw this in the disassembly, e.g.,
        // FUN_1001059b) and "swallows" access violations, simply continuing
        // to the next frame. SetUnhandledExceptionFilter is called by the OS ONLY
        // if the exception is NOT handled by anyone -- and since the game handles
        // it every frame, our SetUnhandledExceptionFilter never triggers.
        // AddVectoredExceptionHandler is called on EVERY exception, BEFORE
        // any SEH handlers -- exactly what we need to catch the real state
        // at the moment of the crash, even if the game later "swallows" it.
        AddVectoredExceptionHandler(1, [](EXCEPTION_POINTERS* ex) -> LONG {
            static int loggedCount = 0;
            static volatile bool inHandler = false;  // protection against recursive crash inside the handler itself
            const DWORD exceptionCode = ex->ExceptionRecord->ExceptionCode;
            // OutputDebugString raises DBG_PRINTEXCEPTION_C / DBG_PRINTEXCEPTION_WIDE_C
            // in a debugger-aware process. They are normal diagnostics, not game
            // faults; logging them used up the first-chance exception budget before
            // the actual failure happened.
            if ((exceptionCode & 0xFFFF0000) == 0x40010000 ||
                exceptionCode == EXCEPTION_BREAKPOINT ||
                exceptionCode == EXCEPTION_SINGLE_STEP) {
                return EXCEPTION_CONTINUE_SEARCH;
            }
            if (loggedCount < 12 && !inHandler) {
                inHandler = true;
                loggedCount++;
                Log("[Proxy] ========================================");
                Log("[Proxy] VECTORED EXCEPTION code=0x%08X (occurrence %d)",
                    exceptionCode, loggedCount);
                Log("[Proxy] ExceptionAddress: 0x%p", ex->ExceptionRecord->ExceptionAddress);
                Log("[Proxy] Last dispatcher: %s param2=0x%08X param3=0x%08X param4=0x%08X",
                    g_LastDispatchName, g_LastDispatchParams[0],
                    g_LastDispatchParams[1], g_LastDispatchParams[2]);
                LogLastCallbackContext();
                if (exceptionCode == EXCEPTION_ACCESS_VIOLATION) {
                    Log("[Proxy] Access Violation! Address: 0x%p",
                        ex->ExceptionRecord->ExceptionInformation[1]);
                    // 2026-11-03: the intermittent "dies right after LOGIN
                    // COMPLETE" case is the known dispatcher-epilogue crash
                    // (corrupted EBP -> LEAVE/RET -> EIP=0) in the variant
                    // where ESP is ALSO garbage (e.g. ESP=0x0034004B, EBP=0
                    // in the 2026-10 log). The kernel still delivered the
                    // exception (so this handler runs), but the stack
                    // pointer is outside this thread's real stack: every
                    // __try in the deep dump below pushes an SEH frame at
                    // that bogus ESP, the OS then sees an exception chain
                    // outside the TEB stack limits on the very first probe
                    // fault and terminates the process -- the log stops at
                    // "Registers as strings". So: if ESP is outside
                    // [StackLimit, StackBase), log that fact and bail out
                    // WITHOUT any SEH-based probing, letting the dispatcher
                    // hook's own __except (registered while the stack was
                    // still sane) absorb the crash like it does in the
                    // "ESP still valid" runs.
                    const NT_TIB* tib = reinterpret_cast<const NT_TIB*>(NtCurrentTeb());
                    const uintptr_t esp = ex->ContextRecord->Esp;
                    const uintptr_t stackLo = reinterpret_cast<uintptr_t>(tib->StackLimit);
                    const uintptr_t stackHi = reinterpret_cast<uintptr_t>(tib->StackBase);
                    if (esp < stackLo || esp >= stackHi) {
                        Log("[Proxy] ESP=0x%08X is OUTSIDE this thread's stack [0x%08X,0x%08X) -- EIP=0x%p EBP=0x%08X; skipping deep dump (SEH probes would kill the process)",
                            (unsigned)esp, (unsigned)stackLo, (unsigned)stackHi,
                            ex->ExceptionRecord->ExceptionAddress, (unsigned)ex->ContextRecord->Ebp);
                    } else {
                        DumpCrashDiagnostics(ex);
                    }
                    // A fault while the UI command dispatcher is running under our
                    // guard: repair the context and continue past the dispatcher
                    // (no unwinding involved -- works with a trashed stack too).
                    if (TryResumeGuardedDispatcher(ex)) {
                        Log("[Proxy] ========================================");
                        inHandler = false;
                        return EXCEPTION_CONTINUE_EXECUTION;
                    }
                }
                Log("[Proxy] ========================================");
                inHandler = false;
            }
            // Not ours to recover -- but if a guarded dispatcher call is active
            // and this is an access violation we did not log (budget exhausted),
            // still recover it rather than let the OS fail to unwind.
            if (exceptionCode == EXCEPTION_ACCESS_VIOLATION && !inHandler && TryResumeGuardedDispatcher(ex)) {
                return EXCEPTION_CONTINUE_EXECUTION;
            }
            return EXCEPTION_CONTINUE_SEARCH;  // don't interfere with the game's own handling
        });

        SetUnhandledExceptionFilter([](EXCEPTION_POINTERS* ex) -> LONG {
            Log("[Proxy] ========================================");
            Log("[Proxy] UNHANDLED EXCEPTION!");
            Log("[Proxy] ExceptionCode: 0x%08X", ex->ExceptionRecord->ExceptionCode);
            Log("[Proxy] ExceptionAddress: 0x%p", ex->ExceptionRecord->ExceptionAddress);
            LogLastCallbackContext();
            if (ex->ExceptionRecord->ExceptionCode == 0xC0000005) {
                Log("[Proxy] Access Violation! Address: 0x%p", 
                    ex->ExceptionRecord->ExceptionInformation[1]);
            }
            DumpCrashDiagnostics(ex);
            Log("[Proxy] ========================================");
            return EXCEPTION_EXECUTE_HANDLER;
        });
        
        Log("[Proxy] ========================================");
        Log("[Proxy] ZNetwork PROXY LOADED (safe version, no byte-scan patching)");
        Log("[Proxy] Process id=%lu; callback trace enabled", GetCurrentProcessId());
        Log("[Proxy] ========================================");

        // APPLY PATCHES IMMEDIATELY ON LOAD
        // GameClient.dll has already loaded us via LoadLibrary("Game\\ZNetwork.dll"),
        // meaning by this point it's fully mapped into memory.
        // Engine.exe is also already loaded (it loaded GameClient.dll).
        ApplyAllPatches();
    } else if (reason == DLL_PROCESS_DETACH) {
        if (g_LogFile) {
            Log("[Proxy] DLL_PROCESS_DETACH reserved=0x%p roomCreateRequested=%d matchStartSent=%d lastDispatch=\"%s\"",
                reserved, g_RoomCreateRequested ? 1 : 0, g_MatchStartSent ? 1 : 0,
                g_LastDispatchName);
            LogLastCallbackContext();
            fclose(g_LogFile);
            g_LogFile = nullptr;
        }
    }
    return TRUE;
}
