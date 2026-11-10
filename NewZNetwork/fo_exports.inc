// ============================================================================
//  fo_exports.inc -- Send_* exports answered by the OFFLINE WORLD MODEL
//  (textually #included by znetwork_proxy.cpp before SEND_LIST; depends on
//   fo_world.inc, Send_ROOM_GAME_START / g_IsHost / g_MatchStartSent above).
//
//  Every export below is cdecl with scalar stack arguments, exactly as in
//  the original ZNetwork.dll (verified: plain `ret`, the GameClient caller
//  cleans the stack). Each one updates the model, then posts the server
//  answer as a job (delivered from Tick(); inline when Tick() is silent).
// ============================================================================

#define FO_EXPORT extern "C" __declspec(dllexport)

// ---------------------------------------------------------------------------
// Lobby / room
// ---------------------------------------------------------------------------
FO_EXPORT int Send_LOBBY_ROOM_CREATE_EX(const wchar_t* title, const wchar_t* password,
                                        int maxUsers, int mapIndex, int goalKill, int goalRound, int mode,
                                        int isPublic, int state, int notSent, int teamBalance, int instruction,
                                        int goalTime, int deathChat) {
    wchar_t t[64], p[16];
    SafeWCopy(t, 64, title);
    SafeWCopy(p, 16, password);
    Log("[Proxy] Send_LOBBY_ROOM_CREATE_EX(title=\"%ls\" pw=\"%ls\" max=%d map=%d kill=%d round=%d mode=%d public=%d state=%d [a10=%d] teamBal=%d instr=%d time=%d deathChat=%d)",
        t, p, maxUsers & 0xFF, mapIndex & 0xFFFF, goalKill & 0xFF, goalRound & 0xFF, mode & 0xFF, isPublic & 0xFF, state & 0xFF,
        notSent, teamBalance & 0xFF, instruction & 0xFF, goalTime & 0xFFFF, deathChat & 0xFF);
    const DWORD now = GetTickCount();
    if (g_FoRoom.active && g_FoLastCreateTick != 0 && (now - g_FoLastCreateTick) < 2000) {
        Log("[Proxy] Send_LOBBY_ROOM_CREATE_EX: duplicate request within 2 s -- ignored");
        return 1;
    }
    g_FoLastCreateTick = now;
    FoInitRoom(g_FoRoom, (uint32_t)InterlockedIncrement(&g_FoRoomSeq), t[0] ? t : L"Offline room", g_Me.nick, g_Me.userId,
               (uint32_t)(mapIndex & 0xFFFF));
    if (!FoMapFind(g_FoRoom.mapIndex)) Log("[Proxy] WARNING: map %u is not in the catalogue (MapInfo); the client will try to load it anyway", g_FoRoom.mapIndex);
    WCopy(g_FoRoom.password, 16, p);
    if (maxUsers & 0xFF)  g_FoRoom.maxUsers  = (uint32_t)(maxUsers & 0xFF);
    g_FoRoom.goalKill    = (uint32_t)(goalKill & 0xFF);
    g_FoRoom.goalRound   = (uint32_t)(goalRound & 0xFF);
    g_FoRoom.goalTime    = (uint32_t)(goalTime & 0xFFFF);
    if (mode & 0xFF)      g_FoRoom.mode      = (uint32_t)(mode & 0xFF);
    g_FoRoom.pub         = (uint8_t)isPublic;
    g_FoRoom.state       = 0;
    g_FoRoom.teamBal     = (uint8_t)teamBalance;
    g_FoRoom.instruction = (uint8_t)instruction;
    g_FoRoom.deathChat   = (uint8_t)deathChat;
    g_FoRoom.perkUse     = 1;
    g_FoRoom.thirdView   = 0;
    g_FoMyTeam = 1;
    g_RoomCreateRequested = true;
    g_MatchStartSent = false;
    InterlockedExchange(&g_FoMatchActive, 0);
    FoPost(JobEnterRoom, "EnterRoom(create)");
    return 1;
}

