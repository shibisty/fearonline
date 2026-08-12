// ============================================================================
//  ZNetwork.dll proxy/stub  --  auto-generated (v3)
//
//  - GetServerData, GetServerHostData, init: 0 параметров (подтверждено
//    декомпиляцией GameClient.dll, FUN_1034c4d0), СВОЯ реализация,
//    возвращают 0 (эксперимент -- вариант 1, минимальный риск).
//  - 6 функций с подтверждённой сигнатурой (LoginServer_EX и т.д.) --
//    своя типизированная реализация, форвардит в оригинал.
//  - Все остальные -- безопасный naked-трамплин (сохраняет регистры,
//    логирует и tail-jmp в оригинал, calling convention не важен).
//
//   ТРЕБОВАНИЕ: ZNetwork_orig.dll (оригинал, переименованный) должен лежать
//   рядом с этой proxy-DLL.
//
//   Сборка (x86/Win32 Developer Command Prompt):
//     cl /LD /MT znetwork_proxy.cpp /link /DEF:znetwork_proxy.def /OUT:ZNetwork.dll
// ============================================================================
#include <windows.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>

static HMODULE g_hOrig = NULL;
static void EnsureInitialized(); // forward declaration

static void Log(const char* fmt, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
    va_end(ap);
    OutputDebugStringA(buf);
}


// ------------- Свои реализации: 0 параметров -------------

static BYTE g_buf_GetServerData[4096] = {0};
extern "C" DWORD __stdcall GetServerData() {
    EnsureInitialized();
    Log("[ZNetworkProxy][CUSTOM] GetServerData() -> 0x%p (zeroed 4096-byte buffer)\n", g_buf_GetServerData);
    return (DWORD)(void*)g_buf_GetServerData;
}

static BYTE g_buf_GetServerHostData[4096] = {0};
extern "C" DWORD __stdcall GetServerHostData() {
    EnsureInitialized();
    Log("[ZNetworkProxy][CUSTOM] GetServerHostData() -> 0x%p (zeroed 4096-byte buffer)\n", g_buf_GetServerHostData);
    return (DWORD)(void*)g_buf_GetServerHostData;
}

