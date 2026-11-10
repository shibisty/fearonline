// ============================================================================
//  fo_world.inc -- OFFLINE WORLD MODEL for the ZNetwork.dll replacement
//  (textually #included by znetwork_proxy.cpp after the callback helpers;
//   not a standalone translation unit).
//
//  2026-11-10. Everything in here reproduces, in our own memory, the data
//  contract the ORIGINAL ZNetwork.dll keeps inside the GetServerData() ("sd")
//  and GetServerHostData() ("hd") buffers, as reverse-engineered from the
//  original DLL's deserializers and GameClient.dll's readers (see
//  docs/reference_comparison_and_next_steps.md, entries of 2026-11-03/10, and
//  the RE notes they cite). The rules:
//
//   * sd/hd are the contract. A callback is only a "please re-read sd" signal
//     (all of them are cdecl; the few that take arguments are listed per
//     call site). We write the bytes the original DLL would have written,
//     then fire the same callback it fires.
//   * MSVC std::vector<T*> layouts ({begin,end,cap} of 4-byte pointers to
//     records) are reproduced with static arrays. GameClient only READS
//     them (count = (end-begin)>>2); it never frees anything, so the records
//     can live in our static storage for the process lifetime.
//   * Server answers are delivered from Tick() through a small job queue
//     (the real server is asynchronous; synchronous re-entry from inside a
//     Send_* export -- i.e. from inside the UI command dispatcher -- is what
//     produced the dispatcher crashes). If Tick() is not running (in-game),
//     jobs execute inline, same rule as the existing load-complete answer.
//   * Item catalogue = fo_catalog.h, generated from the game's own
//     GameItem/MapInfo/FunctionItem tables (d3.Arch01 == reference CSVs).
// ============================================================================

#include <cwctype>
#include "fo_catalog.h"

// ---------------------------------------------------------------------------
// Byte-level accessors into the two 64 KB buffers
// ---------------------------------------------------------------------------
#define SD32(off)  (*reinterpret_cast<uint32_t*>(g_ServerDataBuffer + (off)))
#define SDI32(off) (*reinterpret_cast<int32_t*>(g_ServerDataBuffer + (off)))
#define SD16(off)  (*reinterpret_cast<uint16_t*>(g_ServerDataBuffer + (off)))
#define SD8(off)   (*reinterpret_cast<uint8_t*>(g_ServerDataBuffer + (off)))
#define SDW(off)   (reinterpret_cast<wchar_t*>(g_ServerDataBuffer + (off)))
#define SDPTR(off) (*reinterpret_cast<void**>(g_ServerDataBuffer + (off)))
#define HD32(off)  (*reinterpret_cast<uint32_t*>(g_ServerHostDataBuffer + (off)))
#define HD8(off)   (*reinterpret_cast<uint8_t*>(g_ServerHostDataBuffer + (off)))
#define HDW(off)   (reinterpret_cast<wchar_t*>(g_ServerHostDataBuffer + (off)))
// (C-style casts on purpose: the record pointer may be const where we only read.)
#define R32(rec, off)  (*(uint32_t*)((uint8_t*)(rec) + (off)))
#define R16(rec, off)  (*(uint16_t*)((uint8_t*)(rec) + (off)))
#define R8(rec, off)   (*(uint8_t*)((uint8_t*)(rec) + (off)))
#define RW(rec, off)   ((wchar_t*)((uint8_t*)(rec) + (off)))

// Bounded wide copy (always NUL-terminates; cap = capacity in wchar_t).
static void WCopy(wchar_t* dst, size_t cap, const wchar_t* src) {
    if (!dst || cap == 0) return;
    if (!src) { dst[0] = L'\0'; return; }
    size_t n = 0;
    while (n + 1 < cap && src[n]) { dst[n] = src[n]; n++; }
    dst[n] = L'\0';
}

// SEH-guarded copy of a wide string the CLIENT handed us (Send_* argument).
ASAN_NO_INSTRUMENT static bool SafeWCopy(wchar_t* dst, size_t cap, const wchar_t* src) {
    if (!dst || cap == 0) return false;
    dst[0] = L'\0';
    if (!src) return false;
    size_t n = 0;
    __try {
        while (n + 1 < cap && src[n]) { dst[n] = src[n]; n++; }
        dst[n] = L'\0';
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        dst[0] = L'\0';
        return false;
    }
}

static void PtrVecWrite(size_t beginOff, void** ptrs, size_t count, size_t cap) {
    SDPTR(beginOff)     = ptrs;
    SDPTR(beginOff + 4) = ptrs + count;
    SDPTR(beginOff + 8) = ptrs + cap;
}