FO_EXPORT int Send_ROOM_DETAIL_SETTING_EX(const wchar_t* title, const wchar_t* password,
                                          int maxUsers, int mapIndex, int goalKill, int goalRound, int mode,
                                          int f8, int f9, int notSent, int f11, int f12, int f13) {
    wchar_t t[64], p[16];
    SafeWCopy(t, 64, title);
    SafeWCopy(p, 16, password);
    Log("[Proxy] Send_ROOM_DETAIL_SETTING_EX(title=\"%ls\" pw=\"%ls\" max=%d map=%d kill=%d round=%d mode=%d f8=%d f9=%d [a10=%d] f11=%d f12=%d f13=%d)",
        t, p, maxUsers & 0xFF, mapIndex & 0xFFFF, goalKill & 0xFF, goalRound & 0xFF, mode & 0xFF, f8 & 0xFF, f9 & 0xFF, notSent, f11 & 0xFF, f12 & 0xFF, f13 & 0xFF);
    if (!g_FoRoom.active) { Log("[Proxy] Send_ROOM_DETAIL_SETTING_EX: not in a room -- ignored"); return 1; }
    if (t[0]) WCopy(g_FoRoom.title, 64, t);
    WCopy(g_FoRoom.password, 16, p);
    if ((uint32_t)(mapIndex & 0xFFFF) != g_FoRoom.mapIndex) {
        const uint32_t keepMax = g_FoRoom.maxUsers;
        FoRoomDefaultsFromMap(g_FoRoom, (uint32_t)(mapIndex & 0xFFFF));
        g_FoRoom.maxUsers = keepMax;
    }
    if (maxUsers & 0xFF) g_FoRoom.maxUsers = (uint32_t)(maxUsers & 0xFF);
    if (goalKill & 0xFF)  g_FoRoom.goalKill  = (uint32_t)(goalKill & 0xFF);
    if (goalRound & 0xFF) g_FoRoom.goalRound = (uint32_t)(goalRound & 0xFF);
    if (mode & 0xFF)      g_FoRoom.mode      = (uint32_t)(mode & 0xFF);
    g_FoRoom.pub = (uint8_t)f8; g_FoRoom.teamBal = (uint8_t)f11; g_FoRoom.instruction = (uint8_t)f12; g_FoRoom.deathChat = (uint8_t)f13;
    FoPost(JobRoomInfo, "RoomInfo(setting)");
    return 1;
}

FO_EXPORT int Send_LOBBY_ROOM_ENTER(int roomNumber, const wchar_t* password, int extra) {
    wchar_t p[16];
    SafeWCopy(p, 16, password);
    const uint32_t num = (uint32_t)(roomNumber & 0xFFFF);
    Log("[Proxy] Send_LOBBY_ROOM_ENTER(room=%u pw=\"%ls\" extra=%d)", num, p, extra & 0xFFFF);
    const FoRoom* src = nullptr;
    for (size_t i = 0; i < g_FoLobbyRoomCount; i++) if (g_FoLobbyRooms[i].number == num) src = &g_FoLobbyRooms[i];
    if (!src) {
        if (g_FoRoom.active && g_FoRoom.number == num) src = &g_FoRoom;
        else { Log("[Proxy] Send_LOBBY_ROOM_ENTER: room %u not in the lobby list -- NOTIFY_ERROR", num); FoPostCbArgs("NOTIFY_ERROR", 1, 0xC353); return 1; }
    }
    FoRoom joined = *src;
    // Offline there is nobody else to be the leader: we become the leader of whatever room we join so the
    // START button works (the original would keep the advertised host as leader).
    WCopy(joined.leader, 16, g_Me.nick); joined.leaderId = g_Me.userId; joined.curUsers = 1; joined.state = 0;
    g_FoRoom = joined; g_FoRoom.active = true;
    g_FoMyTeam = 1;
    g_RoomCreateRequested = true;
    g_MatchStartSent = false;
    InterlockedExchange(&g_FoMatchActive, 0);
    FoPost(JobEnterRoom, "EnterRoom(join)");
    return 1;
}

FO_EXPORT int Send_LOBBY_QUICK_JOIN(int mode, int a2, int a3) {
    Log("[Proxy] Send_LOBBY_QUICK_JOIN(mode=%d a2=%d a3=%d)", mode & 0xFF, a2 & 0xFFFF, a3 & 0xFFFF);
    const FoRoom* pick = nullptr;
    for (size_t i = 0; i < g_FoLobbyRoomCount; i++) if (FoMapModeRemapped(g_FoLobbyRooms[i].mapIndex) == (uint8_t)(mode & 0xFF)) { pick = &g_FoLobbyRooms[i]; break; }
    if (!pick && g_FoLobbyRoomCount) pick = &g_FoLobbyRooms[0];
    if (!pick) { FoPostCbArgs("LOBBY_QUICK_JOIN_FAILED", 2, 1, 0); return 1; }
    return Send_LOBBY_ROOM_ENTER((int)pick->number, L"", 0);
}