extern "C" DWORD __stdcall init() {
    EnsureInitialized();
    Log("[ZNetworkProxy][CUSTOM] init() -> 0\n");
    return 0;
}
static FARPROC g_real[419];
static const char* g_names[] = {
    "DediDisconnect",
    "DediStartConnect_EX",
    "GetRoomCount",
    "Get_On_GAME_AI_DEATH_Callback",
    "Get_On_GAME_AI_KILL_Callback",
    "Get_On_GAME_BEGIN_ROUND_Callback",
    "Get_On_GAME_BOMB_Callback",
    "Get_On_GAME_CHECK_BODY_Callback",
    "Get_On_GAME_END_Callback",
    "Get_On_GAME_GHOST_Callback",
    "Get_On_GAME_GHOST_KING_Callback",
    "Get_On_GAME_HOST_CHANGE_Callback",
    "Get_On_GAME_INTRUDE_Callback",
    "Get_On_GAME_LEAVE_Callback",
    "Get_On_GAME_LOAD_COMPLETE_Callback",
    "Get_On_GAME_OCCUPY_Callback",
    "Get_On_GAME_PVE_DATANET_Callback",
    "Get_On_GAME_PVE_DOLL_Callback",
    "Get_On_GAME_PVE_HELP_Callback",
    "Get_On_GAME_RESPAWN_Callback",
    "Get_On_GAME_REWARD_Callback",
    "Get_On_GAME_ROUND_END_Callback",
    "Get_On_GAME_SCORE_Callback",
    "Get_On_GAME_START_Callback",
    "Get_On_GAME_START_PRE_Callback",
    "Get_On_GAME_SUPER_DEFUSING_KIT_Callback",
    "Get_On_GAME_TEAM_CHANGE_Callback",
    "Get_On_GAME_USER_Callback",
    "Get_On_GAME_USER_DEATH_Callback",
    "Get_On_GAME_USER_KILL_Callback",
    "Get_On_GAME_USER_LEAVE_Callback",
    "Get_On_HOST_GAME_AI_DEATH_Callback",
    "Get_On_HOST_GAME_AI_KILL_Callback",
    "Get_On_HOST_GAME_BEGIN_ROUND_Callback",
    "Get_On_HOST_GAME_BOMB_Callback",
    "Get_On_HOST_GAME_CHECK_BODY_Callback",
    "Get_On_HOST_GAME_END_Callback",
    "Get_On_HOST_GAME_GHOST_Callback",
    "Get_On_HOST_GAME_GHOST_KING_Callback",
    "Get_On_HOST_GAME_LEAVE_Callback",
    "Get_On_HOST_GAME_LOAD_COMPLETE_Callback",
    "Get_On_HOST_GAME_OCCUPY_Callback",
    "Get_On_HOST_GAME_PVE_DATANET_Callback",
    "Get_On_HOST_GAME_PVE_DOLL_Callback",
    "Get_On_HOST_GAME_PVE_HELP_Callback",
    "Get_On_HOST_GAME_PVE_RANK_Callback",
    "Get_On_HOST_GAME_RESPAWN_Callback",
    "Get_On_HOST_GAME_ROUND_END_Callback",
    "Get_On_HOST_GAME_SCORE_Callback",
    "Get_On_HOST_GAME_START_Callback",
    "Get_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback",
    "Get_On_HOST_GAME_TEAM_CHANGE_Callback",
    "Get_On_HOST_GAME_USER_Callback",
    "Get_On_HOST_GAME_USER_DEATH_Callback",
    "Get_On_HOST_GAME_USER_EQUIP_Callback",
    "Get_On_HOST_GAME_USER_ITEM_Callback",
    "Get_On_HOST_GAME_USER_KILL_Callback",
    "Get_On_HOST_GAME_USER_LEAVE_Callback",
    "Get_On_HOST_GAME_USER_LOAD_COMPLETE_Callback",
    "Get_On_HOST_GAME_WEAPON_CHANGE_Callback",
    "Get_On_HOST_GAME_ZOMBIE_Callback",
    "Send_CAPSULE_BUY",
    "Send_CAPSULE_CLOSE",
    "Send_CAPSULE_OPEN",
    "Send_CHAT_ALL",
    "Send_CHAT_CLAN",
    "Send_CHAT_CLAN_MANAGER",
    "Send_CHAT_TEAM",
    "Send_CHAT_WHISPER",
    "Send_CHELLENGE_CLEAR",
    "Send_CLAN_CHANGE_CLANNAME",
    "Send_CLAN_CHANGE_MARK",
    "Send_CLAN_CHANGE_MASTER",
    "Send_CLAN_CHECK_CLANNAME",
    "Send_CLAN_CHECK_MARK",
    "Send_CLAN_CLOSE",
    "Send_CLAN_CREATE",
    "Send_CLAN_DELETE",
    "Send_CLAN_INVITE",
    "Send_CLAN_KICK_OUT",
    "Send_CLAN_MATCH_CANCEL",
    "Send_CLAN_MATCH_CREATE",
    "Send_CLAN_MATCH_START",
    "Send_CLAN_MEMBER_POSITION",
    "Send_CLAN_OPEN",
    "Send_CLAN_OUT",
    "Send_CLAN_REQUEST",
    "Send_CLAN_REQUEST_ACCEPT",
    "Send_CLAN_REQUEST_CANCEL",
    "Send_CLAN_REQUEST_REFUSE",
    "Send_CLAN_SEARCH",
    "Send_CLAN_SEARCH_MASTER",
    "Send_CLAN_UPDATE_INTRO",
    "Send_CLAN_UPDATE_NOTICE",
    "Send_DAILY_MISSION_CLEAR",
    "Send_DAILY_MISSION_INFO",
    "Send_DAILY_MISSION_START",
    "Send_FRIEND_DELETE",
    "Send_FRIEND_INVITE",
    "Send_FRIEND_REQUEST",
    "Send_FRIEND_REQUEST_ACCEPT",
    "Send_FRIEND_REQUEST_REFUSE",
    "Send_GAME_HIT_COUNT",
    "Send_GAME_LEAVE",
    "Send_GAME_LOAD_PERCENT",
    "Send_GAME_PING",
    "Send_GAME_START_PRE_COMPLETE",
    "Send_GATE_CHANNEL_JOIN",
    "Send_GATE_LOBBY_INFO",
    "Send_GATE_LOBBY_JOIN",
    "Send_GATE_SERVER_INFO",
    "Send_GATE_SERVER_JOIN",
    "Send_GIFT_DELETE_RECEIVE",
    "Send_GIFT_DELETE_SEND",
    "Send_GIFT_LOAD_RECEIVE_PAGE",
    "Send_GIFT_LOAD_SEND_PAGE",
    "Send_GIFT_READ",
    "Send_GIFT_RECEIVE",
    "Send_GIFT_WRITE",
    "Send_GM_BAN_USER",
    "Send_GM_KICK_USER",
    "Send_GM_NOTICE_MESSAGE",
    "Send_GM_QUIET_USER",
    "Send_GM_WARN_MESSAGE",
    "Send_HANGAR_BREAK_CUSTOM_SLOT",
    "Send_HANGAR_BREAK_WEAR",
    "Send_HANGAR_BUY_CUSTOM_SLOT",
    "Send_HANGAR_CLOSE",
    "Send_HANGAR_CUSTOM_BREAK",
    "Send_HANGAR_CUSTOM_BUY",
    "Send_HANGAR_CUSTOM_EQUIP",
    "Send_HANGAR_EQUIP_CUSTOM_SLOT",
    "Send_HANGAR_EQUIP_WEAR",
    "Send_HANGAR_ITEM_BUY",
    "Send_HANGAR_ITEM_DELETE",
    "Send_HANGAR_ITEM_EQUIP",
    "Send_HANGAR_ITEM_EXPIRE",
    "Send_HANGAR_ITEM_REPAIR",
    "Send_HANGAR_ITEM_RESELL",
    "Send_HANGAR_ITEM_USE",
    "Send_HANGAR_JOIN",
    "Send_HANGAR_LEAVE",
    "Send_HANGAR_MONEY",
    "Send_HANGAR_NICKNAME_CHANGE",
    "Send_HANGAR_OPEN",
    "Send_HANGAR_PARTS_SLOT",
    "Send_HANGAR_RECIPE_ITEM",
    "Send_HANGAR_WEAPON_CUSTOM_SLOT",
    "Send_HANGAR_WEAPON_SLOT",
    "Send_HANGAR_WEAR_SLOT",
    "Send_HOST_DAILY_MISSION_CLEAR",
    "Send_HOST_GAME_BEGIN_ROUND",
    "Send_HOST_GAME_BOMB",
    "Send_HOST_GAME_CHECK_BODY",
    "Send_HOST_GAME_GHOST",
    "Send_HOST_GAME_INSTANT_RESPAWN",
    "Send_HOST_GAME_LEAVE",
    "Send_HOST_GAME_MUST_LOAD_COMPLETE",
    "Send_HOST_GAME_OCCUPY",
    "Send_HOST_GAME_PING",
    "Send_HOST_GAME_PVE_DATANET",
    "Send_HOST_GAME_PVE_DOLL",
    "Send_HOST_GAME_PVE_HELP",
    "Send_HOST_GAME_PVE_RANK",
    "Send_HOST_GAME_RESPAWN",
    "Send_HOST_GAME_RESPAWN_COMPLETE",
    "Send_HOST_GAME_ROUND_END_PVE",
    "Send_HOST_GAME_START_WATCH",
    "Send_HOST_GAME_SUPER_DEFUSING_KIT",
    "Send_HOST_GAME_TIMEOUT",
    "Send_HOST_GAME_USER_DEATH",
    "Send_HOST_GAME_USER_EQUIP",
    "Send_HOST_GAME_USER_ITEM",
    "Send_HOST_GAME_USER_KILL",
    "Send_HOST_GAME_WEAPON_CHANGE",
    "Send_HOST_GAME_ZOMBIE",
    "Send_LOBBY_CLAN_ROOM_INFO",
    "Send_LOBBY_LEAVE",
    "Send_LOBBY_MODE_MAP",
    "Send_LOBBY_QUICK_JOIN",
    "Send_LOBBY_REFRESH_CLAN_ROOM_INFO",
    "Send_LOBBY_REFRESH_ROOM_INFO",
    "Send_LOBBY_ROOM_CREATE",
    "Send_LOBBY_ROOM_CREATE_EX",
    "Send_LOBBY_ROOM_ENTER",
    "Send_LOBBY_ROOM_INFO",
    "Send_LOBBY_ROOM_JOIN",
    "Send_LOBBY_ROOM_MAKE",
    "Send_LOBBY_USER_INFO",
    "Send_MY_GUIDE_CLEAR",
    "Send_MY_NICKNAME_CREATE",
    "Send_PERK_BREAK_SLOT",
    "Send_PERK_CLOSE",
    "Send_PERK_EQUIP_SLOT",
    "Send_PERK_INCREASE_SLOT",
    "Send_PERK_OPEN",
    "Send_PERK_OPEN_NEW",
    "Send_POST_CLEAR_RECEIVE",
    "Send_POST_CLEAR_SEND",
    "Send_POST_CLOSE",
    "Send_POST_DELETE_RECEIVE",
    "Send_POST_DELETE_SEND",
    "Send_POST_LOAD_RECEIVE_PAGE",
    "Send_POST_LOAD_SEND_PAGE",
    "Send_POST_OPEN",
    "Send_POST_READ",
    "Send_POST_WRITE",
    "Send_PURCHASE_FINALIZE",
    "Send_PURCHASE_INIT",
    "Send_RECIPE_CLOSE",
    "Send_RECIPE_ITEM",
    "Send_RECIPE_OPEN",
    "Send_RECIPE_OPEN_NEW",
    "Send_REPORT_USER",
    "Send_ROOM_DETAIL_SETTING",
    "Send_ROOM_DETAIL_SETTING_EX",
    "Send_ROOM_GAME_READY",
    "Send_ROOM_GAME_START",
    "Send_ROOM_INVITE",
    "Send_ROOM_INVITE_USER_INFO",
    "Send_ROOM_LEAVE",
    "Send_ROOM_MAP",
    "Send_ROOM_MAP_CHANGE",
    "Send_ROOM_MATCH_CANCEL",
    "Send_ROOM_MATCH_JOIN",
    "Send_ROOM_MATCH_START",
    "Send_ROOM_SETTING",
    "Send_ROOM_USER_TEAM",
    "Send_SEARCH_USER",
    "Send_TUTORIAL_CLEAR",
    "Send_TUTORIAL_CLOSE",
    "Send_TUTORIAL_OPEN",
    "Set_Command_Enum",
    "Set_OnIoConnectedCallback",
    "Set_OnIoDisonnectedCallback",
    "Set_On_CAPSULE_BUY_Callback",
    "Set_On_CAPSULE_CLOSE_Callback",
    "Set_On_CAPSULE_NOTICE_Callback",
    "Set_On_CAPSULE_OPEN_Callback",
    "Set_On_CHAT_Callback",
    "Set_On_CHELLENGE_CLEAR_FAIL_Callback",
    "Set_On_CHELLENGE_CLEAR_SUCCESS_Callback",
    "Set_On_CHELLENGE_MEDAL_GET_Callback",
    "Set_On_CLAN_CHANGE_MARK_Callback",
    "Set_On_CLAN_CHECK_CLANNAME_Callback",
    "Set_On_CLAN_CHECK_MARK_Callback",
    "Set_On_CLAN_CLOSE_Callback",
    "Set_On_CLAN_DELETE_Callback",
    "Set_On_CLAN_INFO_Callback",
    "Set_On_CLAN_INVITE_Callback",
    "Set_On_CLAN_KICK_OUT_Callback",
    "Set_On_CLAN_MATCH_CANCEL_Callback",
    "Set_On_CLAN_MATCH_START_Callback",
    "Set_On_CLAN_MEMBER_INFO_Callback",
    "Set_On_CLAN_OPEN_Callback",
    "Set_On_CLAN_OUT_Callback",
    "Set_On_CLAN_REQUEST_CANCEL_Callback",
    "Set_On_CLAN_REQUEST_Callback",
    "Set_On_CLAN_REQUEST_INFO_Callback",
    "Set_On_CLAN_REQUEST_USER_Callback",
    "Set_On_CLAN_SEARCH_Callback",
    "Set_On_Command_Enum_Callback",
    "Set_On_DAILY_MISSION_CLEAR_FAIL_Callback",
    "Set_On_DAILY_MISSION_CLEAR_SUCCESS_Callback",
    "Set_On_DAILY_MISSION_INFO_Callback",
    "Set_On_DAILY_MISSION_NEW_INFO_Callback",
    "Set_On_DAILY_MISSION_START_FAIL_Callback",
    "Set_On_DAILY_MISSION_START_SUCCESS_Callback",
    "Set_On_DEDI_DISCONNECT_Callback",
    "Set_On_DEDI_LOGIN_DATA_Callback",
    "Set_On_EXPIRE_ITEM_INFO_Callback",
    "Set_On_FRIEND_INVITE_Callback",
    "Set_On_FRIEND_LIST_Callback",
    "Set_On_FRIEND_REQUEST_Callback",
    "Set_On_FRIEND_REQUEST_LIST_Callback",
    "Set_On_GAME_AI_DEATH_Callback",
    "Set_On_GAME_AI_KILL_Callback",
    "Set_On_GAME_BEGIN_ROUND_Callback",
    "Set_On_GAME_BOMB_Callback",
    "Set_On_GAME_CHECK_BODY_Callback",
    "Set_On_GAME_END_Callback",
    "Set_On_GAME_GHOST_Callback",
    "Set_On_GAME_GHOST_KING_Callback",
    "Set_On_GAME_HOST_CHANGE_Callback",
    "Set_On_GAME_INTRUDE_Callback",
    "Set_On_GAME_LEAVE_Callback",
    "Set_On_GAME_LOAD_COMPLETE_Callback",
    "Set_On_GAME_OCCUPY_Callback",
    "Set_On_GAME_PVE_DATANET_Callback",
    "Set_On_GAME_PVE_DOLL_Callback",
    "Set_On_GAME_PVE_HELP_Callback",
    "Set_On_GAME_RESPAWN_Callback",
    "Set_On_GAME_REWARD_Callback",
    "Set_On_GAME_ROUND_END_Callback",
    "Set_On_GAME_SCORE_Callback",
    "Set_On_GAME_START_Callback",
    "Set_On_GAME_START_PRE_Callback",
    "Set_On_GAME_SUPER_DEFUSING_KIT_Callback",
    "Set_On_GAME_TEAM_CHANGE_Callback",
    "Set_On_GAME_USER_Callback",
    "Set_On_GAME_USER_DEATH_Callback",
    "Set_On_GAME_USER_KILL_Callback",
    "Set_On_GAME_USER_LEAVE_Callback",
    "Set_On_GATE_CHANNEL_INFO_Callback",
    "Set_On_GATE_CHANNEL_JOIN_Callback",
    "Set_On_GATE_LOBBY_INFO_Callback",
    "Set_On_GATE_SERVER_INFO_Callback",
    "Set_On_GATE_SERVER_JOIN_Callback",
    "Set_On_GIFT_RECEIVE_Callback",
    "Set_On_GIFT_RECEIVE_PAGE_INFO_Callback",
    "Set_On_GIFT_SEND_PAGE_INFO_Callback",
    "Set_On_GIFT_WRITE_Callback",
    "Set_On_GM_BAN_USER_BY_USER_Callback",
    "Set_On_GM_KICK_USER_BY_USER_Callback",
    "Set_On_GM_QUIET_USER_BY_USER_Callback",
    "Set_On_GM_WARN_MESSAGE_BY_USER_Callback",
    "Set_On_HANGAR_BREAK_WEAR_Callback",
    "Set_On_HANGAR_CLOSE_Callback",
    "Set_On_HANGAR_CUSTOM_BREAK_Callback",
    "Set_On_HANGAR_CUSTOM_BUY_Callback",
    "Set_On_HANGAR_CUSTOM_EQUIP_Callback",
    "Set_On_HANGAR_CUSTOM_SLOT_Callback",
    "Set_On_HANGAR_EQUIP_WEAR_Callback",
    "Set_On_HANGAR_ITEM_BUY_Callback",
    "Set_On_HANGAR_ITEM_DELETE_Callback",
    "Set_On_HANGAR_ITEM_EQUIP_Callback",
    "Set_On_HANGAR_ITEM_EXPIRE_Callback",
    "Set_On_HANGAR_ITEM_REPAIR_Callback",
    "Set_On_HANGAR_ITEM_RESELL_Callback",
    "Set_On_HANGAR_ITEM_USE_Callback",
    "Set_On_HANGAR_JOIN_Callback",
    "Set_On_HANGAR_LEAVE_Callback",
    "Set_On_HANGAR_MONEY_Callback",
    "Set_On_HANGAR_NICKNAME_CHANGE_Callback",
    "Set_On_HANGAR_OPEN_Callback",
    "Set_On_HANGAR_PARTS_SLOT_Callback",
    "Set_On_HANGAR_RECIPE_ITEM_Callback",
    "Set_On_HANGAR_WEAPON_SLOT_Callback",
    "Set_On_HANGAR_WEAR_SLOT_Callback",
    "Set_On_HOST_GAME_AI_DEATH_Callback",
    "Set_On_HOST_GAME_AI_KILL_Callback",
    "Set_On_HOST_GAME_BEGIN_ROUND_Callback",
    "Set_On_HOST_GAME_BOMB_Callback",
    "Set_On_HOST_GAME_CHECK_BODY_Callback",
    "Set_On_HOST_GAME_END_Callback",
    "Set_On_HOST_GAME_GHOST_Callback",
    "Set_On_HOST_GAME_GHOST_KING_Callback",
    "Set_On_HOST_GAME_LEAVE_Callback",
    "Set_On_HOST_GAME_LOAD_COMPLETE_Callback",
    "Set_On_HOST_GAME_OCCUPY_Callback",
    "Set_On_HOST_GAME_PVE_DATANET_Callback",
    "Set_On_HOST_GAME_PVE_DOLL_Callback",
    "Set_On_HOST_GAME_PVE_HELP_Callback",
    "Set_On_HOST_GAME_PVE_RANK_Callback",
    "Set_On_HOST_GAME_RESPAWN_Callback",
    "Set_On_HOST_GAME_ROUND_END_Callback",
    "Set_On_HOST_GAME_SCORE_Callback",
    "Set_On_HOST_GAME_START_Callback",
    "Set_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback",
    "Set_On_HOST_GAME_TEAM_CHANGE_Callback",
    "Set_On_HOST_GAME_USER_Callback",
    "Set_On_HOST_GAME_USER_DEATH_Callback",
    "Set_On_HOST_GAME_USER_EQUIP_Callback",
    "Set_On_HOST_GAME_USER_ITEM_Callback",
    "Set_On_HOST_GAME_USER_KILL_Callback",
    "Set_On_HOST_GAME_USER_LEAVE_Callback",
    "Set_On_HOST_GAME_USER_LOAD_COMPLETE_Callback",
    "Set_On_HOST_GAME_WEAPON_CHANGE_Callback",
    "Set_On_HOST_GAME_ZOMBIE_Callback",
    "Set_On_ITEM_INFO_Callback",
    "Set_On_LOBBY_ALL_MODE_MAP_Callback",
    "Set_On_LOBBY_CLAN_ROOM_INFO_Callback",
    "Set_On_LOBBY_INVITE_Callback",
    "Set_On_LOBBY_JOIN_Callback",
    "Set_On_LOBBY_MODE_MAP_Callback",
    "Set_On_LOBBY_QUICK_JOIN_FAILED_Callback",
    "Set_On_LOBBY_ROOM_INFO_Callback",
    "Set_On_LOBBY_USER_INFO_Callback",
    "Set_On_LOGIN_DATA_Callback",
    "Set_On_LOGIN_FAIL_Callback",
    "Set_On_MY_ATTENDANCE_Callback",
    "Set_On_MY_NICKNAME_CREATE_Callback",
    "Set_On_MY_USER_INFO_Callback",
    "Set_On_NICKNAME_IN_USE_Callback",
    "Set_On_NOTICE_CHAT_Callback",
    "Set_On_NOTIFY_ERROR_Callback",
    "Set_On_NOTIFY_ERROR_EXIT_Callback",
    "Set_On_PERK_BREAK_SLOT_Callback",
    "Set_On_PERK_CLOSE_Callback",
    "Set_On_PERK_EQUIP_SLOT_Callback",
    "Set_On_PERK_INCREASE_SLOT_Callback",
    "Set_On_PERK_OPEN_Callback",
    "Set_On_POST_CLOSE_Callback",
    "Set_On_POST_OPEN_Callback",
    "Set_On_POST_RECEIVE_PAGE_INFO_Callback",
    "Set_On_POST_SEND_PAGE_INFO_Callback",
    "Set_On_POST_UNREAD_Callback",
    "Set_On_POST_WRITE_Callback",
    "Set_On_PURCHASE_FINALIZE_Callback",
    "Set_On_RECIPE_CLOSE_Callback",
    "Set_On_RECIPE_ITEM_Callback",
    "Set_On_RECIPE_OPEN_Callback",
    "Set_On_REPORT_Callback",
    "Set_On_REWARD_ITEM_INFO_Callback",
    "Set_On_ROOM_EVENT_MAP_INFO_Callback",
    "Set_On_ROOM_INFO_Callback",
    "Set_On_ROOM_INVITE_USER_INFO_Callback",
    "Set_On_ROOM_JOIN_Callback",
    "Set_On_ROOM_MATCH_CANCEL_Callback",
    "Set_On_ROOM_MATCH_INFO_Callback",
    "Set_On_ROOM_MATCH_JOIN_Callback",
    "Set_On_ROOM_OPEN_MAP_INFO_Callback",
    "Set_On_ROOM_USER_INFO_Callback",
    "Set_On_SEARCH_USER_INFO_Callback",
    "Set_On_TUTORIAL_CLEAR_Callback",
    "Set_On_TUTORIAL_CLOSE_Callback",
    "Set_On_TUTORIAL_OPEN_Callback",
    "Tick",
    "XignCodeInit",
};


