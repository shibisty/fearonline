// ============================================================================
//  ZNetwork.dll proxy/stub  --  auto-generated
//
//  СХЕМА:
//   - Set_On_*_Callback  (1 stdcall параметр)  -> своя реализация, логирует и
//     сохраняет указатель колбэка, возвращает 0.
//   - Get_On_*_Callback  (0 параметров)        -> своя реализация, логирует и
//     возвращает сохранённый (или NULL) колбэк.
//   - Функции с подтверждённой сигнатурой (LoginServer_EX, DisConnect,
//     IS_HOST, IS_DEDI, GetPing, destoryed) -> своя реализация.
//   - ВСЁ ОСТАЛЬНОЕ (Send_* и т.п., ~176 функций) -- сигнатура НЕИЗВЕСТНА.
//     Чтобы не разрушить стек, они реализованы как безопасные
//     transit-трамплины: логируют 4 первых "сырых" DWORD'а со стека и
//     передают управление (jmp, не call) в РЕАЛЬНУЮ функцию из оригинальной
//     ZNetwork.dll, переименованной в ZNetwork_orig.dll и лежащей рядом.
//     Настоящая функция сама корректно снимет стек -- нам не нужно знать
//     точное число аргументов, чтобы не уронить процесс.
//
//   ТРЕБОВАНИЕ: файл ZNetwork_orig.dll (оригинальный ZNetwork.dll,
//   переименованный) должен лежать в той же папке, что и эта proxy-DLL.
//
//   Сборка (Visual Studio, x86 / Win32 !!):
//     cl /LD /MT znetwork_proxy.cpp /link /DEF:znetwork_proxy.def
//        /OUT:ZNetwork.dll
//   (наличие __declspec(naked) требует x86-таргета; для x64 не собрать)
// ============================================================================
#include <windows.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>

static HMODULE g_hOrig = NULL;

static void Log(const char* fmt, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
    va_end(ap);
    OutputDebugStringA(buf);
}

// ---------------------------------------------------------------------------
// Реальные указатели для UNKNOWN-функций (транзит в оригинальную ZNetwork_orig.dll)
// ---------------------------------------------------------------------------
static FARPROC g_real[176];
static const char* g_unknownNames[] = {
    "GetServerData",
    "GetServerHostData",
    "XignCodeInit",
    "Set_OnIoConnectedCallback",
    "Set_OnIoDisonnectedCallback",
    "DediStartConnect_EX",
    "DediDisconnect",
    "Send_MY_NICKNAME_CREATE",
    "Send_MY_GUIDE_CLEAR",
    "Send_SEARCH_USER",
    "Send_GATE_SERVER_INFO",
    "Send_GATE_LOBBY_INFO",
    "Send_GATE_SERVER_JOIN",
    "Send_GATE_CHANNEL_JOIN",
    "Send_GATE_LOBBY_JOIN",
    "Send_LOBBY_ROOM_MAKE",
    "Send_LOBBY_ROOM_CREATE",
    "Send_LOBBY_ROOM_JOIN",
    "GetRoomCount",
    "Send_LOBBY_ROOM_CREATE_EX",
    "Send_LOBBY_ROOM_ENTER",
    "Send_LOBBY_LEAVE",
    "Send_LOBBY_MODE_MAP",
    "Send_LOBBY_QUICK_JOIN",
    "Send_LOBBY_USER_INFO",
    "Send_LOBBY_ROOM_INFO",
    "Send_LOBBY_CLAN_ROOM_INFO",
    "Send_LOBBY_REFRESH_ROOM_INFO",
    "Send_LOBBY_REFRESH_CLAN_ROOM_INFO",
    "Send_ROOM_MAP_CHANGE",
    "Send_ROOM_SETTING",
    "Send_ROOM_DETAIL_SETTING",
    "Send_ROOM_DETAIL_SETTING_EX",
    "Send_ROOM_USER_TEAM",
    "Send_ROOM_LEAVE",
    "Send_ROOM_GAME_START",
    "Send_ROOM_GAME_READY",
    "Send_ROOM_MATCH_JOIN",
    "Send_ROOM_MATCH_CANCEL",
    "Send_ROOM_MATCH_START",
    "Send_ROOM_INVITE_USER_INFO",
    "Send_ROOM_INVITE",
    "Send_GAME_LOAD_PERCENT",
    "Send_GAME_LEAVE",
    "Send_GAME_PING",
    "Send_GAME_START_PRE_COMPLETE",
    "Send_GAME_HIT_COUNT",
    "Send_HOST_GAME_MUST_LOAD_COMPLETE",
    "Send_HOST_GAME_START_WATCH",
    "Send_HOST_GAME_USER_DEATH",
    "Send_HOST_GAME_USER_KILL",
    "Send_HOST_GAME_RESPAWN",
    "Send_HOST_GAME_INSTANT_RESPAWN",
    "Send_HOST_GAME_RESPAWN_COMPLETE",
    "Send_HOST_GAME_BOMB",
    "Send_HOST_GAME_OCCUPY",
    "Send_HOST_GAME_SUPER_DEFUSING_KIT",
    "Send_HOST_GAME_GHOST",
    "Send_HOST_GAME_CHECK_BODY",
    "Send_HOST_GAME_PVE_DATANET",
    "Send_HOST_GAME_PVE_DOLL",
    "Send_HOST_GAME_PVE_HELP",
    "Send_HOST_GAME_ZOMBIE",
    "Send_HOST_GAME_TIMEOUT",
    "Send_HOST_GAME_ROUND_END_PVE",
    "Send_HOST_GAME_BEGIN_ROUND",
    "Send_HOST_GAME_LEAVE",
    "Send_HOST_GAME_WEAPON_CHANGE",
    "Send_HOST_GAME_USER_EQUIP",
    "Send_HOST_GAME_PING",
    "Send_HOST_GAME_PVE_RANK",
    "Send_HOST_GAME_USER_ITEM",
    "Send_HANGAR_JOIN",
    "Send_HANGAR_LEAVE",
    "Send_HANGAR_WEAPON_SLOT",
    "Send_HANGAR_WEAPON_CUSTOM_SLOT",
    "Send_HANGAR_WEAR_SLOT",
    "Send_HANGAR_PARTS_SLOT",
    "Send_HANGAR_NICKNAME_CHANGE",
    "Send_HANGAR_RECIPE_ITEM",
    "Send_HANGAR_BUY_CUSTOM_SLOT",
    "Send_HANGAR_EQUIP_CUSTOM_SLOT",
    "Send_HANGAR_BREAK_CUSTOM_SLOT",
    "Send_HANGAR_OPEN",
    "Send_HANGAR_CLOSE",
    "Send_HANGAR_MONEY",
    "Send_HANGAR_ITEM_BUY",
    "Send_HANGAR_ITEM_EQUIP",
    "Send_HANGAR_ITEM_DELETE",
    "Send_HANGAR_ITEM_USE",
    "Send_HANGAR_ITEM_REPAIR",
    "Send_HANGAR_ITEM_EXPIRE",
    "Send_HANGAR_ITEM_RESELL",
    "Send_HANGAR_CUSTOM_BUY",
    "Send_HANGAR_CUSTOM_EQUIP",
    "Send_HANGAR_CUSTOM_BREAK",
    "Send_HANGAR_EQUIP_WEAR",
    "Send_HANGAR_BREAK_WEAR",
    "Send_RECIPE_OPEN",
    "Send_RECIPE_OPEN_NEW",
    "Send_RECIPE_CLOSE",
    "Send_RECIPE_ITEM",
    "Send_PERK_OPEN",
    "Send_PERK_OPEN_NEW",
    "Send_PERK_CLOSE",
    "Send_PERK_EQUIP_SLOT",
    "Send_PERK_BREAK_SLOT",
    "Send_PERK_INCREASE_SLOT",
    "Send_FRIEND_REQUEST",
    "Send_FRIEND_REQUEST_ACCEPT",
    "Send_FRIEND_REQUEST_REFUSE",
    "Send_FRIEND_DELETE",
    "Send_FRIEND_INVITE",
    "Send_CLAN_OPEN",
    "Send_CLAN_CLOSE",
    "Send_CLAN_CREATE",
    "Send_CLAN_DELETE",
    "Send_CLAN_SEARCH",
    "Send_CLAN_SEARCH_MASTER",
    "Send_CLAN_CHECK_CLANNAME",
    "Send_CLAN_CHANGE_CLANNAME",
    "Send_CLAN_UPDATE_NOTICE",
    "Send_CLAN_UPDATE_INTRO",
    "Send_CLAN_CHANGE_MASTER",
    "Send_CLAN_MEMBER_POSITION",
    "Send_CLAN_REQUEST",
    "Send_CLAN_REQUEST_CANCEL",
    "Send_CLAN_REQUEST_ACCEPT",
    "Send_CLAN_REQUEST_REFUSE",
    "Send_CLAN_KICK_OUT",
    "Send_CLAN_OUT",
    "Send_CLAN_INVITE",
    "Send_CLAN_CHECK_MARK",
    "Send_CLAN_CHANGE_MARK",
    "Send_POST_OPEN",
    "Send_POST_CLOSE",
    "Send_POST_WRITE",
    "Send_POST_READ",
    "Send_POST_LOAD_SEND_PAGE",
    "Send_POST_LOAD_RECEIVE_PAGE",
    "Send_POST_DELETE_SEND",
    "Send_POST_DELETE_RECEIVE",
    "Send_POST_CLEAR_SEND",
    "Send_POST_CLEAR_RECEIVE",
    "Send_GIFT_WRITE",
    "Send_GIFT_READ",
    "Send_GIFT_RECEIVE",
    "Send_GIFT_LOAD_SEND_PAGE",
    "Send_GIFT_LOAD_RECEIVE_PAGE",
    "Send_GIFT_DELETE_SEND",
    "Send_GIFT_DELETE_RECEIVE",
    "Send_TUTORIAL_OPEN",
    "Send_TUTORIAL_CLOSE",
    "Send_TUTORIAL_CLEAR",
    "Send_DAILY_MISSION_START",
    "Send_HOST_DAILY_MISSION_CLEAR",
    "Send_DAILY_MISSION_CLEAR",
    "Send_DAILY_MISSION_INFO",
    "Send_CHELLENGE_CLEAR",
    "Send_CAPSULE_OPEN",
    "Send_CAPSULE_CLOSE",
    "Send_CAPSULE_BUY",
    "Send_PURCHASE_INIT",
    "Send_PURCHASE_FINALIZE",
    "Send_GM_WARN_MESSAGE",
    "Send_GM_NOTICE_MESSAGE",
    "Send_GM_QUIET_USER",
    "Send_GM_KICK_USER",
    "Send_GM_BAN_USER",
    "Send_CHAT_ALL",
    "Send_CHAT_TEAM",
    "Send_CHAT_CLAN",
    "Send_CHAT_CLAN_MANAGER",
    "Send_CHAT_WHISPER",
    "Send_REPORT_USER",
    "Set_Command_Enum",
};