FO_EXPORT int Send_LOBBY_ROOM_INFO(int b) {
    Log("[Proxy] Send_LOBBY_ROOM_INFO(%d) -> room list", b & 0xFF);
    FoPost(JobLobbyRoomList, "LobbyRoomList");
    return 1;
}
FO_EXPORT int Send_LOBBY_REFRESH_ROOM_INFO(unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7, unsigned a8, unsigned a9) {
    Log("[Proxy] Send_LOBBY_REFRESH_ROOM_INFO(%u,%u,%u,%u,%u,%u,%u,%u,%u) -> room list", a1, a2, a3, a4, a5, a6, a7, a8, a9);
    FoPost(JobLobbyRoomList, "LobbyRoomList(refresh)");
    return 1;
}
FO_EXPORT int Send_LOBBY_MODE_MAP(int mode, int map) {
    Log("[Proxy] Send_LOBBY_MODE_MAP(mode=%d map=%d) -> LOBBY_MODE_MAP(mode, map)", mode & 0xFF, map & 0xFFFF);
    FoPostCbArgs("LOBBY_MODE_MAP", 2, (uint32_t)(mode & 0xFF), (uint32_t)(map & 0xFFFF));
    return 1;
}
FO_EXPORT int Send_LOBBY_LEAVE() {
    Log("[Proxy] Send_LOBBY_LEAVE() -> acknowledged (no channel/gate screen change offline)");
    return 1;
}
FO_EXPORT int Send_ROOM_LEAVE() {
    Log("[Proxy] Send_ROOM_LEAVE() -> back to the lobby");
    InterlockedExchange(&g_FoMatchActive, 0);
    g_MatchStartSent = false;
    g_RoomCreateRequested = false;
    g_IsHost = false;
    FoPost(JobLeaveRoom, "LeaveRoom");
    return 1;
}
FO_EXPORT int Send_ROOM_USER_TEAM(int team) {
    Log("[Proxy] Send_ROOM_USER_TEAM(%d)", team & 0xFF);
    g_FoMyTeam = (team & 0xFF) ? (team & 0xFF) : 1;
    FoPost(JobRoomUserInfo, "RoomUserInfo(team)");
    return 1;
}
FO_EXPORT int Send_ROOM_INVITE_USER_INFO() {
    Log("[Proxy] Send_ROOM_INVITE_USER_INFO() -> invitable user list");
    FoPost(JobInviteUserList, "InviteUserList");
    return 1;
}
FO_EXPORT int Send_ROOM_INVITE(unsigned userId) {
    Log("[Proxy] Send_ROOM_INVITE(user=%u) -> acknowledged (nobody to invite offline)", userId);
    return 1;
}
FO_EXPORT int Send_ROOM_GAME_READY() {
    Log("[Proxy] Send_ROOM_GAME_READY() -> acknowledged (leader is always ready offline)");
    return 1;
}
FO_EXPORT int Send_GAME_LEAVE() {
    Log("[Proxy] Send_GAME_LEAVE() -> match over, back to the room");
    InterlockedExchange(&g_FoMatchActive, 0);
    g_MatchStartSent = false;
    g_IsHost = false;
    g_ServerHostData.hostMode = 0; g_ServerHostData.isRoomOwner = 0;
    SD32(0xE84) = 0; SD8(0xE80) = 0; SD32(0xE88) = 0;
    FoPost(JobAfterGameLeave, "AfterGameLeave");
    return 1;
}

// ---------------------------------------------------------------------------
// Profile / chat / social
// ---------------------------------------------------------------------------
FO_EXPORT int Send_MY_NICKNAME_CREATE(const wchar_t* nick) {
    wchar_t n[30]; SafeWCopy(n, 30, nick);
    Log("[Proxy] Send_MY_NICKNAME_CREATE(\"%ls\")", n);
    if (!n[0]) { FoPostCb("NICKNAME_IN_USE"); return 1; }
    FoPost(JobNicknameCreate, "NicknameCreate", 0, 0, 0, 0, n);
    return 1;
}
FO_EXPORT int Send_SEARCH_USER(const wchar_t* name) {
    wchar_t n[30]; SafeWCopy(n, 30, name);
    Log("[Proxy] Send_SEARCH_USER(\"%ls\")", n);
    FoPost(JobSearchUser, "SearchUser", 0, 0, 0, 0, n);
    return 1;
}
FO_EXPORT int Send_MY_GUIDE_CLEAR() { Log("[Proxy] Send_MY_GUIDE_CLEAR() -> acknowledged"); return 1; }

static int FoChatSend(const char* which, uint32_t type, const wchar_t* text, int flag, const wchar_t* nameOverride) {
    wchar_t full[200];   // job buffer is 64 wchar; the chat box allows 200 -> long lines bypass the job copy
    SafeWCopy(full, 200, text);
    Log("[Proxy] Send_CHAT_%s(\"%ls\", flag=%d) -> echo as type %u", which, full, flag & 0xFF, type);
    if (!full[0]) return 1;
    if (wcslen(full) < 64) { FoPost(JobChat, "Chat", (int)type, flag & 0xFF, 0, 0, full, nameOverride); return 1; }
    // long line: push directly (one record), deliver the callback as a job
    FoChatPush(type, nameOverride && nameOverride[0] ? nameOverride : g_Me.nick, full, (uint8_t)(flag & 0xFF));
    FoPostCb("CHAT");
    return 1;
}
FO_EXPORT int Send_CHAT_ALL(const wchar_t* text, int flag)  { return FoChatSend("ALL", 0, text, flag, nullptr); }
FO_EXPORT int Send_CHAT_TEAM(const wchar_t* text, int flag) { return FoChatSend("TEAM", 1, text, flag, nullptr); }
FO_EXPORT int Send_CHAT_CLAN(const wchar_t* text, int flag) { return FoChatSend("CLAN", 4, text, flag, nullptr); }
FO_EXPORT int Send_CHAT_WHISPER(const wchar_t* target, const wchar_t* text, int flag) {
    wchar_t to[16]; SafeWCopy(to, 16, target);
    wchar_t prefixed[200]; swprintf_s(prefixed, 200, L"[to %ls] ", to);
    wchar_t body[200]; SafeWCopy(body, 200, text);
    wcsncat_s(prefixed, 200, body, _TRUNCATE);
    return FoChatSend("WHISPER", 3, prefixed, flag, nullptr);
}
FO_EXPORT int Send_REPORT_USER(const wchar_t* name, const wchar_t* text, int type) {
    wchar_t n[16]; SafeWCopy(n, 16, name);
    wchar_t t[64]; SafeWCopy(t, 64, text);
    Log("[Proxy] Send_REPORT_USER(\"%ls\", \"%ls\", %d) -> REPORT", n, t, type & 0xFF);
    FoPostCb("REPORT");
    return 1;
}