extern "C" void __cdecl LogCall(int index, DWORD a0, DWORD a1, DWORD a2, DWORD a3) {
    EnsureInitialized();
    Log("[ZNetworkProxy][PASSTHROUGH] %s(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n",
        g_names[index], a0, a1, a2, a3);
}

// ------------- Безопасные naked-трамплины (passthrough) -------------

extern "C" __declspec(naked) void DediDisconnect_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 0
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+0*4]
    }
}

extern "C" __declspec(naked) void DediStartConnect_EX_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 1
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+1*4]
    }
}

extern "C" __declspec(naked) void GetRoomCount_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 2
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+2*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_AI_DEATH_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 3
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+3*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_AI_KILL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 4
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+4*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_BEGIN_ROUND_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 5
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+5*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_BOMB_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 6
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+6*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_CHECK_BODY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 7
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+7*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_END_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 8
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+8*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_GHOST_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 9
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+9*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_GHOST_KING_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 10
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+10*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_HOST_CHANGE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 11
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+11*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_INTRUDE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 12
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+12*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_LEAVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 13
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+13*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_LOAD_COMPLETE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 14
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+14*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_OCCUPY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 15
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+15*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_PVE_DATANET_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 16
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+16*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_PVE_DOLL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 17
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+17*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_PVE_HELP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 18
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+18*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_RESPAWN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 19
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+19*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_REWARD_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 20
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+20*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_ROUND_END_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 21
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+21*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_SCORE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 22
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+22*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_START_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 23
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+23*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_START_PRE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 24
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+24*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_SUPER_DEFUSING_KIT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 25
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+25*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_TEAM_CHANGE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 26
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+26*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_USER_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 27
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+27*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_USER_DEATH_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 28
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+28*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_USER_KILL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 29
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+29*4]
    }
}