extern "C" void __cdecl LogUnknownCall(int index, DWORD a0, DWORD a1, DWORD a2, DWORD a3) {
    Log("[ZNetworkProxy][PASSTHROUGH] %s(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n",
        g_unknownNames[index], a0, a1, a2, a3);
}

// ------------- UNKNOWN: безопасные транзитные трамплины -------------

extern "C" __declspec(naked) void GetServerData_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 0
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+0*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void GetServerHostData_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 1
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+1*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void XignCodeInit_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 2
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+2*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Set_OnIoConnectedCallback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 3
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+3*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Set_OnIoDisonnectedCallback_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 4
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+4*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void DediStartConnect_EX_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 5
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+5*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void DediDisconnect_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 6
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+6*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_MY_NICKNAME_CREATE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 7
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+7*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_MY_GUIDE_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 8
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+8*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_SEARCH_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 9
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+9*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GATE_SERVER_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 10
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+10*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GATE_LOBBY_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 11
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+11*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GATE_SERVER_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 12
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+12*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GATE_CHANNEL_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 13
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+13*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GATE_LOBBY_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 14
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+14*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_MAKE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 15
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+15*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_CREATE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 16
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+16*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 17
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+17*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void GetRoomCount_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 18
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+18*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_CREATE_EX_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 19
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+19*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_ENTER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 20
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+20*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 21
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+21*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_MODE_MAP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 22
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+22*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_QUICK_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 23
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+23*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_USER_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 24
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+24*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_ROOM_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 25
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+25*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_CLAN_ROOM_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 26
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+26*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_REFRESH_ROOM_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 27
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+27*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_LOBBY_REFRESH_CLAN_ROOM_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 28
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+28*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_MAP_CHANGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 29
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+29*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_SETTING_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 30
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+30*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_DETAIL_SETTING_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 31
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+31*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_DETAIL_SETTING_EX_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 32
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+32*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_USER_TEAM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 33
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+33*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 34
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+34*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_GAME_START_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 35
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+35*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_GAME_READY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 36
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+36*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_MATCH_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 37
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+37*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_MATCH_CANCEL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 38
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+38*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_MATCH_START_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 39
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+39*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_INVITE_USER_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 40
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+40*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_ROOM_INVITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 41
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+41*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GAME_LOAD_PERCENT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 42
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+42*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GAME_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 43
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+43*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GAME_PING_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 44
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+44*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GAME_START_PRE_COMPLETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 45
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+45*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GAME_HIT_COUNT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 46
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+46*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_MUST_LOAD_COMPLETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 47
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+47*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_START_WATCH_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 48
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+48*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_USER_DEATH_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 49
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+49*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_USER_KILL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 50
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+50*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_RESPAWN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 51
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+51*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_INSTANT_RESPAWN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 52
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+52*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_RESPAWN_COMPLETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 53
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+53*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_BOMB_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 54
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+54*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_OCCUPY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 55
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+55*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_SUPER_DEFUSING_KIT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 56
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+56*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_GHOST_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 57
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+57*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_CHECK_BODY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 58
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+58*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PVE_DATANET_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 59
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+59*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PVE_DOLL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 60
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+60*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PVE_HELP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 61
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+61*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_ZOMBIE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 62
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+62*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_TIMEOUT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 63
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+63*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_ROUND_END_PVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 64
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+64*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_BEGIN_ROUND_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 65
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+65*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 66
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+66*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_WEAPON_CHANGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 67
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+67*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_USER_EQUIP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 68
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+68*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PING_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 69
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+69*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_PVE_RANK_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 70
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+70*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_GAME_USER_ITEM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 71
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+71*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_JOIN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 72
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+72*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_LEAVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 73
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+73*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_WEAPON_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 74
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+74*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_WEAPON_CUSTOM_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 75
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+75*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_WEAR_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 76
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+76*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_PARTS_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 77
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+77*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_NICKNAME_CHANGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 78
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+78*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_RECIPE_ITEM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 79
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+79*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_BUY_CUSTOM_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 80
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+80*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_EQUIP_CUSTOM_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 81
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+81*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_BREAK_CUSTOM_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 82
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+82*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 83
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+83*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 84
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+84*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_MONEY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 85
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+85*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_BUY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 86
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+86*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_EQUIP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 87
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+87*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_DELETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 88
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+88*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_USE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 89
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+89*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_REPAIR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 90
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+90*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_EXPIRE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 91
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+91*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_ITEM_RESELL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 92
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+92*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_CUSTOM_BUY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 93
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+93*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_CUSTOM_EQUIP_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 94
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+94*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_CUSTOM_BREAK_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 95
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+95*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_EQUIP_WEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 96
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+96*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HANGAR_BREAK_WEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 97
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+97*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_RECIPE_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 98
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+98*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_RECIPE_OPEN_NEW_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 99
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+99*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_RECIPE_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 100
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+100*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_RECIPE_ITEM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 101
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+101*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_PERK_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 102
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+102*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_PERK_OPEN_NEW_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 103
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+103*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_PERK_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 104
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+104*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_PERK_EQUIP_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 105
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+105*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_PERK_BREAK_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 106
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+106*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_PERK_INCREASE_SLOT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 107
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+107*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_FRIEND_REQUEST_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 108
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+108*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_FRIEND_REQUEST_ACCEPT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 109
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+109*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_FRIEND_REQUEST_REFUSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 110
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+110*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_FRIEND_DELETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 111
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+111*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_FRIEND_INVITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 112
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+112*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 113
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+113*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 114
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+114*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_CREATE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 115
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+115*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_DELETE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 116
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+116*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_SEARCH_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 117
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+117*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_SEARCH_MASTER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 118
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+118*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHECK_CLANNAME_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 119
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+119*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHANGE_CLANNAME_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 120
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+120*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_UPDATE_NOTICE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 121
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+121*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_UPDATE_INTRO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 122
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+122*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHANGE_MASTER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 123
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+123*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_MEMBER_POSITION_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 124
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+124*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_REQUEST_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 125
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+125*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_REQUEST_CANCEL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 126
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+126*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_REQUEST_ACCEPT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 127
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+127*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_REQUEST_REFUSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 128
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+128*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_KICK_OUT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 129
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+129*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_OUT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 130
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+130*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_INVITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 131
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+131*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHECK_MARK_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 132
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+132*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CLAN_CHANGE_MARK_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 133
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+133*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 134
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+134*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 135
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+135*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_WRITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 136
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+136*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_READ_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 137
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+137*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_LOAD_SEND_PAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 138
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+138*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_LOAD_RECEIVE_PAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 139
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+139*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_DELETE_SEND_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 140
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+140*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_DELETE_RECEIVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 141
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+141*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_CLEAR_SEND_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 142
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+142*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_POST_CLEAR_RECEIVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 143
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+143*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GIFT_WRITE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 144
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+144*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GIFT_READ_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 145
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+145*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GIFT_RECEIVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 146
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+146*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GIFT_LOAD_SEND_PAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 147
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+147*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GIFT_LOAD_RECEIVE_PAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 148
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+148*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GIFT_DELETE_SEND_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 149
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+149*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GIFT_DELETE_RECEIVE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 150
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+150*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_TUTORIAL_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 151
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+151*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_TUTORIAL_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 152
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+152*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_TUTORIAL_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 153
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+153*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_DAILY_MISSION_START_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 154
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+154*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_HOST_DAILY_MISSION_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 155
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+155*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_DAILY_MISSION_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 156
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+156*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_DAILY_MISSION_INFO_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 157
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+157*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CHELLENGE_CLEAR_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 158
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+158*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CAPSULE_OPEN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 159
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+159*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CAPSULE_CLOSE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 160
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+160*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CAPSULE_BUY_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 161
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+161*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_PURCHASE_INIT_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 162
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+162*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_PURCHASE_FINALIZE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 163
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+163*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GM_WARN_MESSAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 164
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+164*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GM_NOTICE_MESSAGE_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 165
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+165*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GM_QUIET_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 166
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+166*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GM_KICK_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 167
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+167*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_GM_BAN_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 168
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+168*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CHAT_ALL_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 169
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+169*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CHAT_TEAM_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 170
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+170*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CHAT_CLAN_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 171
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+171*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CHAT_CLAN_MANAGER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 172
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+172*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_CHAT_WHISPER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 173
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+173*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Send_REPORT_USER_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 174
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+174*4]
        pop  ebp
        jmp  eax
    }
}