FO_EXPORT int Send_FRIEND_REQUEST(const wchar_t* name) {
    wchar_t n[16]; SafeWCopy(n, 16, name);
    Log("[Proxy] Send_FRIEND_REQUEST(\"%ls\") -> accepted immediately offline", n);
    if (n[0]) FoFriendAdd(4000 + (uint32_t)(g_FoFriendCount), n, 5, true);
    FoPost(JobFriendLists, "FriendLists", 4 | 2);
    return 1;
}
FO_EXPORT int Send_FRIEND_REQUEST_ACCEPT(int requestId) {
    const uint32_t id = (uint32_t)(requestId & 0xFFFF);
    wchar_t n[16] = L""; uint32_t level = 1;
    Log("[Proxy] Send_FRIEND_REQUEST_ACCEPT(%u)", id);
    if (FoFReqTake(id, n, &level)) FoFriendAdd(id, n, level, true);
    FoPost(JobFriendLists, "FriendLists", 1 | 2);
    return 1;
}
FO_EXPORT int Send_FRIEND_REQUEST_REFUSE(int requestId) {
    const uint32_t id = (uint32_t)(requestId & 0xFFFF);
    Log("[Proxy] Send_FRIEND_REQUEST_REFUSE(%u)", id);
    FoFReqTake(id, nullptr, nullptr);
    FoPost(JobFriendLists, "FriendLists", 1);
    return 1;
}
FO_EXPORT int Send_FRIEND_DELETE(int friendId) {
    const uint32_t id = (uint32_t)(friendId & 0xFFFF);
    Log("[Proxy] Send_FRIEND_DELETE(%u)", id);
    FoFriendRemove(id);
    FoPost(JobFriendLists, "FriendLists", 2);
    return 1;
}
FO_EXPORT int Send_FRIEND_INVITE(int friendId) {
    Log("[Proxy] Send_FRIEND_INVITE(%u) -> acknowledged (nobody to invite offline)", (unsigned)(friendId & 0xFFFF));
    return 1;
}