extern "C" __declspec(naked) void Get_On_GAME_USER_LEAVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 30
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+30*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_AI_DEATH_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 31
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+31*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_AI_KILL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 32
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+32*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_BEGIN_ROUND_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 33
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+33*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_BOMB_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 34
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+34*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_CHECK_BODY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 35
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+35*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_END_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 36
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+36*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_GHOST_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 37
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+37*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_GHOST_KING_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 38
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+38*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_LEAVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 39
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+39*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_LOAD_COMPLETE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 40
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+40*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_OCCUPY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 41
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+41*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_PVE_DATANET_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 42
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+42*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_PVE_DOLL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 43
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+43*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_PVE_HELP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 44
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+44*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_PVE_RANK_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 45
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+45*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_RESPAWN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 46
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+46*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_ROUND_END_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 47
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+47*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_SCORE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 48
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+48*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_START_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 49
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+49*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 50
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+50*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_TEAM_CHANGE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 51
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+51*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_USER_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 52
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+52*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_USER_DEATH_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 53
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+53*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_USER_EQUIP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 54
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+54*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_USER_ITEM_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 55
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+55*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_USER_KILL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 56
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+56*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_USER_LEAVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 57
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+57*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_USER_LOAD_COMPLETE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 58
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+58*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_WEAPON_CHANGE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 59
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+59*4]
    }
}

