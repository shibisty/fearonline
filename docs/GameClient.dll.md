## EtcInfo.csv parser

```
void __fastcall FUN_1035ac00(int param_1)

{
  char cVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 *apuStack_2b4 [173];
  
  if ((*(int *)(param_1 + 4) == 0) || (*(int *)(param_1 + 8) - *(int *)(param_1 + 4) >> 2 == 0)) {
    thunk_FUN_1034c1a0();
    cVar1 = thunk_FUN_1034c240("c3.Arch01",0x2c,0x22);
    if ((cVar1 != '\0') ||
       (cVar1 = thunk_FUN_1034bef0("./CSV/EtcInfo.csv",0x2c,0x22), cVar1 != '\0')) {
      thunk_FUN_10349c80("Index",0);
      thunk_FUN_10349c80("GameServerIP",1);
      thunk_FUN_10349c80("GameServerPort",2);
      thunk_FUN_10349c80("Language",3);
      thunk_FUN_10349c80("EventKey",4);
      thunk_FUN_10349c80("AcceptChannel",5);
      thunk_FUN_10345590(0);
      cVar1 = thunk_FUN_10347c30();
      while (cVar1 != '\0') {
        puVar2 = (undefined4 *)FUN_1036e730(0x54);
        if (puVar2 == (undefined4 *)0x0) {
          puVar2 = (undefined4 *)0x0;
        }
        else {
          *puVar2 = 0;
          memset(puVar2 + 1,0,0x40);
          puVar2[0x11] = 0;
          puVar2[0x13] = 0;
          puVar2[0x14] = 0;
        }
        apuStack_2b4[0] = puVar2;
        uVar3 = thunk_FUN_10349d60("Index");
        uVar3 = thunk_FUN_10349e10(uVar3);
        *puVar2 = uVar3;
        uVar3 = thunk_FUN_10349d60("GameServerIP");
        uVar3 = thunk_FUN_10349f10(uVar3);
        thunk_FUN_10109300(puVar2 + 1,0x40,uVar3);
        uVar3 = thunk_FUN_10349d60("GameServerPort");
        uVar3 = thunk_FUN_10349e10(uVar3);
        puVar2[0x11] = uVar3;
        uVar3 = thunk_FUN_10349d60("Language");
        uVar3 = thunk_FUN_10349f10(uVar3);
        thunk_FUN_10109300(puVar2 + 0x12,4,uVar3);
        uVar3 = thunk_FUN_10349d60("EventKey");
        uVar3 = thunk_FUN_10349e10(uVar3);
        puVar2[0x13] = uVar3;
        uVar3 = thunk_FUN_10349d60("AcceptChannel");
        uVar3 = thunk_FUN_10349e10(uVar3);
        puVar2[0x14] = uVar3;
        thunk_FUN_1035ab70(apuStack_2b4);
        cVar1 = thunk_FUN_10347c30();
      }
    }
    thunk_FUN_1034c1e0();
  }
  return;
}
```

## Client-Server proto

```
undefined4 * __fastcall FUN_1034c4d0(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  *(undefined1 *)(param_1 + 3) = 0;
  iVar1 = FUN_103e4452("Game\\ZNetwork.dll");
  param_1[2] = iVar1;
  if (iVar1 != 0) {
    uVar2 = FUN_103e448d(iVar1,&DAT_1047511c);
    param_1[4] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"destoryed");
    param_1[5] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],&DAT_10475108);
    param_1[6] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"GetServerData");
    param_1[7] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"GetServerHostData");
    param_1[8] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"XignCodeInit");
    param_1[9] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"GetPing");
    param_1[10] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"LoginServer_EX");
    param_1[0xb] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"DisConnect");
    param_1[0xc] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_OnIoConnectedCallback");
    param_1[0xd] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_OnIoDisonnectedCallback");
    param_1[0xe] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOGIN_DATA_Callback");
    param_1[0xf] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOGIN_FAIL_Callback");
    param_1[0x10] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"DediStartConnect_EX");
    param_1[0x11] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"DediDisconnect");
    param_1[0x12] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_DEDI_LOGIN_DATA_Callback");
    param_1[0x13] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_DEDI_DISCONNECT_Callback");
    param_1[0x14] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_MY_NICKNAME_CREATE");
    param_1[0x15] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_MY_GUIDE_CLEAR");
    param_1[0x16] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_MY_USER_INFO_Callback");
    param_1[0x17] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ITEM_INFO_Callback");
    param_1[0x18] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_EXPIRE_ITEM_INFO_Callback");
    param_1[0x19] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_REWARD_ITEM_INFO_Callback");
    param_1[0x1a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_MY_NICKNAME_CREATE_Callback");
    param_1[0x1b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_NICKNAME_IN_USE_Callback");
    param_1[0x1c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_MY_ATTENDANCE_Callback");
    param_1[0x1d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_SEARCH_USER");
    param_1[0x1e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_SEARCH_USER_INFO_Callback");
    param_1[0x1f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GATE_SERVER_INFO");
    param_1[0x23] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GATE_LOBBY_INFO");
    param_1[0x25] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GATE_SERVER_JOIN");
    param_1[0x24] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GATE_CHANNEL_JOIN");
    param_1[0x20] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GATE_LOBBY_JOIN");
    param_1[0x26] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GATE_SERVER_INFO_Callback");
    param_1[0x27] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GATE_CHANNEL_INFO_Callback");
    param_1[0x21] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GATE_LOBBY_INFO_Callback");
    param_1[0x29] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GATE_SERVER_JOIN_Callback");
    param_1[0x28] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GATE_CHANNEL_JOIN_Callback");
    param_1[0x22] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_ROOM_MAKE");
    param_1[0x2a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_ROOM_CREATE");
    param_1[0x2b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_ROOM_JOIN");
    param_1[0x2c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"GetRoomCount");
    param_1[0x2d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_ROOM_CREATE_EX");
    param_1[0x2e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_ROOM_ENTER");
    param_1[0x2f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_LEAVE");
    param_1[0x30] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_MODE_MAP");
    param_1[0x31] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_QUICK_JOIN");
    param_1[0x32] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_USER_INFO");
    param_1[0x33] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_ROOM_INFO");
    param_1[0x34] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_CLAN_ROOM_INFO");
    param_1[0x35] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_REFRESH_ROOM_INFO");
    param_1[0x36] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_LOBBY_REFRESH_CLAN_ROOM_INFO");
    param_1[0x37] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOBBY_JOIN_Callback");
    param_1[0x38] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOBBY_INVITE_Callback");
    param_1[0x39] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOBBY_ALL_MODE_MAP_Callback");
    param_1[0x3a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOBBY_MODE_MAP_Callback");
    param_1[0x3b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOBBY_QUICK_JOIN_FAILED_Callback");
    param_1[0x3c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOBBY_USER_INFO_Callback");
    param_1[0x3d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOBBY_ROOM_INFO_Callback");
    param_1[0x3e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_LOBBY_CLAN_ROOM_INFO_Callback");
    param_1[0x3f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_MAP_CHANGE");
    param_1[0x41] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_SETTING");
    param_1[0x42] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_DETAIL_SETTING");
    param_1[0x43] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_DETAIL_SETTING_EX");
    param_1[0x44] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_USER_TEAM");
    param_1[0x45] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_LEAVE");
    param_1[0x4b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_GAME_START");
    param_1[0x46] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_GAME_READY");
    param_1[0x47] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_MATCH_JOIN");
    param_1[0x48] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_MATCH_CANCEL");
    param_1[0x49] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_MATCH_START");
    param_1[0x4a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_INVITE_USER_INFO");
    param_1[0x4c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_ROOM_INVITE");
    param_1[0x4d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ROOM_JOIN_Callback");
    param_1[0x4e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ROOM_INFO_Callback");
    param_1[0x4f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ROOM_USER_INFO_Callback");
    param_1[0x50] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ROOM_INVITE_USER_INFO_Callback");
    param_1[0x51] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ROOM_OPEN_MAP_INFO_Callback");
    param_1[0x52] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ROOM_EVENT_MAP_INFO_Callback");
    param_1[0x53] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ROOM_MATCH_INFO_Callback");
    param_1[0x54] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ROOM_MATCH_JOIN_Callback");
    param_1[0x55] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_ROOM_MATCH_CANCEL_Callback");
    param_1[0x56] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GAME_LOAD_PERCENT");
    param_1[0x57] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GAME_LEAVE");
    param_1[0x58] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GAME_PING");
    param_1[0x59] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GAME_START_PRE_COMPLETE");
    param_1[0x5a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GAME_HIT_COUNT");
    param_1[0x5b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_START_PRE_Callback");
    param_1[0x5c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_START_Callback");
    param_1[0x5d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_LOAD_COMPLETE_Callback");
    param_1[0x5e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_USER_DEATH_Callback");
    param_1[0x5f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_AI_DEATH_Callback");
    param_1[0x60] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_USER_KILL_Callback");
    param_1[0x61] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_AI_KILL_Callback");
    param_1[0x62] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_RESPAWN_Callback");
    param_1[99] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_BOMB_Callback");
    param_1[100] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_OCCUPY_Callback");
    param_1[0x65] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_SUPER_DEFUSING_KIT_Callback");
    param_1[0x66] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_GHOST_Callback");
    param_1[0x67] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_GHOST_KING_Callback");
    param_1[0x68] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_CHECK_BODY_Callback");
    param_1[0x69] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_PVE_DATANET_Callback");
    param_1[0x6a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_PVE_DOLL_Callback");
    param_1[0x6b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_PVE_HELP_Callback");
    param_1[0x6c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_ZOMBIE_Callback");
    param_1[0xb5] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_SCORE_Callback");
    param_1[0x70] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_USER_Callback");
    param_1[0x71] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_REWARD_Callback");
    param_1[0x72] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_BEGIN_ROUND_Callback");
    param_1[0x6d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_ROUND_END_Callback");
    param_1[0x6e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_END_Callback");
    param_1[0x6f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_LEAVE_Callback");
    param_1[0x73] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_USER_LEAVE_Callback");
    param_1[0x74] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_TEAM_CHANGE_Callback");
    param_1[0x75] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GAME_HOST_CHANGE_Callback");
    param_1[0x76] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_START_PRE_Callback");
    param_1[0x77] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_START_Callback");
    param_1[0x78] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_LOAD_COMPLETE_Callback");
    param_1[0x79] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_USER_DEATH_Callback");
    param_1[0x7a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_AI_DEATH_Callback");
    param_1[0x7b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_USER_KILL_Callback");
    param_1[0x7c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_AI_KILL_Callback");
    param_1[0x7d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_RESPAWN_Callback");
    param_1[0x7e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_BOMB_Callback");
    param_1[0x7f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_OCCUPY_Callback");
    param_1[0x80] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_SUPER_DEFUSING_KIT_Callback");
    param_1[0x81] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_GHOST_Callback");
    param_1[0x82] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_GHOST_KING_Callback");
    param_1[0x83] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_CHECK_BODY_Callback");
    param_1[0x84] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_PVE_DATANET_Callback");
    param_1[0x85] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_PVE_DOLL_Callback");
    param_1[0x86] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_PVE_HELP_Callback");
    param_1[0x87] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_ZOMBIE_Callback");
    param_1[0xdc] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_BEGIN_ROUND_Callback");
    param_1[0x8b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_ROUND_END_Callback");
    param_1[0x8c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_END_Callback");
    param_1[0x8d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_SCORE_Callback");
    param_1[0x88] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_USER_Callback");
    param_1[0x89] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_REWARD_Callback");
    param_1[0x8a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_LEAVE_Callback");
    param_1[0x8e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_USER_LEAVE_Callback");
    param_1[0x8f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_TEAM_CHANGE_Callback");
    param_1[0x90] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_GAME_HOST_CHANGE_Callback");
    param_1[0x91] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"IS_HOST");
    param_1[0x92] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"IS_DEDI");
    param_1[0x93] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_MUST_LOAD_COMPLETE");
    param_1[0x97] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_START_WATCH");
    param_1[0x98] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_USER_DEATH");
    param_1[0x99] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_USER_KILL");
    param_1[0x9c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_RESPAWN");
    param_1[0x9f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_INSTANT_RESPAWN");
    param_1[0xa0] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_RESPAWN_COMPLETE");
    param_1[0xa2] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_BOMB");
    param_1[0xa3] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_OCCUPY");
    param_1[0xa5] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_SUPER_DEFUSING_KIT");
    param_1[0xa7] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_GHOST");
    param_1[0xa9] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_CHECK_BODY");
    param_1[0xac] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_PVE_DATANET");
    param_1[0xae] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_PVE_DOLL");
    param_1[0xb0] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_PVE_HELP");
    param_1[0xb2] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_ZOMBIE");
    param_1[0xb4] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_TIMEOUT");
    param_1[0xb8] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_ROUND_END_PVE");
    param_1[0xb9] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_BEGIN_ROUND");
    param_1[0xba] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_LEAVE");
    param_1[0xbe] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_WEAPON_CHANGE");
    param_1[0xc1] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_USER_EQUIP");
    param_1[0xc4] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_PING");
    param_1[0xc3] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_PVE_RANK");
    param_1[0xc6] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_GAME_USER_ITEM");
    param_1[200] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_START_Callback");
    param_1[0x94] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_LOAD_COMPLETE_Callback");
    param_1[0x95] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_USER_LOAD_COMPLETE_Callback");
    param_1[0x96] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_USER_DEATH_Callback");
    param_1[0x9a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_AI_DEATH_Callback");
    param_1[0x9b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_USER_KILL_Callback");
    param_1[0x9d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_AI_KILL_Callback");
    param_1[0x9e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_RESPAWN_Callback");
    param_1[0xa1] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_BOMB_Callback");
    param_1[0xa4] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_OCCUPY_Callback");
    param_1[0xa6] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback");
    param_1[0xa8] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_GHOST_Callback");
    param_1[0xaa] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_GHOST_KING_Callback");
    param_1[0xab] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_CHECK_BODY_Callback");
    param_1[0xad] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_PVE_DATANET_Callback");
    param_1[0xaf] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_PVE_DOLL_Callback");
    param_1[0xb1] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_PVE_HELP_Callback");
    param_1[0xb3] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_ZOMBIE_Callback");
    param_1[0xb5] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_SCORE_Callback");
    param_1[0xb6] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_USER_Callback");
    param_1[0xb7] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_BEGIN_ROUND_Callback");
    param_1[0xbb] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_ROUND_END_Callback");
    param_1[0xbc] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_END_Callback");
    param_1[0xbd] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_LEAVE_Callback");
    param_1[0xbf] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_USER_LEAVE_Callback");
    param_1[0xc0] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_WEAPON_CHANGE_Callback");
    param_1[0xc2] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_USER_EQUIP_Callback");
    param_1[0xc5] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_PVE_RANK_Callback");
    param_1[199] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_USER_ITEM_Callback");
    param_1[0xc9] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HOST_GAME_TEAM_CHANGE_Callback");
    param_1[0xca] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_START_Callback");
    param_1[0xcb] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_LOAD_COMPLETE_Callback");
    param_1[0xcc] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_USER_LOAD_COMPLETE_Callback");
    param_1[0xcd] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_USER_DEATH_Callback");
    param_1[0xce] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_AI_DEATH_Callback");
    param_1[0xcf] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_USER_KILL_Callback");
    param_1[0xd0] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_AI_KILL_Callback");
    param_1[0xd1] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_RESPAWN_Callback");
    param_1[0xd2] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_BOMB_Callback");
    param_1[0xd3] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_OCCUPY_Callback");
    param_1[0xd4] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_SUPER_DEFUSING_KIT_Callback");
    param_1[0xd5] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_GHOST_Callback");
    param_1[0xd6] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_GHOST_KING_Callback");
    param_1[0xd7] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_CHECK_BODY_Callback");
    param_1[0xd8] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_PVE_DATANET_Callback");
    param_1[0xd9] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_PVE_DOLL_Callback");
    param_1[0xda] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_PVE_HELP_Callback");
    param_1[0xdb] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_ZOMBIE_Callback");
    param_1[0xdc] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_SCORE_Callback");
    param_1[0xdd] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_USER_Callback");
    param_1[0xde] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_BEGIN_ROUND_Callback");
    param_1[0xdf] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_ROUND_END_Callback");
    param_1[0xe0] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_END_Callback");
    param_1[0xe1] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_LEAVE_Callback");
    param_1[0xe2] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_USER_LEAVE_Callback");
    param_1[0xe3] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_WEAPON_CHANGE_Callback");
    param_1[0xe4] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_USER_EQUIP_Callback");
    param_1[0xe5] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_PVE_RANK_Callback");
    param_1[0xe6] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_USER_ITEM_Callback");
    param_1[0xe7] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Get_On_HOST_GAME_TEAM_CHANGE_Callback");
    param_1[0xe8] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_JOIN");
    param_1[0xe9] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_LEAVE");
    param_1[0xea] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_WEAPON_SLOT");
    param_1[0xed] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_WEAPON_CUSTOM_SLOT");
    param_1[0xf8] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_WEAR_SLOT");
    param_1[0xee] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_PARTS_SLOT");
    param_1[0xef] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_NICKNAME_CHANGE");
    param_1[0x114] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_RECIPE_ITEM");
    param_1[0xf3] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_JOIN_Callback");
    param_1[0xeb] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_LEAVE_Callback");
    param_1[0xec] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_WEAPON_SLOT_Callback");
    param_1[0xf0] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_WEAR_SLOT_Callback");
    param_1[0xf1] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_PARTS_SLOT_Callback");
    param_1[0xf2] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_RECIPE_ITEM_Callback");
    param_1[0xf4] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_CUSTOM_SLOT_Callback");
    param_1[0xf9] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_BUY_CUSTOM_SLOT");
    param_1[0xf5] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_EQUIP_CUSTOM_SLOT");
    param_1[0xf6] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_BREAK_CUSTOM_SLOT");
    param_1[0xf7] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_OPEN");
    param_1[0xfa] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_CLOSE");
    param_1[0xfb] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_MONEY");
    param_1[0xfe] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_ITEM_BUY");
    param_1[0x100] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_ITEM_EQUIP");
    param_1[0x101] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_ITEM_DELETE");
    param_1[0x102] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_ITEM_USE");
    param_1[0x103] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_ITEM_REPAIR");
    param_1[0x104] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_ITEM_EXPIRE");
    param_1[0x105] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_ITEM_RESELL");
    param_1[0x106] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_CUSTOM_BUY");
    param_1[0x10e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_CUSTOM_EQUIP");
    param_1[0x10f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_CUSTOM_BREAK");
    param_1[0x110] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_EQUIP_WEAR");
    param_1[0x116] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HANGAR_BREAK_WEAR");
    param_1[0x117] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_OPEN_Callback");
    param_1[0xfc] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_CLOSE_Callback");
    param_1[0xfd] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_MONEY_Callback");
    param_1[0xff] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_ITEM_BUY_Callback");
    param_1[0x107] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_ITEM_EQUIP_Callback");
    param_1[0x108] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_ITEM_DELETE_Callback");
    param_1[0x109] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_ITEM_USE_Callback");
    param_1[0x10a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_ITEM_REPAIR_Callback");
    param_1[0x10b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_ITEM_EXPIRE_Callback");
    param_1[0x10c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_ITEM_RESELL_Callback");
    param_1[0x10d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_CUSTOM_BUY_Callback");
    param_1[0x111] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_CUSTOM_EQUIP_Callback");
    param_1[0x112] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_CUSTOM_BREAK_Callback");
    param_1[0x113] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_NICKNAME_CHANGE_Callback");
    param_1[0x115] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_EQUIP_WEAR_Callback");
    param_1[0x118] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_HANGAR_BREAK_WEAR_Callback");
    param_1[0x119] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_RECIPE_OPEN");
    param_1[0x11a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_RECIPE_OPEN_NEW");
    param_1[0x11b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_RECIPE_CLOSE");
    param_1[0x11c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_RECIPE_ITEM");
    param_1[0x11f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_RECIPE_OPEN_Callback");
    param_1[0x11d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_RECIPE_CLOSE_Callback");
    param_1[0x11e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_RECIPE_ITEM_Callback");
    param_1[0x120] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_PERK_OPEN");
    param_1[0x121] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_PERK_OPEN_NEW");
    param_1[0x122] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_PERK_CLOSE");
    param_1[0x123] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_PERK_EQUIP_SLOT");
    param_1[0x126] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_PERK_BREAK_SLOT");
    param_1[0x127] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_PERK_INCREASE_SLOT");
    param_1[0x128] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_PERK_OPEN_Callback");
    param_1[0x124] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_PERK_CLOSE_Callback");
    param_1[0x125] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_PERK_EQUIP_SLOT_Callback");
    param_1[0x129] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_PERK_BREAK_SLOT_Callback");
    param_1[0x12a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_PERK_INCREASE_SLOT_Callback");
    param_1[299] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_FRIEND_REQUEST");
    param_1[300] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_FRIEND_REQUEST_ACCEPT");
    param_1[0x12d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_FRIEND_REQUEST_REFUSE");
    param_1[0x12e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_FRIEND_DELETE");
    param_1[0x12f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_FRIEND_INVITE");
    param_1[0x130] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_FRIEND_REQUEST_Callback");
    param_1[0x131] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_FRIEND_INVITE_Callback");
    param_1[0x132] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_FRIEND_LIST_Callback");
    param_1[0x133] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_FRIEND_REQUEST_LIST_Callback");
    param_1[0x134] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_OPEN");
    param_1[0x135] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_CLOSE");
    param_1[0x136] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_CREATE");
    param_1[0x137] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_DELETE");
    param_1[0x138] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_SEARCH");
    param_1[0x139] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_SEARCH_MASTER");
    param_1[0x13a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_CHECK_CLANNAME");
    param_1[0x13b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_CHANGE_CLANNAME");
    param_1[0x13c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_UPDATE_NOTICE");
    param_1[0x13d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_UPDATE_INTRO");
    param_1[0x13e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_CHANGE_MASTER");
    param_1[0x13f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_MEMBER_POSITION");
    param_1[0x140] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_REQUEST");
    param_1[0x145] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_REQUEST_CANCEL");
    param_1[0x146] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_REQUEST_ACCEPT");
    param_1[0x141] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_REQUEST_REFUSE");
    param_1[0x142] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_KICK_OUT");
    param_1[0x143] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_OUT");
    param_1[0x144] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_INVITE");
    param_1[0x147] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_CHECK_MARK");
    param_1[0x148] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CLAN_CHANGE_MARK");
    param_1[0x149] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_OPEN_Callback");
    param_1[0x14a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_CLOSE_Callback");
    param_1[0x14b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_DELETE_Callback");
    param_1[0x14c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_SEARCH_Callback");
    param_1[0x14d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_CHECK_CLANNAME_Callback");
    param_1[0x14e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_OUT_Callback");
    param_1[0x14f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_KICK_OUT_Callback");
    param_1[0x150] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_REQUEST_Callback");
    param_1[0x151] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_REQUEST_CANCEL_Callback");
    param_1[0x152] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_INVITE_Callback");
    param_1[0x153] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_INFO_Callback");
    param_1[0x156] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_REQUEST_INFO_Callback");
    param_1[0x157] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_MEMBER_INFO_Callback");
    param_1[0x158] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_REQUEST_USER_Callback");
    param_1[0x159] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_CHECK_MARK_Callback");
    param_1[0x154] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CLAN_CHANGE_MARK_Callback");
    param_1[0x155] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_OPEN");
    param_1[0x15a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_CLOSE");
    param_1[0x15b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_WRITE");
    param_1[0x15c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_READ");
    param_1[0x15d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_LOAD_SEND_PAGE");
    param_1[0x15e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_LOAD_RECEIVE_PAGE");
    param_1[0x15f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_DELETE_SEND");
    param_1[0x160] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_DELETE_RECEIVE");
    param_1[0x161] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_CLEAR_SEND");
    param_1[0x162] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_POST_CLEAR_RECEIVE");
    param_1[0x163] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_POST_OPEN_Callback");
    param_1[0x164] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_POST_CLOSE_Callback");
    param_1[0x165] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_POST_UNREAD_Callback");
    param_1[0x166] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_POST_WRITE_Callback");
    param_1[0x167] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_POST_SEND_PAGE_INFO_Callback");
    param_1[0x168] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_POST_RECEIVE_PAGE_INFO_Callback");
    param_1[0x169] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GIFT_WRITE");
    param_1[0x16a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GIFT_READ");
    param_1[0x16b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GIFT_RECEIVE");
    param_1[0x16c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GIFT_LOAD_SEND_PAGE");
    param_1[0x16d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GIFT_LOAD_RECEIVE_PAGE");
    param_1[0x16e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GIFT_DELETE_SEND");
    param_1[0x16f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GIFT_DELETE_RECEIVE");
    param_1[0x170] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GIFT_WRITE_Callback");
    param_1[0x171] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GIFT_RECEIVE_Callback");
    param_1[0x172] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GIFT_SEND_PAGE_INFO_Callback");
    param_1[0x173] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GIFT_RECEIVE_PAGE_INFO_Callback");
    param_1[0x174] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_TUTORIAL_OPEN");
    param_1[0x175] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_TUTORIAL_CLOSE");
    param_1[0x176] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_TUTORIAL_CLEAR");
    param_1[0x177] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_TUTORIAL_OPEN_Callback");
    param_1[0x178] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_TUTORIAL_CLOSE_Callback");
    param_1[0x179] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_TUTORIAL_CLEAR_Callback");
    param_1[0x17a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_DAILY_MISSION_START");
    param_1[0x17c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_DAILY_MISSION_START_SUCCESS_Callback");
    param_1[0x17d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_DAILY_MISSION_START_FAIL_Callback");
    param_1[0x17e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_HOST_DAILY_MISSION_CLEAR");
    param_1[0x17b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_DAILY_MISSION_CLEAR");
    param_1[0x17f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_DAILY_MISSION_CLEAR_SUCCESS_Callback");
    param_1[0x180] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_DAILY_MISSION_CLEAR_FAIL_Callback");
    param_1[0x181] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_DAILY_MISSION_INFO");
    param_1[0x182] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_DAILY_MISSION_INFO_Callback");
    param_1[0x183] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_DAILY_MISSION_NEW_INFO_Callback");
    param_1[0x184] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CHELLENGE_CLEAR");
    param_1[0x185] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CHELLENGE_CLEAR_SUCCESS_Callback");
    param_1[0x186] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CHELLENGE_CLEAR_FAIL_Callback");
    param_1[0x187] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CHELLENGE_MEDAL_GET_Callback");
    param_1[0x188] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CAPSULE_OPEN");
    param_1[0x189] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CAPSULE_CLOSE");
    param_1[0x18a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CAPSULE_OPEN_Callback");
    param_1[0x18b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CAPSULE_CLOSE_Callback");
    param_1[0x18c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CAPSULE_BUY");
    param_1[0x18d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CAPSULE_BUY_Callback");
    param_1[0x18e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_PURCHASE_INIT");
    param_1[399] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_PURCHASE_FINALIZE");
    param_1[400] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_PURCHASE_FINALIZE_Callback");
    param_1[0x191] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GM_WARN_MESSAGE");
    param_1[0x192] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GM_NOTICE_MESSAGE");
    param_1[0x193] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GM_QUIET_USER");
    param_1[0x194] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GM_KICK_USER");
    param_1[0x195] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_GM_BAN_USER");
    param_1[0x196] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GM_WARN_MESSAGE_BY_USER_Callback");
    param_1[0x197] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GM_QUIET_USER_BY_USER_Callback");
    param_1[0x198] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GM_KICK_USER_BY_USER_Callback");
    param_1[0x199] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_GM_BAN_USER_BY_USER_Callback");
    param_1[0x19a] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CHAT_ALL");
    param_1[0x19b] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CHAT_TEAM");
    param_1[0x19f] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CHAT_CLAN");
    param_1[0x19d] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CHAT_CLAN_MANAGER");
    param_1[0x19e] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_CHAT_WHISPER");
    param_1[0x19c] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CHAT_Callback");
    param_1[0x1a0] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_CAPSULE_NOTICE_Callback");
    param_1[0x1a1] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Send_REPORT_USER");
    param_1[0x1a2] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_REPORT_Callback");
    param_1[0x1a3] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_Command_Enum");
    param_1[0x1a5] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_Command_Enum_Callback");
    param_1[0x1a4] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_NOTICE_CHAT_Callback");
    param_1[0x1a6] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_NOTIFY_ERROR_Callback");
    param_1[0x1a7] = uVar2;
    uVar2 = FUN_103e448d(param_1[2],"Set_On_NOTIFY_ERROR_EXIT_Callback");
    param_1[0x1a8] = uVar2;
    uVar2 = (*(code *)param_1[7])();
    *param_1 = uVar2;
    uVar2 = (*(code *)param_1[8])();
    param_1[1] = uVar2;
    (*(code *)param_1[4])();
  }
  return param_1;
}
```