// ---------------------------------------------------------------------------
// Hangar / items / perks / recipes
// ---------------------------------------------------------------------------
FO_EXPORT int Send_HANGAR_OPEN()  { Log("[Proxy] Send_HANGAR_OPEN()");  FoPost(JobWindowOpen,  "HangarOpen",  5, 0, 0, 0, L"HANGAR_OPEN");  return 1; }
FO_EXPORT int Send_HANGAR_CLOSE() { Log("[Proxy] Send_HANGAR_CLOSE()"); FoPost(JobWindowClose, "HangarClose", 0, 0, 0, 0, L"HANGAR_CLOSE"); return 1; }
FO_EXPORT int Send_HANGAR_MONEY() { Log("[Proxy] Send_HANGAR_MONEY()"); FoPost(JobMoney, "Money"); return 1; }
FO_EXPORT int Send_HANGAR_ITEM_BUY(unsigned itemIndex) { Log("[Proxy] Send_HANGAR_ITEM_BUY(%u)", itemIndex); FoPost(JobItemBuy, "ItemBuy", (int)itemIndex); return 1; }
FO_EXPORT int Send_HANGAR_ITEM_EQUIP(int serial)  { Log("[Proxy] Send_HANGAR_ITEM_EQUIP(%u)",  serial & 0xFFFF); FoPost(JobItemEquip,  "ItemEquip",  serial & 0xFFFF); return 1; }
FO_EXPORT int Send_HANGAR_ITEM_DELETE(int serial) { Log("[Proxy] Send_HANGAR_ITEM_DELETE(%u)", serial & 0xFFFF); FoPost(JobItemDelete, "ItemDelete", serial & 0xFFFF); return 1; }
FO_EXPORT int Send_HANGAR_ITEM_USE(int serial)    { Log("[Proxy] Send_HANGAR_ITEM_USE(%u)",    serial & 0xFFFF); FoPost(JobItemUse,    "ItemUse",    serial & 0xFFFF); return 1; }
FO_EXPORT int Send_HANGAR_ITEM_REPAIR(int serial) { Log("[Proxy] Send_HANGAR_ITEM_REPAIR(%u)", serial & 0xFFFF); FoPost(JobItemRepair, "ItemRepair", serial & 0xFFFF); return 1; }
FO_EXPORT int Send_HANGAR_ITEM_RESELL(int serial) { Log("[Proxy] Send_HANGAR_ITEM_RESELL(%u)", serial & 0xFFFF); FoPost(JobItemResell, "ItemResell", serial & 0xFFFF); return 1; }
FO_EXPORT int Send_HANGAR_ITEM_EXPIRE()           { Log("[Proxy] Send_HANGAR_ITEM_EXPIRE() -> nothing expires offline"); FoPost(JobItemInfo, "ItemInfo(expire)"); return 1; }
FO_EXPORT int Send_HANGAR_CUSTOM_BUY(int weaponSerial, unsigned itemIndex) {
    Log("[Proxy] Send_HANGAR_CUSTOM_BUY(weapon=%u item=%u)", weaponSerial & 0xFFFF, itemIndex);
    FoPost(JobCustomBuy, "CustomBuy", weaponSerial & 0xFFFF, (int)itemIndex); return 1;
}
FO_EXPORT int Send_HANGAR_CUSTOM_EQUIP(int serial) { Log("[Proxy] Send_HANGAR_CUSTOM_EQUIP(%u)", serial & 0xFFFF); FoPost(JobCustomEquip, "CustomEquip", serial & 0xFFFF); return 1; }
FO_EXPORT int Send_HANGAR_CUSTOM_BREAK(int serial) { Log("[Proxy] Send_HANGAR_CUSTOM_BREAK(%u)", serial & 0xFFFF); FoPost(JobCustomBreak, "CustomBreak", serial & 0xFFFF); return 1; }
FO_EXPORT int Send_HANGAR_EQUIP_WEAR(int parent, int child) {
    Log("[Proxy] Send_HANGAR_EQUIP_WEAR(parent=%u child=%u)", parent & 0xFFFF, child & 0xFFFF);
    FoPost(JobEquipWear, "EquipWear", parent & 0xFFFF, child & 0xFFFF); return 1;
}
FO_EXPORT int Send_HANGAR_BREAK_WEAR(int parent, int child) {
    Log("[Proxy] Send_HANGAR_BREAK_WEAR(parent=%u child=%u)", parent & 0xFFFF, child & 0xFFFF);
    FoPost(JobBreakWear, "BreakWear", parent & 0xFFFF, child & 0xFFFF); return 1;
}
FO_EXPORT int Send_PERK_OPEN()  { Log("[Proxy] Send_PERK_OPEN()");  FoPost(JobWindowOpen,  "PerkOpen",  10, 0, 0, 0, L"PERK_OPEN");  return 1; }
FO_EXPORT int Send_PERK_CLOSE() { Log("[Proxy] Send_PERK_CLOSE()"); FoPost(JobWindowClose, "PerkClose", 0,  0, 0, 0, L"PERK_CLOSE"); return 1; }
FO_EXPORT int Send_PERK_OPEN_NEW(unsigned x) { Log("[Proxy] Send_PERK_OPEN_NEW(%u)", x); FoPost(JobWindowOpen, "PerkOpen(new)", 10, 0, 0, 0, L"PERK_OPEN"); return 1; }
FO_EXPORT int Send_PERK_EQUIP_SLOT(int serial, int ordinal, int perkSerial) {
    Log("[Proxy] Send_PERK_EQUIP_SLOT(serial=%u ordinal=%d perk=%u)", serial & 0xFFFF, ordinal, perkSerial & 0xFFFF);
    FoPost(JobPerkEquip, "PerkEquip", serial & 0xFFFF, ordinal, perkSerial & 0xFFFF); return 1;
}
FO_EXPORT int Send_PERK_BREAK_SLOT(int serial, int ordinal) {
    Log("[Proxy] Send_PERK_BREAK_SLOT(serial=%u ordinal=%d)", serial & 0xFFFF, ordinal);
    FoPost(JobPerkBreak, "PerkBreak", serial & 0xFFFF, ordinal); return 1;
}
FO_EXPORT int Send_PERK_INCREASE_SLOT(int serial, int count) {
    Log("[Proxy] Send_PERK_INCREASE_SLOT(serial=%u count=%d)", serial & 0xFFFF, count & 0xFF);
    FoPost(JobPerkIncrease, "PerkIncrease", serial & 0xFFFF, count & 0xFF); return 1;
}
FO_EXPORT int Send_RECIPE_OPEN()  { Log("[Proxy] Send_RECIPE_OPEN()");  FoPost(JobWindowOpen,  "RecipeOpen",  11, 0, 0, 0, L"RECIPE_OPEN");  return 1; }
FO_EXPORT int Send_RECIPE_CLOSE() { Log("[Proxy] Send_RECIPE_CLOSE()"); FoPost(JobWindowClose, "RecipeClose", 0,  0, 0, 0, L"RECIPE_CLOSE"); return 1; }
FO_EXPORT int Send_RECIPE_OPEN_NEW(unsigned x) { Log("[Proxy] Send_RECIPE_OPEN_NEW(%u)", x); FoPost(JobWindowOpen, "RecipeOpen(new)", 11, 0, 0, 0, L"RECIPE_OPEN"); return 1; }
FO_EXPORT int Send_RECIPE_ITEM(unsigned recipeId, int m1, int m2, int m3, int m4, int m5, int m6) {
    Log("[Proxy] Send_RECIPE_ITEM(recipe=%u materials=%d,%d,%d,%d,%d,%d) -> RECIPE_ITEM(recipe) (no crafting model yet)",
        recipeId, m1 & 0xFFFF, m2 & 0xFFFF, m3 & 0xFFFF, m4 & 0xFFFF, m5 & 0xFFFF, m6 & 0xFFFF);
    FoPostCbArgs("RECIPE_ITEM", 1, recipeId);
    return 1;
}