extern "C" __declspec(naked) void Get_On_HOST_GAME_ZOMBIE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 60
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+60*4]
    }
}

extern "C" __declspec(naked) void Send_CAPSULE_BUY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 61
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+61*4]
    }
}

extern "C" __declspec(naked) void Send_CAPSULE_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 62
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+62*4]
    }
}

extern "C" __declspec(naked) void Send_CAPSULE_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 63
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+63*4]
    }
}

extern "C" __declspec(naked) void Send_CHAT_ALL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 64
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+64*4]
    }
}

extern "C" __declspec(naked) void Send_CHAT_CLAN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 65
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+65*4]
    }
}

extern "C" __declspec(naked) void Send_CHAT_CLAN_MANAGER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 66
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+66*4]
    }
}

extern "C" __declspec(naked) void Send_CHAT_TEAM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 67
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+67*4]
    }
}

extern "C" __declspec(naked) void Send_CHAT_WHISPER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 68
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+68*4]
    }
}

extern "C" __declspec(naked) void Send_CHELLENGE_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 69
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+69*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHANGE_CLANNAME_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 70
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+70*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHANGE_MARK_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 71
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+71*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHANGE_MASTER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 72
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+72*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHECK_CLANNAME_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 73
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+73*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHECK_MARK_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 74
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+74*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 75
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+75*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_CREATE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 76
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+76*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_DELETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 77
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+77*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_INVITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 78
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+78*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_KICK_OUT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 79
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+79*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_MATCH_CANCEL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 80
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+80*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_MATCH_CREATE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 81
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+81*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_MATCH_START_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 82
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+82*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_MEMBER_POSITION_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 83
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+83*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 84
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+84*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_OUT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 85
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+85*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_REQUEST_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 86
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+86*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_REQUEST_ACCEPT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 87
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+87*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_REQUEST_CANCEL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 88
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+88*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_REQUEST_REFUSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 89
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+89*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_SEARCH_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 90
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+90*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_SEARCH_MASTER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 91
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+91*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_UPDATE_INTRO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 92
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+92*4]
    }
}

extern "C" __declspec(naked) void Send_CLAN_UPDATE_NOTICE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 93
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+93*4]
    }
}

extern "C" __declspec(naked) void Send_DAILY_MISSION_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 94
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+94*4]
    }
}

extern "C" __declspec(naked) void Send_DAILY_MISSION_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 95
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+95*4]
    }
}

extern "C" __declspec(naked) void Send_DAILY_MISSION_START_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 96
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+96*4]
    }
}

extern "C" __declspec(naked) void Send_FRIEND_DELETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 97
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+97*4]
    }
}

extern "C" __declspec(naked) void Send_FRIEND_INVITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 98
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+98*4]
    }
}

extern "C" __declspec(naked) void Send_FRIEND_REQUEST_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 99
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+99*4]
    }
}

extern "C" __declspec(naked) void Send_FRIEND_REQUEST_ACCEPT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 100
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+100*4]
    }
}

extern "C" __declspec(naked) void Send_FRIEND_REQUEST_REFUSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 101
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+101*4]
    }
}

extern "C" __declspec(naked) void Send_GAME_HIT_COUNT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 102
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+102*4]
    }
}

extern "C" __declspec(naked) void Send_GAME_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 103
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+103*4]
    }
}

extern "C" __declspec(naked) void Send_GAME_LOAD_PERCENT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 104
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+104*4]
    }
}

extern "C" __declspec(naked) void Send_GAME_PING_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 105
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+105*4]
    }
}

extern "C" __declspec(naked) void Send_GAME_START_PRE_COMPLETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 106
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+106*4]
    }
}

extern "C" __declspec(naked) void Send_GATE_CHANNEL_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 107
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+107*4]
    }
}

extern "C" __declspec(naked) void Send_GATE_LOBBY_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 108
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+108*4]
    }
}

extern "C" __declspec(naked) void Send_GATE_LOBBY_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 109
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+109*4]
    }
}

extern "C" __declspec(naked) void Send_GATE_SERVER_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 110
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+110*4]
    }
}

extern "C" __declspec(naked) void Send_GATE_SERVER_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 111
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+111*4]
    }
}

extern "C" __declspec(naked) void Send_GIFT_DELETE_RECEIVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 112
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+112*4]
    }
}

extern "C" __declspec(naked) void Send_GIFT_DELETE_SEND_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 113
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+113*4]
    }
}

extern "C" __declspec(naked) void Send_GIFT_LOAD_RECEIVE_PAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 114
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+114*4]
    }
}

extern "C" __declspec(naked) void Send_GIFT_LOAD_SEND_PAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 115
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+115*4]
    }
}

extern "C" __declspec(naked) void Send_GIFT_READ_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 116
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+116*4]
    }
}

extern "C" __declspec(naked) void Send_GIFT_RECEIVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 117
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+117*4]
    }
}

extern "C" __declspec(naked) void Send_GIFT_WRITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 118
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+118*4]
    }
}

extern "C" __declspec(naked) void Send_GM_BAN_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 119
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+119*4]
    }
}

extern "C" __declspec(naked) void Send_GM_KICK_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 120
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+120*4]
    }
}

extern "C" __declspec(naked) void Send_GM_NOTICE_MESSAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 121
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+121*4]
    }
}

extern "C" __declspec(naked) void Send_GM_QUIET_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 122
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+122*4]
    }
}

extern "C" __declspec(naked) void Send_GM_WARN_MESSAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 123
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+123*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_BREAK_CUSTOM_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 124
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+124*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_BREAK_WEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 125
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+125*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_BUY_CUSTOM_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 126
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+126*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 127
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+127*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_CUSTOM_BREAK_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 128
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+128*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_CUSTOM_BUY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 129
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+129*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_CUSTOM_EQUIP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 130
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+130*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_EQUIP_CUSTOM_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 131
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+131*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_EQUIP_WEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 132
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+132*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_BUY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 133
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+133*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_DELETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 134
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+134*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_EQUIP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 135
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+135*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_EXPIRE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 136
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+136*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_REPAIR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 137
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+137*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_RESELL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 138
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+138*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_USE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 139
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+139*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 140
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+140*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 141
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+141*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_MONEY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 142
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+142*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_NICKNAME_CHANGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 143
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+143*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 144
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+144*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_PARTS_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 145
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+145*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_RECIPE_ITEM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 146
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+146*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_WEAPON_CUSTOM_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 147
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+147*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_WEAPON_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 148
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+148*4]
    }
}

extern "C" __declspec(naked) void Send_HANGAR_WEAR_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 149
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+149*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_DAILY_MISSION_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 150
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+150*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_BEGIN_ROUND_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 151
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+151*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_BOMB_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 152
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+152*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_CHECK_BODY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 153
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+153*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_GHOST_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 154
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+154*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_INSTANT_RESPAWN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 155
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+155*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 156
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+156*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_MUST_LOAD_COMPLETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 157
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+157*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_OCCUPY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 158
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+158*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PING_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 159
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+159*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PVE_DATANET_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 160
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+160*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PVE_DOLL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 161
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+161*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PVE_HELP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 162
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+162*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PVE_RANK_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 163
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+163*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_RESPAWN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 164
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+164*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_RESPAWN_COMPLETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 165
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+165*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_ROUND_END_PVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 166
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+166*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_START_WATCH_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 167
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+167*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_SUPER_DEFUSING_KIT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 168
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+168*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_TIMEOUT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 169
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+169*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_USER_DEATH_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 170
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+170*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_USER_EQUIP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 171
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+171*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_USER_ITEM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 172
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+172*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_USER_KILL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 173
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+173*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_WEAPON_CHANGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 174
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+174*4]
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_ZOMBIE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 175
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+175*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_CLAN_ROOM_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 176
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+176*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 177
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+177*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_MODE_MAP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 178
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+178*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_QUICK_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 179
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+179*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_REFRESH_CLAN_ROOM_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 180
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+180*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_REFRESH_ROOM_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 181
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+181*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_CREATE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 182
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+182*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_CREATE_EX_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 183
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+183*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_ENTER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 184
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+184*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 185
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+185*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 186
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+186*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_MAKE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 187
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+187*4]
    }
}