## Clan fields

```
void FUN_10075710(int *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  uint uVar3;
  uint unaff_ESI;
  uint uVar4;
  undefined4 uStack_134;
  char *pcStack_130;
  uint uStack_12c;
  undefined4 *puStack_128;
  undefined4 uStack_124;
  undefined4 uStack_120;
  char *pcStack_11c;
  uint uStack_118;
  undefined4 *puStack_114;
  undefined4 uStack_110;
  undefined4 uStack_10c;
  char *pcStack_108;
  uint uStack_104;
  undefined4 *puStack_100;
  undefined4 uStack_fc;
  undefined4 uStack_f8;
  char *pcStack_f4;
  uint uStack_f0;
  undefined4 *puStack_ec;
  undefined4 uStack_e8;
  undefined4 uStack_e4;
  char *pcStack_e0;
  uint uStack_dc;
  undefined4 *puStack_d8;
  undefined4 uStack_d4;
  undefined4 uStack_d0;
  char *pcStack_cc;
  uint uStack_c8;
  undefined4 *puStack_c4;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  char *pcStack_b8;
  uint uStack_b4;
  undefined4 *puStack_b0;
  undefined4 uStack_ac;
  undefined4 uStack_a8;
  char *pcStack_a4;
  uint uStack_a0;
  uint **ppuStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  char *pcStack_90;
  uint uStack_8c;
  char ***pppcStack_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  char *pcStack_7c;
  uint uStack_78;
  char ***pppcStack_74;
  uint uStack_70;
  uint *puStack_6c;
  char *pcStack_68;
  uint uStack_64;
  char *pcStack_60;
  uint uStack_5c;
  char **ppcStack_58;
  char *pcStack_54;
  uint uStack_50;
  char *pcStack_4c;
  uint uStack_48;
  char **ppcStack_44;
  char *pcStack_40;
  uint uStack_3c;
  char *pcStack_38;
  int *piStack_34;
  char *pcStack_28;
  undefined8 uStack_c;
  
  pcStack_28 = "@@@\t\tCActionScriptMultiJoin::SetClanMemberList()";
  FUN_10370420();
  pcStack_28 = "_global.g_asMonolithMultiplayerMyClanMemberName";
  uVar4 = 0;
  iVar2 = (**(code **)(*param_1 + 0x34))();
  if (iVar2 != 0) {
    do {
      pcStack_38 = "_global.g_asMonolithMultiplayerMyClanMemberName";
      uStack_3c = 4;
      pcStack_40 = (char *)0x10075764;
      piStack_34 = (int *)uVar4;
      (**(code **)(*param_1 + 0x2c))();
      pcStack_40 = (char *)0x1;
      ppcStack_44 = &pcStack_28;
      pcStack_4c = "_global.g_abMonolithMultiplayerMyClanMemberLoginState";
      uStack_50 = 5;
      pcStack_28 = (char *)0x2;
      unaff_ESI = unaff_ESI & 0xffffff00;
      pcStack_54 = (char *)0x10075789;
      uStack_48 = uVar4;
      (**(code **)(*param_1 + 0x2c))();
      pcStack_54 = (char *)0x1;
      ppcStack_58 = &pcStack_28;
      pcStack_60 = "_global.g_anMonolithMultiplayerMyClanMemberClanLevel";
      uStack_64 = 0;
      pcStack_28 = (char *)0xffffffff;
      pcStack_68 = (char *)0x100757a5;
      uStack_5c = uVar4;
      (**(code **)(*param_1 + 0x2c))();
      pcStack_68 = (char *)0x1;
      puStack_6c = &uStack_3c;
      pppcStack_74 = (char ***)0x1042bf98;
      uStack_78 = 0;
      uStack_3c = 0xffffffff;
      pcStack_7c = (char *)0x100757c1;
      uStack_70 = uVar4;
      (**(code **)(*param_1 + 0x2c))();
      pcStack_7c = "_global.g_asMonolithMultiplayerMyClanMemberName";
      uVar4 = uVar4 + 1;
      uStack_80 = 0x100757d2;
      uVar3 = (**(code **)(*param_1 + 0x34))();
    } while (uVar4 < uVar3);
  }
  thunk_FUN_103505f0();
  uVar4 = thunk_FUN_1034ef00();
  uVar3 = 0;
  if (uVar4 != 0) {
    if (*(int *)(uVar4 + 0x2630) == 0) {
      iVar2 = 0;
    }
    else {
      iVar2 = *(int *)(uVar4 + 0x2634) - *(int *)(uVar4 + 0x2630) >> 2;
    }
    uStack_c = (double)iVar2;
    piStack_34 = (int *)0x10075838;
    (**(code **)(*param_1 + 0x24))();
    for (; (iVar2 = *(int *)(uVar4 + 0x2630), iVar2 != 0 &&
           (uVar3 < (uint)(*(int *)(uVar4 + 0x2634) - iVar2 >> 2))); uVar3 = uVar3 + 1) {
      if ((iVar2 == 0) || ((uint)(*(int *)(uVar4 + 0x2634) - iVar2 >> 2) <= uVar3)) {
        piStack_34 = (int *)0x1007587c;
        _invalid_parameter_noinfo();
      }
      puVar1 = *(undefined4 **)(*(int *)(uVar4 + 0x2630) + uVar3 * 4);
      if (puVar1 != (undefined4 *)0x0) {
        piStack_34 = (int *)0x1;
        pcStack_38 = (char *)((int)&uStack_c + 4);
        pcStack_40 = "_global.g_anMonolithMultiplayerMyClanMemberID";
        uStack_c = (double)CONCAT44(*puVar1,(undefined4)uStack_c);
        ppcStack_44 = (char **)0x0;
        uStack_48 = 0x100758ac;
        uStack_3c = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_48 = 1;
        pcStack_4c = &stack0xffffffe4;
        pcStack_54 = "_global.g_asMonolithMultiplayerMyClanMemberName";
        ppcStack_58 = (char **)0x4;
        uStack_5c = 0x100758cb;
        uStack_50 = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_5c = 1;
        pcStack_60 = &stack0xffffffd0;
        pcStack_68 = "_global.g_anMonolithMultiplayerMyClanMemberLevel";
        puStack_6c = (uint *)0x0;
        uStack_70 = 0x100758ea;
        uStack_64 = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_70 = 1;
        pppcStack_74 = &ppcStack_58;
        pcStack_7c = "_global.g_abMonolithMultiplayerMyClanMemberLoginState";
        uStack_80 = 5;
        ppcStack_58 = (char **)0x2;
        uStack_50 = CONCAT31(uStack_50._1_3_,*(char *)(puVar1 + 0x2d) == '\x01');
        uStack_84 = 0x10075919;
        uStack_78 = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        ppcStack_58 = (char **)puVar1[0x1b];
        uStack_84 = 1;
        pppcStack_88 = &ppcStack_58;
        pcStack_90 = "_global.g_anMonolithMultiplayerMyClanMemberRoomIndex";
        uStack_94 = 0;
        uStack_98 = 0x10075938;
        uStack_8c = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        puStack_6c = (uint *)puVar1[0x2c];
        uStack_98 = 1;
        ppuStack_9c = &puStack_6c;
        pcStack_a4 = "_global.g_anMonolithMultiplayerMyClanMemberClanLevel";
        uStack_a8 = 0;
        uStack_ac = 0x1007595a;
        uStack_a0 = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_80 = puVar1[0x1c];
        uStack_ac = 1;
        puStack_b0 = &uStack_80;
        pcStack_b8 = "_global.g_anMonolithMultiplayerMyClanMemberRecordWin";
        uStack_bc = 0;
        uStack_c0 = 0x10075979;
        uStack_b4 = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_94 = puVar1[0x1e];
        uStack_c0 = 1;
        puStack_c4 = &uStack_94;
        pcStack_cc = "_global.g_anMonolithMultiplayerMyClanMemberRecordDraw";
        uStack_d0 = 0;
        uStack_d4 = 0x10075998;
        uStack_c8 = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_a8 = puVar1[0x1d];
        uStack_d4 = 1;
        puStack_d8 = &uStack_a8;
        pcStack_e0 = "_global.g_anMonolithMultiplayerMyClanMemberRecordLose";
        uStack_e4 = 0;
        uStack_e8 = 0x100759b7;
        uStack_dc = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_bc = puVar1[0x20];
        uStack_e8 = 1;
        puStack_ec = &uStack_bc;
        pcStack_f4 = "_global.g_anMonolithMultiplayerMyClanMemberRecordKill";
        uStack_f8 = 0;
        uStack_fc = 0x100759d9;
        uStack_f0 = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_d0 = puVar1[0x21];
        uStack_fc = 1;
        puStack_100 = &uStack_d0;
        pcStack_108 = "_global.g_anMonolithMultiplayerMyClanMemberRecordDeath";
        uStack_10c = 0;
        uStack_110 = 0x100759fb;
        uStack_104 = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_e4 = puVar1[0x24];
        uStack_110 = 1;
        puStack_114 = &uStack_e4;
        pcStack_11c = "_global.g_anMonolithMultiplayerMyClanMemberClanRecordWin";
        uStack_120 = 0;
        uStack_124 = 0x10075a1d;
        uStack_118 = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_f8 = puVar1[0x26];
        uStack_124 = 1;
        puStack_128 = &uStack_f8;
        pcStack_130 = "_global.g_anMonolithMultiplayerMyClanMemberClanRecordDraw";
        uStack_134 = 0;
        uStack_12c = uVar3;
        (**(code **)(*param_1 + 0x2c))();
        uStack_10c = puVar1[0x25];
        (**(code **)(*param_1 + 0x2c))
                  (0,"_global.g_anMonolithMultiplayerMyClanMemberClanRecordLose",uVar3,&uStack_1 0c,1
                  );
        uStack_120 = puVar1[0x28];
        (**(code **)(*param_1 + 0x2c))
                  (0,"_global.g_anMonolithMultiplayerMyClanMemberClanRecordKill",uVar3,&uStack_1 20,1
                  );
        uStack_134 = puVar1[0x29];
        (**(code **)(*param_1 + 0x2c))
                  (0,"_global.g_anMonolithMultiplayerMyClanMemberClanRecordDeath",uVar3,&uStack_ 134,
                   1);
      }
      uVar4 = unaff_ESI;
    }
    piStack_34 = param_1;
    pcStack_38 = &DAT_104a47f0;
    uStack_3c = 0x10075abc;
    thunk_FUN_10073110();
  }
  return;
}
```

## Global states