extern "C" __declspec(naked) void Set_Command_Enum_stub() {
    __asm {
        push ebp
        mov  ebp, esp
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        push 175
        call LogUnknownCall
        add  esp, 20
        mov  eax, dword ptr [g_real+175*4]
        pop  ebp
        jmp  eax
    }
}

// ------------- Set_On_*_Callback : 1 stdcall параметр -------------

static void* g_cb_Set_On_LOGIN_DATA_Callback = NULL;
extern "C" int __stdcall Set_On_LOGIN_DATA_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOGIN_DATA_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOGIN_DATA_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_LOGIN_FAIL_Callback = NULL;
extern "C" int __stdcall Set_On_LOGIN_FAIL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOGIN_FAIL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOGIN_FAIL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_DEDI_LOGIN_DATA_Callback = NULL;
extern "C" int __stdcall Set_On_DEDI_LOGIN_DATA_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_DEDI_LOGIN_DATA_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_DEDI_LOGIN_DATA_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_DEDI_DISCONNECT_Callback = NULL;
extern "C" int __stdcall Set_On_DEDI_DISCONNECT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_DEDI_DISCONNECT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_DEDI_DISCONNECT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_MY_USER_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_MY_USER_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_MY_USER_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_MY_USER_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ITEM_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_ITEM_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ITEM_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ITEM_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_EXPIRE_ITEM_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_EXPIRE_ITEM_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_EXPIRE_ITEM_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_EXPIRE_ITEM_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_REWARD_ITEM_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_REWARD_ITEM_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_REWARD_ITEM_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_REWARD_ITEM_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_MY_NICKNAME_CREATE_Callback = NULL;
extern "C" int __stdcall Set_On_MY_NICKNAME_CREATE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_MY_NICKNAME_CREATE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_MY_NICKNAME_CREATE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_NICKNAME_IN_USE_Callback = NULL;
extern "C" int __stdcall Set_On_NICKNAME_IN_USE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_NICKNAME_IN_USE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_NICKNAME_IN_USE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_MY_ATTENDANCE_Callback = NULL;
extern "C" int __stdcall Set_On_MY_ATTENDANCE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_MY_ATTENDANCE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_MY_ATTENDANCE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_SEARCH_USER_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_SEARCH_USER_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_SEARCH_USER_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_SEARCH_USER_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GATE_SERVER_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_GATE_SERVER_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GATE_SERVER_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GATE_SERVER_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GATE_CHANNEL_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_GATE_CHANNEL_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GATE_CHANNEL_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GATE_CHANNEL_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GATE_LOBBY_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_GATE_LOBBY_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GATE_LOBBY_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GATE_LOBBY_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GATE_SERVER_JOIN_Callback = NULL;
extern "C" int __stdcall Set_On_GATE_SERVER_JOIN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GATE_SERVER_JOIN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GATE_SERVER_JOIN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GATE_CHANNEL_JOIN_Callback = NULL;
extern "C" int __stdcall Set_On_GATE_CHANNEL_JOIN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GATE_CHANNEL_JOIN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GATE_CHANNEL_JOIN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_LOBBY_JOIN_Callback = NULL;
extern "C" int __stdcall Set_On_LOBBY_JOIN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOBBY_JOIN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOBBY_JOIN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_LOBBY_INVITE_Callback = NULL;
extern "C" int __stdcall Set_On_LOBBY_INVITE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOBBY_INVITE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOBBY_INVITE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_LOBBY_ALL_MODE_MAP_Callback = NULL;
extern "C" int __stdcall Set_On_LOBBY_ALL_MODE_MAP_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOBBY_ALL_MODE_MAP_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOBBY_ALL_MODE_MAP_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_LOBBY_MODE_MAP_Callback = NULL;
extern "C" int __stdcall Set_On_LOBBY_MODE_MAP_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOBBY_MODE_MAP_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOBBY_MODE_MAP_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_LOBBY_QUICK_JOIN_FAILED_Callback = NULL;
extern "C" int __stdcall Set_On_LOBBY_QUICK_JOIN_FAILED_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOBBY_QUICK_JOIN_FAILED_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOBBY_QUICK_JOIN_FAILED_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_LOBBY_USER_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_LOBBY_USER_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOBBY_USER_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOBBY_USER_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_LOBBY_ROOM_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_LOBBY_ROOM_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOBBY_ROOM_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOBBY_ROOM_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_LOBBY_CLAN_ROOM_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_LOBBY_CLAN_ROOM_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_LOBBY_CLAN_ROOM_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_LOBBY_CLAN_ROOM_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ROOM_JOIN_Callback = NULL;
extern "C" int __stdcall Set_On_ROOM_JOIN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ROOM_JOIN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ROOM_JOIN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ROOM_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_ROOM_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ROOM_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ROOM_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ROOM_USER_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_ROOM_USER_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ROOM_USER_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ROOM_USER_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ROOM_INVITE_USER_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_ROOM_INVITE_USER_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ROOM_INVITE_USER_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ROOM_INVITE_USER_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ROOM_OPEN_MAP_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_ROOM_OPEN_MAP_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ROOM_OPEN_MAP_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ROOM_OPEN_MAP_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ROOM_EVENT_MAP_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_ROOM_EVENT_MAP_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ROOM_EVENT_MAP_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ROOM_EVENT_MAP_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ROOM_MATCH_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_ROOM_MATCH_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ROOM_MATCH_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ROOM_MATCH_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ROOM_MATCH_JOIN_Callback = NULL;
extern "C" int __stdcall Set_On_ROOM_MATCH_JOIN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ROOM_MATCH_JOIN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ROOM_MATCH_JOIN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_ROOM_MATCH_CANCEL_Callback = NULL;
extern "C" int __stdcall Set_On_ROOM_MATCH_CANCEL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_ROOM_MATCH_CANCEL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_ROOM_MATCH_CANCEL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_START_PRE_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_START_PRE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_START_PRE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_START_PRE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_START_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_START_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_START_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_START_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_LOAD_COMPLETE_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_LOAD_COMPLETE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_LOAD_COMPLETE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_LOAD_COMPLETE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_USER_DEATH_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_USER_DEATH_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_USER_DEATH_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_USER_DEATH_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_AI_DEATH_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_AI_DEATH_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_AI_DEATH_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_AI_DEATH_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_USER_KILL_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_USER_KILL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_USER_KILL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_USER_KILL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_AI_KILL_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_AI_KILL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_AI_KILL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_AI_KILL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_RESPAWN_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_RESPAWN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_RESPAWN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_RESPAWN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_BOMB_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_BOMB_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_BOMB_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_BOMB_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_OCCUPY_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_OCCUPY_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_OCCUPY_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_OCCUPY_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_SUPER_DEFUSING_KIT_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_SUPER_DEFUSING_KIT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_SUPER_DEFUSING_KIT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_SUPER_DEFUSING_KIT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_GHOST_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_GHOST_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_GHOST_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_GHOST_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_GHOST_KING_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_GHOST_KING_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_GHOST_KING_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_GHOST_KING_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_CHECK_BODY_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_CHECK_BODY_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_CHECK_BODY_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_CHECK_BODY_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_PVE_DATANET_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_PVE_DATANET_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_PVE_DATANET_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_PVE_DATANET_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_PVE_DOLL_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_PVE_DOLL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_PVE_DOLL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_PVE_DOLL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_PVE_HELP_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_PVE_HELP_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_PVE_HELP_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_PVE_HELP_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_ZOMBIE_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_ZOMBIE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_ZOMBIE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_ZOMBIE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_SCORE_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_SCORE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_SCORE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_SCORE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_USER_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_USER_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_USER_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_USER_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_REWARD_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_REWARD_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_REWARD_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_REWARD_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_BEGIN_ROUND_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_BEGIN_ROUND_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_BEGIN_ROUND_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_BEGIN_ROUND_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_ROUND_END_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_ROUND_END_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_ROUND_END_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_ROUND_END_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_END_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_END_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_END_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_END_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_LEAVE_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_LEAVE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_LEAVE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_LEAVE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_USER_LEAVE_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_USER_LEAVE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_USER_LEAVE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_USER_LEAVE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_TEAM_CHANGE_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_TEAM_CHANGE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_TEAM_CHANGE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_TEAM_CHANGE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GAME_HOST_CHANGE_Callback = NULL;
extern "C" int __stdcall Set_On_GAME_HOST_CHANGE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GAME_HOST_CHANGE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GAME_HOST_CHANGE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_START_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_START_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_START_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_START_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_LOAD_COMPLETE_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_LOAD_COMPLETE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_LOAD_COMPLETE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_LOAD_COMPLETE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_USER_LOAD_COMPLETE_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_USER_LOAD_COMPLETE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_USER_LOAD_COMPLETE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_USER_LOAD_COMPLETE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_USER_DEATH_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_USER_DEATH_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_USER_DEATH_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_USER_DEATH_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_AI_DEATH_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_AI_DEATH_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_AI_DEATH_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_AI_DEATH_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_USER_KILL_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_USER_KILL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_USER_KILL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_USER_KILL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_AI_KILL_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_AI_KILL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_AI_KILL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_AI_KILL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_RESPAWN_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_RESPAWN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_RESPAWN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_RESPAWN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_BOMB_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_BOMB_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_BOMB_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_BOMB_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_OCCUPY_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_OCCUPY_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_OCCUPY_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_OCCUPY_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_GHOST_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_GHOST_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_GHOST_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_GHOST_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_GHOST_KING_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_GHOST_KING_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_GHOST_KING_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_GHOST_KING_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_CHECK_BODY_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_CHECK_BODY_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_CHECK_BODY_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_CHECK_BODY_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_PVE_DATANET_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_PVE_DATANET_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_PVE_DATANET_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_PVE_DATANET_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_PVE_DOLL_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_PVE_DOLL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_PVE_DOLL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_PVE_DOLL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_PVE_HELP_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_PVE_HELP_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_PVE_HELP_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_PVE_HELP_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_SCORE_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_SCORE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_SCORE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_SCORE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_USER_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_USER_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_USER_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_USER_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_BEGIN_ROUND_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_BEGIN_ROUND_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_BEGIN_ROUND_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_BEGIN_ROUND_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_ROUND_END_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_ROUND_END_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_ROUND_END_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_ROUND_END_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_END_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_END_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_END_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_END_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_LEAVE_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_LEAVE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_LEAVE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_LEAVE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_USER_LEAVE_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_USER_LEAVE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_USER_LEAVE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_USER_LEAVE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_WEAPON_CHANGE_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_WEAPON_CHANGE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_WEAPON_CHANGE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_WEAPON_CHANGE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_USER_EQUIP_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_USER_EQUIP_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_USER_EQUIP_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_USER_EQUIP_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_PVE_RANK_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_PVE_RANK_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_PVE_RANK_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_PVE_RANK_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_USER_ITEM_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_USER_ITEM_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_USER_ITEM_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_USER_ITEM_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HOST_GAME_TEAM_CHANGE_Callback = NULL;
extern "C" int __stdcall Set_On_HOST_GAME_TEAM_CHANGE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HOST_GAME_TEAM_CHANGE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HOST_GAME_TEAM_CHANGE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_JOIN_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_JOIN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_JOIN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_JOIN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_LEAVE_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_LEAVE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_LEAVE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_LEAVE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_WEAPON_SLOT_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_WEAPON_SLOT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_WEAPON_SLOT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_WEAPON_SLOT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_WEAR_SLOT_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_WEAR_SLOT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_WEAR_SLOT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_WEAR_SLOT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_PARTS_SLOT_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_PARTS_SLOT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_PARTS_SLOT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_PARTS_SLOT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_RECIPE_ITEM_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_RECIPE_ITEM_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_RECIPE_ITEM_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_RECIPE_ITEM_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_CUSTOM_SLOT_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_CUSTOM_SLOT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_CUSTOM_SLOT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_CUSTOM_SLOT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_OPEN_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_OPEN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_OPEN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_OPEN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_CLOSE_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_CLOSE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_CLOSE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_CLOSE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_MONEY_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_MONEY_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_MONEY_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_MONEY_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_ITEM_BUY_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_ITEM_BUY_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_ITEM_BUY_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_ITEM_BUY_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_ITEM_EQUIP_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_ITEM_EQUIP_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_ITEM_EQUIP_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_ITEM_EQUIP_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_ITEM_DELETE_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_ITEM_DELETE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_ITEM_DELETE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_ITEM_DELETE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_ITEM_USE_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_ITEM_USE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_ITEM_USE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_ITEM_USE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_ITEM_REPAIR_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_ITEM_REPAIR_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_ITEM_REPAIR_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_ITEM_REPAIR_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_ITEM_EXPIRE_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_ITEM_EXPIRE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_ITEM_EXPIRE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_ITEM_EXPIRE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_ITEM_RESELL_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_ITEM_RESELL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_ITEM_RESELL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_ITEM_RESELL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_CUSTOM_BUY_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_CUSTOM_BUY_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_CUSTOM_BUY_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_CUSTOM_BUY_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_CUSTOM_EQUIP_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_CUSTOM_EQUIP_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_CUSTOM_EQUIP_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_CUSTOM_EQUIP_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_CUSTOM_BREAK_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_CUSTOM_BREAK_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_CUSTOM_BREAK_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_CUSTOM_BREAK_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_NICKNAME_CHANGE_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_NICKNAME_CHANGE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_NICKNAME_CHANGE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_NICKNAME_CHANGE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_EQUIP_WEAR_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_EQUIP_WEAR_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_EQUIP_WEAR_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_EQUIP_WEAR_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_HANGAR_BREAK_WEAR_Callback = NULL;
extern "C" int __stdcall Set_On_HANGAR_BREAK_WEAR_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_HANGAR_BREAK_WEAR_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_HANGAR_BREAK_WEAR_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_RECIPE_OPEN_Callback = NULL;
extern "C" int __stdcall Set_On_RECIPE_OPEN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_RECIPE_OPEN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_RECIPE_OPEN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_RECIPE_CLOSE_Callback = NULL;
extern "C" int __stdcall Set_On_RECIPE_CLOSE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_RECIPE_CLOSE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_RECIPE_CLOSE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_RECIPE_ITEM_Callback = NULL;
extern "C" int __stdcall Set_On_RECIPE_ITEM_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_RECIPE_ITEM_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_RECIPE_ITEM_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_PERK_OPEN_Callback = NULL;
extern "C" int __stdcall Set_On_PERK_OPEN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_PERK_OPEN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_PERK_OPEN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_PERK_CLOSE_Callback = NULL;
extern "C" int __stdcall Set_On_PERK_CLOSE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_PERK_CLOSE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_PERK_CLOSE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_PERK_EQUIP_SLOT_Callback = NULL;
extern "C" int __stdcall Set_On_PERK_EQUIP_SLOT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_PERK_EQUIP_SLOT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_PERK_EQUIP_SLOT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_PERK_BREAK_SLOT_Callback = NULL;
extern "C" int __stdcall Set_On_PERK_BREAK_SLOT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_PERK_BREAK_SLOT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_PERK_BREAK_SLOT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_PERK_INCREASE_SLOT_Callback = NULL;
extern "C" int __stdcall Set_On_PERK_INCREASE_SLOT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_PERK_INCREASE_SLOT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_PERK_INCREASE_SLOT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_FRIEND_REQUEST_Callback = NULL;
extern "C" int __stdcall Set_On_FRIEND_REQUEST_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_FRIEND_REQUEST_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_FRIEND_REQUEST_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_FRIEND_INVITE_Callback = NULL;
extern "C" int __stdcall Set_On_FRIEND_INVITE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_FRIEND_INVITE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_FRIEND_INVITE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_FRIEND_LIST_Callback = NULL;
extern "C" int __stdcall Set_On_FRIEND_LIST_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_FRIEND_LIST_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_FRIEND_LIST_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_FRIEND_REQUEST_LIST_Callback = NULL;
extern "C" int __stdcall Set_On_FRIEND_REQUEST_LIST_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_FRIEND_REQUEST_LIST_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_FRIEND_REQUEST_LIST_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_OPEN_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_OPEN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_OPEN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_OPEN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_CLOSE_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_CLOSE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_CLOSE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_CLOSE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_DELETE_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_DELETE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_DELETE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_DELETE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_SEARCH_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_SEARCH_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_SEARCH_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_SEARCH_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_CHECK_CLANNAME_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_CHECK_CLANNAME_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_CHECK_CLANNAME_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_CHECK_CLANNAME_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_OUT_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_OUT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_OUT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_OUT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_KICK_OUT_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_KICK_OUT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_KICK_OUT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_KICK_OUT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_REQUEST_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_REQUEST_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_REQUEST_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_REQUEST_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_REQUEST_CANCEL_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_REQUEST_CANCEL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_REQUEST_CANCEL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_REQUEST_CANCEL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_INVITE_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_INVITE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_INVITE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_INVITE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_REQUEST_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_REQUEST_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_REQUEST_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_REQUEST_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_MEMBER_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_MEMBER_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_MEMBER_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_MEMBER_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_REQUEST_USER_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_REQUEST_USER_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_REQUEST_USER_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_REQUEST_USER_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_CHECK_MARK_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_CHECK_MARK_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_CHECK_MARK_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_CHECK_MARK_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CLAN_CHANGE_MARK_Callback = NULL;
extern "C" int __stdcall Set_On_CLAN_CHANGE_MARK_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CLAN_CHANGE_MARK_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CLAN_CHANGE_MARK_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_POST_OPEN_Callback = NULL;
extern "C" int __stdcall Set_On_POST_OPEN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_POST_OPEN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_POST_OPEN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_POST_CLOSE_Callback = NULL;
extern "C" int __stdcall Set_On_POST_CLOSE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_POST_CLOSE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_POST_CLOSE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_POST_UNREAD_Callback = NULL;
extern "C" int __stdcall Set_On_POST_UNREAD_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_POST_UNREAD_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_POST_UNREAD_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_POST_WRITE_Callback = NULL;
extern "C" int __stdcall Set_On_POST_WRITE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_POST_WRITE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_POST_WRITE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_POST_SEND_PAGE_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_POST_SEND_PAGE_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_POST_SEND_PAGE_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_POST_SEND_PAGE_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_POST_RECEIVE_PAGE_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_POST_RECEIVE_PAGE_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_POST_RECEIVE_PAGE_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_POST_RECEIVE_PAGE_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GIFT_WRITE_Callback = NULL;
extern "C" int __stdcall Set_On_GIFT_WRITE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GIFT_WRITE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GIFT_WRITE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GIFT_RECEIVE_Callback = NULL;
extern "C" int __stdcall Set_On_GIFT_RECEIVE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GIFT_RECEIVE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GIFT_RECEIVE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GIFT_SEND_PAGE_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_GIFT_SEND_PAGE_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GIFT_SEND_PAGE_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GIFT_SEND_PAGE_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GIFT_RECEIVE_PAGE_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_GIFT_RECEIVE_PAGE_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GIFT_RECEIVE_PAGE_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GIFT_RECEIVE_PAGE_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_TUTORIAL_OPEN_Callback = NULL;
extern "C" int __stdcall Set_On_TUTORIAL_OPEN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_TUTORIAL_OPEN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_TUTORIAL_OPEN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_TUTORIAL_CLOSE_Callback = NULL;
extern "C" int __stdcall Set_On_TUTORIAL_CLOSE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_TUTORIAL_CLOSE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_TUTORIAL_CLOSE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_TUTORIAL_CLEAR_Callback = NULL;
extern "C" int __stdcall Set_On_TUTORIAL_CLEAR_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_TUTORIAL_CLEAR_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_TUTORIAL_CLEAR_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_DAILY_MISSION_START_SUCCESS_Callback = NULL;
extern "C" int __stdcall Set_On_DAILY_MISSION_START_SUCCESS_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_DAILY_MISSION_START_SUCCESS_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_DAILY_MISSION_START_SUCCESS_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_DAILY_MISSION_START_FAIL_Callback = NULL;
extern "C" int __stdcall Set_On_DAILY_MISSION_START_FAIL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_DAILY_MISSION_START_FAIL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_DAILY_MISSION_START_FAIL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_DAILY_MISSION_CLEAR_SUCCESS_Callback = NULL;
extern "C" int __stdcall Set_On_DAILY_MISSION_CLEAR_SUCCESS_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_DAILY_MISSION_CLEAR_SUCCESS_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_DAILY_MISSION_CLEAR_SUCCESS_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_DAILY_MISSION_CLEAR_FAIL_Callback = NULL;
extern "C" int __stdcall Set_On_DAILY_MISSION_CLEAR_FAIL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_DAILY_MISSION_CLEAR_FAIL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_DAILY_MISSION_CLEAR_FAIL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_DAILY_MISSION_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_DAILY_MISSION_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_DAILY_MISSION_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_DAILY_MISSION_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_DAILY_MISSION_NEW_INFO_Callback = NULL;
extern "C" int __stdcall Set_On_DAILY_MISSION_NEW_INFO_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_DAILY_MISSION_NEW_INFO_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_DAILY_MISSION_NEW_INFO_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CHELLENGE_CLEAR_SUCCESS_Callback = NULL;
extern "C" int __stdcall Set_On_CHELLENGE_CLEAR_SUCCESS_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CHELLENGE_CLEAR_SUCCESS_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CHELLENGE_CLEAR_SUCCESS_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CHELLENGE_CLEAR_FAIL_Callback = NULL;
extern "C" int __stdcall Set_On_CHELLENGE_CLEAR_FAIL_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CHELLENGE_CLEAR_FAIL_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CHELLENGE_CLEAR_FAIL_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CHELLENGE_MEDAL_GET_Callback = NULL;
extern "C" int __stdcall Set_On_CHELLENGE_MEDAL_GET_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CHELLENGE_MEDAL_GET_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CHELLENGE_MEDAL_GET_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CAPSULE_OPEN_Callback = NULL;
extern "C" int __stdcall Set_On_CAPSULE_OPEN_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CAPSULE_OPEN_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CAPSULE_OPEN_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CAPSULE_CLOSE_Callback = NULL;
extern "C" int __stdcall Set_On_CAPSULE_CLOSE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CAPSULE_CLOSE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CAPSULE_CLOSE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CAPSULE_BUY_Callback = NULL;
extern "C" int __stdcall Set_On_CAPSULE_BUY_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CAPSULE_BUY_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CAPSULE_BUY_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_PURCHASE_FINALIZE_Callback = NULL;
extern "C" int __stdcall Set_On_PURCHASE_FINALIZE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_PURCHASE_FINALIZE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_PURCHASE_FINALIZE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GM_WARN_MESSAGE_BY_USER_Callback = NULL;
extern "C" int __stdcall Set_On_GM_WARN_MESSAGE_BY_USER_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GM_WARN_MESSAGE_BY_USER_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GM_WARN_MESSAGE_BY_USER_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GM_QUIET_USER_BY_USER_Callback = NULL;
extern "C" int __stdcall Set_On_GM_QUIET_USER_BY_USER_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GM_QUIET_USER_BY_USER_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GM_QUIET_USER_BY_USER_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GM_KICK_USER_BY_USER_Callback = NULL;
extern "C" int __stdcall Set_On_GM_KICK_USER_BY_USER_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GM_KICK_USER_BY_USER_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GM_KICK_USER_BY_USER_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_GM_BAN_USER_BY_USER_Callback = NULL;
extern "C" int __stdcall Set_On_GM_BAN_USER_BY_USER_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_GM_BAN_USER_BY_USER_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_GM_BAN_USER_BY_USER_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CHAT_Callback = NULL;
extern "C" int __stdcall Set_On_CHAT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CHAT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CHAT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_CAPSULE_NOTICE_Callback = NULL;
extern "C" int __stdcall Set_On_CAPSULE_NOTICE_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_CAPSULE_NOTICE_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_CAPSULE_NOTICE_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_REPORT_Callback = NULL;
extern "C" int __stdcall Set_On_REPORT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_REPORT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_REPORT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_Command_Enum_Callback = NULL;
extern "C" int __stdcall Set_On_Command_Enum_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_Command_Enum_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_Command_Enum_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_NOTICE_CHAT_Callback = NULL;
extern "C" int __stdcall Set_On_NOTICE_CHAT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_NOTICE_CHAT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_NOTICE_CHAT_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_NOTIFY_ERROR_Callback = NULL;
extern "C" int __stdcall Set_On_NOTIFY_ERROR_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_NOTIFY_ERROR_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_NOTIFY_ERROR_Callback = callback;
    return 0;
}