extern "C" __declspec(naked) void Send_LOBBY_USER_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 188
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+188*4]
    }
}

extern "C" __declspec(naked) void Send_MY_GUIDE_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 189
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+189*4]
    }
}

extern "C" __declspec(naked) void Send_MY_NICKNAME_CREATE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 190
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+190*4]
    }
}

extern "C" __declspec(naked) void Send_PERK_BREAK_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 191
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+191*4]
    }
}

extern "C" __declspec(naked) void Send_PERK_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 192
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+192*4]
    }
}

extern "C" __declspec(naked) void Send_PERK_EQUIP_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 193
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+193*4]
    }
}

extern "C" __declspec(naked) void Send_PERK_INCREASE_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 194
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+194*4]
    }
}

extern "C" __declspec(naked) void Send_PERK_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 195
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+195*4]
    }
}

extern "C" __declspec(naked) void Send_PERK_OPEN_NEW_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 196
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+196*4]
    }
}

extern "C" __declspec(naked) void Send_POST_CLEAR_RECEIVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 197
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+197*4]
    }
}

extern "C" __declspec(naked) void Send_POST_CLEAR_SEND_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 198
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+198*4]
    }
}

extern "C" __declspec(naked) void Send_POST_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 199
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+199*4]
    }
}

extern "C" __declspec(naked) void Send_POST_DELETE_RECEIVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 200
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+200*4]
    }
}

extern "C" __declspec(naked) void Send_POST_DELETE_SEND_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 201
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+201*4]
    }
}

extern "C" __declspec(naked) void Send_POST_LOAD_RECEIVE_PAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 202
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+202*4]
    }
}

extern "C" __declspec(naked) void Send_POST_LOAD_SEND_PAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 203
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+203*4]
    }
}

extern "C" __declspec(naked) void Send_POST_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 204
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+204*4]
    }
}

extern "C" __declspec(naked) void Send_POST_READ_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 205
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+205*4]
    }
}

extern "C" __declspec(naked) void Send_POST_WRITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 206
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+206*4]
    }
}

extern "C" __declspec(naked) void Send_PURCHASE_FINALIZE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 207
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+207*4]
    }
}

extern "C" __declspec(naked) void Send_PURCHASE_INIT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 208
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+208*4]
    }
}

extern "C" __declspec(naked) void Send_RECIPE_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 209
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+209*4]
    }
}

extern "C" __declspec(naked) void Send_RECIPE_ITEM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 210
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+210*4]
    }
}

extern "C" __declspec(naked) void Send_RECIPE_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 211
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+211*4]
    }
}

extern "C" __declspec(naked) void Send_RECIPE_OPEN_NEW_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 212
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+212*4]
    }
}

extern "C" __declspec(naked) void Send_REPORT_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 213
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+213*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_DETAIL_SETTING_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 214
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+214*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_DETAIL_SETTING_EX_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 215
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+215*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_GAME_READY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 216
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+216*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_GAME_START_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 217
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+217*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_INVITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 218
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+218*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_INVITE_USER_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 219
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+219*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 220
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+220*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_MAP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 221
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+221*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_MAP_CHANGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 222
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+222*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_MATCH_CANCEL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 223
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+223*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_MATCH_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 224
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+224*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_MATCH_START_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 225
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+225*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_SETTING_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 226
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+226*4]
    }
}

extern "C" __declspec(naked) void Send_ROOM_USER_TEAM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 227
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+227*4]
    }
}

extern "C" __declspec(naked) void Send_SEARCH_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 228
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+228*4]
    }
}

extern "C" __declspec(naked) void Send_TUTORIAL_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 229
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+229*4]
    }
}

extern "C" __declspec(naked) void Send_TUTORIAL_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 230
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+230*4]
    }
}

extern "C" __declspec(naked) void Send_TUTORIAL_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 231
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+231*4]
    }
}

extern "C" __declspec(naked) void Set_Command_Enum_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 232
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+232*4]
    }
}

extern "C" __declspec(naked) void Set_OnIoConnectedCallback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 233
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+233*4]
    }
}

extern "C" __declspec(naked) void Set_OnIoDisonnectedCallback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 234
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+234*4]
    }
}

extern "C" __declspec(naked) void Set_On_CAPSULE_BUY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 235
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+235*4]
    }
}

extern "C" __declspec(naked) void Set_On_CAPSULE_CLOSE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 236
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+236*4]
    }
}

extern "C" __declspec(naked) void Set_On_CAPSULE_NOTICE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 237
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+237*4]
    }
}

extern "C" __declspec(naked) void Set_On_CAPSULE_OPEN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 238
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+238*4]
    }
}

extern "C" __declspec(naked) void Set_On_CHAT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 239
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+239*4]
    }
}

extern "C" __declspec(naked) void Set_On_CHELLENGE_CLEAR_FAIL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 240
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+240*4]
    }
}

extern "C" __declspec(naked) void Set_On_CHELLENGE_CLEAR_SUCCESS_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 241
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+241*4]
    }
}

extern "C" __declspec(naked) void Set_On_CHELLENGE_MEDAL_GET_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 242
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+242*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_CHANGE_MARK_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 243
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+243*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_CHECK_CLANNAME_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 244
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+244*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_CHECK_MARK_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 245
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+245*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_CLOSE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 246
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+246*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_DELETE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 247
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+247*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 248
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+248*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_INVITE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 249
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+249*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_KICK_OUT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 250
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+250*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_MATCH_CANCEL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 251
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+251*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_MATCH_START_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 252
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+252*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_MEMBER_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 253
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+253*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_OPEN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 254
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+254*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_OUT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 255
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+255*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_REQUEST_CANCEL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 256
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+256*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_REQUEST_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 257
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+257*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_REQUEST_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 258
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+258*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_REQUEST_USER_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 259
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+259*4]
    }
}

extern "C" __declspec(naked) void Set_On_CLAN_SEARCH_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 260
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+260*4]
    }
}

extern "C" __declspec(naked) void Set_On_Command_Enum_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 261
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+261*4]
    }
}

extern "C" __declspec(naked) void Set_On_DAILY_MISSION_CLEAR_FAIL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 262
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+262*4]
    }
}

extern "C" __declspec(naked) void Set_On_DAILY_MISSION_CLEAR_SUCCESS_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 263
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+263*4]
    }
}

extern "C" __declspec(naked) void Set_On_DAILY_MISSION_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 264
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+264*4]
    }
}

extern "C" __declspec(naked) void Set_On_DAILY_MISSION_NEW_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 265
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+265*4]
    }
}

extern "C" __declspec(naked) void Set_On_DAILY_MISSION_START_FAIL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 266
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+266*4]
    }
}

extern "C" __declspec(naked) void Set_On_DAILY_MISSION_START_SUCCESS_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 267
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+267*4]
    }
}