// ---------------------------------------------------------------------------
// Callback with 0..4 32-bit arguments, cdecl, ESP restored by us, SEH-wrapped.
// ---------------------------------------------------------------------------
static bool CallCallbackArgs(const char* name, int argc, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {
    void* cb = GetCallback(name);
    if (!cb) { Log("[Proxy] Callback %s is not registered; skipping", name); return false; }
    Log("[Proxy] Calling %s(argc=%d: 0x%X, 0x%X, 0x%X, 0x%X)", name, argc, a0, a1, a2, a3);
    uint32_t savedEsp = 0;
    bool returned = false;
    const LONG traceSequence = LogCallbackBegin(name, cb, nullptr, a0, a1, (uintptr_t)_ReturnAddress());
    __try {
        __asm {
            mov savedEsp, esp
            mov eax, argc
            cmp eax, 4
            jl  fo_cb_push3
            push a3
        fo_cb_push3:
            cmp eax, 3
            jl  fo_cb_push2
            push a2
        fo_cb_push2:
            cmp eax, 2
            jl  fo_cb_push1
            push a1
        fo_cb_push1:
            cmp eax, 1
            jl  fo_cb_call
            push a0
        fo_cb_call:
            call cb
            mov esp, savedEsp
        }
        returned = true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        __asm { mov esp, savedEsp }
        Log("[Proxy] EXCEPTION in CALLBACK #%ld name=\"%s\"; exception absorbed", traceSequence, name);
    }
    if (returned) LogCallbackReturn(traceSequence, name);
    return returned;
}
static bool CallCb(const char* name) { return CallCallbackNotification(name); }
static bool CallCb1(const char* name, uint32_t a) { return CallCallbackArgs(name, 1, a, 0, 0, 0); }
static bool CallCb2(const char* name, uint32_t a, uint32_t b) { return CallCallbackArgs(name, 2, a, b, 0, 0); }
static bool CallCb3(const char* name, uint32_t a, uint32_t b, uint32_t c) { return CallCallbackArgs(name, 3, a, b, c, 0); }

// ---------------------------------------------------------------------------
// Deferred "server answer" jobs, executed from Tick()
// ---------------------------------------------------------------------------
struct FoJob;
typedef void (*FoJobFn)(const FoJob& j);
struct FoJob {
    FoJobFn     fn;
    const char* tag;
    int32_t     a, b, c, d, e, f;
    wchar_t     s1[64];
    wchar_t     s2[64];
};
static const size_t kFoJobRing = 128;
static FoJob g_FoJobs[kFoJobRing];
static volatile LONG g_FoJobHead = 0;
static volatile LONG g_FoJobTail = 0;
static volatile LONG g_FoJobsRunning = 0;

static void FoRunQueuedJobs() {
    if (InterlockedCompareExchange(&g_FoJobsRunning, 1, 0) != 0) return;
    int guard = 0;
    while (g_FoJobHead != g_FoJobTail && guard++ < 512) {
        FoJob j = g_FoJobs[(size_t)g_FoJobHead % kFoJobRing];
        InterlockedIncrement(&g_FoJobHead);
        Log("[Proxy] job(%s) from Tick(): a=%d b=%d c=%d d=%d", j.tag, j.a, j.b, j.c, j.d);
        j.fn(j);
    }
    InterlockedExchange(&g_FoJobsRunning, 0);
}

static void FoPost(FoJobFn fn, const char* tag, int a = 0, int b = 0, int c = 0, int d = 0,
                   const wchar_t* s1 = nullptr, const wchar_t* s2 = nullptr, int e = 0, int f = 0) {
    FoJob j;
    memset(&j, 0, sizeof(j));
    j.fn = fn; j.tag = tag; j.a = a; j.b = b; j.c = c; j.d = d; j.e = e; j.f = f;
    WCopy(j.s1, 64, s1);
    WCopy(j.s2, 64, s2);
    const DWORD sinceTick = GetTickCount() - g_LastTickMs;
    const bool tickAlive = (g_LastTickMs != 0 && sinceTick < 1000);
    if (!tickAlive && g_FoJobsRunning == 0) {
        Log("[Proxy] job(%s): Tick() silent (%lu ms) -> running inline", tag, (unsigned long)sinceTick);
        j.fn(j);
        return;
    }
    if (g_FoJobTail - g_FoJobHead >= (LONG)kFoJobRing) {
        Log("[Proxy] job(%s): queue full -> running inline", tag);
        j.fn(j);
        return;
    }
    g_FoJobs[(size_t)g_FoJobTail % kFoJobRing] = j;
    InterlockedIncrement(&g_FoJobTail);
    Log("[Proxy] job(%s) queued for Tick()", tag);
}

// ---------------------------------------------------------------------------
// PLAYER PROFILE  (re_notes/profile.md)
// ---------------------------------------------------------------------------
struct FoProfile {
    wchar_t  nick[30];
    uint32_t userId;
    uint32_t level, grade, exp;
    uint32_t pvpGames, pvpWin, pvpLose, pvpDraw, kill, death, headshot;
    uint32_t pveComplete, pveGames, npcKill, pveDeath;
    uint32_t hitHead, hitArms, hitBody, hitLegs, hitMiss;
    uint64_t point, cash;
};
static FoProfile g_Me = {
    L"TestUser", 1,          // userId == my room user index (sd+0x000 / record+0) -- keep them equal
    10, 1, 12345,
    20, 12, 8, 0, 150, 90, 30,
    5, 8, 300, 12,
    40, 30, 120, 20, 60,
    999999ull, 999999ull
};

static void FoWriteProfile() {
    WCopy(SDW(0x004), 30, g_Me.nick);
    SD32(0x038) = g_Me.grade;             // 1..5 (NotifyUserGrade), 0 would break the icon
    SD32(0x040) = g_Me.level;
    SD32(0x048) = g_Me.userId; SD32(0x04C) = 0;
    SD32(0x050) = 0; SD32(0x054) = 0; SD32(0x058) = 0; SD32(0x05C) = 0;
    SD32(0x060) = g_Me.pvpGames; SD32(0x064) = g_Me.pvpWin; SD32(0x068) = g_Me.pvpLose; SD32(0x06C) = g_Me.pvpDraw;
    SD32(0x070) = g_Me.kill; SD32(0x074) = g_Me.death; SD32(0x078) = g_Me.headshot; SD32(0x07C) = 0;
    SD32(0x080) = g_Me.pveComplete; SD32(0x084) = g_Me.pveGames; SD32(0x088) = 0; SD32(0x08C) = 0;
    SD32(0x090) = g_Me.npcKill; SD32(0x094) = g_Me.pveDeath; SD32(0x098) = 0; SD32(0x09C) = 0;
    SD32(0x0C0) = g_Me.hitHead; SD32(0x0C4) = g_Me.hitArms; SD32(0x0C8) = g_Me.hitBody; SD32(0x0CC) = g_Me.hitLegs; SD32(0x0D0) = g_Me.hitMiss;
    SD32(0x0D4) = g_Me.level;
    SD32(0x0D8) = (g_Me.exp > 0) ? 1 : 3;
    SD32(0x0DC) = g_Me.exp;
    SD8(0x1C8) = 0; SD8(0x1C9) = 0;       // silence / adult flags
    SD8(0x259B) = 0; SD8(0x259C) = 0; SD8(0x259D) = 0;   // option bytes (LOGIN_DATA reads 0x259C/D)
    SD32(0x25A4) = 20;                     // scenario ticket max
    SD32(0x25A8) = 0;                      // ticket need time
    SD16(0x2708) = 0; SD8(0x270A) = 0;     // attendance: no popup
    Log("[Proxy] profile written: nick=%ls id=%u level=%u grade=%u exp=%u", g_Me.nick, g_Me.userId, g_Me.level, g_Me.grade, g_Me.exp);
}

static void FoWriteMoney() {
    SD32(0x120) = (uint32_t)(g_Me.point & 0xFFFFFFFFu); SD32(0x124) = (uint32_t)(g_Me.point >> 32);
    SD32(0x128) = (uint32_t)(g_Me.cash  & 0xFFFFFFFFu); SD32(0x12C) = (uint32_t)(g_Me.cash  >> 32);
}

// SEARCH_USER_INFO block = NotifyCharacter layout shifted (nick +0x210, rest +0x208).
static void FoWriteSearchBlock(const wchar_t* nick) {
    WCopy(SDW(0x214), 26, nick);
    SD32(0x248) = 7;                      // level
    SD32(0x250) = 2002; SD32(0x254) = 0;  // id
    SD32(0x258) = 0; SD32(0x25C) = 0; SD32(0x260) = 0; SD32(0x264) = 0;
    SD32(0x268) = 30; SD32(0x26C) = 15; SD32(0x270) = 15; SD32(0x274) = 0;
    SD32(0x278) = 200; SD32(0x27C) = 180; SD32(0x280) = 40; SD32(0x284) = 0;
    SD32(0x288) = 2; SD32(0x28C) = 4; SD32(0x290) = 0; SD32(0x294) = 0;
    SD32(0x298) = 100; SD32(0x29C) = 9; SD32(0x2A0) = 0; SD32(0x2A4) = 0;
    SD32(0x2C8) = 50; SD32(0x2CC) = 40; SD32(0x2D0) = 150; SD32(0x2D4) = 30; SD32(0x2D8) = 90;
    SD32(0x2DC) = 7; SD32(0x2E0) = 1; SD32(0x2E4) = 4321;
    SD32(0x2E8) = 0; SD32(0x2EC) = 0; SD32(0x2F0) = 0; SD32(0x2F4) = 0;
    SDW(0x2F8)[0] = L'\0';
    SD8(0x318) = 0; SD32(0x31C) = 0; SD8(0x320) = 0; SD8(0x321) = 0;
    SD8(0x324) = 0;                       // no clan
    SD8(0x325) = 0;
}

// ---------------------------------------------------------------------------
// INVENTORY / HANGAR  (re_notes/items_hangar.md)
// ---------------------------------------------------------------------------
static const size_t kFoMaxItems = 2048;
static const size_t kFoMaxParts = 512;
static uint8_t  g_FoItemRecs[kFoMaxItems][0x84];
static void*    g_FoItemPtrs[kFoMaxItems];
static size_t   g_FoItemCount = 0;
static uint8_t  g_FoPartRecs[kFoMaxParts][0x18];
static void*    g_FoPartPtrs[kFoMaxParts];
static size_t   g_FoPartCount = 0;
static uint32_t g_FoNextSerial = 1;        // u16 on the wire
static bool     g_FoInventoryBuilt = false;
// Equipped serials per slot ordinal of the sd+0x130 table:
//   0 = char ATC (0x0B), 1 = char TF (0x0C), 2..6 = weapon slots 0x15..0x19.
static uint32_t g_FoEquipped[7] = { 0, 0, 0, 0, 0, 0, 0 };

static const FoCatItem* FoCatFind(uint32_t itemIndex) {
    for (size_t i = 0; i < g_FoCatalogCount; i++) if (g_FoCatalog[i].itemIndex == itemIndex) return &g_FoCatalog[i];
    return nullptr;
}
static bool FoTypeIsMirrored(uint8_t t) {
    return t == 0x0B || t == 0x0C || (t >= 0x15 && t <= 0x19) || (t >= 0x1F && t <= 0x22) ||
           (t >= 0x2A && t <= 0x2D) || t == 0x30 || t == 0x31 || t == 0x51;
}
static bool FoTypeIsCustomPart(uint8_t t) { return t >= 0x3D && t <= 0x42; }
static int  FoEquipOrdinal(uint8_t t) {
    if (t == 0x0B) return 0;
    if (t == 0x0C) return 1;
    if (t >= 0x15 && t <= 0x19) return 2 + (t - 0x15);
    return -1;
}
static uint8_t* FoItemBySerial(uint32_t serial) {
    for (size_t i = 0; i < g_FoItemCount; i++) if (R32(g_FoItemRecs[i], 0) == serial) return g_FoItemRecs[i];
    return nullptr;
}
static uint8_t* FoPartBySerial(uint32_t serial) {
    for (size_t i = 0; i < g_FoPartCount; i++) if (R32(g_FoPartRecs[i], 0) == serial) return g_FoPartRecs[i];
    return nullptr;
}
static const FoCatItem* FoCatOfRec(const uint8_t* rec) { return rec ? FoCatFind(R32(rec, 4)) : nullptr; }

static uint8_t* FoAddItem(const FoCatItem* c, uint32_t value) {
    if (!c || g_FoItemCount >= kFoMaxItems) return nullptr;
    uint8_t* rec = g_FoItemRecs[g_FoItemCount];
    memset(rec, 0, 0x84);
    const uint32_t serial = g_FoNextSerial++;
    R32(rec, 0x00) = serial;
    R32(rec, 0x04) = c->itemIndex;
    R32(rec, 0x08) = value; R32(rec, 0x0C) = value; R32(rec, 0x10) = value;
    R8(rec, 0x14)  = (c->itemType == 0x0B || c->itemType == 0x0C) ? (c->slotCount ? c->slotCount : 3) : 0;
    R8(rec, 0x15)  = 1;    // active (state 1)
    R8(rec, 0x18)  = 0;    // not expired
    g_FoItemPtrs[g_FoItemCount] = rec;
    g_FoItemCount++;
    return rec;
}
static uint8_t* FoAddPart(const FoCatItem* c, uint32_t parentSerial, uint32_t value, bool mounted) {
    if (!c || g_FoPartCount >= kFoMaxParts) return nullptr;
    uint8_t* rec = g_FoPartRecs[g_FoPartCount];
    memset(rec, 0, 0x18);
    R32(rec, 0x00) = g_FoNextSerial++;
    R32(rec, 0x04) = parentSerial;
    R32(rec, 0x08) = c->itemIndex;
    R32(rec, 0x0C) = (uint32_t)(c->itemType - 0x3D + 1);   // type ordinal 1..6
    R32(rec, 0x10) = value;
    R8(rec, 0x14)  = 1;                                     // have
    R8(rec, 0x15)  = mounted ? 1 : 0;
    g_FoPartPtrs[g_FoPartCount] = rec;
    g_FoPartCount++;
    return rec;
}
static void FoRemoveItem(uint32_t serial) {
    for (size_t i = 0; i < g_FoItemCount; i++) {
        if (R32(g_FoItemRecs[i], 0) != serial) continue;
        for (size_t k = i + 1; k < g_FoItemCount; k++) memcpy(g_FoItemRecs[k - 1], g_FoItemRecs[k], 0x84);
        g_FoItemCount--;
        memset(g_FoItemRecs[g_FoItemCount], 0, 0x84);
        for (int o = 0; o < 7; o++) if (g_FoEquipped[o] == serial) g_FoEquipped[o] = 0;
        return;
    }
}
static void FoUnmountSameSlot(uint32_t parentSerial, uint32_t ordinal, const uint8_t* except) {
    for (size_t i = 0; i < g_FoPartCount; i++) {
        uint8_t* p = g_FoPartRecs[i];
        if (p != except && R32(p, 4) == parentSerial && R32(p, 0xC) == ordinal) R8(p, 0x15) = 0;
    }
}

static void FoRecomputeConsumables() {
    uint32_t respawn = 0, defuse = 0, pveTicket = 0;
    for (size_t i = 0; i < g_FoItemCount; i++) {
        const FoCatItem* c = FoCatOfRec(g_FoItemRecs[i]);
        if (!c) continue;
        const uint32_t v = R32(g_FoItemRecs[i], 8);
        if (c->functionType == 11) respawn += v;
        else if (c->functionType == 12) defuse += v;
        else if (c->functionType == 17) pveTicket += v;
    }
    SD32(0xEE0) = respawn; SD32(0xEE4) = defuse; SD32(0x25A0) = pveTicket;
}

static void FoWriteEquipTable() {
    // ordinals 0..6 -> sd+0x130 + n*8 {serial, itemIndex}; wear 7..10 zero; custom 11..16 from the primary weapon
    for (int o = 0; o < 17; o++) { SD32(0x130 + o * 8) = 0; SD32(0x134 + o * 8) = 0; }
    for (int o = 0; o < 7; o++) {
        const uint8_t* rec = g_FoEquipped[o] ? FoItemBySerial(g_FoEquipped[o]) : nullptr;
        if (!rec) continue;
        SD32(0x130 + o * 8) = R32(rec, 0); SD32(0x134 + o * 8) = R32(rec, 4);
    }
    const uint32_t primary = g_FoEquipped[2];
    if (primary) {
        for (size_t i = 0; i < g_FoPartCount; i++) {
            const uint8_t* p = g_FoPartRecs[i];
            if (R32(p, 4) != primary || R8(p, 0x15) == 0) continue;
            const uint32_t ord = R32(p, 0xC);
            if (ord < 1 || ord > 6) continue;
            SD32(0x130 + (10 + ord) * 8) = R32(p, 0); SD32(0x134 + (10 + ord) * 8) = R32(p, 8);
        }
    }
}

static void FoSyncInventoryToSd() {
    PtrVecWrite(0x26C0, g_FoItemPtrs, g_FoItemCount, kFoMaxItems);
    PtrVecWrite(0x26D0, g_FoPartPtrs, g_FoPartCount, kFoMaxParts);
    FoWriteEquipTable();
    FoRecomputeConsumables();
    FoWriteMoney();
}

static void FoBuildDefaultInventory() {
    if (g_FoInventoryBuilt) return;
    g_FoInventoryBuilt = true;
    size_t items = 0, parts = 0, skipped = 0;
    for (size_t i = 0; i < g_FoCatalogCount; i++) {
        const FoCatItem& c = g_FoCatalog[i];
        if (!c.own) continue;
        if (FoTypeIsMirrored(c.itemType)) {
            uint8_t* rec = FoAddItem(&c, c.defaultValue);
            if (!rec) { skipped++; continue; }
            items++;
            const int o = FoEquipOrdinal(c.itemType);
            if (o >= 0 && g_FoEquipped[o] == 0) g_FoEquipped[o] = R32(rec, 0);
        } else if (FoTypeIsCustomPart(c.itemType)) {
            if (FoAddPart(&c, 0, c.defaultValue, false)) parts++; else skipped++;
        } else {
            skipped++;
        }
    }
    // Prefer the real starters as the equipped set (catalog.go StarterItems): Benedict / Jack Conrad / M16.
    const uint32_t starters[3] = { 11100101u, 12100101u, 21102201u };
    for (int s = 0; s < 3; s++) {
        for (size_t i = 0; i < g_FoItemCount; i++) {
            if (R32(g_FoItemRecs[i], 4) != starters[s]) continue;
            const FoCatItem* c = FoCatOfRec(g_FoItemRecs[i]);
            const int o = c ? FoEquipOrdinal(c->itemType) : -1;
            if (o >= 0) g_FoEquipped[o] = R32(g_FoItemRecs[i], 0);
        }
    }
    FoSyncInventoryToSd();
    Log("[Proxy] default inventory built: %u item records, %u custom-part records, %u catalogue rows skipped; equipped ATC=%u TF=%u W1..5=%u/%u/%u/%u/%u",
        (unsigned)items, (unsigned)parts, (unsigned)skipped,
        g_FoEquipped[0], g_FoEquipped[1], g_FoEquipped[2], g_FoEquipped[3], g_FoEquipped[4], g_FoEquipped[5], g_FoEquipped[6]);
}

// ---------------------------------------------------------------------------
// LOBBY USERS (LOBBY_USER_INFO vector sd+0x2600, 0x40-byte records)
// ---------------------------------------------------------------------------
struct FoFakeUser { uint32_t id; const wchar_t* name; uint32_t level; };
static const FoFakeUser g_FoFakeUsers[] = {
    { 2002, L"Alpha_Wolf", 7 }, { 2003, L"Sgt_Reaper", 23 }, { 2004, L"Nightfall", 15 }
};
static const size_t kFoFakeUsers = sizeof(g_FoFakeUsers) / sizeof(g_FoFakeUsers[0]);
static uint8_t g_FoLobbyUserRecs[8][0x40];
static void*   g_FoLobbyUserPtrs[8];

static void FoWriteLobbyUsers() {
    size_t n = 0;
    uint8_t* me = g_FoLobbyUserRecs[n]; memset(me, 0, 0x40);
    R32(me, 0x00) = g_Me.userId; R32(me, 0x04) = g_Me.level; R8(me, 0x18) = 0; WCopy(RW(me, 0x1A), 17, g_Me.nick); R32(me, 0x3C) = 0;
    g_FoLobbyUserPtrs[n] = me; n++;
    for (size_t i = 0; i < kFoFakeUsers && n < 8; i++) {
        uint8_t* r = g_FoLobbyUserRecs[n]; memset(r, 0, 0x40);
        R32(r, 0x00) = g_FoFakeUsers[i].id; R32(r, 0x04) = g_FoFakeUsers[i].level; WCopy(RW(r, 0x1A), 17, g_FoFakeUsers[i].name);
        g_FoLobbyUserPtrs[n] = r; n++;
    }
    PtrVecWrite(0x2600, g_FoLobbyUserPtrs, n, 8);
}

// ---------------------------------------------------------------------------
// ROOMS / MAPS  (re_notes/room_maps.md)
// ---------------------------------------------------------------------------
struct FoRoom {
    bool     active;
    uint32_t number, type;
    wchar_t  title[64];
    wchar_t  password[16];
    wchar_t  leader[16];
    uint32_t leaderId;
    uint32_t mapIndex, maxUsers, curUsers, goalKill, goalRound, goalTime, mode;
    uint8_t  pub, state, teamBal, instruction, deathChat, perkUse, thirdView;
};
static FoRoom   g_FoRoom;                 // the room I am in (when active)
static FoRoom   g_FoLobbyRooms[4];        // rooms advertised in the lobby list (joinable fakes)
static size_t   g_FoLobbyRoomCount = 0;
static uint8_t  g_FoRoomListRecs[8][0x118];
static void*    g_FoRoomListPtrs[8];
static uint16_t g_FoOpenMaps[64];
static size_t   g_FoOpenMapCount = 0;
static int      g_FoMyTeam = 1;
static volatile LONG g_FoRoomSeq = 0;
static volatile LONG g_FoMatchActive = 0;   // set by Send_ROOM_GAME_START, cleared on GAME_LEAVE/ROOM_LEAVE
static DWORD    g_FoLastCreateTick = 0;

static const FoCatMap* FoMapFind(uint32_t mapIndex) {
    for (size_t i = 0; i < g_FoMapCount; i++) if (g_FoMaps[i].mapIndex == mapIndex) return &g_FoMaps[i];
    return nullptr;
}
static uint8_t FoMapModeRemapped(uint32_t mapIndex) {
    const FoCatMap* m = FoMapFind(mapIndex);
    if (!m) return 5;
    return (m->mapMode == 4) ? 7 : m->mapMode;   // client remaps Runaway(4) -> 7
}
static void FoRoomDefaultsFromMap(FoRoom& r, uint32_t mapIndex) {
    const FoCatMap* m = FoMapFind(mapIndex);
    r.mapIndex  = mapIndex;
    r.maxUsers  = m && m->userDefault ? m->userDefault : 16;
    r.goalKill  = m ? m->killDefault : 100;
    r.goalRound = m ? m->roundDefault : 1;
    r.goalTime  = m ? m->timeDefault : 10;
    r.mode      = FoMapModeRemapped(mapIndex);
}
static void FoInitRoom(FoRoom& r, uint32_t number, const wchar_t* title, const wchar_t* leader, uint32_t leaderId, uint32_t mapIndex) {
    memset(&r, 0, sizeof(r));
    r.active = true; r.number = number; r.type = 0;
    WCopy(r.title, 64, title); r.password[0] = L'\0'; WCopy(r.leader, 16, leader); r.leaderId = leaderId;
    FoRoomDefaultsFromMap(r, mapIndex);
    r.curUsers = 1; r.pub = 1; r.state = 0; r.teamBal = 0; r.instruction = 0; r.deathChat = 1; r.perkUse = 1; r.thirdView = 0;
}

static void FoWriteOpenMaps() {
    g_FoOpenMapCount = 0;
    for (size_t i = 0; i < g_FoMapCount && g_FoOpenMapCount < 64; i++) g_FoOpenMaps[g_FoOpenMapCount++] = g_FoMaps[i].mapIndex;
    SDPTR(0x2690) = g_FoOpenMaps;
    SDPTR(0x2694) = g_FoOpenMaps + g_FoOpenMapCount;
    SDPTR(0x2698) = g_FoOpenMaps + 64;
}

// sd room block (+0x374..) and its hd mirror (sd_off - 0x160), exactly as FUN_10087df0 / FUN_10087fa0.
static void FoWriteRoomBlock(const FoRoom& r) {
    SD32(0x374) = r.number; SD32(0x378) = r.type;
    WCopy(SDW(0x37C), 64, r.title);
    WCopy(SDW(0x3FC), 16, r.password);
    WCopy(SDW(0x41C), 16, r.leader);
    SD8(0x43C) = r.pub; if (!g_FoMatchActive) SD8(0x43D) = r.state;
    SD32(0x440) = r.mapIndex; SD32(0x444) = r.curUsers; SD32(0x448) = r.maxUsers; SD32(0x44C) = r.leaderId;
    if (!g_FoMatchActive) { SD32(0x450) = 0; SD32(0x454) = 0; }   // +0x450 = host index once a match starts (Send_ROOM_GAME_START)
    SD8(0x45C) = r.teamBal; SD8(0x45D) = r.instruction; SD8(0x45E) = r.deathChat; SD8(0x45F) = r.perkUse; SD8(0x460) = r.thirdView;
    SD32(0x464) = r.mapIndex; SD32(0x468) = r.goalRound; SD32(0x46C) = r.goalKill; SD32(0x470) = 0; SD32(0x474) = r.goalTime; SD32(0x478) = 0;
    // hd mirror
    HD32(0x214) = r.number; HD32(0x218) = r.type;
    WCopy(HDW(0x21C), 64, r.title); WCopy(HDW(0x29C), 16, r.password); WCopy(HDW(0x2BC), 16, r.leader);
    HD8(0x2DC) = r.pub; HD8(0x2DD) = r.state; HD32(0x2E0) = r.mapIndex; HD32(0x2E4) = r.curUsers; HD32(0x2E8) = r.maxUsers; HD32(0x2EC) = r.leaderId;
    HD32(0x2F0) = 0; HD32(0x2F4) = 0;
    HD8(0x2FC) = r.teamBal; HD8(0x2FD) = r.instruction; HD8(0x2FE) = r.deathChat; HD8(0x2FF) = r.perkUse; HD8(0x300) = r.thirdView;
    HD32(0x304) = r.mapIndex; HD32(0x308) = r.goalRound; HD32(0x30C) = r.goalKill; HD32(0x310) = 0; HD32(0x314) = r.goalTime; HD32(0x318) = 0;
}

static void FoFillRoomListRec(uint8_t* rec, const FoRoom& r) {
    memset(rec, 0, 0x118);
    R32(rec, 0x00) = r.number; R32(rec, 0x04) = r.type;
    WCopy(RW(rec, 0x08), 64, r.title);
    WCopy(RW(rec, 0xA8), 16, r.leader);
    R8(rec, 0xC8) = r.pub; R8(rec, 0xC9) = r.state;
    R32(rec, 0xCC) = r.mapIndex; R32(rec, 0xD0) = r.curUsers; R32(rec, 0xD4) = r.maxUsers; R32(rec, 0xD8) = r.leaderId;
    R32(rec, 0xE4) = 30;   // ping
    R8(rec, 0xE8) = r.teamBal; R8(rec, 0xE9) = r.instruction; R8(rec, 0xEA) = r.deathChat; R8(rec, 0xEB) = r.perkUse; R8(rec, 0xEC) = r.thirdView;
    R32(rec, 0xF0) = r.mapIndex; R32(rec, 0xF4) = r.goalRound; R32(rec, 0xF8) = r.goalKill; R32(rec, 0xFC) = 0; R32(rec, 0x100) = r.goalTime; R32(rec, 0x104) = 0;
}

static void FoWriteRoomList() {
    size_t n = 0;
    if (g_FoRoom.active) { FoFillRoomListRec(g_FoRoomListRecs[n], g_FoRoom); g_FoRoomListPtrs[n] = g_FoRoomListRecs[n]; n++; }
    for (size_t i = 0; i < g_FoLobbyRoomCount && n < 8; i++) {
        if (g_FoRoom.active && g_FoLobbyRooms[i].number == g_FoRoom.number) continue;
        FoFillRoomListRec(g_FoRoomListRecs[n], g_FoLobbyRooms[i]); g_FoRoomListPtrs[n] = g_FoRoomListRecs[n]; n++;
    }
    PtrVecWrite(0x2650, g_FoRoomListPtrs, n, 8);
    PtrVecWrite(0x2660, g_FoRoomListPtrs, 0, 0);     // clan rooms: none
}

static void FoInitLobbyRooms() {
    g_FoLobbyRoomCount = 0;
    FoInitRoom(g_FoLobbyRooms[g_FoLobbyRoomCount++], 11, L"Killhouse TDM (join me)", L"Alpha_Wolf", 2002, 518);
    FoInitRoom(g_FoLobbyRooms[g_FoLobbyRoomCount++], 12, L"PentHouse Demolition",    L"Sgt_Reaper", 2003, 204);
    FoInitRoom(g_FoLobbyRooms[g_FoLobbyRoomCount++], 13, L"Downfall knives only",    L"Nightfall",  2004, 321);
    g_FoLobbyRooms[1].curUsers = 3; g_FoLobbyRooms[2].curUsers = 2; g_FoLobbyRooms[2].state = 1;
}

// One occupied slot (me) in the 20-entry room user table at sd+0x48C (FUN_10088290 layout).
static void FoWriteRoomUserRecord() {
    uint8_t* rec = g_ServerDataBuffer + 0x48C;
    memset(rec, 0, 0x74);
    R32(rec, 0x00) = 1;                   // user index (== sd+0x000)
    R8(rec, 0x04)  = 1;                   // occupied
    R32(rec, 0x08) = 0;                   // state
    R32(rec, 0x0C) = 0;                   // 2 would mean "left"
    R32(rec, 0x10) = (uint32_t)g_FoMyTeam;
    R32(rec, 0x14) = g_Me.level;
    WCopy(RW(rec, 0x18), 16, g_Me.nick);
    RW(rec, 0x48)[0] = L'\0';             // clan name
    R8(rec, 0x70) = 0;
    SDI32(0x3C) = g_FoMyTeam;
    SDI32(0x000) = 1;                     // "is me" index
    SDI32(0x44C) = g_FoRoom.active ? g_FoRoom.leaderId : 1;
}

// ---------------------------------------------------------------------------
// CHAT  (re_notes/chat_social.md): MSVC10 std::deque<ChatInfo*> at sd+0x26DC
// ---------------------------------------------------------------------------
static const size_t kFoChatMapSize = 128;        // blocks of 4 pointers -> 512 slots
static const size_t kFoChatRecs    = 512;
static void*   g_FoChatBlocks[kFoChatMapSize][4];
static void*   g_FoChatMap[kFoChatMapSize];
static uint8_t g_FoChatRecs[kFoChatRecs][0x1B8];
static size_t  g_FoChatNextRec = 0;
static bool    g_FoChatInit = false;

static void FoChatInit() {
    for (size_t i = 0; i < kFoChatMapSize; i++) g_FoChatMap[i] = g_FoChatBlocks[i];
    SDPTR(0x26DC) = nullptr;
    SDPTR(0x26E0) = g_FoChatMap;
    SD32(0x26E4) = (uint32_t)kFoChatMapSize;
    SD32(0x26E8) = 0;
    SD32(0x26EC) = 0;
    g_FoChatInit = true;
}
static void FoChatSync() {
    if (!g_FoChatInit) FoChatInit();
    SDPTR(0x26E0) = g_FoChatMap;
    SD32(0x26E4) = (uint32_t)kFoChatMapSize;
}
// type: 0 = normal line, 4 = clan line (original clan-chat path), 6 = GM line (name "[GM]").
static void FoChatPush(uint32_t type, const wchar_t* name, const wchar_t* text, uint8_t flag) {
    FoChatSync();
    uint8_t* rec = g_FoChatRecs[g_FoChatNextRec % kFoChatRecs];
    g_FoChatNextRec++;
    memset(rec, 0, 0x1B8);
    R32(rec, 0) = type;
    WCopy(RW(rec, 0x004), 16, name);
    WCopy(RW(rec, 0x024), 200, text);
    R8(rec, 0x1B4) = flag;
    uint32_t offset = SD32(0x26E8), size = SD32(0x26EC);
    const uint32_t L = offset + size;
    uint32_t blk = L >> 2;
    if (blk >= kFoChatMapSize) blk -= (uint32_t)kFoChatMapSize;
    g_FoChatBlocks[blk][L & 3] = rec;
    size++;
    if (size > 500) {                     // trim like the original (keeps <= 500)
        offset++;
        if (offset >= kFoChatMapSize * 4) offset = 0;
        size--;
    }
    SD32(0x26E8) = offset;
    SD32(0x26EC) = size;
}
static void FoNotice(const wchar_t* text) {
    WCopy(SDW(0x2278), 200, text);
    WCopy(SDW(0x2408), 200, text);        // the dedicated notice RMI writes here; both kept in sync
}

// ---------------------------------------------------------------------------
// FRIENDS / MAIL / GIFTS
// ---------------------------------------------------------------------------
static uint8_t g_FoFriendRecs[16][0x80];  static void* g_FoFriendPtrs[16];  static size_t g_FoFriendCount = 0;
static uint8_t g_FoFReqRecs[8][0x80];     static void* g_FoFReqPtrs[8];     static size_t g_FoFReqCount = 0;
static uint8_t g_FoMailRecvRecs[8][0x228]; static void* g_FoMailRecvPtrs[8]; static size_t g_FoMailRecvCount = 0;
static uint8_t g_FoMailSendRecs[8][0x228]; static void* g_FoMailSendPtrs[8]; static size_t g_FoMailSendCount = 0;
static uint8_t g_FoGiftRecvRecs[8][0x208]; static void* g_FoGiftRecvPtrs[8]; static size_t g_FoGiftRecvCount = 0;
static uint8_t g_FoGiftSendRecs[8][0x208]; static void* g_FoGiftSendPtrs[8]; static size_t g_FoGiftSendCount = 0;
static uint8_t g_FoInviteUserRecs[8][0x9C]; static void* g_FoInviteUserPtrs[8];
static uint32_t g_FoNextMailId = 100;

static void FoFriendRecFill(uint8_t* r, uint32_t id, const wchar_t* name, uint32_t level, bool online) {
    memset(r, 0, 0x80);
    R32(r, 0x00) = id; R32(r, 0x04) = 0; WCopy(RW(r, 0x08), 16, name); R32(r, 0x28) = level;
    R8(r, 0x64) = 0; R32(r, 0x6C) = 0; R32(r, 0x70) = 0; R32(r, 0x74) = 0; R8(r, 0x78) = online ? 1 : 0; R8(r, 0x79) = 0;
}
static void FoSyncSocialVectors() {
    PtrVecWrite(0x25C0, g_FoFriendPtrs, g_FoFriendCount, 16);
    PtrVecWrite(0x25B0, g_FoFReqPtrs, g_FoFReqCount, 8);
    PtrVecWrite(0x2710, g_FoMailSendPtrs, g_FoMailSendCount, 8);
    PtrVecWrite(0x2720, g_FoMailRecvPtrs, g_FoMailRecvCount, 8);
    PtrVecWrite(0x2730, g_FoGiftSendPtrs, g_FoGiftSendCount, 8);
    PtrVecWrite(0x2740, g_FoGiftRecvPtrs, g_FoGiftRecvCount, 8);
}
static bool FoFriendAdd(uint32_t id, const wchar_t* name, uint32_t level, bool online) {
    for (size_t i = 0; i < g_FoFriendCount; i++) if (R32(g_FoFriendRecs[i], 0) == id) { FoFriendRecFill(g_FoFriendRecs[i], id, name, level, online); return true; }
    if (g_FoFriendCount >= 16) return false;
    FoFriendRecFill(g_FoFriendRecs[g_FoFriendCount], id, name, level, online);
    g_FoFriendPtrs[g_FoFriendCount] = g_FoFriendRecs[g_FoFriendCount]; g_FoFriendCount++;
    return true;
}
static void FoFriendRemove(uint32_t id) {
    for (size_t i = 0; i < g_FoFriendCount; i++) {
        if (R32(g_FoFriendRecs[i], 0) != id) continue;
        for (size_t k = i + 1; k < g_FoFriendCount; k++) memcpy(g_FoFriendRecs[k - 1], g_FoFriendRecs[k], 0x80);
        g_FoFriendCount--; return;
    }
}
static bool FoFReqAdd(uint32_t id, const wchar_t* name, uint32_t level) {
    for (size_t i = 0; i < g_FoFReqCount; i++) if (R32(g_FoFReqRecs[i], 0) == id) return true;
    if (g_FoFReqCount >= 8) return false;
    uint8_t* r = g_FoFReqRecs[g_FoFReqCount]; memset(r, 0, 0x80);
    R32(r, 0) = id; WCopy(RW(r, 8), 16, name); R32(r, 0x28) = level;
    g_FoFReqPtrs[g_FoFReqCount] = r; g_FoFReqCount++;
    return true;
}
static bool FoFReqTake(uint32_t id, wchar_t* nameOut, uint32_t* levelOut) {
    for (size_t i = 0; i < g_FoFReqCount; i++) {
        if (R32(g_FoFReqRecs[i], 0) != id) continue;
        if (nameOut) WCopy(nameOut, 16, RW(g_FoFReqRecs[i], 8));
        if (levelOut) *levelOut = R32(g_FoFReqRecs[i], 0x28);
        for (size_t k = i + 1; k < g_FoFReqCount; k++) memcpy(g_FoFReqRecs[k - 1], g_FoFReqRecs[k], 0x80);
        g_FoFReqCount--; return true;
    }
    return false;
}
static void FoMailFill(uint8_t* r, uint32_t id, const wchar_t* from, const wchar_t* to, const wchar_t* subject, const wchar_t* body, bool read) {
    memset(r, 0, 0x228);
    R32(r, 0x000) = id; R32(r, 0x004) = 0;
    WCopy(RW(r, 0x008), 16, from); WCopy(RW(r, 0x028), 16, to); WCopy(RW(r, 0x048), 21, subject); WCopy(RW(r, 0x072), 200, body);
    SYSTEMTIME st; GetLocalTime(&st);
    R32(r, 0x204) = st.wYear; R32(r, 0x208) = st.wMonth; R32(r, 0x20C) = st.wDay; R32(r, 0x210) = st.wHour; R32(r, 0x214) = st.wMinute; R32(r, 0x218) = st.wSecond;
    R32(r, 0x21C) = 0; R32(r, 0x220) = 0; R8(r, 0x224) = read ? 1 : 0;
}
static void FoSeedSocial() {
    g_FoFriendCount = 0; g_FoFReqCount = 0; g_FoMailRecvCount = 0; g_FoMailSendCount = 0; g_FoGiftRecvCount = 0; g_FoGiftSendCount = 0;
    FoFriendAdd(2002, L"Alpha_Wolf", 7, true);
    FoFriendAdd(2003, L"Sgt_Reaper", 23, true);
    FoFriendAdd(2005, L"Ghost_Rider", 31, false);
    FoFReqAdd(2004, L"Nightfall", 15);
    FoMailFill(g_FoMailRecvRecs[0], g_FoNextMailId++, L"Alpha_Wolf", g_Me.nick, L"Welcome", L"Offline mode is live. Everything you see here is served by the local ZNetwork stub.", false);
    g_FoMailRecvPtrs[0] = g_FoMailRecvRecs[0]; g_FoMailRecvCount = 1;
    FoMailFill(g_FoMailRecvRecs[1], g_FoNextMailId++, L"Sgt_Reaper", g_Me.nick, L"Clan test", L"Create a clan, invite, kick, change marks - all answered locally.", true);
    g_FoMailRecvPtrs[1] = g_FoMailRecvRecs[1]; g_FoMailRecvCount = 2;
    SD32(0x1E38) = 1; SD32(0x1E3C) = 0; SD32(0x1E40) = 0; SD32(0x1E44) = 0;
    FoSyncSocialVectors();
}

// ---------------------------------------------------------------------------
// CLAN  (re_notes/clan.md)
// ---------------------------------------------------------------------------
struct FoClanMember { uint32_t id; wchar_t name[16]; uint32_t level; uint32_t position; bool online; bool inGame; };
struct FoClan {
    bool     has;
    uint32_t id;
    wchar_t  name[16];
    wchar_t  master[16];
    uint32_t marks[4];
    uint32_t level, rank, score;
    wchar_t  notice[200];
    wchar_t  intro[206];
    uint32_t capacity;
    FoClanMember members[16];
    size_t   memberCount;
    FoClanMember applicants[8];
    size_t   applicantCount;
};
static FoClan   g_FoClan;
static uint8_t  g_FoClanMemberRecs[16][0xB8];  static void* g_FoClanMemberPtrs[16];
static uint8_t  g_FoClanAppRecs[8][0x1FC];     static void* g_FoClanAppPtrs[8];
static uint8_t  g_FoClanSearchRecs[4][0x3E8];  static void* g_FoClanSearchPtrs[4]; static size_t g_FoClanSearchCount = 0;
static uint32_t g_FoNextClanUserId = 3001;

static uint32_t FoClanIdFromName(const wchar_t* name) {   // Go: crc32 of the lower-cased name; any stable hash is fine offline
    uint32_t h = 2166136261u;
    for (const wchar_t* p = name; p && *p; ++p) { h ^= (uint32_t)towlower(*p); h *= 16777619u; }
    return h ? h : 1;
}
static size_t FoClanCountPos(uint32_t pos) {
    size_t n = 0; for (size_t i = 0; i < g_FoClan.memberCount; i++) if (g_FoClan.members[i].position == pos) n++; return n;
}
static void FoClanWriteInfoBlock() {
    const FoClan& c = g_FoClan;
    if (!c.has) {
        SD8(0x118) = 0; SD32(0xE0) = 0; SD32(0xE4) = 0; SD32(0xE8) = 0; SD32(0xEC) = 0;
        memset(g_ServerDataBuffer + 0xF0, 0, 0x20);
        SD8(0x110) = 0; SD32(0x114) = 0;
        memset(g_ServerDataBuffer + 0x1E48, 0, 1000);
        memset(g_ServerDataBuffer + 0x2230, 0, 0x48);
        return;
    }
    SD8(0x118) = 1;
    for (int i = 0; i < 4; i++) { SD32(0xE0 + i * 4) = c.marks[i]; SD32(0x1E90 + i * 4) = c.marks[i]; }
    WCopy(SDW(0xF0), 16, c.name);
    SD8(0x110) = (uint8_t)c.level; SD32(0x114) = c.rank;
    SD32(0x1E48) = c.id;
    WCopy(SDW(0x1E4C), 16, c.name);
    WCopy(SDW(0x1E6C), 16, c.master);
    SD32(0x1E8C) = 0;
    SD32(0x1EA0) = c.level;
    SD32(0x1EA8) = c.score; SD32(0x1EAC) = 0;
    SD32(0x1EB0) = c.rank; SD32(0x1EB4) = c.rank ? 3 : 0; SD32(0x1EB8) = 0;
    SD32(0x1EBC) = 0; SD32(0x1EC0) = 1;
    SD32(0x1EC4) = 3; SD32(0x1EC8) = 1; SD32(0x1ECC) = 2;         // win/draw/lose-like
    SD32(0x1ED0) = (uint32_t)c.memberCount;
    SD32(0x1ED4) = (uint32_t)FoClanCountPos(2);
    SD32(0x1ED8) = (uint32_t)FoClanCountPos(8);
    SD32(0x1EDC) = (uint32_t)FoClanCountPos(9);
    SD32(0x1EE0) = c.capacity;
    SD32(0x1EE4) = 0;
    WCopy(SDW(0x1EE8), 200, c.notice);
    WCopy(SDW(0x2078), 206, c.intro);
    SYSTEMTIME st; GetLocalTime(&st);
    SD32(0x2214) = st.wDay; SD32(0x2218) = st.wMonth; SD32(0x221C) = st.wYear;
    memset(g_ServerDataBuffer + 0x2230, 0, 0x48);      // no pending request once in a clan
}
static void FoClanWriteMembers() {
    for (size_t i = 0; i < g_FoClan.memberCount; i++) {
        const FoClanMember& m = g_FoClan.members[i];
        uint8_t* r = g_FoClanMemberRecs[i]; memset(r, 0, 0xB8);
        R32(r, 0x00) = m.id; WCopy(RW(r, 0x04), 16, m.name); R32(r, 0x24) = m.level;
        R32(r, 0x64) = 0; R32(r, 0x68) = 0; R32(r, 0x6C) = 0;
        R32(r, 0x70) = 12; R32(r, 0x74) = 8; R32(r, 0x78) = 0; R32(r, 0x7C) = 0; R32(r, 0x80) = 150; R32(r, 0x84) = 90; R32(r, 0x88) = 30; R32(r, 0x8C) = 0;
        R32(r, 0x90) = 3; R32(r, 0x94) = 2; R32(r, 0x98) = 1; R32(r, 0x9C) = 0; R32(r, 0xA0) = 40; R32(r, 0xA4) = 25; R32(r, 0xA8) = 5; R32(r, 0xAC) = 0;
        R32(r, 0xB0) = m.position; R8(r, 0xB4) = m.online ? 1 : 0; R8(r, 0xB5) = m.inGame ? 1 : 0;
        g_FoClanMemberPtrs[i] = r;
    }
    PtrVecWrite(0x2630, g_FoClanMemberPtrs, g_FoClan.memberCount, 16);
    SD32(0x1ED0) = (uint32_t)g_FoClan.memberCount;
}
static void FoClanWriteApplicants() {
    SYSTEMTIME st; GetLocalTime(&st);
    for (size_t i = 0; i < g_FoClan.applicantCount; i++) {
        const FoClanMember& m = g_FoClan.applicants[i];
        uint8_t* r = g_FoClanAppRecs[i]; memset(r, 0, 0x1FC);
        R32(r, 0x00) = m.id; WCopy(RW(r, 0x04), 16, m.name); R32(r, 0x24) = m.level;
        R32(r, 0x28) = 5; R32(r, 0x2C) = 3; R32(r, 0x30) = 0; R32(r, 0x34) = 0; R32(r, 0x38) = 60; R32(r, 0x3C) = 40; R32(r, 0x40) = 10; R32(r, 0x44) = 0;
        R32(r, 0x1E4) = st.wDay; R32(r, 0x1E8) = st.wMonth; R32(r, 0x1EC) = st.wYear;
        g_FoClanAppPtrs[i] = r;
    }
    PtrVecWrite(0x2640, g_FoClanAppPtrs, g_FoClan.applicantCount, 8);
}
static void FoClanSyncVectors() {
    PtrVecWrite(0x2630, g_FoClanMemberPtrs, g_FoClan.has ? g_FoClan.memberCount : 0, 16);
    PtrVecWrite(0x2640, g_FoClanAppPtrs, g_FoClan.has ? g_FoClan.applicantCount : 0, 8);
    PtrVecWrite(0x2620, g_FoClanSearchPtrs, g_FoClanSearchCount, 4);
    PtrVecWrite(0x2610, g_FoClanSearchPtrs, 0, 0);
}
static FoClanMember* FoClanMemberById(uint32_t id) {
    for (size_t i = 0; i < g_FoClan.memberCount; i++) if (g_FoClan.members[i].id == id) return &g_FoClan.members[i];
    return nullptr;
}
static void FoClanMemberRemove(uint32_t id) {
    for (size_t i = 0; i < g_FoClan.memberCount; i++) {
        if (g_FoClan.members[i].id != id) continue;
        for (size_t k = i + 1; k < g_FoClan.memberCount; k++) g_FoClan.members[k - 1] = g_FoClan.members[k];
        g_FoClan.memberCount--; return;
    }
}
static bool FoClanApplicantTake(uint32_t id, FoClanMember* out) {
    for (size_t i = 0; i < g_FoClan.applicantCount; i++) {
        if (g_FoClan.applicants[i].id != id) continue;
        if (out) *out = g_FoClan.applicants[i];
        for (size_t k = i + 1; k < g_FoClan.applicantCount; k++) g_FoClan.applicants[k - 1] = g_FoClan.applicants[k];
        g_FoClan.applicantCount--; return true;
    }
    return false;
}
static void FoClanAddMember(uint32_t id, const wchar_t* name, uint32_t level, uint32_t position, bool online) {
    if (g_FoClan.memberCount >= 16) return;
    FoClanMember& m = g_FoClan.members[g_FoClan.memberCount++];
    m.id = id; WCopy(m.name, 16, name); m.level = level; m.position = position; m.online = online; m.inGame = false;
}
static void FoClanCreate(const wchar_t* name, const uint32_t* marks) {
    memset(&g_FoClan, 0, sizeof(g_FoClan));
    g_FoClan.has = true;
    WCopy(g_FoClan.name, 16, name);
    WCopy(g_FoClan.master, 16, g_Me.nick);
    for (int i = 0; i < 4; i++) g_FoClan.marks[i] = marks[i];
    g_FoClan.id = FoClanIdFromName(g_FoClan.name);
    g_FoClan.level = 1; g_FoClan.rank = 1; g_FoClan.score = 1000; g_FoClan.capacity = 30;
    WCopy(g_FoClan.notice, 200, L"Offline test clan - notice text");
    WCopy(g_FoClan.intro, 206, L"Created locally by the ZNetwork stub.");
    FoClanAddMember(g_Me.userId, g_Me.nick, g_Me.level, 1, true);
    FoClanAddMember(g_FoNextClanUserId++, L"Alpha_Wolf", 7, 2, true);
    FoClanAddMember(g_FoNextClanUserId++, L"Sgt_Reaper", 23, 8, false);
    g_FoClan.applicantCount = 0;
    FoClanMember& a = g_FoClan.applicants[g_FoClan.applicantCount++];
    a.id = g_FoNextClanUserId++; WCopy(a.name, 16, L"Nightfall"); a.level = 15; a.position = 9; a.online = true; a.inGame = false;
    FoClanWriteInfoBlock(); FoClanWriteMembers(); FoClanWriteApplicants(); FoClanSyncVectors();
}
static void FoClanClear() {
    memset(&g_FoClan, 0, sizeof(g_FoClan));
    FoClanWriteInfoBlock(); FoClanSyncVectors();
}
static void FoClanSearchFill(const wchar_t* needle) {
    static const wchar_t* names[3]   = { L"NightOps", L"Replica", L"Point Man" };
    static const wchar_t* masters[3] = { L"Alpha_Wolf", L"Sgt_Reaper", L"Nightfall" };
    SYSTEMTIME st; GetLocalTime(&st);
    g_FoClanSearchCount = 0;
    for (int i = 0; i < 3; i++) {
        uint8_t* r = g_FoClanSearchRecs[g_FoClanSearchCount]; memset(r, 0, 0x3E8);
        R32(r, 0x00) = FoClanIdFromName(names[i]);
        WCopy(RW(r, 0x04), 16, names[i]); WCopy(RW(r, 0x24), 16, masters[i]);
        R32(r, 0x44) = 0;
        R32(r, 0x48) = 1 + i; R32(r, 0x4C) = 2; R32(r, 0x50) = 3; R32(r, 0x54) = 0;
        R32(r, 0x58) = 2 + i; R32(r, 0x60) = 5000 * (i + 1); R32(r, 0x64) = 0;
        R32(r, 0x68) = 10 + i; R32(r, 0x6C) = 3; R32(r, 0x70) = 0;
        R32(r, 0x7C) = 10; R32(r, 0x80) = 2; R32(r, 0x84) = 4;
        R32(r, 0x88) = 5 + i; R32(r, 0x8C) = 1; R32(r, 0x90) = 3 + i; R32(r, 0x94) = 1; R32(r, 0x98) = 30; R32(r, 0x9C) = 0;
        WCopy(RW(r, 0xA0), 100, L"Recruiting - offline sample clan");
        R32(r, 0x3CC) = st.wDay; R32(r, 0x3D0) = st.wMonth; R32(r, 0x3D4) = st.wYear;
        g_FoClanSearchPtrs[g_FoClanSearchCount] = r; g_FoClanSearchCount++;
    }
    (void)needle;
    FoClanSyncVectors();
}

// ---------------------------------------------------------------------------
// Re-assert everything that lives in sd (pointers + model-derived blocks).
// Called once from InitFakeServerDataOnce() and on every Tick().
// ---------------------------------------------------------------------------
static bool g_FoWorldInit = false;
static void FoWorldInitOnce() {
    if (g_FoWorldInit) return;
    g_FoWorldInit = true;
    FoWriteProfile();
    FoWriteMoney();
    FoBuildDefaultInventory();
    FoWriteOpenMaps();
    FoInitLobbyRooms();
    memset(&g_FoRoom, 0, sizeof(g_FoRoom));
    FoWriteRoomList();
    FoWriteLobbyUsers();
    FoChatInit();
    FoSeedSocial();
    FoClanClear();
    FoWriteRoomUserRecord();
    Log("[Proxy] offline world initialised (profile, inventory, %u open maps, %u lobby rooms, friends, mail, chat deque)",
        (unsigned)g_FoOpenMapCount, (unsigned)g_FoLobbyRoomCount);
}
static void FoReassertSd() {
    if (!g_FoWorldInit) return;
    SDPTR(0x2690) = g_FoOpenMaps; SDPTR(0x2694) = g_FoOpenMaps + g_FoOpenMapCount; SDPTR(0x2698) = g_FoOpenMaps + 64;
    PtrVecWrite(0x26C0, g_FoItemPtrs, g_FoItemCount, kFoMaxItems);
    PtrVecWrite(0x26D0, g_FoPartPtrs, g_FoPartCount, kFoMaxParts);
    FoChatSync();
    FoSyncSocialVectors();
    FoClanSyncVectors();
    if (g_FoRoom.active && !g_FoMatchActive) FoWriteRoomBlock(g_FoRoom);
    FoWriteRoomUserRecord();
}

// ---------------------------------------------------------------------------
// Jobs (server answers), executed from Tick()
// ---------------------------------------------------------------------------
static int g_FoUiModeBeforeWindow = 2;    // sd+0x34 before HANGAR/CLAN/POST/PERK/RECIPE open (2 lobby, 3 room)
static void FoOpenWindow(uint32_t mode) {
    const uint32_t cur = SD32(0x34);
    if (cur == 2 || cur == 3 || cur == 4) g_FoUiModeBeforeWindow = (int)cur;
    SD32(0x34) = mode;
}
static void FoCloseWindow() { SD32(0x34) = (uint32_t)g_FoUiModeBeforeWindow; }

static void JobLoginSequence(const FoJob&) {
    FoWorldInitOnce();
    FoWriteProfile();
    Log("[Proxy] --- login sequence (offline) ---");
    CallCb("LOGIN_DATA");
    CallCb("MY_USER_INFO");
    CallCb("GATE_SERVER_INFO");
    CallCb("GATE_CHANNEL_INFO");
    CallCb("GATE_LOBBY_INFO");
    CallCb("GATE_SERVER_JOIN");
    CallCb("GATE_CHANNEL_JOIN");
    SD32(0x34) = 2; SD32(0x2C) = 0;       // lobby AnswerJoinSuccess
    CallCb("LOBBY_JOIN");
    FoWriteLobbyUsers();      CallCb("LOBBY_USER_INFO");
    FoWriteOpenMaps();        CallCb("ROOM_OPEN_MAP_INFO");
    FoWriteRoomList();        CallCb("LOBBY_ROOM_INFO");
    FoSyncInventoryToSd();    CallCb("ITEM_INFO");
    FoWriteMoney();           CallCb("HANGAR_MONEY");
    FoSyncSocialVectors();    CallCb("FRIEND_LIST"); CallCb("FRIEND_REQUEST_LIST");
    CallCb("POST_UNREAD");
    FoClanWriteInfoBlock();   CallCb("CLAN_INFO");
    FoChatPush(6, L"[GM]", L"Offline mode: local ZNetwork stub is serving this session.", 1);
    CallCb("CHAT");
    Log("[Proxy] --- login sequence done ---");
}

// ----- rooms -----
static void JobEnterRoom(const FoJob&) {
    FoWriteRoomBlock(g_FoRoom);
    FoWriteRoomUserRecord();
    FoWriteRoomList();
    SD32(0x34) = 3;                       // AnswerCreateSuccess / AnswerJoinSuccess
    CallCb("ROOM_OPEN_MAP_INFO");
    CallCb("ROOM_INFO");
    CallCb("ROOM_JOIN");
    CallCb("ROOM_USER_INFO");
    Log("[Proxy] room entered: #%u \"%ls\" map=%u (%s) mode=%u max=%u kill=%u round=%u time=%u leader=%ls",
        g_FoRoom.number, g_FoRoom.title, g_FoRoom.mapIndex, FoMapFind(g_FoRoom.mapIndex) ? FoMapFind(g_FoRoom.mapIndex)->mapName : "?",
        g_FoRoom.mode, g_FoRoom.maxUsers, g_FoRoom.goalKill, g_FoRoom.goalRound, g_FoRoom.goalTime, g_FoRoom.leader);
}
static void JobRoomInfo(const FoJob&) {
    if (!g_FoRoom.active) return;
    FoWriteRoomBlock(g_FoRoom);
    FoWriteRoomList();
    CallCb("ROOM_INFO");
}
static void JobRoomUserInfo(const FoJob&) {
    FoWriteRoomUserRecord();
    CallCb("ROOM_USER_INFO");
}
static void JobLeaveRoom(const FoJob&) {
    g_FoRoom.active = false;
    FoWriteRoomList();
    SD32(0x34) = 2;
    CallCb("LOBBY_JOIN");                 // AnswerLeaveSuccess ends in LOBBY_JOIN
    FoWriteLobbyUsers(); CallCb("LOBBY_USER_INFO");
    CallCb("LOBBY_ROOM_INFO");
}
static void JobLobbyUsers(const FoJob&) {
    FoWriteLobbyUsers();
    CallCb("LOBBY_USER_INFO");
}
static void JobLobbyRoomList(const FoJob&) {
    FoWriteRoomList();
    CallCb("LOBBY_ROOM_INFO");
}
static void JobInviteUserList(const FoJob&) {
    size_t n = 0;
    for (size_t i = 0; i < kFoFakeUsers && n < 8; i++) {
        uint8_t* r = g_FoInviteUserRecs[n]; memset(r, 0, 0x9C);
        R32(r, 0x00) = g_FoFakeUsers[i].id; R32(r, 0x04) = g_FoFakeUsers[i].level; R8(r, 0x38) = 0;
        WCopy(RW(r, 0x18), 16, L""); WCopy(RW(r, 0x3A), 48, g_FoFakeUsers[i].name);
        g_FoInviteUserPtrs[n] = r; n++;
    }
    PtrVecWrite(0x2670, g_FoInviteUserPtrs, n, 8);
    CallCb("ROOM_INVITE_USER_INFO");
}

// ----- profile / social -----
static void JobNicknameCreate(const FoJob& j) {
    WCopy(g_Me.nick, 30, j.s1);
    FoWriteProfile();
    CallCb("MY_NICKNAME_CREATE");
    CallCb("MY_USER_INFO");
}
static void JobSearchUser(const FoJob& j) {
    FoWriteSearchBlock(j.s1);
    CallCb("SEARCH_USER_INFO");
}
static void JobChat(const FoJob& j) {
    FoChatPush((uint32_t)j.a, j.s2[0] ? j.s2 : g_Me.nick, j.s1, (uint8_t)j.b);
    CallCb("CHAT");
}
static void JobNotice(const FoJob& j) {
    FoNotice(j.s1);
    CallCb("NOTICE_CHAT");
}
static void JobFriendLists(const FoJob& j) {
    FoSyncSocialVectors();
    if (j.a & 1) CallCb("FRIEND_REQUEST_LIST");
    if (j.a & 2) CallCb("FRIEND_LIST");
    if (j.a & 4) CallCb("FRIEND_REQUEST");
}
static void JobSimpleCb(const FoJob& j) {           // s1 = callback name (narrowed), a.. ignored
    char name[64]; size_t n = 0;
    while (n < 63 && j.s1[n]) { name[n] = (char)j.s1[n]; n++; }
    name[n] = '\0';
    CallCb(name);
}
static void JobCbArgs(const FoJob& j) {             // s1 = callback name, a = argc, b,c,d,e = args
    char name[64]; size_t n = 0;
    while (n < 63 && j.s1[n]) { name[n] = (char)j.s1[n]; n++; }
    name[n] = '\0';
    CallCallbackArgs(name, j.a, (uint32_t)j.b, (uint32_t)j.c, (uint32_t)j.d, (uint32_t)j.e);
}
static void FoPostCb(const char* cbName) {
    wchar_t w[64]; size_t n = 0; while (n < 63 && cbName[n]) { w[n] = (wchar_t)cbName[n]; n++; } w[n] = L'\0';
    FoPost(JobSimpleCb, cbName, 0, 0, 0, 0, w);
}
static void FoPostCbArgs(const char* cbName, int argc, uint32_t a0, uint32_t a1 = 0, uint32_t a2 = 0, uint32_t a3 = 0) {
    wchar_t w[64]; size_t n = 0; while (n < 63 && cbName[n]) { w[n] = (wchar_t)cbName[n]; n++; } w[n] = L'\0';
    FoPost(JobCbArgs, cbName, argc, (int)a0, (int)a1, (int)a2, w, nullptr, (int)a3, 0);
}

// ----- hangar -----
static void JobWindowOpen(const FoJob& j)  { FoOpenWindow((uint32_t)j.a); FoSyncInventoryToSd(); JobSimpleCb(j); }
static void JobWindowClose(const FoJob& j) { FoCloseWindow(); JobSimpleCb(j); }
static void JobItemInfo(const FoJob&)      { FoSyncInventoryToSd(); CallCb("ITEM_INFO"); }
static void JobMoney(const FoJob&)         { FoWriteMoney(); CallCb("HANGAR_MONEY"); }

static void JobItemBuy(const FoJob& j) {
    const uint32_t itemIndex = (uint32_t)j.a;
    const FoCatItem* c = FoCatFind(itemIndex);
    if (!c) { Log("[Proxy] HANGAR_ITEM_BUY: itemIndex %u not in catalogue -> NOTIFY_ERROR", itemIndex); CallCb1("NOTIFY_ERROR", 0xC353); return; }
    uint32_t serial = 0;
    if (FoTypeIsCustomPart(c->itemType)) {
        uint8_t* p = FoAddPart(c, 0, c->defaultValue, false);
        if (p) serial = R32(p, 0);
    } else if (FoTypeIsMirrored(c->itemType)) {
        uint8_t* r = FoAddItem(c, c->defaultValue);
        if (r) {
            serial = R32(r, 0);
            const int o = FoEquipOrdinal(c->itemType);
            if (o >= 0 && g_FoEquipped[o] == 0) g_FoEquipped[o] = serial;
        }
    } else {
        Log("[Proxy] HANGAR_ITEM_BUY: itemType 0x%02X is not mirrored to sd (capsule/etc.) -- acknowledged only", c->itemType);
    }
    if (c->pointPrice && g_Me.point >= c->pointPrice) g_Me.point -= c->pointPrice;
    else if (c->cashPrice && g_Me.cash >= c->cashPrice) g_Me.cash -= c->cashPrice;
    FoSyncInventoryToSd();
    Log("[Proxy] HANGAR_ITEM_BUY: itemIndex=%u type=0x%02X -> serial %u (point %llu cash %llu)", itemIndex, c->itemType, serial,
        (unsigned long long)g_Me.point, (unsigned long long)g_Me.cash);
    CallCb("ITEM_INFO");
    CallCb("HANGAR_MONEY");
    CallCb2("HANGAR_ITEM_BUY", serial, 0);
}
static void JobItemEquip(const FoJob& j) {
    const uint32_t serial = (uint32_t)j.a;
    const uint8_t* rec = FoItemBySerial(serial);
    const FoCatItem* c = FoCatOfRec(rec);
    const int o = c ? FoEquipOrdinal(c->itemType) : -1;
    if (o >= 0) g_FoEquipped[o] = serial;
    else Log("[Proxy] HANGAR_ITEM_EQUIP: serial %u has no equip slot (type 0x%02X)", serial, c ? c->itemType : 0);
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb1("HANGAR_ITEM_EQUIP", serial);
}
static void JobItemDelete(const FoJob& j) {
    FoRemoveItem((uint32_t)j.a);
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb1("HANGAR_ITEM_DELETE", (uint32_t)j.a);
}
static void JobItemUse(const FoJob& j) {
    uint8_t* rec = FoItemBySerial((uint32_t)j.a);
    if (rec) {
        uint32_t v = R32(rec, 8);
        if (v > 1) { v--; R32(rec, 8) = v; R32(rec, 0xC) = v; R32(rec, 0x10) = v; }
        else FoRemoveItem((uint32_t)j.a);
    }
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb1("HANGAR_ITEM_USE", (uint32_t)j.a);
}
static void JobItemRepair(const FoJob& j) {
    uint8_t* rec = FoItemBySerial((uint32_t)j.a);
    const FoCatItem* c = FoCatOfRec(rec);
    if (rec && c) { R32(rec, 8) = c->defaultValue; R32(rec, 0xC) = c->defaultValue; R32(rec, 0x10) = c->defaultValue; }
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb1("HANGAR_ITEM_REPAIR", (uint32_t)j.a);
}
static void JobItemResell(const FoJob& j) {
    const uint8_t* rec = FoItemBySerial((uint32_t)j.a);
    const FoCatItem* c = FoCatOfRec(rec);
    if (c) g_Me.point += c->pointPrice / 2;
    FoRemoveItem((uint32_t)j.a);
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb("HANGAR_MONEY");
    CallCb1("HANGAR_ITEM_RESELL", (uint32_t)j.a);
}
static void JobCustomBuy(const FoJob& j) {
    const uint32_t weaponSerial = (uint32_t)j.a, itemIndex = (uint32_t)j.b;
    const FoCatItem* c = FoCatFind(itemIndex);
    uint32_t serial = 0;
    if (c && FoTypeIsCustomPart(c->itemType)) {
        uint8_t* p = FoAddPart(c, weaponSerial, c->defaultValue, true);
        if (p) { serial = R32(p, 0); FoUnmountSameSlot(weaponSerial, R32(p, 0xC), p); }
    } else {
        Log("[Proxy] HANGAR_CUSTOM_BUY: itemIndex %u is not a custom part", itemIndex);
    }
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb2("HANGAR_CUSTOM_BUY", serial, itemIndex);
}
static void JobCustomEquip(const FoJob& j) {
    uint8_t* p = FoPartBySerial((uint32_t)j.a);
    if (p) {
        if (R32(p, 4) == 0) R32(p, 4) = g_FoEquipped[2];   // mount on the primary weapon if the part was free
        R8(p, 0x15) = 1;
        FoUnmountSameSlot(R32(p, 4), R32(p, 0xC), p);
    }
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb1("HANGAR_CUSTOM_EQUIP", (uint32_t)j.a);
}
static void JobCustomBreak(const FoJob& j) {
    uint8_t* p = FoPartBySerial((uint32_t)j.a);
    if (p) R8(p, 0x15) = 0;
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb1("HANGAR_CUSTOM_BREAK", (uint32_t)j.a);
}
static void JobEquipWear(const FoJob& j) {
    const uint32_t parent = (uint32_t)j.a, child = (uint32_t)j.b;
    uint8_t* prec = FoItemBySerial(parent);
    const uint8_t* crec = FoItemBySerial(child);
    const FoCatItem* cc = FoCatOfRec(crec);
    if (prec && cc && cc->itemType >= 0x1F && cc->itemType <= 0x22) {
        const size_t off = 0x34 + (cc->itemType - 0x1F) * 8;
        R32(prec, off) = child; R32(prec, off + 4) = cc->itemIndex;
    } else Log("[Proxy] HANGAR_EQUIP_WEAR: parent %u / child %u not a (character, wear) pair", parent, child);
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb2("HANGAR_EQUIP_WEAR", parent, child);
}
static void JobBreakWear(const FoJob& j) {
    const uint32_t parent = (uint32_t)j.a, child = (uint32_t)j.b;
    uint8_t* prec = FoItemBySerial(parent);
    if (prec) for (int s = 0; s < 4; s++) if (R32(prec, 0x34 + s * 8) == child) { R32(prec, 0x34 + s * 8) = 0; R32(prec, 0x38 + s * 8) = 0; }
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb2("HANGAR_BREAK_WEAR", parent, child);
}
static void JobPerkEquip(const FoJob& j) {
    const uint32_t serial = (uint32_t)j.a, ordinal = (uint32_t)j.b, perkSerial = (uint32_t)j.c;
    uint8_t* prec = FoItemBySerial(serial);
    const uint8_t* perk = FoItemBySerial(perkSerial);
    if (prec && perk && ordinal >= 1 && ordinal <= 3) { R32(prec, 0x1C + (ordinal - 1) * 8) = perkSerial; R32(prec, 0x20 + (ordinal - 1) * 8) = R32(perk, 4); }
    else Log("[Proxy] PERK_EQUIP_SLOT: bad args serial=%u ordinal=%u perk=%u", serial, ordinal, perkSerial);
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb3("PERK_EQUIP_SLOT", serial, ordinal, perkSerial);
}
static void JobPerkBreak(const FoJob& j) {
    const uint32_t serial = (uint32_t)j.a, ordinal = (uint32_t)j.b;
    uint8_t* prec = FoItemBySerial(serial);
    if (prec && ordinal >= 1 && ordinal <= 3) { R32(prec, 0x1C + (ordinal - 1) * 8) = 0; R32(prec, 0x20 + (ordinal - 1) * 8) = 0; }
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb2("PERK_BREAK_SLOT", serial, ordinal);
}
static void JobPerkIncrease(const FoJob& j) {
    uint8_t* prec = FoItemBySerial((uint32_t)j.a);
    if (prec) R8(prec, 0x14) = (uint8_t)j.b;
    FoSyncInventoryToSd();
    CallCb("ITEM_INFO");
    CallCb2("PERK_INCREASE_SLOT", (uint32_t)j.a, (uint32_t)j.b);
}

// ----- clan -----
static void JobClanOpen(const FoJob&)  { FoOpenWindow(7); FoClanWriteInfoBlock(); FoClanSyncVectors(); CallCb("CLAN_OPEN"); }
static void JobClanClose(const FoJob&) { FoCloseWindow(); CallCb("CLAN_CLOSE"); }
static void JobClanCreate(const FoJob& j) {
    const uint32_t marks[4] = { (uint32_t)j.a, (uint32_t)j.b, (uint32_t)j.c, (uint32_t)j.d };
    FoClanCreate(j.s1, marks);
    Log("[Proxy] clan created: \"%ls\" id=%u marks=%u/%u/%u/%u members=%u applicants=%u",
        g_FoClan.name, g_FoClan.id, marks[0], marks[1], marks[2], marks[3], (unsigned)g_FoClan.memberCount, (unsigned)g_FoClan.applicantCount);
    CallCb("CLAN_INFO");          // A60B
    CallCb("MY_USER_INFO");
    CallCb("CLAN_MEMBER_INFO");   // A60F
    CallCb("CLAN_INFO");          // A642 member counts
    CallCb("CLAN_REQUEST_USER");  // applicants
}
static void JobClanRefresh(const FoJob& j) {      // a: bit0 INFO, bit1 MEMBER, bit2 MY_USER_INFO, bit3 REQUEST_USER, bit4 REQUEST_INFO
    FoClanWriteInfoBlock();
    if (g_FoClan.has) { FoClanWriteMembers(); FoClanWriteApplicants(); }
    FoClanSyncVectors();
    if (j.a & 2)  CallCb("CLAN_MEMBER_INFO");
    if (j.a & 1)  CallCb("CLAN_INFO");
    if (j.a & 4)  CallCb("MY_USER_INFO");
    if (j.a & 8)  CallCb("CLAN_REQUEST_USER");
    if (j.a & 16) CallCb("CLAN_REQUEST_INFO");
}
static void JobClanChangeMark(const FoJob& j) {
    const uint32_t marks[4] = { (uint32_t)j.a, (uint32_t)j.b, (uint32_t)j.c, (uint32_t)j.d };
    for (int i = 0; i < 4; i++) { g_FoClan.marks[i] = marks[i]; SD32(0xE0 + i * 4) = marks[i]; SD32(0x1E90 + i * 4) = marks[i]; }
    CallCb("CLAN_CHANGE_MARK");
}
static void JobClanSearch(const FoJob& j) {
    FoClanSearchFill(j.s1);
    CallCb("CLAN_SEARCH");
}
static void JobClanRequest(const FoJob& j) {       // I applied to clan s1
    memset(g_ServerDataBuffer + 0x2230, 0, 0x48);
    WCopy(SDW(0x2230), 22, j.s1);
    SD32(0x225C) = 3; SD32(0x2260) = 2; SD32(0x2264) = 1;
    SD8(0x2274) = 1;
    CallCb("CLAN_REQUEST");
    CallCb("CLAN_REQUEST_INFO");
}
static void JobClanRequestCancel(const FoJob&) {
    memset(g_ServerDataBuffer + 0x2230, 0, 0x48);
    CallCb("CLAN_REQUEST_INFO");
}

// ----- mail / gifts -----
static void JobPostOpen(const FoJob&)  { FoOpenWindow(6); FoSyncSocialVectors(); CallCb("POST_OPEN"); }
static void JobPostClose(const FoJob&) { FoCloseWindow(); CallCb("POST_CLOSE"); }
static void JobPostPage(const FoJob& j) {          // a = page, b = 0 receive / 1 send
    SD32(0x1E3C) = (uint32_t)j.a;
    FoSyncSocialVectors();
    CallCb(j.b ? "POST_SEND_PAGE_INFO" : "POST_RECEIVE_PAGE_INFO");
}
static void JobPostWrite(const FoJob& j) {         // s1 = recipient, s2 = subject; body stored in a static buffer
    if (g_FoMailSendCount < 8) {
        FoMailFill(g_FoMailSendRecs[g_FoMailSendCount], g_FoNextMailId++, g_Me.nick, j.s1, j.s2, L"(sent offline)", true);
        g_FoMailSendPtrs[g_FoMailSendCount] = g_FoMailSendRecs[g_FoMailSendCount]; g_FoMailSendCount++;
    }
    FoSyncSocialVectors();
    CallCb("POST_WRITE");
}
static void JobPostRead(const FoJob& j) {
    for (size_t i = 0; i < g_FoMailRecvCount; i++) if (R32(g_FoMailRecvRecs[i], 0) == (uint32_t)j.a) R8(g_FoMailRecvRecs[i], 0x224) = 1;
    bool unread = false;
    for (size_t i = 0; i < g_FoMailRecvCount; i++) if (R8(g_FoMailRecvRecs[i], 0x224) == 0) unread = true;
    SD32(0x1E38) = unread ? 1 : 0;
    CallCb("POST_RECEIVE_PAGE_INFO");
}
static void JobPostDelete(const FoJob& j) {        // a = id, b = 0 receive / 1 send
    uint8_t (*recs)[0x228] = j.b ? g_FoMailSendRecs : g_FoMailRecvRecs;
    size_t& count = j.b ? g_FoMailSendCount : g_FoMailRecvCount;
    for (size_t i = 0; i < count; i++) {
        if (R32(recs[i], 0) != (uint32_t)j.a) continue;
        for (size_t k = i + 1; k < count; k++) memcpy(recs[k - 1], recs[k], 0x228);
        count--; break;
    }
    FoSyncSocialVectors();
    CallCb(j.b ? "POST_SEND_PAGE_INFO" : "POST_RECEIVE_PAGE_INFO");
}
static void JobPostClear(const FoJob& j) {
    if (j.b) g_FoMailSendCount = 0; else g_FoMailRecvCount = 0;
    FoSyncSocialVectors();
    CallCb(j.b ? "POST_SEND_PAGE_INFO" : "POST_RECEIVE_PAGE_INFO");
}
static void JobGiftPage(const FoJob& j) {
    SD32(0x1E44) = (uint32_t)j.a;
    FoSyncSocialVectors();
    CallCb(j.b ? "GIFT_SEND_PAGE_INFO" : "GIFT_RECEIVE_PAGE_INFO");
}

// ----- game leave (match finished) -----
static void JobAfterGameLeave(const FoJob&) {
    if (!g_FoRoom.active) return;
    g_FoRoom.state = 0;
    FoWriteRoomBlock(g_FoRoom);
    FoWriteRoomUserRecord();
    SD32(0x34) = 3;
    CallCb("ROOM_INFO");
    CallCb("ROOM_USER_INFO");
}