static void* g_cb_Set_On_NOTIFY_ERROR_EXIT_Callback = NULL;
extern "C" int __stdcall Set_On_NOTIFY_ERROR_EXIT_Callback(void* callback) {
    Log("[ZNetworkProxy] Set_On_NOTIFY_ERROR_EXIT_Callback(callback=0x%p)\n", callback);
    g_cb_Set_On_NOTIFY_ERROR_EXIT_Callback = callback;
    return 0;
}

// ------------- Get_On_*_Callback : 0 параметров -------------

extern "C" void* __stdcall Get_On_GAME_START_PRE_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_START_PRE_Callback()\n");
    return g_cb_Set_On_GAME_START_PRE_Callback;
}

extern "C" void* __stdcall Get_On_GAME_START_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_START_Callback()\n");
    return g_cb_Set_On_GAME_START_Callback;
}

extern "C" void* __stdcall Get_On_GAME_LOAD_COMPLETE_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_LOAD_COMPLETE_Callback()\n");
    return g_cb_Set_On_GAME_LOAD_COMPLETE_Callback;
}

extern "C" void* __stdcall Get_On_GAME_USER_DEATH_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_USER_DEATH_Callback()\n");
    return g_cb_Set_On_GAME_USER_DEATH_Callback;
}

extern "C" void* __stdcall Get_On_GAME_AI_DEATH_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_AI_DEATH_Callback()\n");
    return g_cb_Set_On_GAME_AI_DEATH_Callback;
}