```
DEFINED	10420c20	s_Global_10420c20	ds "Global"	"Global"	string	7	true
DEFINED	10420fe0	s__global.g_nMonolithGlobalPlatfor_10420fe0	ds "_global.g_nMonolithGlobalPlatform"	"_global.g_nMonolithGlobalPlatform"	string	34	true
DEFINED	1042100c	s__global.g_bMonolithGlobalIsASMes_1042100c	ds "_global.g_bMonolithGlobalIsASMessageBoxBeingShown"	"_global.g_bMonolithGlobalIsASMessageBoxBeingShown"	string	50	true
DEFINED	10421050	s__global.g_bMonolithGlobalIsColle_10421050	ds "_global.g_bMonolithGlobalIsCollectorsEdition"	"_global.g_bMonolithGlobalIsCollectorsEdition"	string	45	true
DEFINED	1042108c	s__global.g_nMonolithGlobalLanguag_1042108c	ds "_global.g_nMonolithGlobalLanguage"	"_global.g_nMonolithGlobalLanguage"	string	34	true
DEFINED	1042127c	s__global.g_bMonolithPerformanceFu_1042127c	ds "_global.g_bMonolithPerformanceFullScreenMode"	"_global.g_bMonolithPerformanceFullScreenMode"	string	45	true
DEFINED	10421584	s_Global.SetFullScreenModeOption_10421584	ds "Global.SetFullScreenModeOption"	"Global.SetFullScreenModeOption"	string	31	true
DEFINED	104215ac	s_Game_Global_AchievementGranted_104215ac	ds "Game_Global_AchievementGranted"	"Game_Global_AchievementGranted"	string	31	true
DEFINED	104215d4	s_Global.IsLowViolenceMode_104215d4	ds "Global.IsLowViolenceMode"	"Global.IsLowViolenceMode"	string	25	true
DEFINED	104215f4	s_Global.PromptDeviceSelection_104215f4	ds "Global.PromptDeviceSelection"	"Global.PromptDeviceSelection"	string	29	true
DEFINED	10421618	s_Global.IsSignedIntoAProfile_10421618	ds "Global.IsSignedIntoAProfile"	"Global.IsSignedIntoAProfile"	string	28	true
DEFINED	1042163c	s_Global.GetAccumulatedTime_1042163c	ds "Global.GetAccumulatedTime"	"Global.GetAccumulatedTime"	string	26	true
DEFINED	1042165c	s_Global.Localize_1042165c	ds "Global.Localize"	"Global.Localize"	string	16	true
DEFINED	10421670	s_Global.MessageBoxResponse_10421670	ds "Global.MessageBoxResponse"	"Global.MessageBoxResponse"	string	26	true
DEFINED	10421690	s_Game_Global_HideMessageBox_10421690	ds "Game_Global_HideMessageBox"	"Game_Global_HideMessageBox"	string	27	true
DEFINED	104216b0	s_Game_Global_ShowMessageBox_104216b0	ds "Game_Global_ShowMessageBox"	"Game_Global_ShowMessageBox"	string	27	true
DEFINED	104216d0	s_Global.StopAllVideo_104216d0	ds "Global.StopAllVideo"	"Global.StopAllVideo"	string	20	true
DEFINED	104216e8	s_Global.StopVideo_104216e8	ds "Global.StopVideo"	"Global.StopVideo"	string	17	true
DEFINED	104216fc	s_Global.PlayVideoLooping_104216fc	ds "Global.PlayVideoLooping"	"Global.PlayVideoLooping"	string	24	true
DEFINED	10421718	s_Global.PlayVideo_10421718	ds "Global.PlayVideo"	"Global.PlayVideo"	string	17	true
DEFINED	1042172c	s_Game_Global_VideoDone_1042172c	ds "Game_Global_VideoDone"	"Game_Global_VideoDone"	string	22	true
DEFINED	10421748	s_Global.StopSoundLoop_10421748	ds "Global.StopSoundLoop"	"Global.StopSoundLoop"	string	21	true
DEFINED	10421764	s_Global.PlaySound_10421764	ds "Global.PlaySound"	"Global.PlaySound"	string	17	true
DEFINED	10421778	s_Global.Exit_10421778	ds "Global.Exit"	"Global.Exit"	string	12	true
DEFINED	10421788	s_Global.OnDestruct_10421788	ds "Global.OnDestruct"	"Global.OnDestruct"	string	18	true
DEFINED	104217a0	s_Global.OnConstruct_104217a0	ds "Global.OnConstruct"	"Global.OnConstruct"	string	19	true
DEFINED	1042182c	s__global.g_bMonolithKeyBindingsHa_1042182c	ds "_global.g_bMonolithKeyBindingsHaltingInput"	"_global.g_bMonolithKeyBindingsHaltingInput"	string	43	true
DEFINED	1042264c	s__global.g_nMonolithMenuNumUsers_1042264c	ds "_global.g_nMonolithMenuNumUsers"	"_global.g_nMonolithMenuNumUsers"	string	32	true
DEFINED	10422674	s__global.g_anMonolithMenuUserInde_10422674	ds "_global.g_anMonolithMenuUserIndex"	"_global.g_anMonolithMenuUserIndex"	string	34	true
DEFINED	1042269c	s__global.g_asMonolithMenuUserName_1042269c	ds "_global.g_asMonolithMenuUserName"	"_global.g_asMonolithMenuUserName"	string	33	true
DEFINED	104226c4	s__global.g_anMonolithMenuUserLeve_104226c4	ds "_global.g_anMonolithMenuUserLevel"	"_global.g_anMonolithMenuUserLevel"	string	34	true
DEFINED	104226ec	s__global.g_abMonolithMenuUserIsCl_104226ec	ds "_global.g_abMonolithMenuUserIsClan"	"_global.g_abMonolithMenuUserIsClan"	string	35	true
DEFINED	10422718	s__global.g_asMonolithMenuUserClan_10422718	ds "_global.g_asMonolithMenuUserClanName"	"_global.g_asMonolithMenuUserClanName"	string	37	true
DEFINED	10422744	s__global.g_anMonolithMenuUserGrad_10422744	ds "_global.g_anMonolithMenuUserGrade"	"_global.g_anMonolithMenuUserGrade"	string	34	true
DEFINED	1042276c	s__global.g_anMonolithMenuUserTeam_1042276c	ds "_global.g_anMonolithMenuUserTeamIndex"	"_global.g_anMonolithMenuUserTeamIndex"	string	38	true
DEFINED	1042279c	s__global.g_anMonolithMenuUserRoom_1042279c	ds "_global.g_anMonolithMenuUserRoomState"	"_global.g_anMonolithMenuUserRoomState"	string	38	true
DEFINED	104227cc	s__global.g_nMonolithMenuUserMyInd_104227cc	ds "_global.g_nMonolithMenuUserMyIndex"	"_global.g_nMonolithMenuUserMyIndex"	string	35	true
DEFINED	104227f8	s__global.g_nMonolithMenuUserHostI_104227f8	ds "_global.g_nMonolithMenuUserHostIndex"	"_global.g_nMonolithMenuUserHostIndex"	string	37	true
DEFINED	10422824	s__global.g_nMonolithMenuPing2_10422824	ds "_global.g_nMonolithMenuPing2"	"_global.g_nMonolithMenuPing2"	string	29	true
DEFINED	10422848	s__global.g_asMonolithMenuCustomLe_10422848	ds "_global.g_asMonolithMenuCustomLevels"	"_global.g_asMonolithMenuCustomLevels"	string	37	true
DEFINED	10422890	s__global.g_asMonolithMenuLBHeader_10422890	ds "_global.g_asMonolithMenuLBHeaderNames"	"_global.g_asMonolithMenuLBHeaderNames"	string	38	true
DEFINED	104228c0	s__global.g_nMonolithMenuNumLBHead_104228c0	ds "_global.g_nMonolithMenuNumLBHeaderNames"	"_global.g_nMonolithMenuNumLBHeaderNames"	string	40	true
DEFINED	104228f0	s__global.g_sMonolithMenuLBName_104228f0	ds "_global.g_sMonolithMenuLBName"	"_global.g_sMonolithMenuLBName"	string	30	true
DEFINED	10422914	s__global.g_sMonolithMenuLBProfile_10422914	ds "_global.g_sMonolithMenuLBProfileName"	"_global.g_sMonolithMenuLBProfileName"	string	37	true
DEFINED	10422940	s__global.g_nMonolithMenuLBRank_10422940	ds "_global.g_nMonolithMenuLBRank"	"_global.g_nMonolithMenuLBRank"	string	30	true
DEFINED	10422964	s__global.g_nMonolithMenuLBScore_10422964	ds "_global.g_nMonolithMenuLBScore"	"_global.g_nMonolithMenuLBScore"	string	31	true
DEFINED	1042298c	s__global.g_nMonolithMenuLBPlayerC_1042298c	ds "_global.g_nMonolithMenuLBPlayerClass"	"_global.g_nMonolithMenuLBPlayerClass"	string	37	true
DEFINED	104229b8	s__global.g_sMonolithMenuLBPlayerC_104229b8	ds "_global.g_sMonolithMenuLBPlayerClassName"	"_global.g_sMonolithMenuLBPlayerClassName"	string	41	true
DEFINED	104229ec	s__global.g_nMonolithMenuLBXP_104229ec	ds "_global.g_nMonolithMenuLBXP"	"_global.g_nMonolithMenuLBXP"	string	28	true
DEFINED	10422a10	s__global.g_anMonolithMenuLBStats_10422a10	ds "_global.g_anMonolithMenuLBStats"	"_global.g_anMonolithMenuLBStats"	string	32	true
DEFINED	10422a38	s__global.g_nMonolithMenuNumLBStat_10422a38	ds "_global.g_nMonolithMenuNumLBStats"	"_global.g_nMonolithMenuNumLBStats"	string	34	true
DEFINED	10422a78	s__global.g_bMonolithMultiplayerMy_10422a78	ds "_global.g_bMonolithMultiplayerMyClanJoined"	"_global.g_bMonolithMultiplayerMyClanJoined"	string	43	true
DEFINED	10423e58	s__global.g_sMonolithMultiplayerHo_10423e58	ds "_global.g_sMonolithMultiplayerHostName"	"_global.g_sMonolithMultiplayerHostName"	string	39	true
DEFINED	10423e88	s__global.g_nMonolithMultiplayerHo_10423e88	ds "_global.g_nMonolithMultiplayerHostID"	"_global.g_nMonolithMultiplayerHostID"	string	37	true
DEFINED	10423eb4	s__global.g_sMonolithMultiplayerGa_10423eb4	ds "_global.g_sMonolithMultiplayerGameMode"	"_global.g_sMonolithMultiplayerGameMode"	string	39	true
DEFINED	10423ee4	s__global.g_sMonolithMultiplayerGa_10423ee4	ds "_global.g_sMonolithMultiplayerGameModeLocalized"	"_global.g_sMonolithMultiplayerGameModeLocalized"	string	48	true
DEFINED	10423f20	s__global.g_sMonolithMultiplayerGa_10423f20	ds "_global.g_sMonolithMultiplayerGameModeInternal"	"_global.g_sMonolithMultiplayerGameModeInternal"	string	47	true
DEFINED	10423f58	s__global.g_nMonolithMultiplayerGa_10423f58	ds "_global.g_nMonolithMultiplayerGameLaunchCountdownTimer"	"_global.g_nMonolithMultiplayerGameLaunchCountdownTimer"	string	55	true
DEFINED	10423f9c	s__global.g_sMonolithMultiplayerMa_10423f9c	ds "_global.g_sMonolithMultiplayerMapName"	"_global.g_sMonolithMultiplayerMapName"	string	38	true
DEFINED	10423fcc	s__global.g_nMonolithMultiplayerMa_10423fcc	ds "_global.g_nMonolithMultiplayerMapIndex"	"_global.g_nMonolithMultiplayerMapIndex"	string	39	true
DEFINED	10423ffc	s__global.g_bMonolithMultiplayerIs_10423ffc	ds "_global.g_bMonolithMultiplayerIsRanked"	"_global.g_bMonolithMultiplayerIsRanked"	string	39	true
DEFINED	1042402c	s__global.g_bMonolithMultiplayerLa_1042402c	ds "_global.g_bMonolithMultiplayerLanOnly"	"_global.g_bMonolithMultiplayerLanOnly"	string	38	true
DEFINED	1042405c	s__global.g_nMonolithMultiplayerTi_1042405c	ds "_global.g_nMonolithMultiplayerTimeLimit"	"_global.g_nMonolithMultiplayerTimeLimit"	string	40	true
DEFINED	1042408c	s__global.g_nMonolithMultiplayerSc_1042408c	ds "_global.g_nMonolithMultiplayerScoreLimit"	"_global.g_nMonolithMultiplayerScoreLimit"	string	41	true
DEFINED	104240c0	s__global.g_nMonolithMultiplayerPl_104240c0	ds "_global.g_nMonolithMultiplayerPlayerLimit"	"_global.g_nMonolithMultiplayerPlayerLimit"	string	42	true
DEFINED	104240f4	s__global.g_nMonolithMultiplayerPr_104240f4	ds "_global.g_nMonolithMultiplayerPrivateSlots"	"_global.g_nMonolithMultiplayerPrivateSlots"	string	43	true
DEFINED	10424128	s__global.g_nMonolithMultiplayerMi_10424128	ds "_global.g_nMonolithMultiplayerMinRankedPlayers"	"_global.g_nMonolithMultiplayerMinRankedPlayers"	string	47	true
DEFINED	10424160	s__global.g_nMonolithMultiplayerNu_10424160	ds "_global.g_nMonolithMultiplayerNumRounds"	"_global.g_nMonolithMultiplayerNumRounds"	string	40	true
DEFINED	10424190	s__global.g_nMonolithMultiplayerCu_10424190	ds "_global.g_nMonolithMultiplayerCurRound"	"_global.g_nMonolithMultiplayerCurRound"	string	39	true
DEFINED	104241c0	s__global.g_nMonolithMultiplayerNu_104241c0	ds "_global.g_nMonolithMultiplayerNumPlayers"	"_global.g_nMonolithMultiplayerNumPlayers"	string	41	true
DEFINED	104241f4	s__global.g_anMonolithMultiplayerP_104241f4	ds "_global.g_anMonolithMultiplayerPlayerLevel"	"_global.g_anMonolithMultiplayerPlayerLevel"	string	43	true
DEFINED	10424228	s__global.g_anMonolithMultiplayerP_10424228	ds "_global.g_anMonolithMultiplayerPlayerPing"	"_global.g_anMonolithMultiplayerPlayerPing"	string	42	true
DEFINED	1042425c	s__global.g_asMonolithMultiplayerP_1042425c	ds "_global.g_asMonolithMultiplayerPlayerName"	"_global.g_asMonolithMultiplayerPlayerName"	string	42	true
DEFINED	10424290	s__global.g_asMonolithMultiplayerP_10424290	ds "_global.g_asMonolithMultiplayerPlayerModel"	"_global.g_asMonolithMultiplayerPlayerModel"	string	43	true
DEFINED	104242c4	s__global.g_asMonolithMultiplayerP_104242c4	ds "_global.g_asMonolithMultiplayerPlayerLoadout"	"_global.g_asMonolithMultiplayerPlayerLoadout"	string	45	true
DEFINED	104242fc	s__global.g_anMonolithMultiplayerP_104242fc	ds "_global.g_anMonolithMultiplayerPlayerVoice"	"_global.g_anMonolithMultiplayerPlayerVoice"	string	43	true
DEFINED	10424330	s__global.g_anMonolithMultiplayerP_10424330	ds "_global.g_anMonolithMultiplayerPlayerTeam"	"_global.g_anMonolithMultiplayerPlayerTeam"	string	42	true
DEFINED	10424364	s__global.g_anMonolithMultiplayerP_10424364	ds "_global.g_anMonolithMultiplayerPlayerRequestedTeam"	"_global.g_anMonolithMultiplayerPlayerRequestedTeam"	string	51	true
DEFINED	104243a4	s__global.g_anMonolithMultiplayerP_104243a4	ds "_global.g_anMonolithMultiplayerPlayerState"	"_global.g_anMonolithMultiplayerPlayerState"	string	43	true
DEFINED	104243d8	s__global.g_anMonolithMultiplayerP_104243d8	ds "_global.g_anMonolithMultiplayerPlayerKills"	"_global.g_anMonolithMultiplayerPlayerKills"	string	43	true
DEFINED	1042440c	s__global.g_anMonolithMultiplayerP_1042440c	ds "_global.g_anMonolithMultiplayerPlayerDeaths"	"_global.g_anMonolithMultiplayerPlayerDeaths"	string	44	true
DEFINED	10424440	s__global.g_anMonolithMultiplayerP_10424440	ds "_global.g_anMonolithMultiplayerPlayerDamageDealt"	"_global.g_anMonolithMultiplayerPlayerDamageDealt"	string	49	true
DEFINED	1042447c	s__global.g_anMonolithMultiplayerP_1042447c	ds "_global.g_anMonolithMultiplayerPlayerDamageTaken"	"_global.g_anMonolithMultiplayerPlayerDamageTaken"	string	49	true
DEFINED	104244b8	s__global.g_anMonolithMultiplayerP_104244b8	ds "_global.g_anMonolithMultiplayerPlayerObjective"	"_global.g_anMonolithMultiplayerPlayerObjective"	string	47	true
DEFINED	104244f0	s__global.g_anMonolithMultiplayerP_104244f0	ds "_global.g_anMonolithMultiplayerPlayerScore"	"_global.g_anMonolithMultiplayerPlayerScore"	string	43	true
DEFINED	10424524	s__global.g_anMonolithMultiplayerP_10424524	ds "_global.g_anMonolithMultiplayerPlayerBonus"	"_global.g_anMonolithMultiplayerPlayerBonus"	string	43	true
DEFINED	10424558	s__global.g_anMonolithMultiplayerP_10424558	ds "_global.g_anMonolithMultiplayerPlayerScans"	"_global.g_anMonolithMultiplayerPlayerScans"	string	43	true
DEFINED	1042458c	s__global.g_anMonolithMultiplayerP_1042458c	ds "_global.g_anMonolithMultiplayerPlayerSupport"	"_global.g_anMonolithMultiplayerPlayerSupport"	string	45	true
DEFINED	104245c4	s__global.g_anMonolithMultiplayerP_104245c4	ds "_global.g_anMonolithMultiplayerPlayerDefenses"	"_global.g_anMonolithMultiplayerPlayerDefenses"	string	46	true
DEFINED	104245fc	s__global.g_anMonolithMultiplayerP_104245fc	ds "_global.g_anMonolithMultiplayerPlayerFinishes"	"_global.g_anMonolithMultiplayerPlayerFinishes"	string	46	true
DEFINED	10424634	s__global.g_anMonolithMultiplayerP_10424634	ds "_global.g_anMonolithMultiplayerPlayerID"	"_global.g_anMonolithMultiplayerPlayerID"	string	40	true
DEFINED	10424664	s__global.g_abMonolithMultiplayerP_10424664	ds "_global.g_abMonolithMultiplayerPlayerReady"	"_global.g_abMonolithMultiplayerPlayerReady"	string	43	true
DEFINED	10424698	s__global.g_abMonolithMultiplayerP_10424698	ds "_global.g_abMonolithMultiplayerPlayerAlive"	"_global.g_abMonolithMultiplayerPlayerAlive"	string	43	true
DEFINED	104246cc	s__global.g_nMonolithMultiplayerNu_104246cc	ds "_global.g_nMonolithMultiplayerNumTeams"	"_global.g_nMonolithMultiplayerNumTeams"	string	39	true
DEFINED	104246fc	s__global.g_asMonolithMultiplayerT_104246fc	ds "_global.g_asMonolithMultiplayerTeamName"	"_global.g_asMonolithMultiplayerTeamName"	string	40	true
DEFINED	1042472c	s__global.g_anMonolithMultiplayerT_1042472c	ds "_global.g_anMonolithMultiplayerTeamScore"	"_global.g_anMonolithMultiplayerTeamScore"	string	41	true
DEFINED	10424760	s__global.g_anMonolithMultiplayerT_10424760	ds "_global.g_anMonolithMultiplayerTeamRoundsWon"	"_global.g_anMonolithMultiplayerTeamRoundsWon"	string	45	true
DEFINED	10424798	s__global.g_nMonolithMultiplayerBa_10424798	ds "_global.g_nMonolithMultiplayerBackGroundMusicID"	"_global.g_nMonolithMultiplayerBackGroundMusicID"	string	48	true
DEFINED	104247d4	s__global.g_nMonolithMultiplayerTa_104247d4	ds "_global.g_nMonolithMultiplayerTableEventKey"	"_global.g_nMonolithMultiplayerTableEventKey"	string	44	true
DEFINED	10424808	s__global.g_asMonolithMultiplayerR_10424808	ds "_global.g_asMonolithMultiplayerRadioMsg_X"	"_global.g_asMonolithMultiplayerRadioMsg_X"	string	42	true
DEFINED	1042483c	s__global.g_asMonolithMultiplayerR_1042483c	ds "_global.g_asMonolithMultiplayerRadioMsg_Z"	"_global.g_asMonolithMultiplayerRadioMsg_Z"	string	42	true
DEFINED	10424870	s__global.g_asMonolithMultiplayerR_10424870	ds "_global.g_asMonolithMultiplayerRadioMsg_C"	"_global.g_asMonolithMultiplayerRadioMsg_C"	string	42	true
DEFINED	104248a4	s__global.g_bMonolithMultiplayerFi_104248a4	ds "_global.g_bMonolithMultiplayerFinalBuild"	"_global.g_bMonolithMultiplayerFinalBuild"	string	41	true
DEFINED	104248d8	s__global.g_nMonolithMultiplayerLa_104248d8	ds "_global.g_nMonolithMultiplayerLaunchTimer"	"_global.g_nMonolithMultiplayerLaunchTimer"	string	42	true
DEFINED	1042490c	s__global.g_anMonolithMultiplayerP_1042490c	ds "_global.g_anMonolithMultiplayerPostIndex"	"_global.g_anMonolithMultiplayerPostIndex"	string	41	true
DEFINED	10424940	s__global.g_anMonolithMultiplayerP_10424940	ds "_global.g_anMonolithMultiplayerPostType"	"_global.g_anMonolithMultiplayerPostType"	string	40	true
DEFINED	10424970	s__global.g_asMonolithMultiplayerP_10424970	ds "_global.g_asMonolithMultiplayerPostSenderName"	"_global.g_asMonolithMultiplayerPostSenderName"	string	46	true
DEFINED	104249a8	s__global.g_asMonolithMultiplayerP_104249a8	ds "_global.g_asMonolithMultiplayerPostReceveurName"	"_global.g_asMonolithMultiplayerPostReceveurName"	string	48	true
DEFINED	104249e4	s__global.g_anMonolithMultiplayerP_104249e4	ds "_global.g_anMonolithMultiplayerPostReceveurLevel"	"_global.g_anMonolithMultiplayerPostReceveurLevel"	string	49	true
DEFINED	10424a20	s__global.g_asMonolithMultiplayerP_10424a20	ds "_global.g_asMonolithMultiplayerPostTitle"	"_global.g_asMonolithMultiplayerPostTitle"	string	41	true
DEFINED	10424a54	s__global.g_asMonolithMultiplayerP_10424a54	ds "_global.g_asMonolithMultiplayerPostMsg"	"_global.g_asMonolithMultiplayerPostMsg"	string	39	true
DEFINED	10424a84	s__global.g_anMonolithMultiplayerP_10424a84	ds "_global.g_anMonolithMultiplayerPostReceiveDateYear"	"_global.g_anMonolithMultiplayerPostReceiveDateYear"	string	51	true
DEFINED	10424ac4	s__global.g_anMonolithMultiplayerP_10424ac4	ds "_global.g_anMonolithMultiplayerPostReceiveDateMonth"	"_global.g_anMonolithMultiplayerPostReceiveDateMonth"	string	52	true
DEFINED	10424b04	s__global.g_anMonolithMultiplayerP_10424b04	ds "_global.g_anMonolithMultiplayerPostReceiveDateDay"	"_global.g_anMonolithMultiplayerPostReceiveDateDay"	string	50	true
DEFINED	10424b40	s__global.g_anMonolithMultiplayerP_10424b40	ds "_global.g_anMonolithMultiplayerPostReceiveDateHour"	"_global.g_anMonolithMultiplayerPostReceiveDateHour"	string	51	true
DEFINED	10424b80	s__global.g_anMonolithMultiplayerP_10424b80	ds "_global.g_anMonolithMultiplayerPostReceiveDateMin"	"_global.g_anMonolithMultiplayerPostReceiveDateMin"	string	50	true
DEFINED	10424bbc	s__global.g_anMonolithMultiplayerP_10424bbc	ds "_global.g_anMonolithMultiplayerPostReadTerm"	"_global.g_anMonolithMultiplayerPostReadTerm"	string	44	true
DEFINED	10424bf0	s__global.g_abMonolithMultiplayerP_10424bf0	ds "_global.g_abMonolithMultiplayerPostReadCheck"	"_global.g_abMonolithMultiplayerPostReadCheck"	string	45	true
DEFINED	10424c28	s__global.g_anMonolithMultiplayerP_10424c28	ds "_global.g_anMonolithMultiplayerPostGiftReadState"	"_global.g_anMonolithMultiplayerPostGiftReadState"	string	49	true
DEFINED	10424c64	s__global.g_anMonolithMultiplayerP_10424c64	ds "_global.g_anMonolithMultiplayerPostGiftItemIndex"	"_global.g_anMonolithMultiplayerPostGiftItemIndex"	string	49	true
DEFINED	10424ca0	s__global.g_nMonolithMultiplayerPo_10424ca0	ds "_global.g_nMonolithMultiplayerPostListCount"	"_global.g_nMonolithMultiplayerPostListCount"	string	44	true
DEFINED	10424cd4	s__global.g_anMonolithMultiplayerU_10424cd4	ds "_global.g_anMonolithMultiplayerUserInfoScoreModeIndex"	"_global.g_anMonolithMultiplayerUserInfoScoreModeIndex"	string	54	true
DEFINED	10424d14	s__global.g_anMonolithMultiplayerU_10424d14	ds "_global.g_anMonolithMultiplayerUserInfoScoreModeWin"	"_global.g_anMonolithMultiplayerUserInfoScoreModeWin"	string	52	true
DEFINED	10424d54	s__global.g_anMonolithMultiplayerU_10424d54	ds "_global.g_anMonolithMultiplayerUserInfoScoreModeDraw"	"_global.g_anMonolithMultiplayerUserInfoScoreModeDraw"	string	53	true
DEFINED	10424d94	s__global.g_anMonolithMultiplayerU_10424d94	ds "_global.g_anMonolithMultiplayerUserInfoScoreModeLose"	"_global.g_anMonolithMultiplayerUserInfoScoreModeLose"	string	53	true
DEFINED	10424dd4	s__global.g_nMonolithMultiplayerMo_10424dd4	ds "_global.g_nMonolithMultiplayerModeScoreListCount"	"_global.g_nMonolithMultiplayerModeScoreListCount"	string	49	true
DEFINED	10424e10	s__global.g_anMonolithMultiplayerU_10424e10	ds "_global.g_anMonolithMultiplayerUserInfoScoreMapIndex"	"_global.g_anMonolithMultiplayerUserInfoScoreMapIndex"	string	53	true
DEFINED	10424e50	s__global.g_anMonolithMultiplayerU_10424e50	ds "_global.g_anMonolithMultiplayerUserInfoScoreMapWin"	"_global.g_anMonolithMultiplayerUserInfoScoreMapWin"	string	51	true
DEFINED	10424e90	s__global.g_anMonolithMultiplayerU_10424e90	ds "_global.g_anMonolithMultiplayerUserInfoScoreMapDraw"	"_global.g_anMonolithMultiplayerUserInfoScoreMapDraw"	string	52	true
DEFINED	10424ed0	s__global.g_anMonolithMultiplayerU_10424ed0	ds "_global.g_anMonolithMultiplayerUserInfoScoreMapLose"	"_global.g_anMonolithMultiplayerUserInfoScoreMapLose"	string	52	true
DEFINED	10424f10	s__global.g_nMonolithMultiplayerMa_10424f10	ds "_global.g_nMonolithMultiplayerMapScoreListCount"	"_global.g_nMonolithMultiplayerMapScoreListCount"	string	48	true
DEFINED	10424f4c	s__global.g_nMonolithMultiplayerLo_10424f4c	ds "_global.g_nMonolithMultiplayerLoginWin_PVP"	"_global.g_nMonolithMultiplayerLoginWin_PVP"	string	43	true
DEFINED	10424f80	s__global.g_nMonolithMultiplayerLo_10424f80	ds "_global.g_nMonolithMultiplayerLoginDraw_PVP"	"_global.g_nMonolithMultiplayerLoginDraw_PVP"	string	44	true
DEFINED	10424fb4	s__global.g_nMonolithMultiplayerLo_10424fb4	ds "_global.g_nMonolithMultiplayerLoginLose_PVP"	"_global.g_nMonolithMultiplayerLoginLose_PVP"	string	44	true
DEFINED	10424fe8	s__global.g_nMonolithMultiplayerLo_10424fe8	ds "_global.g_nMonolithMultiplayerLoginWin_PVE"	"_global.g_nMonolithMultiplayerLoginWin_PVE"	string	43	true
DEFINED	1042501c	s__global.g_nMonolithMultiplayerLo_1042501c	ds "_global.g_nMonolithMultiplayerLoginLose_PVE"	"_global.g_nMonolithMultiplayerLoginLose_PVE"	string	44	true
DEFINED	10425050	s__global.g_anMonolithMultiplayerC_10425050	ds "_global.g_anMonolithMultiplayerChallengesIndex"	"_global.g_anMonolithMultiplayerChallengesIndex"	string	47	true
DEFINED	10425088	s__global.g_anMonolithMultiplayerC_10425088	ds "_global.g_anMonolithMultiplayerChallengesPhase"	"_global.g_anMonolithMultiplayerChallengesPhase"	string	47	true
DEFINED	104250c0	s__global.g_anMonolithMultiplayerE_104250c0	ds "_global.g_anMonolithMultiplayerEventType"	"_global.g_anMonolithMultiplayerEventType"	string	41	true
DEFINED	104250f4	s__global.g_anMonolithMultiplayerE_104250f4	ds "_global.g_anMonolithMultiplayerEventModeIndex"	"_global.g_anMonolithMultiplayerEventModeIndex"	string	46	true
DEFINED	1042512c	s__global.g_anMonolithMultiplayerE_1042512c	ds "_global.g_anMonolithMultiplayerEventMapIndex"	"_global.g_anMonolithMultiplayerEventMapIndex"	string	45	true
DEFINED	10425164	s__global.g_anMonolithMultiplayerE_10425164	ds "_global.g_anMonolithMultiplayerEventExpRatio"	"_global.g_anMonolithMultiplayerEventExpRatio"	string	45	true
DEFINED	1042519c	s__global.g_anMonolithMultiplayerE_1042519c	ds "_global.g_anMonolithMultiplayerEventPointRatio"	"_global.g_anMonolithMultiplayerEventPointRatio"	string	47	true
DEFINED	104251d4	s__global.g_nMonolithMultiplayerEv_104251d4	ds "_global.g_nMonolithMultiplayerEventCount"	"_global.g_nMonolithMultiplayerEventCount"	string	41	true
DEFINED	10425208	s__global.g_sMonolithMultiplayerBa_10425208	ds "_global.g_sMonolithMultiplayerBannerURL1"	"_global.g_sMonolithMultiplayerBannerURL1"	string	41	true
DEFINED	1042523c	s__global.g_sMonolithMultiplayerBa_1042523c	ds "_global.g_sMonolithMultiplayerBannerURL2"	"_global.g_sMonolithMultiplayerBannerURL2"	string	41	true
DEFINED	10425270	s__global.g_sMonolithMultiplayerBa_10425270	ds "_global.g_sMonolithMultiplayerBannerURL3"	"_global.g_sMonolithMultiplayerBannerURL3"	string	41	true
DEFINED	104252a4	s__global.g_sMonolithMultiplayerBa_104252a4	ds "_global.g_sMonolithMultiplayerBannerURL4"	"_global.g_sMonolithMultiplayerBannerURL4"	string	41	true
DEFINED	104252d8	s__global.g_sMonolithMultiplayerBa_104252d8	ds "_global.g_sMonolithMultiplayerBannerURL5"	"_global.g_sMonolithMultiplayerBannerURL5"	string	41	true
DEFINED	1042530c	s__global.g_sMonolithMultiplayerBa_1042530c	ds "_global.g_sMonolithMultiplayerBannerURL6"	"_global.g_sMonolithMultiplayerBannerURL6"	string	41	true
DEFINED	10425340	s__global.g_nMonolithMultiplayerBa_10425340	ds "_global.g_nMonolithMultiplayerBannerCount"	"_global.g_nMonolithMultiplayerBannerCount"	string	42	true
DEFINED	10425374	s__global.g_asMonolithMultiplayerM_10425374	ds "_global.g_asMonolithMultiplayerModelList"	"_global.g_asMonolithMultiplayerModelList"	string	41	true
DEFINED	104253a8	s__global.g_nMonolithMultiplayerMo_104253a8	ds "_global.g_nMonolithMultiplayerModelListCount"	"_global.g_nMonolithMultiplayerModelListCount"	string	45	true
DEFINED	104253e0	s__global.g_asMonolithMultiplayerL_104253e0	ds "_global.g_asMonolithMultiplayerLoadoutList"	"_global.g_asMonolithMultiplayerLoadoutList"	string	43	true
DEFINED	10425414	s__global.g_nMonolithMultiplayerLo_10425414	ds "_global.g_nMonolithMultiplayerLoadoutListCount"	"_global.g_nMonolithMultiplayerLoadoutListCount"	string	47	true
DEFINED	1042544c	s__global.g_nMonolithMultiplayerFi_1042544c	ds "_global.g_nMonolithMultiplayerFilterGameModeIndex"	"_global.g_nMonolithMultiplayerFilterGameModeIndex"	string	50	true
DEFINED	10425488	s__global.g_asMonolithMultiplayerF_10425488	ds "_global.g_asMonolithMultiplayerFilterGameModes"	"_global.g_asMonolithMultiplayerFilterGameModes"	string	47	true
DEFINED	104254c0	s__global.g_nMonolithMultiplayerNu_104254c0	ds "_global.g_nMonolithMultiplayerNumFilterGameModes"	"_global.g_nMonolithMultiplayerNumFilterGameModes"	string	49	true
DEFINED	104254fc	s__global.g_nMonolithMultiplayerFi_104254fc	ds "_global.g_nMonolithMultiplayerFilterMapIndex"	"_global.g_nMonolithMultiplayerFilterMapIndex"	string	45	true
DEFINED	10425534	s__global.g_asMonolithMultiplayerF_10425534	ds "_global.g_asMonolithMultiplayerFilterMaps"	"_global.g_asMonolithMultiplayerFilterMaps"	string	42	true
DEFINED	10425568	s__global.g_asMonolithMultiplayerF_10425568	ds "_global.g_asMonolithMultiplayerFilterMapRecords"	"_global.g_asMonolithMultiplayerFilterMapRecords"	string	48	true
DEFINED	104255a4	s__global.g_nMonolithMultiplayerNu_104255a4	ds "_global.g_nMonolithMultiplayerNumFilterMaps"	"_global.g_nMonolithMultiplayerNumFilterMaps"	string	44	true
DEFINED	104255d8	s__global.g_bMonolithMultiplayerFi_104255d8	ds "_global.g_bMonolithMultiplayerFilterIsRanked"	"_global.g_bMonolithMultiplayerFilterIsRanked"	string	45	true
DEFINED	10425610	s__global.g_bMonolithMultiplayerFi_10425610	ds "_global.g_bMonolithMultiplayerFilterEmpty"	"_global.g_bMonolithMultiplayerFilterEmpty"	string	42	true
DEFINED	10425644	s__global.g_bMonolithMultiplayerFi_10425644	ds "_global.g_bMonolithMultiplayerFilterPassword"	"_global.g_bMonolithMultiplayerFilterPassword"	string	45	true
DEFINED	1042567c	s__global.g_nMonolithMultiplayerFi_1042567c	ds "_global.g_nMonolithMultiplayerFilterPingIndex"	"_global.g_nMonolithMultiplayerFilterPingIndex"	string	46	true
DEFINED	104256b4	s__global.g_anMonolithMultiplayerF_104256b4	ds "_global.g_anMonolithMultiplayerFilterPingArray"	"_global.g_anMonolithMultiplayerFilterPingArray"	string	47	true
DEFINED	104256ec	s__global.g_nMonolithMultiplayerNu_104256ec	ds "_global.g_nMonolithMultiplayerNumFilterPingArrayEntries"	"_global.g_nMonolithMultiplayerNumFilterPingArrayEntries"	string	56	true
DEFINED	10425730	s__global.g_nMonolithMultiplayerWi_10425730	ds "_global.g_nMonolithMultiplayerWinningTeam"	"_global.g_nMonolithMultiplayerWinningTeam"	string	42	true
DEFINED	10425764	s__global.g_nMonolithMultiplayerWi_10425764	ds "_global.g_nMonolithMultiplayerWinningPlayerID"	"_global.g_nMonolithMultiplayerWinningPlayerID"	string	46	true
DEFINED	1042579c	s__global.g_nMonolithMultiplayerRo_1042579c	ds "_global.g_nMonolithMultiplayerRoundEndCondition"	"_global.g_nMonolithMultiplayerRoundEndCondition"	string	48	true
DEFINED	104257d8	s__global.g_nMonolithMultiplayerEn_104257d8	ds "_global.g_nMonolithMultiplayerEndRoundTime"	"_global.g_nMonolithMultiplayerEndRoundTime"	string	43	true
DEFINED	1042580c	s__global.g_anMonolithMenuUserClan_1042580c	ds "_global.g_anMonolithMenuUserClanMark0"	"_global.g_anMonolithMenuUserClanMark0"	string	38	true
DEFINED	1042583c	s__global.g_anMonolithMenuUserClan_1042583c	ds "_global.g_anMonolithMenuUserClanMark1"	"_global.g_anMonolithMenuUserClanMark1"	string	38	true
DEFINED	1042586c	s__global.g_anMonolithMenuUserClan_1042586c	ds "_global.g_anMonolithMenuUserClanMark2"	"_global.g_anMonolithMenuUserClanMark2"	string	38	true
DEFINED	104258c4	s__global.g_asMonolithMultiplayerM_104258c4	ds "_global.g_asMonolithMultiplayerMyNickName"	"_global.g_asMonolithMultiplayerMyNickName"	string	42	true
DEFINED	104258f8	s__global.g_nMonolithMultiplayerMy_104258f8	ds "_global.g_nMonolithMultiplayerMyLevel"	"_global.g_nMonolithMultiplayerMyLevel"	string	38	true
DEFINED	10425928	s__global.g_nMonolithMultiplayerMy_10425928	ds "_global.g_nMonolithMultiplayerMyUserGrade"	"_global.g_nMonolithMultiplayerMyUserGrade"	string	42	true
DEFINED	10426cb0	s__global.g_abMonolithMultiplayerP_10426cb0	ds "_global.g_abMonolithMultiplayerPostReadTerm"	"_global.g_abMonolithMultiplayerPostReadTerm"	string	44	true
DEFINED	1042831c	s__global.g_asMonolithMultiplayerH_1042831c	ds "_global.g_asMonolithMultiplayerHostGameModes"	"_global.g_asMonolithMultiplayerHostGameModes"	string	45	true
DEFINED	10428354	s__global.g_asMonolithMultiplayerH_10428354	ds "_global.g_asMonolithMultiplayerHostGameModesInternal"	"_global.g_asMonolithMultiplayerHostGameModesInternal"	string	53	true
DEFINED	10428394	s__global.g_asMonolithMultiplayerH_10428394	ds "_global.g_asMonolithMultiplayerHostRestrictableItems"	"_global.g_asMonolithMultiplayerHostRestrictableItems"	string	53	true
DEFINED	104283d4	s__global.g_nMonolithMultiplayerHo_104283d4	ds "_global.g_nMonolithMultiplayerHostGameMode"	"_global.g_nMonolithMultiplayerHostGameMode"	string	43	true
DEFINED	10428408	s__global.g_abMonolithMultiplayerH_10428408	ds "_global.g_abMonolithMultiplayerHostRestrictedItems"	"_global.g_abMonolithMultiplayerHostRestrictedItems"	string	51	true
DEFINED	10428448	s__global.g_bMonolithMultiplayerHo_10428448	ds "_global.g_bMonolithMultiplayerHostLockServer"	"_global.g_bMonolithMultiplayerHostLockServer"	string	45	true
DEFINED	10428480	s__global.g_sMonolithMultiplayerHo_10428480	ds "_global.g_sMonolithMultiplayerHostPassword"	"_global.g_sMonolithMultiplayerHostPassword"	string	43	true
DEFINED	104284b4	s__global.g_bMonolithMultiplayerHo_104284b4	ds "_global.g_bMonolithMultiplayerHostLanOnly"	"_global.g_bMonolithMultiplayerHostLanOnly"	string	42	true
DEFINED	104284e8	s__global.g_bMonolithMultiplayerHo_104284e8	ds "_global.g_bMonolithMultiplayerHostIsRanked"	"_global.g_bMonolithMultiplayerHostIsRanked"	string	43	true
DEFINED	1042851c	s__global.g_nMonolithMultiplayerHo_1042851c	ds "_global.g_nMonolithMultiplayerHostNumPrivateSlots"	"_global.g_nMonolithMultiplayerHostNumPrivateSlots"	string	50	true
DEFINED	10428558	s__global.g_nMonolithMultiplayerHo_10428558	ds "_global.g_nMonolithMultiplayerHostNumAvailableMaps"	"_global.g_nMonolithMultiplayerHostNumAvailableMaps"	string	51	true
DEFINED	10428598	s__global.g_asMonolithMultiplayerH_10428598	ds "_global.g_asMonolithMultiplayerHostAvailableMaps"	"_global.g_asMonolithMultiplayerHostAvailableMaps"	string	49	true
DEFINED	104285d4	s__global.g_asMonolithMultiplayerH_104285d4	ds "_global.g_asMonolithMultiplayerHostAvailableMapRecords"	"_global.g_asMonolithMultiplayerHostAvailableMapRecords"	string	55	true
DEFINED	10428618	s__global.g_nMonolithMultiplayerHo_10428618	ds "_global.g_nMonolithMultiplayerHostNumSelectedMaps"	"_global.g_nMonolithMultiplayerHostNumSelectedMaps"	string	50	true
DEFINED	10428654	s__global.g_asMonolithMultiplayerH_10428654	ds "_global.g_asMonolithMultiplayerHostSelectedMaps"	"_global.g_asMonolithMultiplayerHostSelectedMaps"	string	48	true
DEFINED	10428690	s__global.g_asMonolithMultiplayerH_10428690	ds "_global.g_asMonolithMultiplayerHostRuleName"	"_global.g_asMonolithMultiplayerHostRuleName"	string	44	true
DEFINED	104286c4	s__global.g_asMonolithMultiplayerH_104286c4	ds "_global.g_asMonolithMultiplayerHostRuleFriendlyName"	"_global.g_asMonolithMultiplayerHostRuleFriendlyName"	string	52	true
DEFINED	10428704	s__global.g_afMonolithMultiplayerH_10428704	ds "_global.g_afMonolithMultiplayerHostRuleValue"	"_global.g_afMonolithMultiplayerHostRuleValue"	string	45	true
DEFINED	1042873c	s__global.g_afMonolithMultiplayerH_1042873c	ds "_global.g_afMonolithMultiplayerHostRuleMinValue"	"_global.g_afMonolithMultiplayerHostRuleMinValue"	string	48	true
DEFINED	10428778	s__global.g_afMonolithMultiplayerH_10428778	ds "_global.g_afMonolithMultiplayerHostRuleMaxValue"	"_global.g_afMonolithMultiplayerHostRuleMaxValue"	string	48	true
DEFINED	104287b4	s__global.g_afMonolithMultiplayerH_104287b4	ds "_global.g_afMonolithMultiplayerHostRuleIncAmount"	"_global.g_afMonolithMultiplayerHostRuleIncAmount"	string	49	true
DEFINED	104287f0	s__global.g_nMonolithMultiplayerHo_104287f0	ds "_global.g_nMonolithMultiplayerHostNumRules"	"_global.g_nMonolithMultiplayerHostNumRules"	string	43	true
DEFINED	10428824	s__global.g_asMonolithMultiplayerH_10428824	ds "_global.g_asMonolithMultiplayerHostRuleTeamSizeBalancingArray"	"_global.g_asMonolithMultiplayerHostRuleTeamSizeBalancingArray"	string	62	true
DEFINED	10428870	s__global.g_asMonolithMultiplayerH_10428870	ds "_global.g_asMonolithMultiplayerHostRuleTeamScoreBalancingArray"	"_global.g_asMonolithMultiplayerHostRuleTeamScoreBalancingArray"	string	63	true
DEFINED	10428da8	s__global.g_nMonolithMultiplayerCh_10428da8	ds "_global.g_nMonolithMultiplayerChallengesListCount"	"_global.g_nMonolithMultiplayerChallengesListCount"	string	50	true
DEFINED	10428de4	s__global.g_asMonolithMultiplayerJ_10428de4	ds "_global.g_asMonolithMultiplayerJoinServerName"	"_global.g_asMonolithMultiplayerJoinServerName"	string	46	true
DEFINED	10428e1c	s__global.g_anMonolithMultiplayerJ_10428e1c	ds "_global.g_anMonolithMultiplayerJoinCurNumPlayers"	"_global.g_anMonolithMultiplayerJoinCurNumPlayers"	string	49	true
DEFINED	10428e58	s__global.g_anMonolithMultiplayerJ_10428e58	ds "_global.g_anMonolithMultiplayerJoinMaxPlayers"	"_global.g_anMonolithMultiplayerJoinMaxPlayers"	string	46	true
DEFINED	10428e90	s__global.g_asMonolithMultiplayerJ_10428e90	ds "_global.g_asMonolithMultiplayerJoinMapName"	"_global.g_asMonolithMultiplayerJoinMapName"	string	43	true
DEFINED	10428ec4	s__global.g_asMonolithMultiplayerJ_10428ec4	ds "_global.g_asMonolithMultiplayerJoinMapRecordName"	"_global.g_asMonolithMultiplayerJoinMapRecordName"	string	49	true
DEFINED	10428f00	s__global.g_asMonolithMultiplayerJ_10428f00	ds "_global.g_asMonolithMultiplayerJoinGameType"	"_global.g_asMonolithMultiplayerJoinGameType"	string	44	true
DEFINED	10428f34	s__global.g_anMonolithMultiplayerJ_10428f34	ds "_global.g_anMonolithMultiplayerJoinPing"	"_global.g_anMonolithMultiplayerJoinPing"	string	40	true
DEFINED	10428f64	s__global.g_abMonolithMultiplayerJ_10428f64	ds "_global.g_abMonolithMultiplayerJoinPasswordProtected"	"_global.g_abMonolithMultiplayerJoinPasswordProtected"	string	53	true
DEFINED	10428fa4	s__global.g_asMonolithMultiplayerJ_10428fa4	ds "_global.g_asMonolithMultiplayerJoinVersion"	"_global.g_asMonolithMultiplayerJoinVersion"	string	43	true
DEFINED	10428fd8	s__global.g_nMonolithMultiplayerJo_10428fd8	ds "_global.g_nMonolithMultiplayerJoinNumServers"	"_global.g_nMonolithMultiplayerJoinNumServers"	string	45	true
DEFINED	10429010	s__global.g_asMonolithMultiplayerJ_10429010	ds "_global.g_asMonolithMultiplayerJoinRoomName"	"_global.g_asMonolithMultiplayerJoinRoomName"	string	44	true
DEFINED	10429044	s__global.g_anMonolithMultiplayerJ_10429044	ds "_global.g_anMonolithMultiplayerJoinRoomNum"	"_global.g_anMonolithMultiplayerJoinRoomNum"	string	43	true
DEFINED	10429078	s__global.g_anMonolithMultiplayerJ_10429078	ds "_global.g_anMonolithMultiplayerJoinRoomMap"	"_global.g_anMonolithMultiplayerJoinRoomMap"	string	43	true
DEFINED	104290ac	s__global.g_anMonolithMultiplayerJ_104290ac	ds "_global.g_anMonolithMultiplayerJoinRoomMode"	"_global.g_anMonolithMultiplayerJoinRoomMode"	string	44	true
DEFINED	104290e0	s__global.g_anMonolithMultiplayerJ_104290e0	ds "_global.g_anMonolithMultiplayerJoinRoomCurPlayers"	"_global.g_anMonolithMultiplayerJoinRoomCurPlayers"	string	50	true
DEFINED	1042911c	s__global.g_anMonolithMultiplayerJ_1042911c	ds "_global.g_anMonolithMultiplayerJoinRoomMaxPlayers"	"_global.g_anMonolithMultiplayerJoinRoomMaxPlayers"	string	50	true
DEFINED	10429158	s__global.g_abMonolithMultiplayerJ_10429158	ds "_global.g_abMonolithMultiplayerJoinRoomState"	"_global.g_abMonolithMultiplayerJoinRoomState"	string	45	true
DEFINED	10429190	s__global.g_anMonolithMultiplayerJ_10429190	ds "_global.g_anMonolithMultiplayerJoinRoomPing"	"_global.g_anMonolithMultiplayerJoinRoomPing"	string	44	true
DEFINED	104291c4	s__global.g_abMonolithMultiplayerJ_104291c4	ds "_global.g_abMonolithMultiplayerJoinRoomPublic"	"_global.g_abMonolithMultiplayerJoinRoomPublic"	string	46	true
DEFINED	104291fc	s__global.g_asMonolithMultiplayerJ_104291fc	ds "_global.g_asMonolithMultiplayerJoinRoomHostName"	"_global.g_asMonolithMultiplayerJoinRoomHostName"	string	48	true
DEFINED	10429238	s__global.g_anMonolithMultiplayerJ_10429238	ds "_global.g_anMonolithMultiplayerJoinRoomGoalKill"	"_global.g_anMonolithMultiplayerJoinRoomGoalKill"	string	48	true
DEFINED	10429274	s__global.g_anMonolithMultiplayerJ_10429274	ds "_global.g_anMonolithMultiplayerJoinRoomGoalTime"	"_global.g_anMonolithMultiplayerJoinRoomGoalTime"	string	48	true
DEFINED	104292b0	s__global.g_anMonolithMultiplayerJ_104292b0	ds "_global.g_anMonolithMultiplayerJoinRoomGoalRound"	"_global.g_anMonolithMultiplayerJoinRoomGoalRound"	string	49	true
DEFINED	104292ec	s__global.g_abMonolithMultiplayerJ_104292ec	ds "_global.g_abMonolithMultiplayerJoinRoomTeamBal"	"_global.g_abMonolithMultiplayerJoinRoomTeamBal"	string	47	true
DEFINED	10429324	s__global.g_abMonolithMultiplayerJ_10429324	ds "_global.g_abMonolithMultiplayerJoinRoomDeathChat"	"_global.g_abMonolithMultiplayerJoinRoomDeathChat"	string	49	true
DEFINED	10429360	s__global.g_abMonolithMultiplayerJ_10429360	ds "_global.g_abMonolithMultiplayerJoinRoomInstruction"	"_global.g_abMonolithMultiplayerJoinRoomInstruction"	string	51	true
DEFINED	104293a0	s__global.g_abMonolithMultiplayerJ_104293a0	ds "_global.g_abMonolithMultiplayerJoinRoomPerkUse"	"_global.g_abMonolithMultiplayerJoinRoomPerkUse"	string	47	true
DEFINED	104293d8	s__global.g_abMonolithMultiplayerJ_104293d8	ds "_global.g_abMonolithMultiplayerJoinRoomThirdViewUse"	"_global.g_abMonolithMultiplayerJoinRoomThirdViewUse"	string	52	true
DEFINED	10429418	s__global.g_anMonolithMultiplayerJ_10429418	ds "_global.g_anMonolithMultiplayerJoinRoomType"	"_global.g_anMonolithMultiplayerJoinRoomType"	string	44	true
DEFINED	1042944c	s__global.g_anMonolithMultiplayerJ_1042944c	ds "_global.g_anMonolithMultiplayerJoinRoomTableIndex"	"_global.g_anMonolithMultiplayerJoinRoomTableIndex"	string	50	true
DEFINED	10429488	s__global.g_anMonolithMultiplayerM_10429488	ds "_global.g_anMonolithMultiplayerMapTableMapIndex"	"_global.g_anMonolithMultiplayerMapTableMapIndex"	string	48	true
DEFINED	104294c4	s__global.g_anMonolithMultiplayerM_104294c4	ds "_global.g_anMonolithMultiplayerMapTableMapMode"	"_global.g_anMonolithMultiplayerMapTableMapMode"	string	47	true
DEFINED	104294fc	s__global.g_asMonolithMultiplayerM_104294fc	ds "_global.g_asMonolithMultiplayerMapTableUser"	"_global.g_asMonolithMultiplayerMapTableUser"	string	44	true
DEFINED	10429530	s__global.g_anMonolithMultiplayerM_10429530	ds "_global.g_anMonolithMultiplayerMapTableUserDefault"	"_global.g_anMonolithMultiplayerMapTableUserDefault"	string	51	true
DEFINED	10429570	s__global.g_asMonolithMultiplayerM_10429570	ds "_global.g_asMonolithMultiplayerMapTableTime"	"_global.g_asMonolithMultiplayerMapTableTime"	string	44	true
DEFINED	104295a4	s__global.g_anMonolithMultiplayerM_104295a4	ds "_global.g_anMonolithMultiplayerMapTableTimeDefault"	"_global.g_anMonolithMultiplayerMapTableTimeDefault"	string	51	true
DEFINED	104295e4	s__global.g_asMonolithMultiplayerM_104295e4	ds "_global.g_asMonolithMultiplayerMapTableKill"	"_global.g_asMonolithMultiplayerMapTableKill"	string	44	true
DEFINED	10429618	s__global.g_anMonolithMultiplayerM_10429618	ds "_global.g_anMonolithMultiplayerMapTableKillDefault"	"_global.g_anMonolithMultiplayerMapTableKillDefault"	string	51	true
DEFINED	10429658	s__global.g_asMonolithMultiplayerM_10429658	ds "_global.g_asMonolithMultiplayerMapTableGoal"	"_global.g_asMonolithMultiplayerMapTableGoal"	string	44	true
DEFINED	1042968c	s__global.g_anMonolithMultiplayerM_1042968c	ds "_global.g_anMonolithMultiplayerMapTableGoalDefault"	"_global.g_anMonolithMultiplayerMapTableGoalDefault"	string	51	true
DEFINED	104296cc	s__global.g_asMonolithMultiplayerM_104296cc	ds "_global.g_asMonolithMultiplayerMapTableRound"	"_global.g_asMonolithMultiplayerMapTableRound"	string	45	true
DEFINED	10429704	s__global.g_anMonolithMultiplayerM_10429704	ds "_global.g_anMonolithMultiplayerMapTableRoundDefault"	"_global.g_anMonolithMultiplayerMapTableRoundDefault"	string	52	true
DEFINED	10429744	s__global.g_asMonolithMultiplayerM_10429744	ds "_global.g_asMonolithMultiplayerMapTableMapName"	"_global.g_asMonolithMultiplayerMapTableMapName"	string	47	true
DEFINED	1042977c	s__global.g_asMonolithMultiplayerM_1042977c	ds "_global.g_asMonolithMultiplayerMapTableModeName"	"_global.g_asMonolithMultiplayerMapTableModeName"	string	48	true
DEFINED	104297b8	s__global.g_nMonolithMultiplayerMa_104297b8	ds "_global.g_nMonolithMultiplayerMapTableMapCount"	"_global.g_nMonolithMultiplayerMapTableMapCount"	string	47	true
DEFINED	104297f0	s__global.g_asMonolithMultiplayerM_104297f0	ds "_global.g_asMonolithMultiplayerMapTableMapDisplayName"	"_global.g_asMonolithMultiplayerMapTableMapDisplayName"	string	54	true
DEFINED	10429830	s__global.g_asMonolithMultiplayerM_10429830	ds "_global.g_asMonolithMultiplayerMapTableModeDisplayName"	"_global.g_asMonolithMultiplayerMapTableModeDisplayName"	string	55	true
DEFINED	10429874	s__global.g_anMonolithMultiplayerM_10429874	ds "_global.g_anMonolithMultiplayerMapTableDisplayOrder"	"_global.g_anMonolithMultiplayerMapTableDisplayOrder"	string	52	true
DEFINED	104298b4	s__global.g_anMonolithMultiplayerM_104298b4	ds "_global.g_anMonolithMultiplayerMapTableDifficulty"	"_global.g_anMonolithMultiplayerMapTableDifficulty"	string	50	true
DEFINED	104298f0	s__global.g_anMonolithMultiplayerM_104298f0	ds "_global.g_anMonolithMultiplayerMapTableLimitWeapon"	"_global.g_anMonolithMultiplayerMapTableLimitWeapon"	string	51	true
DEFINED	10429930	s__global.g_anMonolithMultiplayerM_10429930	ds "_global.g_anMonolithMultiplayerMapTableWinRewardIndex"	"_global.g_anMonolithMultiplayerMapTableWinRewardIndex"	string	54	true
DEFINED	10429970	s__global.g_anMonolithMultiplayerM_10429970	ds "_global.g_anMonolithMultiplayerMapTableLoseRewardIndex"	"_global.g_anMonolithMultiplayerMapTableLoseRewardIndex"	string	55	true
DEFINED	104299b4	s__global.g_anMonolithMultiplayerM_104299b4	ds "_global.g_anMonolithMultiplayerMapTableDrawRewardIndex"	"_global.g_anMonolithMultiplayerMapTableDrawRewardIndex"	string	55	true
DEFINED	104299f8	s__global.g_anMonolithMultiplayerM_104299f8	ds "_global.g_anMonolithMultiplayerMapTableStreamingType"	"_global.g_anMonolithMultiplayerMapTableStreamingType"	string	53	true
DEFINED	10429a38	s__global.g_abMonolithMultiplayerM_10429a38	ds "_global.g_abMonolithMultiplayerMapFinishStreaming"	"_global.g_abMonolithMultiplayerMapFinishStreaming"	string	50	true
DEFINED	10429a74	s__global.g_abMonolithMultiplayerM_10429a74	ds "_global.g_abMonolithMultiplayerMapTableIsOpenMap"	"_global.g_abMonolithMultiplayerMapTableIsOpenMap"	string	49	true
DEFINED	10429ab0	s__global.g_abMonolithMultiplayerM_10429ab0	ds "_global.g_abMonolithMultiplayerMapTableIsMatchMap"	"_global.g_abMonolithMultiplayerMapTableIsMatchMap"	string	50	true
DEFINED	10429aec	s__global.g_anMonolithMultiplayerC_10429aec	ds "_global.g_anMonolithMultiplayerClassTableClassIndex"	"_global.g_anMonolithMultiplayerClassTableClassIndex"	string	52	true
DEFINED	10429b2c	s__global.g_anMonolithMultiplayerC_10429b2c	ds "_global.g_anMonolithMultiplayerClassTableClassLevel"	"_global.g_anMonolithMultiplayerClassTableClassLevel"	string	52	true
DEFINED	10429b6c	s__global.g_anMonolithMultiplayerC_10429b6c	ds "_global.g_anMonolithMultiplayerClassTableClassType"	"_global.g_anMonolithMultiplayerClassTableClassType"	string	51	true
DEFINED	10429bac	s__global.g_anMonolithMultiplayerC_10429bac	ds "_global.g_anMonolithMultiplayerClassTableClassCareer"	"_global.g_anMonolithMultiplayerClassTableClassCareer"	string	53	true
DEFINED	10429bec	s__global.g_asMonolithMultiplayerC_10429bec	ds "_global.g_asMonolithMultiplayerClassTableClassName"	"_global.g_asMonolithMultiplayerClassTableClassName"	string	51	true
DEFINED	10429c2c	s__global.g_anMonolithMultiplayerC_10429c2c	ds "_global.g_anMonolithMultiplayerClassTableExpMin"	"_global.g_anMonolithMultiplayerClassTableExpMin"	string	48	true
DEFINED	10429c68	s__global.g_anMonolithMultiplayerC_10429c68	ds "_global.g_anMonolithMultiplayerClassTableExpMax"	"_global.g_anMonolithMultiplayerClassTableExpMax"	string	48	true
DEFINED	10429ca4	s__global.g_anMonolithMultiplayerC_10429ca4	ds "_global.g_anMonolithMultiplayerClassTablePointValue"	"_global.g_anMonolithMultiplayerClassTablePointValue"	string	52	true
DEFINED	10429ce4	s__global.g_anMonolithMultiplayerC_10429ce4	ds "_global.g_anMonolithMultiplayerClassTableRewardItem1"	"_global.g_anMonolithMultiplayerClassTableRewardItem1"	string	53	true
DEFINED	10429d24	s__global.g_anMonolithMultiplayerC_10429d24	ds "_global.g_anMonolithMultiplayerClassTableRewardItem2"	"_global.g_anMonolithMultiplayerClassTableRewardItem2"	string	53	true
DEFINED	10429d64	s__global.g_anMonolithMultiplayerC_10429d64	ds "_global.g_anMonolithMultiplayerClassTableRewardItem3"	"_global.g_anMonolithMultiplayerClassTableRewardItem3"	string	53	true
DEFINED	10429da4	s__global.g_anMonolithMultiplayerH_10429da4	ds "_global.g_anMonolithMultiplayerHangarSerialIndex"	"_global.g_anMonolithMultiplayerHangarSerialIndex"	string	49	true
DEFINED	10429de0	s__global.g_anMonolithMultiplayerH_10429de0	ds "_global.g_anMonolithMultiplayerHangarItemIndex"	"_global.g_anMonolithMultiplayerHangarItemIndex"	string	47	true
DEFINED	10429e18	s__global.g_anMonolithMultiplayerH_10429e18	ds "_global.g_anMonolithMultiplayerHangarUseCount"	"_global.g_anMonolithMultiplayerHangarUseCount"	string	46	true
DEFINED	10429e50	s__global.g_anMonolithMultiplayerH_10429e50	ds "_global.g_anMonolithMultiplayerHangarUseTime"	"_global.g_anMonolithMultiplayerHangarUseTime"	string	45	true
DEFINED	10429e88	s__global.g_abMonolithMultiplayerH_10429e88	ds "_global.g_abMonolithMultiplayerHangarbActive"	"_global.g_abMonolithMultiplayerHangarbActive"	string	45	true
DEFINED	10429ec0	s__global.g_anMonolithMultiplayerH_10429ec0	ds "_global.g_anMonolithMultiplayerHangarPerkItemIndex0"	"_global.g_anMonolithMultiplayerHangarPerkItemIndex0"	string	52	true
DEFINED	10429f00	s__global.g_anMonolithMultiplayerH_10429f00	ds "_global.g_anMonolithMultiplayerHangarPerkItemIndex1"	"_global.g_anMonolithMultiplayerHangarPerkItemIndex1"	string	52	true
DEFINED	10429f40	s__global.g_anMonolithMultiplayerH_10429f40	ds "_global.g_anMonolithMultiplayerHangarPerkItemIndex2"	"_global.g_anMonolithMultiplayerHangarPerkItemIndex2"	string	52	true
DEFINED	10429f80	s__global.g_anMonolithMultiplayerH_10429f80	ds "_global.g_anMonolithMultiplayerHangarPerkSerialIndex0"	"_global.g_anMonolithMultiplayerHangarPerkSerialIndex0"	string	54	true
DEFINED	10429fc0	s__global.g_anMonolithMultiplayerH_10429fc0	ds "_global.g_anMonolithMultiplayerHangarPerkSerialIndex1"	"_global.g_anMonolithMultiplayerHangarPerkSerialIndex1"	string	54	true
DEFINED	1042a000	s__global.g_anMonolithMultiplayerH_1042a000	ds "_global.g_anMonolithMultiplayerHangarPerkSerialIndex2"	"_global.g_anMonolithMultiplayerHangarPerkSerialIndex2"	string	54	true
DEFINED	1042a040	s__global.g_abMonolithMultiplayerH_1042a040	ds "_global.g_abMonolithMultiplayerHangarItemExpire"	"_global.g_abMonolithMultiplayerHangarItemExpire"	string	48	true
DEFINED	1042a07c	s__global.g_anMonolithMultiplayerH_1042a07c	ds "_global.g_anMonolithMultiplayerHangarCustomHaveSerialIndex"	"_global.g_anMonolithMultiplayerHangarCustomHaveSerialIndex"	string	59	true
DEFINED	1042a0c8	s__global.g_anMonolithMultiplayerH_1042a0c8	ds "_global.g_anMonolithMultiplayerHangarCustomHaveParentSerialIndex"	"_global.g_anMonolithMultiplayerHangarCustomHaveParentSerialIndex"	string	65	true
DEFINED	1042a118	s__global.g_anMonolithMultiplayerH_1042a118	ds "_global.g_anMonolithMultiplayerHangarCustomHaveItemIndex"	"_global.g_anMonolithMultiplayerHangarCustomHaveItemIndex"	string	57	true
DEFINED	1042a15c	s__global.g_anMonolithMultiplayerH_1042a15c	ds "_global.g_anMonolithMultiplayerHangarCustomHaveType"	"_global.g_anMonolithMultiplayerHangarCustomHaveType"	string	52	true
DEFINED	1042a19c	s__global.g_abMonolithMultiplayerH_1042a19c	ds "_global.g_abMonolithMultiplayerHangarCustomHaveActive"	"_global.g_abMonolithMultiplayerHangarCustomHaveActive"	string	54	true
DEFINED	1042a1dc	s__global.g_abMonolithMultiplayerH_1042a1dc	ds "_global.g_abMonolithMultiplayerHangarCustomHaveEquip"	"_global.g_abMonolithMultiplayerHangarCustomHaveEquip"	string	53	true
DEFINED	1042a21c	s__global.g_anMonolithMultiplayerH_1042a21c	ds "_global.g_anMonolithMultiplayerHangarCustomHaveUseValue"	"_global.g_anMonolithMultiplayerHangarCustomHaveUseValue"	string	56	true
DEFINED	1042a260	s__global.g_anMonolithMultiplayerH_1042a260	ds "_global.g_anMonolithMultiplayerHangarMySlotWeapon"	"_global.g_anMonolithMultiplayerHangarMySlotWeapon"	string	50	true
DEFINED	1042a29c	s__global.g_anMonolithMultiplayerH_1042a29c	ds "_global.g_anMonolithMultiplayerHangarMySlotHero"	"_global.g_anMonolithMultiplayerHangarMySlotHero"	string	48	true
DEFINED	1042a2d8	s__global.g_anMonolithMultiplayerH_1042a2d8	ds "_global.g_anMonolithMultiplayerHangarMyCustomWearSerialIndex"	"_global.g_anMonolithMultiplayerHangarMyCustomWearSerialIndex"	string	61	true
DEFINED	1042a324	s__global.g_anMonolithMultiplayerH_1042a324	ds "_global.g_anMonolithMultiplayerHangarMyCustomWearItemIndex"	"_global.g_anMonolithMultiplayerHangarMyCustomWearItemIndex"	string	59	true
DEFINED	1042a36c	s__global.g_anMonolithMultiplayerH_1042a36c	ds "_global.g_anMonolithMultiplayerHangarMyWearSerialIndex0"	"_global.g_anMonolithMultiplayerHangarMyWearSerialIndex0"	string	56	true
DEFINED	1042a3b0	s__global.g_anMonolithMultiplayerH_1042a3b0	ds "_global.g_anMonolithMultiplayerHangarMyWearSerialIndex1"	"_global.g_anMonolithMultiplayerHangarMyWearSerialIndex1"	string	56	true
DEFINED	1042a3f4	s__global.g_anMonolithMultiplayerH_1042a3f4	ds "_global.g_anMonolithMultiplayerHangarMyWearSerialIndex2"	"_global.g_anMonolithMultiplayerHangarMyWearSerialIndex2"	string	56	true
DEFINED	1042a438	s__global.g_anMonolithMultiplayerH_1042a438	ds "_global.g_anMonolithMultiplayerHangarMyWearSerialIndex3"	"_global.g_anMonolithMultiplayerHangarMyWearSerialIndex3"	string	56	true
DEFINED	1042a47c	s__global.g_anMonolithMultiplayerH_1042a47c	ds "_global.g_anMonolithMultiplayerHangarMyWearItemIndex0"	"_global.g_anMonolithMultiplayerHangarMyWearItemIndex0"	string	54	true
DEFINED	1042a4bc	s__global.g_anMonolithMultiplayerH_1042a4bc	ds "_global.g_anMonolithMultiplayerHangarMyWearItemIndex1"	"_global.g_anMonolithMultiplayerHangarMyWearItemIndex1"	string	54	true
DEFINED	1042a4fc	s__global.g_anMonolithMultiplayerH_1042a4fc	ds "_global.g_anMonolithMultiplayerHangarMyWearItemIndex2"	"_global.g_anMonolithMultiplayerHangarMyWearItemIndex2"	string	54	true
DEFINED	1042a53c	s__global.g_anMonolithMultiplayerH_1042a53c	ds "_global.g_anMonolithMultiplayerHangarMyWearItemIndex3"	"_global.g_anMonolithMultiplayerHangarMyWearItemIndex3"	string	54	true
DEFINED	1042a57c	s__global.g_nMonolithMultiplayerHa_1042a57c	ds "_global.g_nMonolithMultiplayerHangarMyPoint"	"_global.g_nMonolithMultiplayerHangarMyPoint"	string	44	true
DEFINED	1042a5b0	s__global.g_nMonolithMultiplayerHa_1042a5b0	ds "_global.g_nMonolithMultiplayerHangarMyCash"	"_global.g_nMonolithMultiplayerHangarMyCash"	string	43	true
DEFINED	1042a5e4	s__global.g_anMonolithMultiplayerJ_1042a5e4	ds "_global.g_anMonolithMultiplayerJoinChannelNum"	"_global.g_anMonolithMultiplayerJoinChannelNum"	string	46	true
DEFINED	1042a61c	s__global.g_asMonolithMultiplayerJ_1042a61c	ds "_global.g_asMonolithMultiplayerJoinChannelName"	"_global.g_asMonolithMultiplayerJoinChannelName"	string	47	true
DEFINED	1042a654	s__global.g_anMonolithMultiplayerJ_1042a654	ds "_global.g_anMonolithMultiplayerJoinChannelUserCount"	"_global.g_anMonolithMultiplayerJoinChannelUserCount"	string	52	true
DEFINED	1042a694	s__global.g_anMonolithMultiplayerJ_1042a694	ds "_global.g_anMonolithMultiplayerJoinChannelMaxUserCount"	"_global.g_anMonolithMultiplayerJoinChannelMaxUserCount"	string	55	true
DEFINED	1042a6d8	s__global.g_anMonolithMultiplayerJ_1042a6d8	ds "_global.g_anMonolithMultiplayerJoinChannelType"	"_global.g_anMonolithMultiplayerJoinChannelType"	string	47	true
DEFINED	1042a710	s__global.g_anMonolithMultiplayerJ_1042a710	ds "_global.g_anMonolithMultiplayerJoinChannelPing"	"_global.g_anMonolithMultiplayerJoinChannelPing"	string	47	true
DEFINED	1042a748	s__global.g_anMonolithMultiplayerJ_1042a748	ds "_global.g_anMonolithMultiplayerJoinLobbyNum"	"_global.g_anMonolithMultiplayerJoinLobbyNum"	string	44	true
DEFINED	1042a77c	s__global.g_anMonolithMultiplayerJ_1042a77c	ds "_global.g_anMonolithMultiplayerJoinLobbytoChannelNum"	"_global.g_anMonolithMultiplayerJoinLobbytoChannelNum"	string	53	true
DEFINED	1042a7bc	s__global.g_asMonolithMultiplayerJ_1042a7bc	ds "_global.g_asMonolithMultiplayerJoinLobbyName"	"_global.g_asMonolithMultiplayerJoinLobbyName"	string	45	true
DEFINED	1042a7f4	s__global.g_anMonolithMultiplayerJ_1042a7f4	ds "_global.g_anMonolithMultiplayerJoinLobbyUserCount"	"_global.g_anMonolithMultiplayerJoinLobbyUserCount"	string	50	true
DEFINED	1042a830	s__global.g_anMonolithMultiplayerJ_1042a830	ds "_global.g_anMonolithMultiplayerJoinLobbyMaxUserCount"	"_global.g_anMonolithMultiplayerJoinLobbyMaxUserCount"	string	53	true
DEFINED	1042a870	s__global.g_anMonolithMultiplayerJ_1042a870	ds "_global.g_anMonolithMultiplayerJoinLobbyExpMax"	"_global.g_anMonolithMultiplayerJoinLobbyExpMax"	string	47	true
DEFINED	1042a8a8	s__global.g_anMonolithMultiplayerJ_1042a8a8	ds "_global.g_anMonolithMultiplayerJoinUserInviteIndex"	"_global.g_anMonolithMultiplayerJoinUserInviteIndex"	string	51	true
DEFINED	1042a8e8	s__global.g_anMonolithMultiplayerJ_1042a8e8	ds "_global.g_anMonolithMultiplayerJoinUserInviteLevel"	"_global.g_anMonolithMultiplayerJoinUserInviteLevel"	string	51	true
DEFINED	1042a928	s__global.g_asMonolithMultiplayerJ_1042a928	ds "_global.g_asMonolithMultiplayerJoinUserInviteName"	"_global.g_asMonolithMultiplayerJoinUserInviteName"	string	50	true
DEFINED	1042a964	s__global.g_abMonolithMultiplayerJ_1042a964	ds "_global.g_abMonolithMultiplayerJoinUserInviteIsClan"	"_global.g_abMonolithMultiplayerJoinUserInviteIsClan"	string	52	true
DEFINED	1042a9a4	s__global.g_asMonolithMultiplayerJ_1042a9a4	ds "_global.g_asMonolithMultiplayerJoinUserInviteClanName"	"_global.g_asMonolithMultiplayerJoinUserInviteClanName"	string	54	true
DEFINED	1042a9e4	s__global.g_nMonolithMultiplayerJo_1042a9e4	ds "_global.g_nMonolithMultiplayerJoinUserInviteCount"	"_global.g_nMonolithMultiplayerJoinUserInviteCount"	string	50	true
DEFINED	1042aa20	s__global.g_nMonolithMultiplayerJo_1042aa20	ds "_global.g_nMonolithMultiplayerJoinInviteRoomIndex"	"_global.g_nMonolithMultiplayerJoinInviteRoomIndex"	string	50	true
DEFINED	1042aa5c	s__global.g_anMonolithMultiplayerJ_1042aa5c	ds "_global.g_anMonolithMultiplayerJoinUserInviteClanMark0"	"_global.g_anMonolithMultiplayerJoinUserInviteClanMark0"	string	55	true
DEFINED	1042aaa0	s__global.g_anMonolithMultiplayerJ_1042aaa0	ds "_global.g_anMonolithMultiplayerJoinUserInviteClanMark1"	"_global.g_anMonolithMultiplayerJoinUserInviteClanMark1"	string	55	true
DEFINED	1042aae4	s__global.g_anMonolithMultiplayerJ_1042aae4	ds "_global.g_anMonolithMultiplayerJoinUserInviteClanMark2"	"_global.g_anMonolithMultiplayerJoinUserInviteClanMark2"	string	55	true
DEFINED	1042ab28	s__global.g_anMonolithMultiplayerH_1042ab28	ds "_global.g_anMonolithMultiplayerHangarTableItemIndex"	"_global.g_anMonolithMultiplayerHangarTableItemIndex"	string	52	true
DEFINED	1042ab68	s__global.g_anMonolithMultiplayerH_1042ab68	ds "_global.g_anMonolithMultiplayerHangarTableRepresentIndex"	"_global.g_anMonolithMultiplayerHangarTableRepresentIndex"	string	57	true
DEFINED	1042abac	s__global.g_anMonolithMultiplayerH_1042abac	ds "_global.g_anMonolithMultiplayerHangarTableHighGroupIndex"	"_global.g_anMonolithMultiplayerHangarTableHighGroupIndex"	string	57	true
DEFINED	1042abf0	s__global.g_anMonolithMultiplayerH_1042abf0	ds "_global.g_anMonolithMultiplayerHangarTableMiddleGroupIndex"	"_global.g_anMonolithMultiplayerHangarTableMiddleGroupIndex"	string	59	true
DEFINED	1042ac38	s__global.g_anMonolithMultiplayerH_1042ac38	ds "_global.g_anMonolithMultiplayerHangarTableFunctionIndex"	"_global.g_anMonolithMultiplayerHangarTableFunctionIndex"	string	56	true
DEFINED	1042ac7c	s__global.g_anMonolithMultiplayerH_1042ac7c	ds "_global.g_anMonolithMultiplayerHangarTableIsResell"	"_global.g_anMonolithMultiplayerHangarTableIsResell"	string	51	true
DEFINED	1042acbc	s__global.g_anMonolithMultiplayerH_1042acbc	ds "_global.g_anMonolithMultiplayerHangarTableResellPoint"	"_global.g_anMonolithMultiplayerHangarTableResellPoint"	string	54	true
DEFINED	1042acfc	s__global.g_anMonolithMultiplayerH_1042acfc	ds "_global.g_anMonolithMultiplayerHangarTableUseTypeIndex"	"_global.g_anMonolithMultiplayerHangarTableUseTypeIndex"	string	55	true
DEFINED	1042ad40	s__global.g_anMonolithMultiplayerH_1042ad40	ds "_global.g_anMonolithMultiplayerHangarTableDurability"	"_global.g_anMonolithMultiplayerHangarTableDurability"	string	53	true
DEFINED	1042ad80	s__global.g_anMonolithMultiplayerH_1042ad80	ds "_global.g_anMonolithMultiplayerHangarTableUseCount"	"_global.g_anMonolithMultiplayerHangarTableUseCount"	string	51	true
DEFINED	1042adc0	s__global.g_anMonolithMultiplayerH_1042adc0	ds "_global.g_anMonolithMultiplayerHangarTableIsSell"	"_global.g_anMonolithMultiplayerHangarTableIsSell"	string	49	true
DEFINED	1042adfc	s__global.g_anMonolithMultiplayerH_1042adfc	ds "_global.g_anMonolithMultiplayerHangarTableItemDisplayOrder"	"_global.g_anMonolithMultiplayerHangarTableItemDisplayOrder"	string	59	true
DEFINED	1042ae44	s__global.g_anMonolithMultiplayerH_1042ae44	ds "_global.g_anMonolithMultiplayerHangarTableIsShopShow"	"_global.g_anMonolithMultiplayerHangarTableIsShopShow"	string	53	true
DEFINED	1042ae84	s__global.g_anMonolithMultiplayerH_1042ae84	ds "_global.g_anMonolithMultiplayerHangarTableUseTime"	"_global.g_anMonolithMultiplayerHangarTableUseTime"	string	50	true
DEFINED	1042aec0	s__global.g_asMonolithMultiplayerH_1042aec0	ds "_global.g_asMonolithMultiplayerHangarTableItemName"	"_global.g_asMonolithMultiplayerHangarTableItemName"	string	51	true
DEFINED	1042af00	s__global.g_asMonolithMultiplayerH_1042af00	ds "_global.g_asMonolithMultiplayerHangarTableRecordName"	"_global.g_asMonolithMultiplayerHangarTableRecordName"	string	53	true
DEFINED	1042af40	s__global.g_anMonolithMultiplayerH_1042af40	ds "_global.g_anMonolithMultiplayerHangarTableBuyPopupOrder"	"_global.g_anMonolithMultiplayerHangarTableBuyPopupOrder"	string	56	true
DEFINED	1042af84	s__global.g_anMonolithMultiplayerH_1042af84	ds "_global.g_anMonolithMultiplayerHangarTableRowNum"	"_global.g_anMonolithMultiplayerHangarTableRowNum"	string	49	true
DEFINED	1042afc0	s__global.g_anMonolithMultiplayerH_1042afc0	ds "_global.g_anMonolithMultiplayerHangarTablePointPrice"	"_global.g_anMonolithMultiplayerHangarTablePointPrice"	string	53	true
DEFINED	1042b000	s__global.g_anMonolithMultiplayerH_1042b000	ds "_global.g_anMonolithMultiplayerHangarTableCashPrice"	"_global.g_anMonolithMultiplayerHangarTableCashPrice"	string	52	true
DEFINED	1042b040	s__global.g_anMonolithMultiplayerH_1042b040	ds "_global.g_anMonolithMultiplayerHangarTableBonusPoint"	"_global.g_anMonolithMultiplayerHangarTableBonusPoint"	string	53	true
DEFINED	1042b080	s__global.g_anMonolithMultiplayerH_1042b080	ds "_global.g_anMonolithMultiplayerHangarTableCashType"	"_global.g_anMonolithMultiplayerHangarTableCashType"	string	51	true
DEFINED	1042b0c0	s__global.g_anMonolithMultiplayerH_1042b0c0	ds "_global.g_anMonolithMultiplayerHangarTableAttack"	"_global.g_anMonolithMultiplayerHangarTableAttack"	string	49	true
DEFINED	1042b0fc	s__global.g_anMonolithMultiplayerH_1042b0fc	ds "_global.g_anMonolithMultiplayerHangarTableAccuracy"	"_global.g_anMonolithMultiplayerHangarTableAccuracy"	string	51	true
DEFINED	1042b13c	s__global.g_anMonolithMultiplayerH_1042b13c	ds "_global.g_anMonolithMultiplayerHangarTableShotSpeed"	"_global.g_anMonolithMultiplayerHangarTableShotSpeed"	string	52	true
DEFINED	1042b17c	s__global.g_anMonolithMultiplayerH_1042b17c	ds "_global.g_anMonolithMultiplayerHangarTableMagazine"	"_global.g_anMonolithMultiplayerHangarTableMagazine"	string	51	true
DEFINED	1042b1bc	s__global.g_anMonolithMultiplayerH_1042b1bc	ds "_global.g_anMonolithMultiplayerHangarTableSupply"	"_global.g_anMonolithMultiplayerHangarTableSupply"	string	49	true
DEFINED	1042b1f8	s__global.g_anMonolithMultiplayerH_1042b1f8	ds "_global.g_anMonolithMultiplayerHangarTableIsInvenShow"	"_global.g_anMonolithMultiplayerHangarTableIsInvenShow"	string	54	true
DEFINED	1042b238	s__global.g_asMonolithMultiplayerH_1042b238	ds "_global.g_asMonolithMultiplayerHangarTableItemDesc"	"_global.g_asMonolithMultiplayerHangarTableItemDesc"	string	51	true
DEFINED	1042b278	s__global.g_anMonolithMultiplayerH_1042b278	ds "_global.g_anMonolithMultiplayerHangarTableBuyLevelLimit"	"_global.g_anMonolithMultiplayerHangarTableBuyLevelLimit"	string	56	true
DEFINED	1042b2bc	s__global.g_anMonolithMultiplayerH_1042b2bc	ds "_global.g_anMonolithMultiplayerHangarTableEquipLevelLimit"	"_global.g_anMonolithMultiplayerHangarTableEquipLevelLimit"	string	58	true
DEFINED	1042b304	s__global.g_abMonolithMultiplayerH_1042b304	ds "_global.g_abMonolithMultiplayerHangarIsSna"	"_global.g_abMonolithMultiplayerHangarIsSna"	string	43	true
DEFINED	1042b338	s__global.g_anMonolithMultiplayerH_1042b338	ds "_global.g_anMonolithMultiplayerHangarTableIsNew"	"_global.g_anMonolithMultiplayerHangarTableIsNew"	string	48	true
DEFINED	1042b374	s__global.g_anMonolithMultiplayerH_1042b374	ds "_global.g_anMonolithMultiplayerHangarTableIsHot"	"_global.g_anMonolithMultiplayerHangarTableIsHot"	string	48	true
DEFINED	1042b3b0	s__global.g_anMonolithMultiplayerH_1042b3b0	ds "_global.g_anMonolithMultiplayerHangarTableIsSale"	"_global.g_anMonolithMultiplayerHangarTableIsSale"	string	49	true
DEFINED	1042b3ec	s__global.g_anMonolithMultiplayerH_1042b3ec	ds "_global.g_anMonolithMultiplayerHangarTableUseState"	"_global.g_anMonolithMultiplayerHangarTableUseState"	string	51	true
DEFINED	1042b42c	s__global.g_anMonolithMultiplayerH_1042b42c	ds "_global.g_anMonolithMultiplayerHangarTableSlotCount"	"_global.g_anMonolithMultiplayerHangarTableSlotCount"	string	52	true
DEFINED	1042b46c	s__global.g_anMonolithMultiplayerH_1042b46c	ds "_global.g_anMonolithMultiplayerHangarTableCapsuleGrade"	"_global.g_anMonolithMultiplayerHangarTableCapsuleGrade"	string	55	true
DEFINED	1042b4b0	s__global.g_anMonolithMultiplayerH_1042b4b0	ds "_global.g_anMonolithMultiplayerHangarTableStability"	"_global.g_anMonolithMultiplayerHangarTableStability"	string	52	true
DEFINED	1042b4f0	s__global.g_anMonolithMultiplayerH_1042b4f0	ds "_global.g_anMonolithMultiplayerHangarTableCarry"	"_global.g_anMonolithMultiplayerHangarTableCarry"	string	48	true
DEFINED	1042b52c	s__global.g_anMonolithMultiplayerH_1042b52c	ds "_global.g_anMonolithMultiplayerHangarTableCanGift"	"_global.g_anMonolithMultiplayerHangarTableCanGift"	string	50	true
DEFINED	1042b568	s__global.g_anMonolithMultiplayerH_1042b568	ds "_global.g_anMonolithMultiplayerHangarTableRepairPoint"	"_global.g_anMonolithMultiplayerHangarTableRepairPoint"	string	54	true
DEFINED	1042b5a8	s__global.g_abMonolithMultiplayerH_1042b5a8	ds "_global.g_abMonolithMultiplayerHangarItemGDBUseNightVision"	"_global.g_abMonolithMultiplayerHangarItemGDBUseNightVision"	string	59	true
DEFINED	1042b5f0	s__global.g_afMonolithMultiplayerH_1042b5f0	ds "_global.g_afMonolithMultiplayerHangarItemGDBFlashbangCorrect"	"_global.g_afMonolithMultiplayerHangarItemGDBFlashbangCorrect"	string	61	true
DEFINED	1042b63c	s__global.g_afMonolithMultiplayerH_1042b63c	ds "_global.g_afMonolithMultiplayerHangarItemGDBDamageshakeCorrect"	"_global.g_afMonolithMultiplayerHangarItemGDBDamageshakeCorrect"	string	63	true
DEFINED	1042b688	s__global.g_anMonolithMultiplayerH_1042b688	ds "_global.g_anMonolithMultiplayerHangarTableFunctionType"	"_global.g_anMonolithMultiplayerHangarTableFunctionType"	string	55	true
DEFINED	1042b6cc	s__global.g_asMonolithMultiplayerH_1042b6cc	ds "_global.g_asMonolithMultiplayerHangarTableFunctionDesc"	"_global.g_asMonolithMultiplayerHangarTableFunctionDesc"	string	55	true
DEFINED	1042b710	s__global.g_anMonolithMultiplayerH_1042b710	ds "_global.g_anMonolithMultiplayerHangarTableFunctionValue"	"_global.g_anMonolithMultiplayerHangarTableFunctionValue"	string	56	true
DEFINED	1042b754	s__global.g_anMonolithMultiplayerH_1042b754	ds "_global.g_anMonolithMultiplayerHangarLinkTableWeaponIndex"	"_global.g_anMonolithMultiplayerHangarLinkTableWeaponIndex"	string	58	true
DEFINED	1042b79c	s__global.g_anMonolithMultiplayerH_1042b79c	ds "_global.g_anMonolithMultiplayerHangarLinkTablePartIndex"	"_global.g_anMonolithMultiplayerHangarLinkTablePartIndex"	string	56	true
DEFINED	1042b7e0	s__global.g_anMonolithMultiplayerH_1042b7e0	ds "_global.g_anMonolithMultiplayerHangarLinkTableDefault"	"_global.g_anMonolithMultiplayerHangarLinkTableDefault"	string	54	true
DEFINED	1042b820	s__global.g_nMonolithMultiplayerHa_1042b820	ds "_global.g_nMonolithMultiplayerHangarLinkTableRowNum"	"_global.g_nMonolithMultiplayerHangarLinkTableRowNum"	string	52	true
DEFINED	1042b860	s__global.g_anMonolithMultiplayerH_1042b860	ds "_global.g_anMonolithMultiplayerHangarTableCustomPartItemIndex"	"_global.g_anMonolithMultiplayerHangarTableCustomPartItemIndex"	string	62	true
DEFINED	1042b8ac	s__global.g_anMonolithMultiplayerH_1042b8ac	ds "_global.g_anMonolithMultiplayerHangarTableCustomPartItemAttack"	"_global.g_anMonolithMultiplayerHangarTableCustomPartItemAttack"	string	63	true
DEFINED	1042b8f8	s__global.g_anMonolithMultiplayerH_1042b8f8	ds "_global.g_anMonolithMultiplayerHangarTableCustomPartItemAccuracy"	"_global.g_anMonolithMultiplayerHangarTableCustomPartItemAccuracy"	string	65	true
DEFINED	1042b948	s__global.g_anMonolithMultiplayerH_1042b948	ds "_global.g_anMonolithMultiplayerHangarTableCustomPartItemStability"	"_global.g_anMonolithMultiplayerHangarTableCustomPartItemStability"	string	66	true
DEFINED	1042b998	s__global.g_anMonolithMultiplayerH_1042b998	ds "_global.g_anMonolithMultiplayerHangarTableCustomPartItemShotSpeed"	"_global.g_anMonolithMultiplayerHangarTableCustomPartItemShotSpeed"	string	66	true
DEFINED	1042b9e8	s__global.g_anMonolithMultiplayerH_1042b9e8	ds "_global.g_anMonolithMultiplayerHangarTableCustomPartItemAbleToMode"	"_global.g_anMonolithMultiplayerHangarTableCustomPartItemAbleToMode"	string	67	true
DEFINED	1042ba38	s__global.g_nMonolithMultiplayerMy_1042ba38	ds "_global.g_nMonolithMultiplayerMyServerState"	"_global.g_nMonolithMultiplayerMyServerState"	string	44	true
DEFINED	1042ba6c	s__global.g_bMonolithMultiplayerCl_1042ba6c	ds "_global.g_bMonolithMultiplayerClearTutorial"	"_global.g_bMonolithMultiplayerClearTutorial"	string	44	true
DEFINED	1042baa0	s__global.g_bMonolithMultiplayerCl_1042baa0	ds "_global.g_bMonolithMultiplayerClearTutorialNow"	"_global.g_bMonolithMultiplayerClearTutorialNow"	string	47	true
DEFINED	1042bad8	s__global.g_asMonolithMultiplayerM_1042bad8	ds "_global.g_asMonolithMultiplayerMyFriendNickName"	"_global.g_asMonolithMultiplayerMyFriendNickName"	string	48	true
DEFINED	1042bb14	s__global.g_anMonolithMultiplayerM_1042bb14	ds "_global.g_anMonolithMultiplayerMyFriendLevel"	"_global.g_anMonolithMultiplayerMyFriendLevel"	string	45	true
DEFINED	1042bb4c	s__global.g_anMonolithMultiplayerM_1042bb4c	ds "_global.g_anMonolithMultiplayerMyFriendIndex"	"_global.g_anMonolithMultiplayerMyFriendIndex"	string	45	true
DEFINED	1042bb84	s__global.g_anMonolithMultiplayerM_1042bb84	ds "_global.g_anMonolithMultiplayerMyFriendServer"	"_global.g_anMonolithMultiplayerMyFriendServer"	string	46	true
DEFINED	1042bbbc	s__global.g_anMonolithMultiplayerM_1042bbbc	ds "_global.g_anMonolithMultiplayerMyFriendChannel"	"_global.g_anMonolithMultiplayerMyFriendChannel"	string	47	true
DEFINED	1042bbf4	s__global.g_anMonolithMultiplayerM_1042bbf4	ds "_global.g_anMonolithMultiplayerMyFriendRoomIndex"	"_global.g_anMonolithMultiplayerMyFriendRoomIndex"	string	49	true
DEFINED	1042bc30	s__global.g_abMonolithMultiplayerM_1042bc30	ds "_global.g_abMonolithMultiplayerMyFriendLogOn"	"_global.g_abMonolithMultiplayerMyFriendLogOn"	string	45	true
DEFINED	1042bc68	s__global.g_abMonolithMultiplayerM_1042bc68	ds "_global.g_abMonolithMultiplayerMyFriendIsClan"	"_global.g_abMonolithMultiplayerMyFriendIsClan"	string	46	true
DEFINED	1042bca0	s__global.g_asMonolithMultiplayerM_1042bca0	ds "_global.g_asMonolithMultiplayerMyFriendClanName"	"_global.g_asMonolithMultiplayerMyFriendClanName"	string	48	true
DEFINED	1042bcdc	s__global.g_anMonolithMultiplayerM_1042bcdc	ds "_global.g_anMonolithMultiplayerMyFriendMark0"	"_global.g_anMonolithMultiplayerMyFriendMark0"	string	45	true
DEFINED	1042bd14	s__global.g_anMonolithMultiplayerM_1042bd14	ds "_global.g_anMonolithMultiplayerMyFriendMark1"	"_global.g_anMonolithMultiplayerMyFriendMark1"	string	45	true
DEFINED	1042bd4c	s__global.g_anMonolithMultiplayerM_1042bd4c	ds "_global.g_anMonolithMultiplayerMyFriendMark2"	"_global.g_anMonolithMultiplayerMyFriendMark2"	string	45	true
DEFINED	1042bd84	s__global.g_bMonolithMultiplayerIs_1042bd84	ds "_global.g_bMonolithMultiplayerIsAutoMove"	"_global.g_bMonolithMultiplayerIsAutoMove"	string	41	true
DEFINED	1042bdb8	s__global.g_nMonolithMultiplayerAu_1042bdb8	ds "_global.g_nMonolithMultiplayerAutoMoveLobbyIdx"	"_global.g_nMonolithMultiplayerAutoMoveLobbyIdx"	string	47	true
DEFINED	1042bdf0	s__global.g_nMonolithMultiplayerAu_1042bdf0	ds "_global.g_nMonolithMultiplayerAutoMoveChannelIdx"	"_global.g_nMonolithMultiplayerAutoMoveChannelIdx"	string	49	true
DEFINED	1042be2c	s__global.g_nMonolithMultiplayerAu_1042be2c	ds "_global.g_nMonolithMultiplayerAutoMoveGameRoomIdx"	"_global.g_nMonolithMultiplayerAutoMoveGameRoomIdx"	string	50	true
DEFINED	1042be68	s__global.g_anMonolithMultiplayerM_1042be68	ds "_global.g_anMonolithMultiplayerMyClanMemberID"	"_global.g_anMonolithMultiplayerMyClanMemberID"	string	46	true
DEFINED	1042bea0	s__global.g_asMonolithMultiplayerM_1042bea0	ds "_global.g_asMonolithMultiplayerMyClanMemberName"	"_global.g_asMonolithMultiplayerMyClanMemberName"	string	48	true
DEFINED	1042bedc	s__global.g_anMonolithMultiplayerM_1042bedc	ds "_global.g_anMonolithMultiplayerMyClanMemberLevel"	"_global.g_anMonolithMultiplayerMyClanMemberLevel"	string	49	true
DEFINED	1042bf18	s__global.g_anMonolithMultiplayerM_1042bf18	ds "_global.g_anMonolithMultiplayerMyClanMemberClanLevel"	"_global.g_anMonolithMultiplayerMyClanMemberClanLevel"	string	53	true
DEFINED	1042bf58	s__global.g_abMonolithMultiplayerM_1042bf58	ds "_global.g_abMonolithMultiplayerMyClanMemberLoginState"	"_global.g_abMonolithMultiplayerMyClanMemberLoginState"	string	54	true
DEFINED	1042bf98	s__global.g_anMonolithMultiplayerM_1042bf98	ds "_global.g_anMonolithMultiplayerMyClanMemberRoomIndex"	"_global.g_anMonolithMultiplayerMyClanMemberRoomIndex"	string	53	true
DEFINED	1042bfd8	s__global.g_nMonolithMultiplayerMy_1042bfd8	ds "_global.g_nMonolithMultiplayerMyClanMemberCount"	"_global.g_nMonolithMultiplayerMyClanMemberCount"	string	48	true
DEFINED	1042c014	s__global.g_nMonolithMultiplayerRe_1042c014	ds "_global.g_nMonolithMultiplayerRequestMemberCount"	"_global.g_nMonolithMultiplayerRequestMemberCount"	string	49	true
DEFINED	1042c050	s__global.g_anMonolithMultiplayerM_1042c050	ds "_global.g_anMonolithMultiplayerMyClanMemberRecordWin"	"_global.g_anMonolithMultiplayerMyClanMemberRecordWin"	string	53	true
DEFINED	1042c090	s__global.g_anMonolithMultiplayerM_1042c090	ds "_global.g_anMonolithMultiplayerMyClanMemberRecordDraw"	"_global.g_anMonolithMultiplayerMyClanMemberRecordDraw"	string	54	true
DEFINED	1042c0d0	s__global.g_anMonolithMultiplayerM_1042c0d0	ds "_global.g_anMonolithMultiplayerMyClanMemberRecordLose"	"_global.g_anMonolithMultiplayerMyClanMemberRecordLose"	string	54	true
DEFINED	1042c110	s__global.g_anMonolithMultiplayerM_1042c110	ds "_global.g_anMonolithMultiplayerMyClanMemberRecordKill"	"_global.g_anMonolithMultiplayerMyClanMemberRecordKill"	string	54	true
DEFINED	1042c150	s__global.g_anMonolithMultiplayerM_1042c150	ds "_global.g_anMonolithMultiplayerMyClanMemberRecordDeath"	"_global.g_anMonolithMultiplayerMyClanMemberRecordDeath"	string	55	true
DEFINED	1042c194	s__global.g_anMonolithMultiplayerM_1042c194	ds "_global.g_anMonolithMultiplayerMyClanMemberClanRecordWin"	"_global.g_anMonolithMultiplayerMyClanMemberClanRecordWin"	string	57	true
DEFINED	1042c1d8	s__global.g_anMonolithMultiplayerM_1042c1d8	ds "_global.g_anMonolithMultiplayerMyClanMemberClanRecordDraw"	"_global.g_anMonolithMultiplayerMyClanMemberClanRecordDraw"	string	58	true
DEFINED	1042c220	s__global.g_anMonolithMultiplayerM_1042c220	ds "_global.g_anMonolithMultiplayerMyClanMemberClanRecordLose"	"_global.g_anMonolithMultiplayerMyClanMemberClanRecordLose"	string	58	true
DEFINED	1042c268	s__global.g_anMonolithMultiplayerM_1042c268	ds "_global.g_anMonolithMultiplayerMyClanMemberClanRecordKill"	"_global.g_anMonolithMultiplayerMyClanMemberClanRecordKill"	string	58	true
DEFINED	1042c2b0	s__global.g_anMonolithMultiplayerM_1042c2b0	ds "_global.g_anMonolithMultiplayerMyClanMemberClanRecordDeath"	"_global.g_anMonolithMultiplayerMyClanMemberClanRecordDeath"	string	59	true
DEFINED	1042c2f8	s__global.g_anMonolithMultiplayerR_1042c2f8	ds "_global.g_anMonolithMultiplayerRequestClanMemberNumber"	"_global.g_anMonolithMultiplayerRequestClanMemberNumber"	string	55	true
DEFINED	1042c33c	s__global.g_asMonolithMultiplayerR_1042c33c	ds "_global.g_asMonolithMultiplayerRequestClanMemberName"	"_global.g_asMonolithMultiplayerRequestClanMemberName"	string	53	true
DEFINED	1042c37c	s__global.g_anMonolithMultiplayerR_1042c37c	ds "_global.g_anMonolithMultiplayerRequestClanMemberLevel"	"_global.g_anMonolithMultiplayerRequestClanMemberLevel"	string	54	true
DEFINED	1042c3bc	s__global.g_anMonolithMultiplayerR_1042c3bc	ds "_global.g_anMonolithMultiplayerRequestClanMemberWin"	"_global.g_anMonolithMultiplayerRequestClanMemberWin"	string	52	true
DEFINED	1042c3fc	s__global.g_anMonolithMultiplayerR_1042c3fc	ds "_global.g_anMonolithMultiplayerRequestClanMemberDraw"	"_global.g_anMonolithMultiplayerRequestClanMemberDraw"	string	53	true
DEFINED	1042c43c	s__global.g_anMonolithMultiplayerR_1042c43c	ds "_global.g_anMonolithMultiplayerRequestClanMemberLose"	"_global.g_anMonolithMultiplayerRequestClanMemberLose"	string	53	true
DEFINED	1042c47c	s__global.g_anMonolithMultiplayerR_1042c47c	ds "_global.g_anMonolithMultiplayerRequestClanMemberKill"	"_global.g_anMonolithMultiplayerRequestClanMemberKill"	string	53	true
DEFINED	1042c4bc	s__global.g_anMonolithMultiplayerR_1042c4bc	ds "_global.g_anMonolithMultiplayerRequestClanMemberDeath"	"_global.g_anMonolithMultiplayerRequestClanMemberDeath"	string	54	true
DEFINED	1042c4fc	s__global.g_asMonolithMultiplayerR_1042c4fc	ds "_global.g_asMonolithMultiplayerRequestClanMemberMessage"	"_global.g_asMonolithMultiplayerRequestClanMemberMessage"	string	56	true
DEFINED	1042c540	s__global.g_anMonolithMultiplayerR_1042c540	ds "_global.g_anMonolithMultiplayerRequestClanMemberYear"	"_global.g_anMonolithMultiplayerRequestClanMemberYear"	string	53	true
DEFINED	1042c580	s__global.g_anMonolithMultiplayerR_1042c580	ds "_global.g_anMonolithMultiplayerRequestClanMemberMon"	"_global.g_anMonolithMultiplayerRequestClanMemberMon"	string	52	true
DEFINED	1042c5c0	s__global.g_anMonolithMultiplayerR_1042c5c0	ds "_global.g_anMonolithMultiplayerRequestClanMemberDay"	"_global.g_anMonolithMultiplayerRequestClanMemberDay"	string	52	true
DEFINED	1042c600	s__global.g_anMonolithMultiplayerR_1042c600	ds "_global.g_anMonolithMultiplayerRequestClanMemberHour"	"_global.g_anMonolithMultiplayerRequestClanMemberHour"	string	53	true
DEFINED	1042c640	s__global.g_anMonolithMultiplayerR_1042c640	ds "_global.g_anMonolithMultiplayerRequestClanMemberMinute"	"_global.g_anMonolithMultiplayerRequestClanMemberMinute"	string	55	true
DEFINED	1042c684	s__global.g_anMonolithMultiplayerR_1042c684	ds "_global.g_anMonolithMultiplayerRequestClanMemberSecond"	"_global.g_anMonolithMultiplayerRequestClanMemberSecond"	string	55	true
DEFINED	1042c6c8	s__global.g_anMonolithMultiplayerJ_1042c6c8	ds "_global.g_anMonolithMultiplayerJoinRewardIndex"	"_global.g_anMonolithMultiplayerJoinRewardIndex"	string	47	true
DEFINED	1042c700	s__global.g_anMonolithMultiplayerJ_1042c700	ds "_global.g_anMonolithMultiplayerJoinRewardMin"	"_global.g_anMonolithMultiplayerJoinRewardMin"	string	45	true
DEFINED	1042c738	s__global.g_anMonolithMultiplayerJ_1042c738	ds "_global.g_anMonolithMultiplayerJoinRewardMax"	"_global.g_anMonolithMultiplayerJoinRewardMax"	string	45	true
DEFINED	1042c770	s__global.g_anMonolithMultiplayerJ_1042c770	ds "_global.g_anMonolithMultiplayerJoinRewardRandomMax"	"_global.g_anMonolithMultiplayerJoinRewardRandomMax"	string	51	true
DEFINED	1042c7b0	s__global.g_anMonolithMultiplayerJ_1042c7b0	ds "_global.g_anMonolithMultiplayerJoinRewardItem"	"_global.g_anMonolithMultiplayerJoinRewardItem"	string	46	true
DEFINED	1042c7e8	s__global.g_anMonolithMultiplayerJ_1042c7e8	ds "_global.g_anMonolithMultiplayerJoinRewardExp"	"_global.g_anMonolithMultiplayerJoinRewardExp"	string	45	true
DEFINED	1042c820	s__global.g_anMonolithMultiplayerJ_1042c820	ds "_global.g_anMonolithMultiplayerJoinRewardPoint"	"_global.g_anMonolithMultiplayerJoinRewardPoint"	string	47	true
DEFINED	1042c858	s__global.g_anMonolithMultiplayerJ_1042c858	ds "_global.g_anMonolithMultiplayerJoinRewardType"	"_global.g_anMonolithMultiplayerJoinRewardType"	string	46	true
DEFINED	1042c890	s__global.g_anMonolithMultiplayerJ_1042c890	ds "_global.g_anMonolithMultiplayerJoinTableMaterialIndex"	"_global.g_anMonolithMultiplayerJoinTableMaterialIndex"	string	54	true
DEFINED	1042c8d0	s__global.g_anMonolithMultiplayerJ_1042c8d0	ds "_global.g_anMonolithMultiplayerJoinTableMaterialExist"	"_global.g_anMonolithMultiplayerJoinTableMaterialExist"	string	54	true
DEFINED	1042c910	s__global.g_anMonolithMultiplayerJ_1042c910	ds "_global.g_anMonolithMultiplayerJoinTableMaterialIsShopShow"	"_global.g_anMonolithMultiplayerJoinTableMaterialIsShopShow"	string	59	true
DEFINED	1042c958	s__global.g_asMonolithMultiplayerJ_1042c958	ds "_global.g_asMonolithMultiplayerJoinTableMaterialName"	"_global.g_asMonolithMultiplayerJoinTableMaterialName"	string	53	true
DEFINED	1042c998	s__global.g_anMonolithMultiplayerJ_1042c998	ds "_global.g_anMonolithMultiplayerJoinTableMaterialGrade"	"_global.g_anMonolithMultiplayerJoinTableMaterialGrade"	string	54	true
DEFINED	1042c9d8	s__global.g_anMonolithMultiplayerJ_1042c9d8	ds "_global.g_anMonolithMultiplayerJoinTableMaterialRequireLv"	"_global.g_anMonolithMultiplayerJoinTableMaterialRequireLv"	string	58	true
DEFINED	1042ca20	s__global.g_anMonolithMultiplayerJ_1042ca20	ds "_global.g_anMonolithMultiplayerJoinTableMaterialPriceType"	"_global.g_anMonolithMultiplayerJoinTableMaterialPriceType"	string	58	true
DEFINED	1042ca68	s__global.g_anMonolithMultiplayerJ_1042ca68	ds "_global.g_anMonolithMultiplayerJoinTableMaterialPriceP"	"_global.g_anMonolithMultiplayerJoinTableMaterialPriceP"	string	55	true
DEFINED	1042caac	s__global.g_anMonolithMultiplayerJ_1042caac	ds "_global.g_anMonolithMultiplayerJoinTableMaterialPriceC"	"_global.g_anMonolithMultiplayerJoinTableMaterialPriceC"	string	55	true
DEFINED	1042caf0	s__global.g_asMonolithMultiplayerJ_1042caf0	ds "_global.g_asMonolithMultiplayerJoinTableMaterialLinkedItem"	"_global.g_asMonolithMultiplayerJoinTableMaterialLinkedItem"	string	59	true
DEFINED	1042cb38	s__global.g_anMonolithMultiplayerJ_1042cb38	ds "_global.g_anMonolithMultiplayerJoinTableMaterialEffect1"	"_global.g_anMonolithMultiplayerJoinTableMaterialEffect1"	string	56	true
DEFINED	1042cb7c	s__global.g_afMonolithMultiplayerJ_1042cb7c	ds "_global.g_afMonolithMultiplayerJoinTableMaterialEvalue1"	"_global.g_afMonolithMultiplayerJoinTableMaterialEvalue1"	string	56	true
DEFINED	1042cbc0	s__global.g_anMonolithMultiplayerJ_1042cbc0	ds "_global.g_anMonolithMultiplayerJoinTableMaterialEffect2"	"_global.g_anMonolithMultiplayerJoinTableMaterialEffect2"	string	56	true
DEFINED	1042cc04	s__global.g_afMonolithMultiplayerJ_1042cc04	ds "_global.g_afMonolithMultiplayerJoinTableMaterialEvalue2"	"_global.g_afMonolithMultiplayerJoinTableMaterialEvalue2"	string	56	true
DEFINED	1042cc48	s__global.g_anMonolithMultiplayerJ_1042cc48	ds "_global.g_anMonolithMultiplayerJoinTableMaterialEffect3"	"_global.g_anMonolithMultiplayerJoinTableMaterialEffect3"	string	56	true
DEFINED	1042cc8c	s__global.g_afMonolithMultiplayerJ_1042cc8c	ds "_global.g_afMonolithMultiplayerJoinTableMaterialEvalue3"	"_global.g_afMonolithMultiplayerJoinTableMaterialEvalue3"	string	56	true
DEFINED	1042ccd0	s__global.g_asMonolithMultiplayerJ_1042ccd0	ds "_global.g_asMonolithMultiplayerJoinTableMaterialDescription"	"_global.g_asMonolithMultiplayerJoinTableMaterialDescription"	string	60	true
DEFINED	1042cd18	s__global.g_anMonolithMultiplayerJ_1042cd18	ds "_global.g_anMonolithMultiplayerJoinTableRecipeId"	"_global.g_anMonolithMultiplayerJoinTableRecipeId"	string	49	true
DEFINED	1042cd54	s__global.g_anMonolithMultiplayerJ_1042cd54	ds "_global.g_anMonolithMultiplayerJoinTableRecipeExist"	"_global.g_anMonolithMultiplayerJoinTableRecipeExist"	string	52	true
DEFINED	1042cd94	s__global.g_anMonolithMultiplayerJ_1042cd94	ds "_global.g_anMonolithMultiplayerJoinTableRecipeRepresentIndex"	"_global.g_anMonolithMultiplayerJoinTableRecipeRepresentIndex"	string	61	true
DEFINED	1042cde0	s__global.g_anMonolithMultiplayerJ_1042cde0	ds "_global.g_anMonolithMultiplayerJoinTableRecipeRequireLv"	"_global.g_anMonolithMultiplayerJoinTableRecipeRequireLv"	string	56	true
DEFINED	1042ce24	s__global.g_anMonolithMultiplayerJ_1042ce24	ds "_global.g_anMonolithMultiplayerJoinTableRecipeRequireTitle"	"_global.g_anMonolithMultiplayerJoinTableRecipeRequireTitle"	string	59	true
DEFINED	1042ce6c	s__global.g_anMonolithMultiplayerJ_1042ce6c	ds "_global.g_anMonolithMultiplayerJoinTableRecipePriceType"	"_global.g_anMonolithMultiplayerJoinTableRecipePriceType"	string	56	true
DEFINED	1042ceb0	s__global.g_anMonolithMultiplayerJ_1042ceb0	ds "_global.g_anMonolithMultiplayerJoinTableRecipePriceP"	"_global.g_anMonolithMultiplayerJoinTableRecipePriceP"	string	53	true
DEFINED	1042cef0	s__global.g_anMonolithMultiplayerJ_1042cef0	ds "_global.g_anMonolithMultiplayerJoinTableRecipePriceC"	"_global.g_anMonolithMultiplayerJoinTableRecipePriceC"	string	53	true
DEFINED	1042cf30	s__global.g_anMonolithMultiplayerJ_1042cf30	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMaterialCount"	"_global.g_anMonolithMultiplayerJoinTableRecipeMaterialCount"	string	60	true
DEFINED	1042cf78	s__global.g_anMonolithMultiplayerJ_1042cf78	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMaterial1"	"_global.g_anMonolithMultiplayerJoinTableRecipeMaterial1"	string	56	true
DEFINED	1042cfbc	s__global.g_anMonolithMultiplayerJ_1042cfbc	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMaterial2"	"_global.g_anMonolithMultiplayerJoinTableRecipeMaterial2"	string	56	true
DEFINED	1042d000	s__global.g_anMonolithMultiplayerJ_1042d000	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMaterial3"	"_global.g_anMonolithMultiplayerJoinTableRecipeMaterial3"	string	56	true
DEFINED	1042d044	s__global.g_anMonolithMultiplayerJ_1042d044	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMaterial4"	"_global.g_anMonolithMultiplayerJoinTableRecipeMaterial4"	string	56	true
DEFINED	1042d088	s__global.g_anMonolithMultiplayerJ_1042d088	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMaterial5"	"_global.g_anMonolithMultiplayerJoinTableRecipeMaterial5"	string	56	true
DEFINED	1042d0cc	s__global.g_anMonolithMultiplayerJ_1042d0cc	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMaterial6"	"_global.g_anMonolithMultiplayerJoinTableRecipeMaterial6"	string	56	true
DEFINED	1042d110	s__global.g_anMonolithMultiplayerJ_1042d110	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMatCount1"	"_global.g_anMonolithMultiplayerJoinTableRecipeMatCount1"	string	56	true
DEFINED	1042d154	s__global.g_anMonolithMultiplayerJ_1042d154	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMatCount2"	"_global.g_anMonolithMultiplayerJoinTableRecipeMatCount2"	string	56	true
DEFINED	1042d198	s__global.g_anMonolithMultiplayerJ_1042d198	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMatCount3"	"_global.g_anMonolithMultiplayerJoinTableRecipeMatCount3"	string	56	true
DEFINED	1042d1dc	s__global.g_anMonolithMultiplayerJ_1042d1dc	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMatCount4"	"_global.g_anMonolithMultiplayerJoinTableRecipeMatCount4"	string	56	true
DEFINED	1042d220	s__global.g_anMonolithMultiplayerJ_1042d220	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMatCount5"	"_global.g_anMonolithMultiplayerJoinTableRecipeMatCount5"	string	56	true
DEFINED	1042d264	s__global.g_anMonolithMultiplayerJ_1042d264	ds "_global.g_anMonolithMultiplayerJoinTableRecipeMatCount6"	"_global.g_anMonolithMultiplayerJoinTableRecipeMatCount6"	string	56	true
DEFINED	1042d2a8	s__global.g_anMonolithMultiplayerJ_1042d2a8	ds "_global.g_anMonolithMultiplayerJoinTableRecipeOutputId"	"_global.g_anMonolithMultiplayerJoinTableRecipeOutputId"	string	55	true
DEFINED	1042d2ec	s__global.g_anMonolithMultiplayerJ_1042d2ec	ds "_global.g_anMonolithMultiplayerJoinTableRecipeCountOutput"	"_global.g_anMonolithMultiplayerJoinTableRecipeCountOutput"	string	58	true
DEFINED	1042d334	s__global.g_anMonolithMultiplayerJ_1042d334	ds "_global.g_anMonolithMultiplayerJoinTableRecipeIsSell"	"_global.g_anMonolithMultiplayerJoinTableRecipeIsSell"	string	53	true
DEFINED	1042d374	s__global.g_anMonolithMultiplayerJ_1042d374	ds "_global.g_anMonolithMultiplayerJoinTableRecipeIsShopShow"	"_global.g_anMonolithMultiplayerJoinTableRecipeIsShopShow"	string	57	true
DEFINED	1042d3b8	s__global.g_anMonolithMultiplayerJ_1042d3b8	ds "_global.g_anMonolithMultiplayerJoinTableRecipeTypeAItem"	"_global.g_anMonolithMultiplayerJoinTableRecipeTypeAItem"	string	56	true
DEFINED	1042d3fc	s__global.g_anMonolithMultiplayerJ_1042d3fc	ds "_global.g_anMonolithMultiplayerJoinTableRecipeTypeBItem"	"_global.g_anMonolithMultiplayerJoinTableRecipeTypeBItem"	string	56	true
DEFINED	1042d440	s__global.g_anMonolithMultiplayerJ_1042d440	ds "_global.g_anMonolithMultiplayerJoinTableMissionIndex"	"_global.g_anMonolithMultiplayerJoinTableMissionIndex"	string	53	true
DEFINED	1042d480	s__global.g_anMonolithMultiplayerJ_1042d480	ds "_global.g_anMonolithMultiplayerJoinTableMissionOpenFactor"	"_global.g_anMonolithMultiplayerJoinTableMissionOpenFactor"	string	58	true
DEFINED	1042d4c8	s__global.g_anMonolithMultiplayerJ_1042d4c8	ds "_global.g_anMonolithMultiplayerJoinTableMissionSlotIndex"	"_global.g_anMonolithMultiplayerJoinTableMissionSlotIndex"	string	57	true
DEFINED	1042d50c	s__global.g_anMonolithMultiplayerJ_1042d50c	ds "_global.g_anMonolithMultiplayerJoinTableMissionIconInfo"	"_global.g_anMonolithMultiplayerJoinTableMissionIconInfo"	string	56	true
DEFINED	1042d550	s__global.g_anMonolithMultiplayerJ_1042d550	ds "_global.g_anMonolithMultiplayerJoinTableMissionAbleToMode"	"_global.g_anMonolithMultiplayerJoinTableMissionAbleToMode"	string	58	true
DEFINED	1042d598	s__global.g_anMonolithMultiplayerJ_1042d598	ds "_global.g_anMonolithMultiplayerJoinTableMissionAbleToSubMode"	"_global.g_anMonolithMultiplayerJoinTableMissionAbleToSubMode"	string	61	true
DEFINED	1042d5e4	s__global.g_anMonolithMultiplayerJ_1042d5e4	ds "_global.g_anMonolithMultiplayerJoinTableMissionMapInfo"	"_global.g_anMonolithMultiplayerJoinTableMissionMapInfo"	string	55	true
DEFINED	1042d628	s__global.g_anMonolithMultiplayerJ_1042d628	ds "_global.g_anMonolithMultiplayerJoinTableMissionDiffType"	"_global.g_anMonolithMultiplayerJoinTableMissionDiffType"	string	56	true
DEFINED	1042d66c	s__global.g_anMonolithMultiplayerJ_1042d66c	ds "_global.g_anMonolithMultiplayerJoinTableMissionActionType"	"_global.g_anMonolithMultiplayerJoinTableMissionActionType"	string	58	true
DEFINED	1042d6b4	s__global.g_anMonolithMultiplayerJ_1042d6b4	ds "_global.g_anMonolithMultiplayerJoinTableMissionActionCount"	"_global.g_anMonolithMultiplayerJoinTableMissionActionCount"	string	59	true
DEFINED	1042d6fc	s__global.g_asMonolithMultiplayerJ_1042d6fc	ds "_global.g_asMonolithMultiplayerJoinTableMissionDesc"	"_global.g_asMonolithMultiplayerJoinTableMissionDesc"	string	52	true
DEFINED	1042d73c	s__global.g_asMonolithMultiplayerJ_1042d73c	ds "_global.g_asMonolithMultiplayerJoinTableMissionSummary"	"_global.g_asMonolithMultiplayerJoinTableMissionSummary"	string	55	true
DEFINED	1042d780	s__global.g_anMonolithMultiplayerJ_1042d780	ds "_global.g_anMonolithMultiplayerJoinTableMissionRewardItemIndex1"	"_global.g_anMonolithMultiplayerJoinTableMissionRewardItemIndex1"	string	64	true
DEFINED	1042d7d0	s__global.g_anMonolithMultiplayerJ_1042d7d0	ds "_global.g_anMonolithMultiplayerJoinTableMissionRewardItemIndex2"	"_global.g_anMonolithMultiplayerJoinTableMissionRewardItemIndex2"	string	64	true
DEFINED	1042d820	s__global.g_anMonolithMultiplayerJ_1042d820	ds "_global.g_anMonolithMultiplayerJoinTableMissionRewardItemIndex3"	"_global.g_anMonolithMultiplayerJoinTableMissionRewardItemIndex3"	string	64	true
DEFINED	1042d86c	s__global.g_anMonolithMultiplayerJ_1042d86c	ds "_global.g_anMonolithMultiplayerJoinTableTodayOpenMissionIndex"	"_global.g_anMonolithMultiplayerJoinTableTodayOpenMissionIndex"	string	62	true
DEFINED	1042d8b8	s__global.g_anMonolithMultiplayerJ_1042d8b8	ds "_global.g_anMonolithMultiplayerJoinTableTodayCompleteMissionIndex"	"_global.g_anMonolithMultiplayerJoinTableTodayCompleteMissionIndex"	string	66	true
DEFINED	1042d908	s__global.g_anMonolithMultiplayerJ_1042d908	ds "_global.g_anMonolithMultiplayerJoinTableTodayLockedMissionIndex"	"_global.g_anMonolithMultiplayerJoinTableTodayLockedMissionIndex"	string	64	true
DEFINED	1042d954	s__global.g_nMonolithMultiplayerJo_1042d954	ds "_global.g_nMonolithMultiplayerJoinCurrentMission"	"_global.g_nMonolithMultiplayerJoinCurrentMission"	string	49	true
DEFINED	1042d990	s__global.g_anMonolithMultiplayerP_1042d990	ds "_global.g_anMonolithMultiplayerPerkTablePerkIndex"	"_global.g_anMonolithMultiplayerPerkTablePerkIndex"	string	50	true
DEFINED	1042d9cc	s__global.g_anMonolithMultiplayerP_1042d9cc	ds "_global.g_anMonolithMultiplayerPerkTablePerkAbleToMode"	"_global.g_anMonolithMultiplayerPerkTablePerkAbleToMode"	string	55	true
DEFINED	1042da10	s__global.g_anMonolithMultiplayerP_1042da10	ds "_global.g_anMonolithMultiplayerPerkTablePerkTimeKeep"	"_global.g_anMonolithMultiplayerPerkTablePerkTimeKeep"	string	53	true
DEFINED	1042da50	s__global.g_anMonolithMultiplayerP_1042da50	ds "_global.g_anMonolithMultiplayerPerkTablePerkReleaseType"	"_global.g_anMonolithMultiplayerPerkTablePerkReleaseType"	string	56	true
DEFINED	1042da94	s__global.g_anMonolithMultiplayerP_1042da94	ds "_global.g_anMonolithMultiplayerPerkTablePerkReleaseValue"	"_global.g_anMonolithMultiplayerPerkTablePerkReleaseValue"	string	57	true
DEFINED	1042dad8	s__global.g_anMonolithMultiplayerP_1042dad8	ds "_global.g_anMonolithMultiplayerPerkTablePerkCondition"	"_global.g_anMonolithMultiplayerPerkTablePerkCondition"	string	54	true
DEFINED	1042db18	s__global.g_anMonolithMultiplayerP_1042db18	ds "_global.g_anMonolithMultiplayerPerkTablePerkConditionValue"	"_global.g_anMonolithMultiplayerPerkTablePerkConditionValue"	string	59	true
DEFINED	1042db60	s__global.g_anMonolithMultiplayerP_1042db60	ds "_global.g_anMonolithMultiplayerPerkTablePerkEffectCount"	"_global.g_anMonolithMultiplayerPerkTablePerkEffectCount"	string	56	true
DEFINED	1042dba4	s__global.g_anMonolithMultiplayerP_1042dba4	ds "_global.g_anMonolithMultiplayerPerkTablePerkEffect1"	"_global.g_anMonolithMultiplayerPerkTablePerkEffect1"	string	52	true
DEFINED	1042dbe4	s__global.g_anMonolithMultiplayerP_1042dbe4	ds "_global.g_anMonolithMultiplayerPerkTablePerkEffect2"	"_global.g_anMonolithMultiplayerPerkTablePerkEffect2"	string	52	true
DEFINED	1042dc24	s__global.g_anMonolithMultiplayerP_1042dc24	ds "_global.g_anMonolithMultiplayerPerkTablePerkEffectOperator1"	"_global.g_anMonolithMultiplayerPerkTablePerkEffectOperator1"	string	60	true
DEFINED	1042dc6c	s__global.g_anMonolithMultiplayerP_1042dc6c	ds "_global.g_anMonolithMultiplayerPerkTablePerkEffectOperator2"	"_global.g_anMonolithMultiplayerPerkTablePerkEffectOperator2"	string	60	true
DEFINED	1042dcb4	s__global.g_afMonolithMultiplayerP_1042dcb4	ds "_global.g_afMonolithMultiplayerPerkTablePerkEffectValue1"	"_global.g_afMonolithMultiplayerPerkTablePerkEffectValue1"	string	57	true
DEFINED	1042dcf8	s__global.g_afMonolithMultiplayerP_1042dcf8	ds "_global.g_afMonolithMultiplayerPerkTablePerkEffectValue2"	"_global.g_afMonolithMultiplayerPerkTablePerkEffectValue2"	string	57	true
DEFINED	1042dd3c	s__global.g_asMonolithMultiplayerP_1042dd3c	ds "_global.g_asMonolithMultiplayerPerkTablePerkSubName"	"_global.g_asMonolithMultiplayerPerkTablePerkSubName"	string	52	true
DEFINED	1042dd7c	s__global.g_asMonolithMultiplayerP_1042dd7c	ds "_global.g_asMonolithMultiplayerPerkTablePerkDescription"	"_global.g_asMonolithMultiplayerPerkTablePerkDescription"	string	56	true
DEFINED	1042ddc0	s__global.g_asMonolithMultiplayerP_1042ddc0	ds "_global.g_asMonolithMultiplayerPerkTablePerkComment"	"_global.g_asMonolithMultiplayerPerkTablePerkComment"	string	52	true
DEFINED	1042de00	s__global.g_asMonolithMultiplayerP_1042de00	ds "_global.g_asMonolithMultiplayerPerkTablePerkSummary"	"_global.g_asMonolithMultiplayerPerkTablePerkSummary"	string	52	true
DEFINED	1042de40	s__global.g_abMonolithMultiplayerP_1042de40	ds "_global.g_abMonolithMultiplayerPerkTablePerkHideCondition"	"_global.g_abMonolithMultiplayerPerkTablePerkHideCondition"	string	58	true
DEFINED	1042de88	s__global.g_anMonolithMultiplayerJ_1042de88	ds "_global.g_anMonolithMultiplayerJoinEventAttendance_Index"	"_global.g_anMonolithMultiplayerJoinEventAttendance_Index"	string	57	true
DEFINED	1042decc	s__global.g_anMonolithMultiplayerJ_1042decc	ds "_global.g_anMonolithMultiplayerJoinEventAttendance_RewardType"	"_global.g_anMonolithMultiplayerJoinEventAttendance_RewardType"	string	62	true
DEFINED	1042df18	s__global.g_anMonolithMultiplayerJ_1042df18	ds "_global.g_anMonolithMultiplayerJoinEventAttendance_RewardItemIndex"	"_global.g_anMonolithMultiplayerJoinEventAttendance_RewardItemIndex"	string	67	true
DEFINED	1042df68	s__global.g_anMonolithMultiplayerJ_1042df68	ds "_global.g_anMonolithMultiplayerJoinEventAttendance_RewardPoint"	"_global.g_anMonolithMultiplayerJoinEventAttendance_RewardPoint"	string	63	true
DEFINED	1042dfb4	s__global.g_nMonolithMultiplayerJo_1042dfb4	ds "_global.g_nMonolithMultiplayerJoinHaveScenarioTicket"	"_global.g_nMonolithMultiplayerJoinHaveScenarioTicket"	string	53	true
DEFINED	1042dff4	s__global.g_nMonolithMultiplayerJo_1042dff4	ds "_global.g_nMonolithMultiplayerJoinHaveScenarioTicketMax"	"_global.g_nMonolithMultiplayerJoinHaveScenarioTicketMax"	string	56	true
DEFINED	1042e038	s__global.g_nMonolithMultiplayerJo_1042e038	ds "_global.g_nMonolithMultiplayerJoinHaveScenarioTicketNeedTime"	"_global.g_nMonolithMultiplayerJoinHaveScenarioTicketNeedTime"	string	61	true
DEFINED	1042f450	s__global.g_anMonolithMultiplayerJ_1042f450	ds "_global.g_anMonolithMultiplayerJoinChannelName"	"_global.g_anMonolithMultiplayerJoinChannelName"	string	47	true
DEFINED	1042f488	s__global.g_asMonolithMultiplayerH_1042f488	ds "_global.g_asMonolithMultiplayerHangarItemIndex"	"_global.g_asMonolithMultiplayerHangarItemIndex"	string	47	true
DEFINED	1042f4c0	s__global.g_asMonolithMultiplayerH_1042f4c0	ds "_global.g_asMonolithMultiplayerHangarSerialIndex"	"_global.g_asMonolithMultiplayerHangarSerialIndex"	string	49	true
DEFINED	10430bcc	s__global.g_nMonolithOptionsDiffic_10430bcc	ds "_global.g_nMonolithOptionsDifficulty"	"_global.g_nMonolithOptionsDifficulty"	string	37	true
DEFINED	10430bf8	s__global.g_bMonolithOptionsSouthP_10430bf8	ds "_global.g_bMonolithOptionsSouthPaw"	"_global.g_bMonolithOptionsSouthPaw"	string	35	true
DEFINED	10430c24	s__global.g_bMonolithOptionsInvert_10430c24	ds "_global.g_bMonolithOptionsInvertYAxis"	"_global.g_bMonolithOptionsInvertYAxis"	string	38	true
DEFINED	10430c54	s__global.g_bMonolithOptionsVibrat_10430c54	ds "_global.g_bMonolithOptionsVibration"	"_global.g_bMonolithOptionsVibration"	string	36	true
DEFINED	10430c80	s__global.g_bMonolithOptionsVibrat_10430c80	ds "_global.g_bMonolithOptionsVibrationIsValid"	"_global.g_bMonolithOptionsVibrationIsValid"	string	43	true
DEFINED	10430cb4	s__global.g_fMonolithOptionsAimSen_10430cb4	ds "_global.g_fMonolithOptionsAimSensitivity"	"_global.g_fMonolithOptionsAimSensitivity"	string	41	true
DEFINED	10430ce8	s__global.g_bMonolithOptionsGore_10430ce8	ds "_global.g_bMonolithOptionsGore"	"_global.g_bMonolithOptionsGore"	string	31	true
DEFINED	10430d10	s__global.g_bMonolithOptionsCrouch_10430d10	ds "_global.g_bMonolithOptionsCrouchToggle"	"_global.g_bMonolithOptionsCrouchToggle"	string	39	true
DEFINED	10430d40	s__global.g_bMonolithOptionsZoomTo_10430d40	ds "_global.g_bMonolithOptionsZoomToggle"	"_global.g_bMonolithOptionsZoomToggle"	string	37	true
DEFINED	10430d6c	s__global.g_bMonolithOptionsSubtit_10430d6c	ds "_global.g_bMonolithOptionsSubtitles"	"_global.g_bMonolithOptionsSubtitles"	string	36	true
DEFINED	10430d98	s__global.g_bMonolithOptionsHeadBo_10430d98	ds "_global.g_bMonolithOptionsHeadBob"	"_global.g_bMonolithOptionsHeadBob"	string	34	true
DEFINED	10430dc0	s__global.g_bMonolithOptionsCrossh_10430dc0	ds "_global.g_bMonolithOptionsCrosshairs"	"_global.g_bMonolithOptionsCrosshairs"	string	37	true
DEFINED	10430dec	s__global.g_fMonolithOptionsGamma_10430dec	ds "_global.g_fMonolithOptionsGamma"	"_global.g_fMonolithOptionsGamma"	string	32	true
DEFINED	10430e14	s__global.g_fMonolithOptionsSoundF_10430e14	ds "_global.g_fMonolithOptionsSoundFXVolume"	"_global.g_fMonolithOptionsSoundFXVolume"	string	40	true
DEFINED	10430e44	s__global.g_fMonolithOptionsMusicV_10430e44	ds "_global.g_fMonolithOptionsMusicVolume"	"_global.g_fMonolithOptionsMusicVolume"	string	38	true
DEFINED	10430e74	s__global.g_fMonolithOptionsSpeech_10430e74	ds "_global.g_fMonolithOptionsSpeechVolume"	"_global.g_fMonolithOptionsSpeechVolume"	string	39	true
DEFINED	10430ea4	s__global.g_bMonolithOptionsAutoWe_10430ea4	ds "_global.g_bMonolithOptionsAutoWeaponSwitch"	"_global.g_bMonolithOptionsAutoWeaponSwitch"	string	43	true
DEFINED	10430ed8	s__global.g_bMonolithOptionsMPAuto_10430ed8	ds "_global.g_bMonolithOptionsMPAutoWeaponSwitch"	"_global.g_bMonolithOptionsMPAutoWeaponSwitch"	string	45	true
DEFINED	10430f10	s__global.g_bMonolithOptionsAimSna_10430f10	ds "_global.g_bMonolithOptionsAimSnap"	"_global.g_bMonolithOptionsAimSnap"	string	34	true
DEFINED	10430f38	s__global.g_nMonolithOptionsMaxSer_10430f38	ds "_global.g_nMonolithOptionsMaxServerSearchResults"	"_global.g_nMonolithOptionsMaxServerSearchResults"	string	49	true
DEFINED	10430f74	s__global.g_bMonolithOptionsUseFil_10430f74	ds "_global.g_bMonolithOptionsUseFilmGrain"	"_global.g_bMonolithOptionsUseFilmGrain"	string	39	true
DEFINED	10430fa4	s__global.g_bMonolithOptionsHitInd_10430fa4	ds "_global.g_bMonolithOptionsHitIndication"	"_global.g_bMonolithOptionsHitIndication"	string	40	true
DEFINED	10430fd4	s__global.g_bMonolithOptionsWorldM_10430fd4	ds "_global.g_bMonolithOptionsWorldMissionShow"	"_global.g_bMonolithOptionsWorldMissionShow"	string	43	true
DEFINED	10431008	s__global.g_bMonolithOptionsWorldM_10431008	ds "_global.g_bMonolithOptionsWorldMissionNotice"	"_global.g_bMonolithOptionsWorldMissionNotice"	string	45	true
DEFINED	10431040	s__global.g_bMonolithOptionsTakeIn_10431040	ds "_global.g_bMonolithOptionsTakeInvite"	"_global.g_bMonolithOptionsTakeInvite"	string	37	true
DEFINED	1043106c	s__global.g_bMonolithOptionsOnDraw_1043106c	ds "_global.g_bMonolithOptionsOnDrawFullScreen"	"_global.g_bMonolithOptionsOnDrawFullScreen"	string	43	true
DEFINED	104310a0	s__global.g_bMonolithOptionsSprint_104310a0	ds "_global.g_bMonolithOptionsSprinttoggle"	"_global.g_bMonolithOptionsSprinttoggle"	string	39	true
DEFINED	104310d0	s__global.g_bMonolithOptionsTakeMa_104310d0	ds "_global.g_bMonolithOptionsTakeMatchInvite"	"_global.g_bMonolithOptionsTakeMatchInvite"	string	42	true
DEFINED	10431104	s__global.g_sMonolithOptionsOnline_10431104	ds "_global.g_sMonolithOptionsOnlineLoginEmail"	"_global.g_sMonolithOptionsOnlineLoginEmail"	string	43	true
DEFINED	10431138	s__global.g_sMonolithOptionsOnline_10431138	ds "_global.g_sMonolithOptionsOnlineLoginPassword"	"_global.g_sMonolithOptionsOnlineLoginPassword"	string	46	true
DEFINED	10431170	s__global.g_bMonolithOptionsOnline_10431170	ds "_global.g_bMonolithOptionsOnlineLoginSaveInfo"	"_global.g_bMonolithOptionsOnlineLoginSaveInfo"	string	46	true
DEFINED	104311a8	s__global.g_nMonolithOptionsResolu_104311a8	ds "_global.g_nMonolithOptionsResolutionIndex"	"_global.g_nMonolithOptionsResolutionIndex"	string	42	true
DEFINED	104311dc	s__global.g_anMonolithOptionsResol_104311dc	ds "_global.g_anMonolithOptionsResolutionWidths"	"_global.g_anMonolithOptionsResolutionWidths"	string	44	true
DEFINED	10431210	s__global.g_anMonolithOptionsResol_10431210	ds "_global.g_anMonolithOptionsResolutionHeights"	"_global.g_anMonolithOptionsResolutionHeights"	string	45	true
DEFINED	10431248	s__global.g_anMonolithOptionsBitDe_10431248	ds "_global.g_anMonolithOptionsBitDepths"	"_global.g_anMonolithOptionsBitDepths"	string	37	true
DEFINED	10431274	s__global.g_nMonolithOptionsTheme_10431274	ds "_global.g_nMonolithOptionsTheme"	"_global.g_nMonolithOptionsTheme"	string	32	true
DEFINED	10431abc	s__global.g_nMonolithPerformanceAc_10431abc	ds "_global.g_nMonolithPerformanceActualResolutionIndex"	"_global.g_nMonolithPerformanceActualResolutionIndex"	string	52	true
DEFINED	10431afc	s__global.g_nMonolithPerformanceRe_10431afc	ds "_global.g_nMonolithPerformanceResolutionIndex"	"_global.g_nMonolithPerformanceResolutionIndex"	string	46	true
DEFINED	10431b34		ds "_global.g_anMonolithPerformanceResolutionWidths"	"_global.g_anMonolithPerformanceResolutionWidths"	string	48	true
DEFINED	10431b70		ds "_global.g_anMonolithPerformanceResolutionHeights"	"_global.g_anMonolithPerformanceResolutionHeights"	string	49	true
DEFINED	10431bac		ds "_global.g_asMonolithPerformanceResolutionList"	"_global.g_asMonolithPerformanceResolutionList"	string	46	true
DEFINED	10431be4	s__global.g_nMonolithPerformanceRe_10431be4	ds "_global.g_nMonolithPerformanceResolutionCount"	"_global.g_nMonolithPerformanceResolutionCount"	string	46	true
DEFINED	104324fc	s__global.g_nMonolithPlayerGameTim_104324fc	ds "_global.g_nMonolithPlayerGameTimeCurrent"	"_global.g_nMonolithPlayerGameTimeCurrent"	string	41	true
DEFINED	10432530	s__global.g_nMonolithPlayerGameBom_10432530	ds "_global.g_nMonolithPlayerGameBombTimeCurrent"	"_global.g_nMonolithPlayerGameBombTimeCurrent"	string	45	true
DEFINED	10432568	s__global.g_nMonolithPlayerGameBom_10432568	ds "_global.g_nMonolithPlayerGameBombTimeStart"	"_global.g_nMonolithPlayerGameBombTimeStart"	string	43	true
DEFINED	1043259c	s__global.g_nMonolithPlayerAccumul_1043259c	ds "_global.g_nMonolithPlayerAccumulatedTime"	"_global.g_nMonolithPlayerAccumulatedTime"	string	41	true
DEFINED	104325d0	s__global.g_fMonolithPlayerFlashli_104325d0	ds "_global.g_fMonolithPlayerFlashlightCharge"	"_global.g_fMonolithPlayerFlashlightCharge"	string	42	true
DEFINED	10432638	s__global.g_nMonolithPlayerGrenade_10432638	ds "_global.g_nMonolithPlayerGrenadeCookTime"	"_global.g_nMonolithPlayerGrenadeCookTime"	string	41	true
DEFINED	10432670	s__global.g_fMonolithPlayerSlowMoC_10432670	ds "_global.g_fMonolithPlayerSlowMoCharge"	"_global.g_fMonolithPlayerSlowMoCharge"	string	38	true
DEFINED	104326a0	s__global.g_nMonolithPlayerCrossha_104326a0	ds "_global.g_nMonolithPlayerCrosshairPerturb"	"_global.g_nMonolithPlayerCrosshairPerturb"	string	42	true
DEFINED	104326d4	s__global.g_nMonolithPlayerCrossha_104326d4	ds "_global.g_nMonolithPlayerCrosshairPerturbPercent"	"_global.g_nMonolithPlayerCrosshairPerturbPercent"	string	49	true
DEFINED	10432710	s__global.g_nMonolithPlayerCrossha_10432710	ds "_global.g_nMonolithPlayerCrosshairTargetType"	"_global.g_nMonolithPlayerCrosshairTargetType"	string	45	true
DEFINED	10432748	s__global.g_fMonolithPlayerCrossha_10432748	ds "_global.g_fMonolithPlayerCrosshairTargetDistance"	"_global.g_fMonolithPlayerCrosshairTargetDistance"	string	49	true
DEFINED	10432784	s__global.g_bMonolithPlayerDisplay_10432784	ds "_global.g_bMonolithPlayerDisplayShotDistance"	"_global.g_bMonolithPlayerDisplayShotDistance"	string	45	true
DEFINED	104327bc	s__global.g_nMonolithPlayerPlayerF_104327bc	ds "_global.g_nMonolithPlayerPlayerFacingAngle"	"_global.g_nMonolithPlayerPlayerFacingAngle"	string	43	true
DEFINED	1043282c	s__global.g_nMonolithPlayerChangeT_1043282c	ds "_global.g_nMonolithPlayerChangeTeamIndex"	"_global.g_nMonolithPlayerChangeTeamIndex"	string	41	true
DEFINED	10432860	s__global.g_bMonolithPlayerInitPic_10432860	ds "_global.g_bMonolithPlayerInitPickupItem"	"_global.g_bMonolithPlayerInitPickupItem"	string	40	true
DEFINED	10432890	s__global.g_bMonolithPlayerIsMulti_10432890	ds "_global.g_bMonolithPlayerIsMultiplayerGame"	"_global.g_bMonolithPlayerIsMultiplayerGame"	string	43	true
DEFINED	104328c4	s__global.g_bMonolithPlayerIsElimi_104328c4	ds "_global.g_bMonolithPlayerIsEliminationGame"	"_global.g_bMonolithPlayerIsEliminationGame"	string	43	true
DEFINED	104328f8	s__global.g_bMonolithPlayerIsTeamG_104328f8	ds "_global.g_bMonolithPlayerIsTeamGame"	"_global.g_bMonolithPlayerIsTeamGame"	string	36	true
DEFINED	10432924	s__global.g_bMonolithPlayerIsAutoR_10432924	ds "_global.g_bMonolithPlayerIsAutoRespawn"	"_global.g_bMonolithPlayerIsAutoRespawn"	string	39	true
DEFINED	10432954	s__global.g_bMonolithPlayerIsBuyMe_10432954	ds "_global.g_bMonolithPlayerIsBuyMenuAllowSpectatorBuying"	"_global.g_bMonolithPlayerIsBuyMenuAllowSpectatorBuying"	string	55	true
DEFINED	104329a4	s__global.g_sMonolithPlayerTargetN_104329a4	ds "_global.g_sMonolithPlayerTargetNameTag"	"_global.g_sMonolithPlayerTargetNameTag"	string	39	true
DEFINED	104329d4	s__global.g_nMonolithPlayerTargetR_104329d4	ds "_global.g_nMonolithPlayerTargetRank"	"_global.g_nMonolithPlayerTargetRank"	string	36	true
DEFINED	10432a78	s__global.g_bMonolithPlayerIsSprin_10432a78	ds "_global.g_bMonolithPlayerIsSprinting"	"_global.g_bMonolithPlayerIsSprinting"	string	37	true
DEFINED	10432aa4	s__global.g_fMonolithPlayerSprintS_10432aa4	ds "_global.g_fMonolithPlayerSprintStamina"	"_global.g_fMonolithPlayerSprintStamina"	string	39	true
DEFINED	10432ad4	s__global.g_nMonolithPlayerCurMapI_10432ad4	ds "_global.g_nMonolithPlayerCurMapIndex"	"_global.g_nMonolithPlayerCurMapIndex"	string	37	true
DEFINED	10432b00	s__global.g_sMonolithPlayerCurMapN_10432b00	ds "_global.g_sMonolithPlayerCurMapName"	"_global.g_sMonolithPlayerCurMapName"	string	36	true
DEFINED	10432b2c	s__global.g_sMonolithPlayerCurMapD_10432b2c	ds "_global.g_sMonolithPlayerCurMapDisPlay"	"_global.g_sMonolithPlayerCurMapDisPlay"	string	39	true
DEFINED	10432b5c	s__global.g_nMonolithPlayerCurMode_10432b5c	ds "_global.g_nMonolithPlayerCurModeIndex"	"_global.g_nMonolithPlayerCurModeIndex"	string	38	true
DEFINED	10432b8c	s__global.g_sMonolithPlayerCurMode_10432b8c	ds "_global.g_sMonolithPlayerCurModeName"	"_global.g_sMonolithPlayerCurModeName"	string	37	true
DEFINED	10432bb8	s__global.g_sMonolithPlayerCurMode_10432bb8	ds "_global.g_sMonolithPlayerCurModeDisPlay"	"_global.g_sMonolithPlayerCurModeDisPlay"	string	40	true
DEFINED	10432be8	s__global.g_nMonolithPlayerCurGoal_10432be8	ds "_global.g_nMonolithPlayerCurGoalRound"	"_global.g_nMonolithPlayerCurGoalRound"	string	38	true
DEFINED	10432c18	s__global.g_nMonolithPlayerCurGoal_10432c18	ds "_global.g_nMonolithPlayerCurGoalTime"	"_global.g_nMonolithPlayerCurGoalTime"	string	37	true
DEFINED	10432c44	s__global.g_nMonolithPlayerCurGoal_10432c44	ds "_global.g_nMonolithPlayerCurGoalKill"	"_global.g_nMonolithPlayerCurGoalKill"	string	37	true
DEFINED	10432c70	s__global.g_sMonolithPlayerCurRoom_10432c70	ds "_global.g_sMonolithPlayerCurRoomTitle"	"_global.g_sMonolithPlayerCurRoomTitle"	string	38	true
DEFINED	10432ca0	s__global.g_nMonolithPlayerCurRoom_10432ca0	ds "_global.g_nMonolithPlayerCurRoomIndex"	"_global.g_nMonolithPlayerCurRoomIndex"	string	38	true
DEFINED	10432cd0	s__global.g_bMonolithPlayerCurDeat_10432cd0	ds "_global.g_bMonolithPlayerCurDeathChating"	"_global.g_bMonolithPlayerCurDeathChating"	string	41	true
DEFINED	10432d04	s__global.g_nMonolithPlayerCurLimi_10432d04	ds "_global.g_nMonolithPlayerCurLimitWeapon"	"_global.g_nMonolithPlayerCurLimitWeapon"	string	40	true
DEFINED	10432d34	s__global.g_nMonolithPlayerCurRewa_10432d34	ds "_global.g_nMonolithPlayerCurRewardItem"	"_global.g_nMonolithPlayerCurRewardItem"	string	39	true
DEFINED	10432d64	s__global.g_bMonolithPlayerCurUseL_10432d64	ds "_global.g_bMonolithPlayerCurUseLoadingScreenMap"	"_global.g_bMonolithPlayerCurUseLoadingScreenMap"	string	48	true
DEFINED	10432da0	s__global.g_sMonolithPlayerCurMapD_10432da0	ds "_global.g_sMonolithPlayerCurMapDescription"	"_global.g_sMonolithPlayerCurMapDescription"	string	43	true
DEFINED	10432dd4	s__global.g_sMonolithPlayerCurLoad_10432dd4	ds "_global.g_sMonolithPlayerCurLoadingPicture"	"_global.g_sMonolithPlayerCurLoadingPicture"	string	43	true
DEFINED	10432e08	s__global.g_nMonolithPlayerCurMapR_10432e08	ds "_global.g_nMonolithPlayerCurMapRespawnInvulnerabilityTime"	"_global.g_nMonolithPlayerCurMapRespawnInvulnerabilityTime"	string	58	true
DEFINED	10432e50	s__global.g_anMonolithPlayerLeftTi_10432e50	ds "_global.g_anMonolithPlayerLeftTimeCheck"	"_global.g_anMonolithPlayerLeftTimeCheck"	string	40	true
DEFINED	10432e8c	s__global.g_fMonolithPlayerFPS_10432e8c	ds "_global.g_fMonolithPlayerFPS"	"_global.g_fMonolithPlayerFPS"	string	29	true
DEFINED	10438374	s__global.g_nMonolithSaveGameSaveF_10438374	ds "_global.g_nMonolithSaveGameSaveFileMissionNumber"	"_global.g_nMonolithSaveGameSaveFileMissionNumber"	string	49	true
DEFINED	1043892c	s__global.g_nMonolithSystemLayerPl_1043892c	ds "_global.g_nMonolithSystemLayerPlatform"	"_global.g_nMonolithSystemLayerPlatform"	string	39	true
DEFINED	1044126c	s_GlobalSoundFilter_1044126c	ds "GlobalSoundFilter"	"GlobalSoundFilter"	string	18	true
DEFINED	1044128c	s_GlobalSoundMixer_1044128c	ds "GlobalSoundMixer"	"GlobalSoundMixer"	string	17	true
DEFINED	10446698	s_GlobalSoundBundle_10446698	ds "GlobalSoundBundle"	"GlobalSoundBundle"	string	18	true
DEFINED	10446780	s__global.g_bMonolithMultiplayerFi_10446780	ds "_global.g_bMonolithMultiplayerFirstOpenTutorialPopup"	"_global.g_bMonolithMultiplayerFirstOpenTutorialPopup"	string	53	true
DEFINED	104467c7		ds "@_global.g_sMonolithPlayerPickupName"	"@_global.g_sMonolithPlayerPickupName"	string	37	true
DEFINED	104467f4	s__global.g_anMonolithPlayerZombie_104467f4	ds "_global.g_anMonolithPlayerZombieUserIndex"	"_global.g_anMonolithPlayerZombieUserIndex"	string	42	true
DEFINED	104527d8	s__global.g_bMonolithPlayerCurUseP_104527d8	ds "_global.g_bMonolithPlayerCurUsePerk"	"_global.g_bMonolithPlayerCurUsePerk"	string	36	true
DEFINED	10452804	s__global.g_bMonolithPlayerCurThir_10452804	ds "_global.g_bMonolithPlayerCurThirdView"	"_global.g_bMonolithPlayerCurThirdView"	string	38	true
```