// ---------------------------------------------------------------------------
// Clan
// ---------------------------------------------------------------------------
FO_EXPORT int Send_CLAN_OPEN()  { Log("[Proxy] Send_CLAN_OPEN()");  FoPost(JobClanOpen,  "ClanOpen");  return 1; }
FO_EXPORT int Send_CLAN_CLOSE() { Log("[Proxy] Send_CLAN_CLOSE()"); FoPost(JobClanClose, "ClanClose"); return 1; }
FO_EXPORT int Send_CLAN_CHECK_CLANNAME(const wchar_t* name) {
    wchar_t n[16]; SafeWCopy(n, 16, name);
    Log("[Proxy] Send_CLAN_CHECK_CLANNAME(\"%ls\") -> available", n);
    FoPostCbArgs("CLAN_CHECK_CLANNAME", 1, 1);
    return 1;
}
FO_EXPORT int Send_CLAN_CHECK_MARK(int m0, int m1, int m2, int m3) {
    Log("[Proxy] Send_CLAN_CHECK_MARK(%d,%d,%d,%d) -> available", m0 & 0xFFFF, m1 & 0xFFFF, m2 & 0xFFFF, m3 & 0xFFFF);
    FoPostCbArgs("CLAN_CHECK_MARK", 1, 1);
    return 1;
}
FO_EXPORT int Send_CLAN_CREATE(const wchar_t* name, int m0, int m1, int m2, int m3) {
    wchar_t n[16]; SafeWCopy(n, 16, name);
    Log("[Proxy] Send_CLAN_CREATE(\"%ls\", %d,%d,%d,%d)", n, m0 & 0xFFFF, m1 & 0xFFFF, m2 & 0xFFFF, m3 & 0xFFFF);
    if (!n[0]) { FoPostCbArgs("NOTIFY_ERROR", 1, 0xC353); return 1; }
    FoPost(JobClanCreate, "ClanCreate", m0 & 0xFFFF, m1 & 0xFFFF, m2 & 0xFFFF, m3 & 0xFFFF, n);
    return 1;
}
FO_EXPORT int Send_CLAN_CHANGE_MARK(int m0, int m1, int m2, int m3) {
    Log("[Proxy] Send_CLAN_CHANGE_MARK(%d,%d,%d,%d)", m0 & 0xFFFF, m1 & 0xFFFF, m2 & 0xFFFF, m3 & 0xFFFF);
    FoPost(JobClanChangeMark, "ClanChangeMark", m0 & 0xFFFF, m1 & 0xFFFF, m2 & 0xFFFF, m3 & 0xFFFF);
    return 1;
}
FO_EXPORT int Send_CLAN_CHANGE_CLANNAME(const wchar_t* name) {
    wchar_t n[16]; SafeWCopy(n, 16, name);
    Log("[Proxy] Send_CLAN_CHANGE_CLANNAME(\"%ls\")", n);
    if (n[0] && g_FoClan.has) { WCopy(g_FoClan.name, 16, n); g_FoClan.id = FoClanIdFromName(n); }
    FoPost(JobClanRefresh, "ClanRefresh(name)", 1 | 4);
    return 1;
}
FO_EXPORT int Send_CLAN_UPDATE_NOTICE(const wchar_t* text) {
    wchar_t t[200]; SafeWCopy(t, 200, text);
    Log("[Proxy] Send_CLAN_UPDATE_NOTICE(\"%ls\")", t);
    if (g_FoClan.has) WCopy(g_FoClan.notice, 200, t);
    FoPost(JobClanRefresh, "ClanRefresh(notice)", 1);
    return 1;
}
FO_EXPORT int Send_CLAN_UPDATE_INTRO(const wchar_t* text) {
    wchar_t t[206]; SafeWCopy(t, 206, text);
    Log("[Proxy] Send_CLAN_UPDATE_INTRO(\"%ls\")", t);
    if (g_FoClan.has) WCopy(g_FoClan.intro, 206, t);
    FoPost(JobClanRefresh, "ClanRefresh(intro)", 1);
    return 1;
}
FO_EXPORT int Send_CLAN_DELETE() {
    Log("[Proxy] Send_CLAN_DELETE() -> clan cleared");
    FoClanClear();
    FoPost(JobClanRefresh, "ClanRefresh(delete)", 2 | 1 | 4);
    return 1;
}
FO_EXPORT int Send_CLAN_OUT() {
    Log("[Proxy] Send_CLAN_OUT() -> left the clan");
    FoClanClear();
    FoPost(JobClanRefresh, "ClanRefresh(out)", 2 | 1 | 4);
    return 1;
}
FO_EXPORT int Send_CLAN_SEARCH(const wchar_t* name) {
    wchar_t n[16]; SafeWCopy(n, 16, name);
    Log("[Proxy] Send_CLAN_SEARCH(\"%ls\")", n);
    FoPost(JobClanSearch, "ClanSearch", 0, 0, 0, 0, n);
    return 1;
}
FO_EXPORT int Send_CLAN_SEARCH_MASTER(const wchar_t* name) {
    wchar_t n[16]; SafeWCopy(n, 16, name);
    Log("[Proxy] Send_CLAN_SEARCH_MASTER(\"%ls\")", n);
    FoPost(JobClanSearch, "ClanSearch(master)", 0, 0, 0, 0, n);
    return 1;
}
FO_EXPORT int Send_CLAN_REQUEST(const wchar_t* clanName, int clanId) {
    wchar_t n[22]; SafeWCopy(n, 22, clanName);
    Log("[Proxy] Send_CLAN_REQUEST(\"%ls\", id=%d) -> pending request", n, clanId);
    FoPost(JobClanRequest, "ClanRequest", clanId, 0, 0, 0, n);
    return 1;
}
FO_EXPORT int Send_CLAN_REQUEST_CANCEL() {
    Log("[Proxy] Send_CLAN_REQUEST_CANCEL()");
    FoPost(JobClanRequestCancel, "ClanRequestCancel");
    return 1;
}
FO_EXPORT int Send_CLAN_REQUEST_ACCEPT(int applicantId) {
    const uint32_t id = (uint32_t)(applicantId & 0xFFFF);
    Log("[Proxy] Send_CLAN_REQUEST_ACCEPT(%u)", id);
    FoClanMember m;
    if (FoClanApplicantTake(id, &m)) FoClanAddMember(m.id, m.name, m.level, 9, m.online);
    FoPost(JobClanRefresh, "ClanRefresh(accept)", 8 | 2 | 1);
    return 1;
}
FO_EXPORT int Send_CLAN_REQUEST_REFUSE(int applicantId) {
    const uint32_t id = (uint32_t)(applicantId & 0xFFFF);
    Log("[Proxy] Send_CLAN_REQUEST_REFUSE(%u)", id);
    FoClanApplicantTake(id, nullptr);
    FoPost(JobClanRefresh, "ClanRefresh(refuse)", 8);
    return 1;
}
FO_EXPORT int Send_CLAN_KICK_OUT(int memberId) {
    const uint32_t id = (uint32_t)(memberId & 0xFFFF);
    Log("[Proxy] Send_CLAN_KICK_OUT(%u)", id);
    if (id != g_Me.userId) FoClanMemberRemove(id);
    FoPost(JobClanRefresh, "ClanRefresh(kick)", 2 | 1);
    return 1;
}
FO_EXPORT int Send_CLAN_CHANGE_MASTER(int memberId) {
    const uint32_t id = (uint32_t)(memberId & 0xFFFF);
    Log("[Proxy] Send_CLAN_CHANGE_MASTER(%u)", id);
    FoClanMember* nm = FoClanMemberById(id);
    if (nm) {
        for (size_t i = 0; i < g_FoClan.memberCount; i++) if (g_FoClan.members[i].position == 1) g_FoClan.members[i].position = 2;
        nm->position = 1;
        WCopy(g_FoClan.master, 16, nm->name);
    }
    FoPost(JobClanRefresh, "ClanRefresh(master)", 2 | 1);
    return 1;
}
FO_EXPORT int Send_CLAN_MEMBER_POSITION(int memberId, int position) {
    const uint32_t id = (uint32_t)(memberId & 0xFFFF);
    Log("[Proxy] Send_CLAN_MEMBER_POSITION(%u, %d)", id, position);
    FoClanMember* m = FoClanMemberById(id);
    if (m && (position == 2 || position == 8 || position == 9)) m->position = (uint32_t)position;
    FoPost(JobClanRefresh, "ClanRefresh(position)", 2 | 1);
    return 1;
}
FO_EXPORT int Send_CLAN_INVITE(int userId) {
    Log("[Proxy] Send_CLAN_INVITE(%u) -> acknowledged (nobody to invite offline)", (unsigned)(userId & 0xFFFF));
    return 1;
}