extern "C" void* __stdcall Get_On_GAME_USER_KILL_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_USER_KILL_Callback()\n");
    return g_cb_Set_On_GAME_USER_KILL_Callback;
}

extern "C" void* __stdcall Get_On_GAME_AI_KILL_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_AI_KILL_Callback()\n");
    return g_cb_Set_On_GAME_AI_KILL_Callback;
}

extern "C" void* __stdcall Get_On_GAME_RESPAWN_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_RESPAWN_Callback()\n");
    return g_cb_Set_On_GAME_RESPAWN_Callback;
}

extern "C" void* __stdcall Get_On_GAME_BOMB_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_BOMB_Callback()\n");
    return g_cb_Set_On_GAME_BOMB_Callback;
}

extern "C" void* __stdcall Get_On_GAME_OCCUPY_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_OCCUPY_Callback()\n");
    return g_cb_Set_On_GAME_OCCUPY_Callback;
}

extern "C" void* __stdcall Get_On_GAME_SUPER_DEFUSING_KIT_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_SUPER_DEFUSING_KIT_Callback()\n");
    return g_cb_Set_On_GAME_SUPER_DEFUSING_KIT_Callback;
}

extern "C" void* __stdcall Get_On_GAME_GHOST_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_GHOST_Callback()\n");
    return g_cb_Set_On_GAME_GHOST_Callback;
}