extern "C" __declspec(naked) void Set_On_DEDI_DISCONNECT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 268
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+268*4]
    }
}

extern "C" __declspec(naked) void Set_On_DEDI_LOGIN_DATA_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 269
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+269*4]
    }
}

extern "C" __declspec(naked) void Set_On_EXPIRE_ITEM_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 270
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+270*4]
    }
}

extern "C" __declspec(naked) void Set_On_FRIEND_INVITE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 271
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+271*4]
    }
}

extern "C" __declspec(naked) void Set_On_FRIEND_LIST_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 272
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+272*4]
    }
}

extern "C" __declspec(naked) void Set_On_FRIEND_REQUEST_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 273
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+273*4]
    }
}

extern "C" __declspec(naked) void Set_On_FRIEND_REQUEST_LIST_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 274
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+274*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_AI_DEATH_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 275
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+275*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_AI_KILL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 276
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+276*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_BEGIN_ROUND_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 277
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+277*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_BOMB_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 278
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+278*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_CHECK_BODY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 279
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+279*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_END_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 280
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+280*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_GHOST_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 281
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+281*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_GHOST_KING_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 282
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+282*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_HOST_CHANGE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 283
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+283*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_INTRUDE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 284
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+284*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_LEAVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 285
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+285*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_LOAD_COMPLETE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 286
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+286*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_OCCUPY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 287
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+287*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_PVE_DATANET_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 288
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+288*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_PVE_DOLL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 289
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+289*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_PVE_HELP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 290
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+290*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_RESPAWN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 291
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+291*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_REWARD_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 292
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+292*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_ROUND_END_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 293
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+293*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_SCORE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 294
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+294*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_START_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 295
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+295*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_START_PRE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 296
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+296*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_SUPER_DEFUSING_KIT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 297
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+297*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_TEAM_CHANGE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 298
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+298*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_USER_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 299
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+299*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_USER_DEATH_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 300
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+300*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_USER_KILL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 301
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+301*4]
    }
}

extern "C" __declspec(naked) void Set_On_GAME_USER_LEAVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 302
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+302*4]
    }
}

extern "C" __declspec(naked) void Set_On_GATE_CHANNEL_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 303
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+303*4]
    }
}

extern "C" __declspec(naked) void Set_On_GATE_CHANNEL_JOIN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 304
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+304*4]
    }
}

extern "C" __declspec(naked) void Set_On_GATE_LOBBY_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 305
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+305*4]
    }
}

extern "C" __declspec(naked) void Set_On_GATE_SERVER_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 306
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+306*4]
    }
}

extern "C" __declspec(naked) void Set_On_GATE_SERVER_JOIN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 307
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+307*4]
    }
}

extern "C" __declspec(naked) void Set_On_GIFT_RECEIVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 308
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+308*4]
    }
}

extern "C" __declspec(naked) void Set_On_GIFT_RECEIVE_PAGE_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 309
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+309*4]
    }
}

extern "C" __declspec(naked) void Set_On_GIFT_SEND_PAGE_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 310
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+310*4]
    }
}

extern "C" __declspec(naked) void Set_On_GIFT_WRITE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 311
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+311*4]
    }
}

extern "C" __declspec(naked) void Set_On_GM_BAN_USER_BY_USER_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 312
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+312*4]
    }
}

extern "C" __declspec(naked) void Set_On_GM_KICK_USER_BY_USER_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 313
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+313*4]
    }
}

extern "C" __declspec(naked) void Set_On_GM_QUIET_USER_BY_USER_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 314
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+314*4]
    }
}

extern "C" __declspec(naked) void Set_On_GM_WARN_MESSAGE_BY_USER_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 315
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+315*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_BREAK_WEAR_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 316
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+316*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_CLOSE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 317
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+317*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_CUSTOM_BREAK_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 318
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+318*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_CUSTOM_BUY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 319
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+319*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_CUSTOM_EQUIP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 320
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+320*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_CUSTOM_SLOT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 321
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+321*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_EQUIP_WEAR_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 322
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+322*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_ITEM_BUY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 323
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+323*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_ITEM_DELETE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 324
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+324*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_ITEM_EQUIP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 325
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+325*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_ITEM_EXPIRE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 326
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+326*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_ITEM_REPAIR_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 327
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+327*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_ITEM_RESELL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 328
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+328*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_ITEM_USE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 329
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+329*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_JOIN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 330
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+330*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_LEAVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 331
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+331*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_MONEY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 332
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+332*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_NICKNAME_CHANGE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 333
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+333*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_OPEN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 334
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+334*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_PARTS_SLOT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 335
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+335*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_RECIPE_ITEM_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 336
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+336*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_WEAPON_SLOT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 337
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+337*4]
    }
}

extern "C" __declspec(naked) void Set_On_HANGAR_WEAR_SLOT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 338
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+338*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_AI_DEATH_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 339
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+339*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_AI_KILL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 340
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+340*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_BEGIN_ROUND_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 341
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+341*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_BOMB_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 342
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+342*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_CHECK_BODY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 343
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+343*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_END_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 344
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+344*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_GHOST_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 345
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+345*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_GHOST_KING_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 346
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+346*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_LEAVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 347
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+347*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_LOAD_COMPLETE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 348
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+348*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_OCCUPY_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 349
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+349*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_PVE_DATANET_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 350
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+350*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_PVE_DOLL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 351
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+351*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_PVE_HELP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 352
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+352*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_PVE_RANK_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 353
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+353*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_RESPAWN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 354
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+354*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_ROUND_END_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 355
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+355*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_SCORE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 356
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+356*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_START_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 357
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+357*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 358
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+358*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_TEAM_CHANGE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 359
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+359*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_USER_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 360
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+360*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_USER_DEATH_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 361
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+361*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_USER_EQUIP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 362
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+362*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_USER_ITEM_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 363
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+363*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_USER_KILL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 364
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+364*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_USER_LEAVE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 365
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+365*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_USER_LOAD_COMPLETE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 366
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+366*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_WEAPON_CHANGE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 367
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+367*4]
    }
}

extern "C" __declspec(naked) void Set_On_HOST_GAME_ZOMBIE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 368
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+368*4]
    }
}

extern "C" __declspec(naked) void Set_On_ITEM_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 369
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+369*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOBBY_ALL_MODE_MAP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 370
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+370*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOBBY_CLAN_ROOM_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 371
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+371*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOBBY_INVITE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 372
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+372*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOBBY_JOIN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 373
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+373*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOBBY_MODE_MAP_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 374
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+374*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOBBY_QUICK_JOIN_FAILED_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 375
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+375*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOBBY_ROOM_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 376
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+376*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOBBY_USER_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 377
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+377*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOGIN_DATA_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 378
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+378*4]
    }
}

extern "C" __declspec(naked) void Set_On_LOGIN_FAIL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 379
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+379*4]
    }
}

extern "C" __declspec(naked) void Set_On_MY_ATTENDANCE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 380
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+380*4]
    }
}

extern "C" __declspec(naked) void Set_On_MY_NICKNAME_CREATE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 381
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+381*4]
    }
}

extern "C" __declspec(naked) void Set_On_MY_USER_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 382
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+382*4]
    }
}

extern "C" __declspec(naked) void Set_On_NICKNAME_IN_USE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 383
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+383*4]
    }
}

extern "C" __declspec(naked) void Set_On_NOTICE_CHAT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 384
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+384*4]
    }
}

