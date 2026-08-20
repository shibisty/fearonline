// ============================================================================
//  ZNetwork.dll proxy/stub  --  SAFE VERSION
//  Patching only known offsets of global variables in GameClient.dll.
//  Blind byte-scan across the entire module memory removed -- too high risk
//  of accidentally overwriting unrelated data/code that matches by 4 bytes.
// ============================================================================

#include <windows.h>
#include <psapi.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <cstdint>
#include <map>
#include <string>
#include <exception>

#pragma comment(lib, "psapi.lib")

// ============ Logging ============
// Path to the log file -- next to the project, for easy copying of contents
// without needing to scroll through DebugView. Overwritten (not appended)
// each time the game starts anew.
static const char* LOG_FILE_PATH = "C:\\fearonline\\znetwork_proxy_log.txt";
static FILE* g_LogFile = nullptr;
static bool g_LogFileInitTried = false;

static void EnsureLogFileOpen() {
    if (g_LogFileInitTried) return;
    g_LogFileInitTried = true;
    fopen_s(&g_LogFile, LOG_FILE_PATH, "w");  // "w" = overwrite from scratch
    // Regular buffering (not _IONBF) -- frequent fflush after each line
    // added noticeable synchronous disk I/O before each callback call,
    // which could shift thread timing enough to manifest a race condition
    // unrelated to the main investigation (see the single crash at address 0x00000000).
    // fflush is now called only explicitly, in crash diagnostics (DumpCrashDiagnostics),
    // where it really matters -- not on every ordinary log line.
}

static void Log(const char* fmt, ...) {
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
    va_end(ap);
    OutputDebugStringA(buf);
    OutputDebugStringA("\n");

    EnsureLogFileOpen();
    if (g_LogFile) {
        fputs(buf, g_LogFile);
        fputc('\n', g_LogFile);
    }
}

// Safe reading of one byte via __try/__except -- used both in the dispatcher hook
// (command name check) and in crash diagnostics. More reliable than obsolete
// IsBadReadPtr/IsBadStringPtr* (see comment later in the file).
static bool SafeReadByte(const uint8_t* p, uint8_t* out) {
    __try {
        *out = *p;
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

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

    // ---------------------------------------------------------------------
    // Player profile fields -- offsets confirmed by disassembling
    // thunk_FUN_10059580 (Multiplayer.OnSetMyInfo), which reads them
    // directly from GetServerData(). The Grade/Level values match what we
    // previously tried to set via an incorrect method (patching string-name
    // addresses instead of the actual storage) -- now we apply them in the
    // right place.
    // ---------------------------------------------------------------------
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x38)   = 10;   // MyUserGrade
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x40)   = 100;  // MyLevel
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x48)   = 0;    // purpose not confirmed -- safe 0
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x50)   = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x58)   = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x60)   = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x64)   = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x68)   = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x70)   = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x74)   = 0;
    *reinterpret_cast<uint8_t*>(g_ServerDataBuffer + 0x118)  = 0;    // byte flag, purpose not confirmed
    *reinterpret_cast<uint8_t*>(g_ServerDataBuffer + 0x1c9)  = 0;    // byte flag, purpose not confirmed
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x1e90) = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x1e94) = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x1e98) = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x1ea0) = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x1ed0) = 0;
    *reinterpret_cast<int32_t*>(g_ServerDataBuffer + 0x1ee0) = 0;
    // +0x1e4c is passed as an ADDRESS directly (not dereferenced before
    // the call) -- looks like an inline string (e.g., nickname). Put a valid,
    // null-terminated string here to avoid reading garbage if something tries
    // to read it as a C-string.
    strcpy_s(reinterpret_cast<char*>(g_ServerDataBuffer + 0x1e4c), 64, "TestUser");

    g_ServerHostData.hostMode = 0;
    g_ServerHostData.hostPlayerId = 0;
    strcpy_s(g_ServerHostData.hostIp, "127.0.0.1");
    g_ServerHostData.hostPort = 7777;
    g_ServerHostData.isRoomOwner = 0;
}

static LoginData g_FakeLoginData = {
    0, 1001, 0x12345678, 0, "TestUser", 1, 1
};

static UserInfo g_FakeUserInfo = {
    1001, 1, 0, 100, "TestUser", 0, 0, 19508, 0
};

static LoginFailData g_FakeLoginFail = {
    0, "Success"
};

static GateServerInfo g_FakeGateServerInfo = {
    1, 1, "Test Server", "127.0.0.1", 7777, 100, 500
};