extern "C" void* __stdcall Get_On_GAME_GHOST_KING_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_GHOST_KING_Callback()\n");
    return g_cb_Set_On_GAME_GHOST_KING_Callback;
}

extern "C" void* __stdcall Get_On_GAME_CHECK_BODY_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_CHECK_BODY_Callback()\n");
    return g_cb_Set_On_GAME_CHECK_BODY_Callback;
}

extern "C" void* __stdcall Get_On_GAME_PVE_DATANET_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_PVE_DATANET_Callback()\n");
    return g_cb_Set_On_GAME_PVE_DATANET_Callback;
}

extern "C" void* __stdcall Get_On_GAME_PVE_DOLL_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_PVE_DOLL_Callback()\n");
    return g_cb_Set_On_GAME_PVE_DOLL_Callback;
}

extern "C" void* __stdcall Get_On_GAME_PVE_HELP_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_PVE_HELP_Callback()\n");
    return g_cb_Set_On_GAME_PVE_HELP_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_ZOMBIE_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_ZOMBIE_Callback()\n");
    return g_cb_Set_On_HOST_GAME_ZOMBIE_Callback;
}

extern "C" void* __stdcall Get_On_GAME_BEGIN_ROUND_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_BEGIN_ROUND_Callback()\n");
    return g_cb_Set_On_GAME_BEGIN_ROUND_Callback;
}

