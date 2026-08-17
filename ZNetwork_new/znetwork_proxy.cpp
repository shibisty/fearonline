// ============================================================================
//  ZNetwork.dll proxy/stub  --  FIXED VERSION
//  С упрощенной структурой ITEM_INFO
// ============================================================================

#include <windows.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <cstdint>
#include <map>
#include <string>
#include <exception>

// ============ Логирование ============
static void Log(const char* fmt, ...) {
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
    va_end(ap);
    OutputDebugStringA(buf);
    OutputDebugStringA("\n");
}

// ============ Структуры ============
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

// Упрощенная структура для ITEM_INFO
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

// ============ Глобальные данные ============
static ServerData g_ServerData = {
    2, 30, 1, "127.0.0.1", 7777, 0x12345678, 
    (void*)0xDEADBEEF, (void*)0xCAFEBABE
};

static ServerHostData g_ServerHostData = {
    0, 0, "127.0.0.1", 7777, 0
};

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

// Упрощенные товары
static ItemInfo g_FakeItems[5] = {
    {1, "Item 1", 100, 0, 1, 1, 0, 1},
    {2, "Item 2", 200, 0, 1, 1, 0, 1},
    {3, "Item 3", 300, 0, 1, 1, 0, 1},
    {4, "Item 4", 400, 0, 1, 1, 0, 1},
    {5, "Item 5", 500, 0, 1, 1, 0, 1}
};

// ============ Хранилище колбэков ============
typedef void* CallbackPtr;
typedef std::map<std::string, CallbackPtr> CallbackMap;
static CallbackMap g_Callbacks;

static void StoreCallback(const char* name, void* callback) {
    if (callback) {
        g_Callbacks[name] = callback;
    }
}

static void* GetCallback(const char* name) {
    auto it = g_Callbacks.find(name);
    return (it != g_Callbacks.end()) ? it->second : nullptr;
}

template<typename T>
static void CallCallback(const char* name, T* data) {
    void* cb = GetCallback(name);
    if (cb) {
        Log("[Proxy] Calling %s", name);
        typedef void (__stdcall *CallbackFunc)(T*);
        ((CallbackFunc)cb)(data);
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

// ============ Основные экспорты ============
// extern "C" __declspec(dllexport) void* GetServerData() {
//     return &g_ServerData;
// }
extern "C" __declspec(dllexport) void* GetServerData() {
    Log("[Proxy] GetServerData() -> 0x%p", &g_ServerData);
    // Убедитесь, что все поля заполнены
    g_ServerData.connectionStatus = 2;  // 2 = подключен
    g_ServerData.ping = 30;
    g_ServerData.serverId = 1;
    strcpy_s(g_ServerData.serverIp, "127.0.0.1");
    g_ServerData.serverPort = 7777;
    g_ServerData.sessionKey = 0x12345678;
    g_ServerData.pNetworkManager = (void*)0xDEADBEEF;
    g_ServerData.pUserSession = (void*)0xCAFEBABE;
    return &g_ServerData;
}

extern "C" __declspec(dllexport) void* GetServerHostData() {
    return &g_ServerHostData;
}

extern "C" __declspec(dllexport) void Tick() {}

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
    
    g_ServerData.connectionStatus = 2;
    g_ServerData.ping = 30;
    g_ServerData.serverId = 1;
    strcpy_s(g_ServerData.serverIp, "127.0.0.1");
    g_ServerData.serverPort = 7777;
    g_ServerData.sessionKey = 0x12345678;
    g_ServerData.pNetworkManager = (void*)0xDEADBEEF;
    g_ServerData.pUserSession = (void*)0xCAFEBABE;
    
    Log("[Proxy] ServerData updated");
    
    CallCallback<LoginData>("LOGIN_DATA", &g_FakeLoginData);
    CallCallback<UserInfo>("MY_USER_INFO", &g_FakeUserInfo);
    CallCallback<GateServerInfo>("GATE_SERVER_INFO", &g_FakeGateServerInfo);
    CallCallbackArray("GATE_CHANNEL_INFO", g_FakeChannels, 3);
    CallCallback<GateLobbyInfo>("GATE_LOBBY_INFO", &g_FakeGateLobbyInfo);
    CallCallback<GateServerInfo>("GATE_SERVER_JOIN", &g_FakeGateServerInfo);
    CallCallback<GateChannelInfo>("GATE_CHANNEL_JOIN", &g_FakeChannels[0]);
    CallCallback<GateLobbyInfo>("LOBBY_JOIN", &g_FakeGateLobbyInfo);
    CallCallback<UserInfo>("LOBBY_USER_INFO", &g_FakeUserInfo);
    CallCallbackArray("LOBBY_ROOM_INFO", g_FakeRooms, 3);
    // CallCallback<LoginFailData>("LOGIN_FAIL", &g_FakeLoginFail);
    
    // ITEM_INFO - передаем только 5 предметов с упрощенной структурой
    CallCallbackArray("ITEM_INFO", g_FakeItems, 5);
    
    // EXPIRE_ITEM_INFO и REWARD_ITEM_INFO пока пропускаем
    // CallCallbackVoid("EXPIRE_ITEM_INFO");
    // CallCallbackVoid("REWARD_ITEM_INFO");
    
    Log("[Proxy] ========================================");
    Log("[Proxy] LOGIN COMPLETE - FAKE SUCCESS");
    Log("[Proxy] ========================================");
    return 1;
}

// ============ XignCodeInit ============
extern "C" __declspec(dllexport) void XignCodeInit() {
    Log("[Proxy] XignCodeInit()");
}

// ============ Set_On_* функции ============
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

// ============ Специальные функции ============
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

// ============ Send_* функции ============
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

// ============ DllMain ============
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        
        SetUnhandledExceptionFilter([](EXCEPTION_POINTERS* ex) -> LONG {
            Log("[Proxy] ========================================");
            Log("[Proxy] UNHANDLED EXCEPTION!");
            Log("[Proxy] ExceptionCode: 0x%08X", ex->ExceptionRecord->ExceptionCode);
            Log("[Proxy] ExceptionAddress: 0x%p", ex->ExceptionRecord->ExceptionAddress);
            if (ex->ExceptionRecord->ExceptionCode == 0xC0000005) {
                Log("[Proxy] Access Violation! Address: 0x%p", 
                    ex->ExceptionRecord->ExceptionInformation[1]);
            }
            Log("[Proxy] ========================================");
            return EXCEPTION_EXECUTE_HANDLER;
        });
        
        Log("[Proxy] ========================================");
        Log("[Proxy] ZNetwork PROXY LOADED (standalone mode)");
        Log("[Proxy] ========================================");
    }
    return TRUE;
}