extern "C" __declspec(naked) void Set_On_NOTIFY_ERROR_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 385
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+385*4]
    }
}

extern "C" __declspec(naked) void Set_On_NOTIFY_ERROR_EXIT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 386
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+386*4]
    }
}

extern "C" __declspec(naked) void Set_On_PERK_BREAK_SLOT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 387
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+387*4]
    }
}

extern "C" __declspec(naked) void Set_On_PERK_CLOSE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 388
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+388*4]
    }
}

extern "C" __declspec(naked) void Set_On_PERK_EQUIP_SLOT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 389
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+389*4]
    }
}

extern "C" __declspec(naked) void Set_On_PERK_INCREASE_SLOT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 390
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+390*4]
    }
}

extern "C" __declspec(naked) void Set_On_PERK_OPEN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 391
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+391*4]
    }
}

extern "C" __declspec(naked) void Set_On_POST_CLOSE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 392
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+392*4]
    }
}

extern "C" __declspec(naked) void Set_On_POST_OPEN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 393
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+393*4]
    }
}

extern "C" __declspec(naked) void Set_On_POST_RECEIVE_PAGE_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 394
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+394*4]
    }
}

extern "C" __declspec(naked) void Set_On_POST_SEND_PAGE_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 395
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+395*4]
    }
}

extern "C" __declspec(naked) void Set_On_POST_UNREAD_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 396
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+396*4]
    }
}

extern "C" __declspec(naked) void Set_On_POST_WRITE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 397
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+397*4]
    }
}

extern "C" __declspec(naked) void Set_On_PURCHASE_FINALIZE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 398
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+398*4]
    }
}

extern "C" __declspec(naked) void Set_On_RECIPE_CLOSE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 399
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+399*4]
    }
}

extern "C" __declspec(naked) void Set_On_RECIPE_ITEM_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 400
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+400*4]
    }
}

extern "C" __declspec(naked) void Set_On_RECIPE_OPEN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 401
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+401*4]
    }
}

extern "C" __declspec(naked) void Set_On_REPORT_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 402
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+402*4]
    }
}

extern "C" __declspec(naked) void Set_On_REWARD_ITEM_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 403
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+403*4]
    }
}

extern "C" __declspec(naked) void Set_On_ROOM_EVENT_MAP_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 404
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+404*4]
    }
}

extern "C" __declspec(naked) void Set_On_ROOM_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 405
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+405*4]
    }
}

extern "C" __declspec(naked) void Set_On_ROOM_INVITE_USER_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 406
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+406*4]
    }
}

extern "C" __declspec(naked) void Set_On_ROOM_JOIN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 407
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+407*4]
    }
}

extern "C" __declspec(naked) void Set_On_ROOM_MATCH_CANCEL_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 408
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+408*4]
    }
}

extern "C" __declspec(naked) void Set_On_ROOM_MATCH_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 409
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+409*4]
    }
}

extern "C" __declspec(naked) void Set_On_ROOM_MATCH_JOIN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 410
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+410*4]
    }
}

extern "C" __declspec(naked) void Set_On_ROOM_OPEN_MAP_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 411
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+411*4]
    }
}

extern "C" __declspec(naked) void Set_On_ROOM_USER_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 412
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+412*4]
    }
}

extern "C" __declspec(naked) void Set_On_SEARCH_USER_INFO_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 413
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+413*4]
    }
}

extern "C" __declspec(naked) void Set_On_TUTORIAL_CLEAR_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 414
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+414*4]
    }
}

extern "C" __declspec(naked) void Set_On_TUTORIAL_CLOSE_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 415
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+415*4]
    }
}

extern "C" __declspec(naked) void Set_On_TUTORIAL_OPEN_Callback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 416
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+416*4]
    }
}

extern "C" __declspec(naked) void Tick_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 417
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+417*4]
    }
}

extern "C" __declspec(naked) void XignCodeInit_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        pushad
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 418
        call LogCall
        add  esp, 20
        popad
        pop  ebp
        jmp  dword ptr [g_real+418*4]
    }
}

// ------------- Функции с подтверждённой сигнатурой -------------

typedef int (__stdcall *LoginServerEx_t)(const char*, short, const wchar_t*, const char*, void*);
static LoginServerEx_t g_realLoginServerEx = NULL;

extern "C" int __stdcall LoginServer_EX(const char* ip, short port, const wchar_t* pb,
                                         const char* uid, void* extra) {
    EnsureInitialized();
    Log("[ZNetworkProxy] LoginServer_EX(ip=%ls, port=%d, pb=%ls, uid=%ls, extra=0x%p)\n",
        ip ? (const wchar_t*)ip : L"(null)", port, pb ? pb : L"(null)",
        uid ? (const wchar_t*)uid : L"(null)", extra);
    if (!g_realLoginServerEx) {
        Log("[ZNetworkProxy] !!! real LoginServer_EX not resolved !!!\n");
        return 0;
    }
    int result = g_realLoginServerEx(ip, port, pb, uid, extra);
    Log("[ZNetworkProxy] LoginServer_EX -> real result = %d\n", result);
    return result;
}

extern "C" void __stdcall DisConnect() {
    EnsureInitialized();
    Log("[ZNetworkProxy] DisConnect()\n");
}

extern "C" int __stdcall IS_HOST() {
    EnsureInitialized();
    Log("[ZNetworkProxy] IS_HOST()\n");
    return 0;
}

extern "C" int __stdcall IS_DEDI() {
    EnsureInitialized();
    Log("[ZNetworkProxy] IS_DEDI()\n");
    return 1;
}

extern "C" int __stdcall GetPing() {
    EnsureInitialized();
    Log("[ZNetworkProxy] GetPing()\n");
    return 30;
}

extern "C" void __stdcall destoryed() {
    EnsureInitialized();
    Log("[ZNetworkProxy] destoryed()\n");
}

// ---------------------------------------------------------------------------
static volatile LONG g_initStarted = 0;
static volatile LONG g_initDone = 0;
static char g_dllDir[MAX_PATH] = {0};

static void EnsureInitialized() {
    if (g_initDone) return;
    if (InterlockedCompareExchange(&g_initStarted, 1, 0) != 0) {
        while (!g_initDone) Sleep(1);
        return;
    }

    char path[MAX_PATH];
    strcpy_s(path, g_dllDir);
    strcat_s(path, "ZNetwork_orig.dll");

    g_hOrig = LoadLibraryA(path);
    if (!g_hOrig) {
        Log("[ZNetworkProxy] !!! ZNetwork_orig.dll NOT FOUND next to proxy DLL !!!\n");
        g_initDone = 1;
        return;
    }
    for (int i = 0; i < 419; i++) {
        g_real[i] = GetProcAddress(g_hOrig, g_names[i]);
        if (!g_real[i]) Log("[ZNetworkProxy] missing export in original: %s\n", g_names[i]);
    }

    g_realLoginServerEx = (LoginServerEx_t)GetProcAddress(g_hOrig, "LoginServer_EX");
    if (!g_realLoginServerEx) Log("[ZNetworkProxy] !!! could not resolve real LoginServer_EX !!!\n");

    Log("[ZNetworkProxy] Lazily loaded, all real exports resolved.\n");
    g_initDone = 1;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        GetModuleFileNameA(hModule, g_dllDir, MAX_PATH);
        char* slash = strrchr(g_dllDir, '\\');
        if (slash) *(slash + 1) = 0;
        DisableThreadLibraryCalls(hModule);
    }
    return TRUE;
}