extern "C" void* __stdcall Get_On_GAME_ROUND_END_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_ROUND_END_Callback()\n");
    return g_cb_Set_On_GAME_ROUND_END_Callback;
}

extern "C" void* __stdcall Get_On_GAME_END_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_END_Callback()\n");
    return g_cb_Set_On_GAME_END_Callback;
}

extern "C" void* __stdcall Get_On_GAME_SCORE_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_SCORE_Callback()\n");
    return g_cb_Set_On_GAME_SCORE_Callback;
}

extern "C" void* __stdcall Get_On_GAME_USER_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_USER_Callback()\n");
    return g_cb_Set_On_GAME_USER_Callback;
}

extern "C" void* __stdcall Get_On_GAME_REWARD_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_REWARD_Callback()\n");
    return g_cb_Set_On_GAME_REWARD_Callback;
}

extern "C" void* __stdcall Get_On_GAME_LEAVE_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_LEAVE_Callback()\n");
    return g_cb_Set_On_GAME_LEAVE_Callback;
}

extern "C" void* __stdcall Get_On_GAME_USER_LEAVE_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_USER_LEAVE_Callback()\n");
    return g_cb_Set_On_GAME_USER_LEAVE_Callback;
}

extern "C" void* __stdcall Get_On_GAME_TEAM_CHANGE_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_TEAM_CHANGE_Callback()\n");
    return g_cb_Set_On_GAME_TEAM_CHANGE_Callback;
}

