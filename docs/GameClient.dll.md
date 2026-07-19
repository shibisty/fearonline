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