static GateChannelInfo g_FakeChannels[3] = {
    {1, "Channel 1", 10, 100, 0},
    {2, "Channel 2", 25, 100, 0},
    {3, "Channel 3", 50, 100, 0}
};

static GateLobbyInfo g_FakeGateLobbyInfo = {
    1, "Lobby 1", 10, 100
};

static LobbyRoomInfo g_FakeRooms[3] = {
    {1, "Room 1", 2, 8, 1, 1, 0, ""},
    {2, "Room 2", 4, 8, 2, 2, 0, ""},
    {3, "Room 3", 1, 8, 1, 3, 0, ""}
};

// Simplified items
static ItemInfo g_FakeItems[5] = {
    {1, "Item 1", 100, 0, 1, 1, 0, 1},
    {2, "Item 2", 200, 0, 1, 1, 0, 1},
    {3, "Item 3", 300, 0, 1, 1, 0, 1},
    {4, "Item 4", 400, 0, 1, 1, 0, 1},
    {5, "Item 5", 500, 0, 1, 1, 0, 1}
};

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

static void __stdcall HookedDispatcher(int* param_1, uint32_t param_2, uint32_t param_3, uint32_t param_4) {
    if (!g_OriginalDispatcher) return;

    __try {
        // Execute the ORIGINAL function in full -- including the real command
        // handler call (the first call). If it reaches the crash in the epilogue
        // (LEAVE with corrupted EBP) -- we'll catch it below, without losing
        // the useful work already performed.
        g_OriginalDispatcher(param_1, param_2, param_3, param_4);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        // Absorb the epilogue crash -- the real command logic has already executed
        // (the first call succeeded), the function just couldn't return normally
        // due to corrupted EBP. Log throttled to avoid flooding the log on
        // frequent triggers.
        static int loggedCount = 0;
        if (loggedCount < 5) {
            loggedCount++;
            const char* cmdName = (const char*)param_2;
            Log("[Proxy] HOOK: absorbed dispatcher crash for command (occurrence %d, param_2=0x%08X)",
                loggedCount, param_2);
        }
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

template<typename T>
static void CallCallback(const char* name, T* data) {
    void* cb = GetCallback(name);
    if (cb) {
        Log("[Proxy] Calling %s", name);
        DumpStructBytes(name, data, sizeof(T));
        typedef void (__stdcall *CallbackFunc)(T*);
        ((CallbackFunc)cb)(data);
        Log("[Proxy] %s returned", name);
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
        typedef void (__stdcall *CallbackFunc)(T*, int);
        ((CallbackFunc)cb)(data, secondArg);
        Log("[Proxy] %s returned", name);
        Sleep(10);
    }
}

static void CallCallbackArray(const char* name, void* data, int count) {
    void* cb = GetCallback(name);
    if (cb) {
        Log("[Proxy] Calling %s (count=%d)", name, count);
        typedef void (__stdcall *CallbackFunc)(void*, int);
        ((CallbackFunc)cb)(data, count);
        Log("[Proxy] %s returned", name);
        Sleep(10);
    }
}

static void CallCallbackVoid(const char* name) {
    void* cb = GetCallback(name);
    if (cb) {
        Log("[Proxy] Calling %s", name);
        typedef void (__stdcall *CallbackFunc)(void*);
        ((CallbackFunc)cb)(nullptr);
        Log("[Proxy] %s returned", name);
        Sleep(10);
    }
}

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

        // The other known "dangerous" vectors (+0x110/0x114/0x118 --
        // MY_USER_INFO-related, +0x25f0/0x25f4 -- lobbies) we deliberately keep empty
        // (begin==end==0), since their exact field layout is not confirmed --
        // we explicitly re-assert this on every tick, so that no extraneous code
        // (our own InitFakeServerDataOnce and other callbacks) accidentally leaves
        // garbage there between calls.
        *reinterpret_cast<uint32_t*>(g_ServerDataBuffer + 0x110) = 0;
        *reinterpret_cast<uint32_t*>(g_ServerDataBuffer + 0x114) = 0;
        *reinterpret_cast<uint32_t*>(g_ServerDataBuffer + 0x118) = 0;
        *reinterpret_cast<uint32_t*>(g_ServerDataBuffer + 0x25f0) = 0;
        *reinterpret_cast<uint32_t*>(g_ServerDataBuffer + 0x25f4) = 0;
    }

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

extern "C" __declspec(dllexport) int IS_HOST() { return 0; }
extern "C" __declspec(dllexport) int IS_DEDI() { return 0; }
extern "C" __declspec(dllexport) int GetPing() { return 30; }

extern "C" __declspec(dllexport) void destoryed() {
    Log("[Proxy] destoryed()");
}

extern "C" __declspec(dllexport) int GetRoomCount() { return 3; }

extern "C" __declspec(dllexport) void DediDisconnect() {
    Log("[Proxy] DediDisconnect()");
}

extern "C" __declspec(dllexport) void DediStartConnect_EX() {
    Log("[Proxy] DediStartConnect_EX()");
}

// ============ LoginServer_EX ============
extern "C" __declspec(dllexport) int __stdcall LoginServer_EX(
    const char* ip, short port, const wchar_t* pb, const char* uid, void* extra
) {
    Log("[Proxy] ========================================");
    Log("[Proxy] LOGIN START");
    Log("[Proxy] ========================================");
    Log("[Proxy] LoginServer_EX(ip=%s, port=%d, uid=%s)", 
        ip ? ip : "null", port, uid ? uid : "null");
    Log("[Proxy] IS_HOST: %d, IS_DEDI: %d", IS_HOST(), IS_DEDI());
    
    InitFakeServerDataOnce();
    
    Log("[Proxy] ServerData updated");
    
    CallCallback<LoginData>("LOGIN_DATA", &g_FakeLoginData);
    CallCallback2<UserInfo>("MY_USER_INFO", &g_FakeUserInfo, 0);
    CallCallback<GateServerInfo>("GATE_SERVER_INFO", &g_FakeGateServerInfo);
    CallCallbackArray("GATE_CHANNEL_INFO", g_FakeChannels, 3);
    CallCallback<GateLobbyInfo>("GATE_LOBBY_INFO", &g_FakeGateLobbyInfo);
    CallCallback<GateServerInfo>("GATE_SERVER_JOIN", &g_FakeGateServerInfo);
    CallCallback2<GateChannelInfo>("GATE_CHANNEL_JOIN", &g_FakeChannels[0], 0);
    CallCallback<GateLobbyInfo>("LOBBY_JOIN", &g_FakeGateLobbyInfo);
    CallCallback<UserInfo>("LOBBY_USER_INFO", &g_FakeUserInfo);
    CallCallbackArray("LOBBY_ROOM_INFO", g_FakeRooms, 3);
    // CallCallback<LoginFailData>("LOGIN_FAIL", &g_FakeLoginFail);
    
    CallCallbackArray("ITEM_INFO", g_FakeItems, 5);
    
    Log("[Proxy] ========================================");
    Log("[Proxy] LOGIN COMPLETE - FAKE SUCCESS");
    Log("[Proxy] ========================================");
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
    Log("[Proxy] Send_LOBBY_USER_INFO() -> 1");
    return 1;
}

extern "C" __declspec(dllexport) int Send_LOBBY_ROOM_CREATE_EX(void* data) {
    Log("[Proxy] Send_LOBBY_ROOM_CREATE_EX(0x%p) -> 1", data);
    return 1;
}

extern "C" __declspec(dllexport) int Send_LOBBY_ROOM_JOIN(int roomId) {
    Log("[Proxy] Send_LOBBY_ROOM_JOIN(%d) -> 1", roomId);
    return 1;
}

extern "C" __declspec(dllexport) int Send_LOBBY_QUICK_JOIN() {
    Log("[Proxy] Send_LOBBY_QUICK_JOIN() -> 1");
    return 1;
}

extern "C" __declspec(dllexport) int Send_LOBBY_ROOM_INFO() {
    Log("[Proxy] Send_LOBBY_ROOM_INFO() -> 1");
    return 1;
}

extern "C" __declspec(dllexport) int Send_ROOM_SETTING() {
    Log("[Proxy] Send_ROOM_SETTING() -> 1");
    return 1;
}

extern "C" __declspec(dllexport) int Send_GAME_LOAD_PERCENT(int percent) {
    Log("[Proxy] Send_GAME_LOAD_PERCENT(%d%%) -> 1", percent);
    return 1;
}

#define SEND_LIST \
    X(CAPSULE_BUY) X(CAPSULE_CLOSE) X(CAPSULE_OPEN) \
    X(CHAT_ALL) X(CHAT_CLAN) X(CHAT_CLAN_MANAGER) X(CHAT_TEAM) X(CHAT_WHISPER) \
    X(CHELLENGE_CLEAR) X(CLAN_CHANGE_CLANNAME) X(CLAN_CHANGE_MARK) \
    X(CLAN_CHANGE_MASTER) X(CLAN_CHECK_CLANNAME) X(CLAN_CHECK_MARK) \
    X(CLAN_CLOSE) X(CLAN_CREATE) X(CLAN_DELETE) X(CLAN_INVITE) \
    X(CLAN_KICK_OUT) X(CLAN_MATCH_CANCEL) X(CLAN_MATCH_CREATE) \
    X(CLAN_MATCH_START) X(CLAN_MEMBER_POSITION) X(CLAN_OPEN) X(CLAN_OUT) \
    X(CLAN_REQUEST) X(CLAN_REQUEST_ACCEPT) X(CLAN_REQUEST_CANCEL) \
    X(CLAN_REQUEST_REFUSE) X(CLAN_SEARCH) X(CLAN_SEARCH_MASTER) \
    X(CLAN_UPDATE_INTRO) X(CLAN_UPDATE_NOTICE) X(DAILY_MISSION_CLEAR) \
    X(DAILY_MISSION_INFO) X(DAILY_MISSION_START) X(FRIEND_DELETE) \
    X(FRIEND_INVITE) X(FRIEND_REQUEST) X(FRIEND_REQUEST_ACCEPT) \
    X(FRIEND_REQUEST_REFUSE) X(GAME_HIT_COUNT) X(GAME_LEAVE) \
    X(GAME_PING) X(GAME_START_PRE_COMPLETE) X(GATE_CHANNEL_JOIN) \
    X(GATE_LOBBY_INFO) X(GATE_LOBBY_JOIN) X(GATE_SERVER_INFO) \
    X(GATE_SERVER_JOIN) X(GIFT_DELETE_RECEIVE) X(GIFT_DELETE_SEND) \
    X(GIFT_LOAD_RECEIVE_PAGE) X(GIFT_LOAD_SEND_PAGE) X(GIFT_READ) \
    X(GIFT_RECEIVE) X(GIFT_WRITE) X(GM_BAN_USER) X(GM_KICK_USER) \
    X(GM_NOTICE_MESSAGE) X(GM_QUIET_USER) X(GM_WARN_MESSAGE) \
    X(HANGAR_BREAK_CUSTOM_SLOT) X(HANGAR_BREAK_WEAR) \
    X(HANGAR_BUY_CUSTOM_SLOT) X(HANGAR_CLOSE) X(HANGAR_CUSTOM_BREAK) \
    X(HANGAR_CUSTOM_BUY) X(HANGAR_CUSTOM_EQUIP) X(HANGAR_EQUIP_CUSTOM_SLOT) \
    X(HANGAR_EQUIP_WEAR) X(HANGAR_ITEM_BUY) X(HANGAR_ITEM_DELETE) \
    X(HANGAR_ITEM_EQUIP) X(HANGAR_ITEM_EXPIRE) X(HANGAR_ITEM_REPAIR) \
    X(HANGAR_ITEM_RESELL) X(HANGAR_ITEM_USE) X(HANGAR_JOIN) X(HANGAR_LEAVE) \
    X(HANGAR_MONEY) X(HANGAR_NICKNAME_CHANGE) X(HANGAR_OPEN) \
    X(HANGAR_PARTS_SLOT) X(HANGAR_RECIPE_ITEM) X(HANGAR_WEAPON_CUSTOM_SLOT) \
    X(HANGAR_WEAPON_SLOT) X(HANGAR_WEAR_SLOT) X(HOST_DAILY_MISSION_CLEAR) \
    X(HOST_GAME_BEGIN_ROUND) X(HOST_GAME_BOMB) X(HOST_GAME_CHECK_BODY) \
    X(HOST_GAME_GHOST) X(HOST_GAME_INSTANT_RESPAWN) X(HOST_GAME_LEAVE) \
    X(HOST_GAME_MUST_LOAD_COMPLETE) X(HOST_GAME_OCCUPY) X(HOST_GAME_PING) \
    X(HOST_GAME_PVE_DATANET) X(HOST_GAME_PVE_DOLL) X(HOST_GAME_PVE_HELP) \
    X(HOST_GAME_PVE_RANK) X(HOST_GAME_RESPAWN) X(HOST_GAME_RESPAWN_COMPLETE) \
    X(HOST_GAME_ROUND_END_PVE) X(HOST_GAME_START_WATCH) \
    X(HOST_GAME_SUPER_DEFUSING_KIT) X(HOST_GAME_TIMEOUT) \
    X(HOST_GAME_USER_DEATH) X(HOST_GAME_USER_EQUIP) X(HOST_GAME_USER_ITEM) \
    X(HOST_GAME_USER_KILL) X(HOST_GAME_WEAPON_CHANGE) X(HOST_GAME_ZOMBIE) \
    X(LOBBY_CLAN_ROOM_INFO) X(LOBBY_LEAVE) X(LOBBY_MODE_MAP) \
    X(LOBBY_REFRESH_CLAN_ROOM_INFO) X(LOBBY_REFRESH_ROOM_INFO) \
    X(LOBBY_ROOM_CREATE) X(LOBBY_ROOM_ENTER) X(LOBBY_ROOM_MAKE) \
    X(MY_GUIDE_CLEAR) X(MY_NICKNAME_CREATE) X(PERK_BREAK_SLOT) \
    X(PERK_CLOSE) X(PERK_EQUIP_SLOT) X(PERK_INCREASE_SLOT) X(PERK_OPEN) \
    X(PERK_OPEN_NEW) X(POST_CLEAR_RECEIVE) X(POST_CLEAR_SEND) X(POST_CLOSE) \
    X(POST_DELETE_RECEIVE) X(POST_DELETE_SEND) X(POST_LOAD_RECEIVE_PAGE) \
    X(POST_LOAD_SEND_PAGE) X(POST_OPEN) X(POST_READ) X(POST_WRITE) \
    X(PURCHASE_FINALIZE) X(PURCHASE_INIT) X(RECIPE_CLOSE) X(RECIPE_ITEM) \
    X(RECIPE_OPEN) X(RECIPE_OPEN_NEW) X(REPORT_USER) X(ROOM_DETAIL_SETTING) \
    X(ROOM_DETAIL_SETTING_EX) X(ROOM_GAME_READY) X(ROOM_GAME_START) \
    X(ROOM_INVITE) X(ROOM_INVITE_USER_INFO) X(ROOM_LEAVE) X(ROOM_MAP) \
    X(ROOM_MAP_CHANGE) X(ROOM_MATCH_CANCEL) X(ROOM_MATCH_JOIN) \
    X(ROOM_MATCH_START) X(ROOM_USER_TEAM) X(SEARCH_USER) \
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

static void TryLogString(const char* label, const void* ptr) {
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

static void TryLogWideString(const char* label, const void* ptr) {
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

static void DumpBufferRegion(const char* label, const uint8_t* buf, size_t offset, size_t len) {
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

static void DumpServerDataHotspots() {
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

static void DumpCrashDiagnostics(EXCEPTION_POINTERS* ex) {
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
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
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
            if (ex->ExceptionRecord->ExceptionCode == 0xC0000005 && loggedCount < 3 && !inHandler) {
                inHandler = true;
                loggedCount++;
                Log("[Proxy] ========================================");
                Log("[Proxy] VECTORED EXCEPTION HANDLER CAUGHT ACCESS VIOLATION (occurrence %d)!", loggedCount);
                Log("[Proxy] (The game will most likely catch and \"swallow\" this via SEH");
                Log("[Proxy] further down the chain -- this is only for our diagnostics.)");
                Log("[Proxy] ExceptionAddress: 0x%p", ex->ExceptionRecord->ExceptionAddress);
                Log("[Proxy] Access Violation! Address: 0x%p",
                    ex->ExceptionRecord->ExceptionInformation[1]);
                DumpCrashDiagnostics(ex);
                Log("[Proxy] ========================================");
                inHandler = false;
            }
            return EXCEPTION_CONTINUE_SEARCH;  // don't interfere with the game's own handling
        });

        SetUnhandledExceptionFilter([](EXCEPTION_POINTERS* ex) -> LONG {
            Log("[Proxy] ========================================");
            Log("[Proxy] UNHANDLED EXCEPTION!");
            Log("[Proxy] ExceptionCode: 0x%08X", ex->ExceptionRecord->ExceptionCode);
            Log("[Proxy] ExceptionAddress: 0x%p", ex->ExceptionRecord->ExceptionAddress);
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
        Log("[Proxy] ========================================");

        // APPLY PATCHES IMMEDIATELY ON LOAD
        // GameClient.dll has already loaded us via LoadLibrary("Game\\ZNetwork.dll"),
        // meaning by this point it's fully mapped into memory.
        // Engine.exe is also already loaded (it loaded GameClient.dll).
        ApplyAllPatches();
    } else if (reason == DLL_PROCESS_DETACH) {
        if (g_LogFile) {
            fclose(g_LogFile);
            g_LogFile = nullptr;
        }
    }
    return TRUE;
}