// ---------------------------------------------------------------------------
// Mail (post) / gifts
// ---------------------------------------------------------------------------
FO_EXPORT int Send_POST_OPEN()  { Log("[Proxy] Send_POST_OPEN()");  FoPost(JobPostOpen,  "PostOpen");  return 1; }
FO_EXPORT int Send_POST_CLOSE() { Log("[Proxy] Send_POST_CLOSE()"); FoPost(JobPostClose, "PostClose"); return 1; }
FO_EXPORT int Send_POST_LOAD_RECEIVE_PAGE(int page) { Log("[Proxy] Send_POST_LOAD_RECEIVE_PAGE(%d)", page & 0xFFFF); FoPost(JobPostPage, "PostPage(recv)", page & 0xFFFF, 0); return 1; }
FO_EXPORT int Send_POST_LOAD_SEND_PAGE(int page)    { Log("[Proxy] Send_POST_LOAD_SEND_PAGE(%d)",    page & 0xFFFF); FoPost(JobPostPage, "PostPage(send)", page & 0xFFFF, 1); return 1; }
FO_EXPORT int Send_POST_WRITE(unsigned arg1, const wchar_t* s2, const wchar_t* s3, const wchar_t* s4, int flag) {
    wchar_t a[16], b[64], c[64];
    SafeWCopy(a, 16, s2); SafeWCopy(b, 64, s3); SafeWCopy(c, 64, s4);
    Log("[Proxy] Send_POST_WRITE(%u, \"%ls\", \"%ls\", \"%ls\", %d)", arg1, a, b, c, flag & 0xFF);
    FoPost(JobPostWrite, "PostWrite", 0, 0, 0, 0, a, b);
    return 1;
}
FO_EXPORT int Send_POST_READ(unsigned idLo, unsigned idHi) { Log("[Proxy] Send_POST_READ(%u:%u)", idHi, idLo); FoPost(JobPostRead, "PostRead", (int)idLo); return 1; }
FO_EXPORT int Send_POST_DELETE_RECEIVE(unsigned idLo, unsigned idHi) { Log("[Proxy] Send_POST_DELETE_RECEIVE(%u:%u)", idHi, idLo); FoPost(JobPostDelete, "PostDelete(recv)", (int)idLo, 0); return 1; }
FO_EXPORT int Send_POST_DELETE_SEND(unsigned idLo, unsigned idHi)    { Log("[Proxy] Send_POST_DELETE_SEND(%u:%u)",    idHi, idLo); FoPost(JobPostDelete, "PostDelete(send)", (int)idLo, 1); return 1; }
FO_EXPORT int Send_POST_CLEAR_RECEIVE() { Log("[Proxy] Send_POST_CLEAR_RECEIVE()"); FoPost(JobPostClear, "PostClear(recv)", 0, 0); return 1; }
FO_EXPORT int Send_POST_CLEAR_SEND()    { Log("[Proxy] Send_POST_CLEAR_SEND()");    FoPost(JobPostClear, "PostClear(send)", 0, 1); return 1; }
FO_EXPORT int Send_GIFT_LOAD_RECEIVE_PAGE(int page) { Log("[Proxy] Send_GIFT_LOAD_RECEIVE_PAGE(%d)", page & 0xFFFF); FoPost(JobGiftPage, "GiftPage(recv)", page & 0xFFFF, 0); return 1; }
FO_EXPORT int Send_GIFT_LOAD_SEND_PAGE(int page)    { Log("[Proxy] Send_GIFT_LOAD_SEND_PAGE(%d)",    page & 0xFFFF); FoPost(JobGiftPage, "GiftPage(send)", page & 0xFFFF, 1); return 1; }
FO_EXPORT int Send_GIFT_WRITE(unsigned arg1, const wchar_t* s2, const wchar_t* s3, unsigned itemId, int flag) {
    wchar_t a[16], b[64]; SafeWCopy(a, 16, s2); SafeWCopy(b, 64, s3);
    Log("[Proxy] Send_GIFT_WRITE(%u, \"%ls\", \"%ls\", item=%u, %d) -> GIFT_WRITE", arg1, a, b, itemId, flag & 0xFF);
    FoPostCb("GIFT_WRITE");
    return 1;
}
FO_EXPORT int Send_GIFT_READ(unsigned idLo, unsigned idHi)    { Log("[Proxy] Send_GIFT_READ(%u:%u)", idHi, idLo); FoPost(JobGiftPage, "GiftPage(read)", (int)SD32(0x1E44), 0); return 1; }
FO_EXPORT int Send_GIFT_RECEIVE(unsigned idLo, unsigned idHi) { Log("[Proxy] Send_GIFT_RECEIVE(%u:%u) -> nothing to receive", idHi, idLo); FoPost(JobGiftPage, "GiftPage(receive)", (int)SD32(0x1E44), 0); return 1; }
FO_EXPORT int Send_GIFT_DELETE_RECEIVE(unsigned idLo, unsigned idHi) { Log("[Proxy] Send_GIFT_DELETE_RECEIVE(%u:%u)", idHi, idLo); FoPost(JobGiftPage, "GiftPage(recv)", (int)SD32(0x1E44), 0); return 1; }
FO_EXPORT int Send_GIFT_DELETE_SEND(unsigned idLo, unsigned idHi)    { Log("[Proxy] Send_GIFT_DELETE_SEND(%u:%u)",    idHi, idLo); FoPost(JobGiftPage, "GiftPage(send)", (int)SD32(0x1E44), 1); return 1; }

#undef FO_EXPORT