extern "C" void* __stdcall Get_On_GAME_HOST_CHANGE_Callback() {
    Log("[ZNetworkProxy] Get_On_GAME_HOST_CHANGE_Callback()\n");
    return g_cb_Set_On_GAME_HOST_CHANGE_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_START_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_START_Callback()\n");
    return g_cb_Set_On_HOST_GAME_START_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_LOAD_COMPLETE_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_LOAD_COMPLETE_Callback()\n");
    return g_cb_Set_On_HOST_GAME_LOAD_COMPLETE_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_USER_LOAD_COMPLETE_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_USER_LOAD_COMPLETE_Callback()\n");
    return g_cb_Set_On_HOST_GAME_USER_LOAD_COMPLETE_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_USER_DEATH_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_USER_DEATH_Callback()\n");
    return g_cb_Set_On_HOST_GAME_USER_DEATH_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_AI_DEATH_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_AI_DEATH_Callback()\n");
    return g_cb_Set_On_HOST_GAME_AI_DEATH_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_USER_KILL_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_USER_KILL_Callback()\n");
    return g_cb_Set_On_HOST_GAME_USER_KILL_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_AI_KILL_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_AI_KILL_Callback()\n");
    return g_cb_Set_On_HOST_GAME_AI_KILL_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_RESPAWN_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_RESPAWN_Callback()\n");
    return g_cb_Set_On_HOST_GAME_RESPAWN_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_BOMB_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_BOMB_Callback()\n");
    return g_cb_Set_On_HOST_GAME_BOMB_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_OCCUPY_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_OCCUPY_Callback()\n");
    return g_cb_Set_On_HOST_GAME_OCCUPY_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback()\n");
    return g_cb_Set_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_GHOST_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_GHOST_Callback()\n");
    return g_cb_Set_On_HOST_GAME_GHOST_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_GHOST_KING_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_GHOST_KING_Callback()\n");
    return g_cb_Set_On_HOST_GAME_GHOST_KING_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_CHECK_BODY_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_CHECK_BODY_Callback()\n");
    return g_cb_Set_On_HOST_GAME_CHECK_BODY_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_PVE_DATANET_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_PVE_DATANET_Callback()\n");
    return g_cb_Set_On_HOST_GAME_PVE_DATANET_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_PVE_DOLL_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_PVE_DOLL_Callback()\n");
    return g_cb_Set_On_HOST_GAME_PVE_DOLL_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_PVE_HELP_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_PVE_HELP_Callback()\n");
    return g_cb_Set_On_HOST_GAME_PVE_HELP_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_SCORE_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_SCORE_Callback()\n");
    return g_cb_Set_On_HOST_GAME_SCORE_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_USER_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_USER_Callback()\n");
    return g_cb_Set_On_HOST_GAME_USER_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_BEGIN_ROUND_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_BEGIN_ROUND_Callback()\n");
    return g_cb_Set_On_HOST_GAME_BEGIN_ROUND_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_ROUND_END_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_ROUND_END_Callback()\n");
    return g_cb_Set_On_HOST_GAME_ROUND_END_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_END_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_END_Callback()\n");
    return g_cb_Set_On_HOST_GAME_END_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_LEAVE_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_LEAVE_Callback()\n");
    return g_cb_Set_On_HOST_GAME_LEAVE_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_USER_LEAVE_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_USER_LEAVE_Callback()\n");
    return g_cb_Set_On_HOST_GAME_USER_LEAVE_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_WEAPON_CHANGE_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_WEAPON_CHANGE_Callback()\n");
    return g_cb_Set_On_HOST_GAME_WEAPON_CHANGE_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_USER_EQUIP_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_USER_EQUIP_Callback()\n");
    return g_cb_Set_On_HOST_GAME_USER_EQUIP_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_PVE_RANK_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_PVE_RANK_Callback()\n");
    return g_cb_Set_On_HOST_GAME_PVE_RANK_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_USER_ITEM_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_USER_ITEM_Callback()\n");
    return g_cb_Set_On_HOST_GAME_USER_ITEM_Callback;
}

extern "C" void* __stdcall Get_On_HOST_GAME_TEAM_CHANGE_Callback() {
    Log("[ZNetworkProxy] Get_On_HOST_GAME_TEAM_CHANGE_Callback()\n");
    return g_cb_Set_On_HOST_GAME_TEAM_CHANGE_Callback;
}

// ------------- Функции с известной сигнатурой -------------

typedef int (__stdcall *LoginServerEx_t)(const char*, short, const wchar_t*, const char*, void*);
static LoginServerEx_t g_realLoginServerEx = NULL;

extern "C" int __stdcall LoginServer_EX(const char* ip, short port, const wchar_t* pb,
                                         const char* uid, void* extra) {
    Log("[ZNetworkProxy] LoginServer_EX(ip=%s, port=%d, pb=%ls, uid=%s, extra=0x%p)\n",
        ip ? ip : "(null)", port, pb ? pb : L"(null)", uid ? uid : "(null)", extra);
    if (!g_realLoginServerEx) {
        Log("[ZNetworkProxy] !!! real LoginServer_EX not resolved, cannot forward !!!\n");
        return 0;
    }
    int result = g_realLoginServerEx(ip, port, pb, uid, extra);
    Log("[ZNetworkProxy] LoginServer_EX -> real result = %d\n", result);
    return result;
}

extern "C" void __stdcall DisConnect() {
    Log("[ZNetworkProxy] DisConnect()\n");
}

extern "C" int __stdcall IS_HOST() {
    Log("[ZNetworkProxy] IS_HOST()\n");
    return 0;
}

extern "C" int __stdcall IS_DEDI() {
    Log("[ZNetworkProxy] IS_DEDI()\n");
    return 1;
}

extern "C" int __stdcall GetPing() {
    Log("[ZNetworkProxy] GetPing()\n");
    return 30;
}

extern "C" void __stdcall destoryed() {
    Log("[ZNetworkProxy] destoryed()\n");
}

// ---------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        char path[MAX_PATH];
        GetModuleFileNameA(hModule, path, MAX_PATH);
        char* slash = strrchr(path, '\\');
        if (slash) *(slash + 1) = 0;
        strcat_s(path, "ZNetwork_orig.dll");

        g_hOrig = LoadLibraryA(path);
        if (!g_hOrig) {
            Log("[ZNetworkProxy] !!! ZNetwork_orig.dll NOT FOUND next to proxy DLL !!!\n");
        } else {
            for (int i = 0; i < 176; i++) {
                g_real[i] = GetProcAddress(g_hOrig, g_unknownNames[i]);
                if (!g_real[i]) Log("[ZNetworkProxy] missing export in original: %s\n", g_unknownNames[i]);
            }
            g_realLoginServerEx = (LoginServerEx_t)GetProcAddress(g_hOrig, "LoginServer_EX");
            if (!g_realLoginServerEx) Log("[ZNetworkProxy] !!! could not resolve real LoginServer_EX !!!\n");

            Log("[ZNetworkProxy] Loaded, %d passthrough exports resolved.\n", 176);
        }
    }
    return TRUE;
}
