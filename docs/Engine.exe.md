## Arguments check before start

```
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_10162b20(int param_1)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  char *pcVar5;
  undefined4 unaff_EBX;
  undefined4 unaff_ESI;
  undefined1 auStack_108 [4];
  undefined1 local_104;
  
  if (DAT_104a8728 != (HWND)0x0) {
    *(HWND *)(DAT_104a8714 + 0x22c) = DAT_104a8728;
  }
  local_104 = 0;
  iVar2 = (**(code **)(*DAT_104ac068 + 0x130))(&DAT_1044afcc);
  if (iVar2 == 0) {
    pcVar5 = "Please Excute \'Launch FEAROnline.exe\'";
LAB_10162c6c:
    MessageBoxA(DAT_104a8728,pcVar5,"FearOnline",0x40000);
    PostQuitMessage(0);
  }
  else {
    uVar3 = (**(code **)(*DAT_104ac068 + 0x144))(iVar2);
    thunk_FUN_100391e0(auStack_108,uVar3,0x104);
    iVar2 = param_1 + 0x440;
    thunk_FUN_10042ff0(auStack_108,iVar2,0x104);
    iVar4 = thunk_FUN_100435f0(iVar2,L"none");
    if (iVar4 != 0) {
      iVar4 = thunk_FUN_100435f0(iVar2,&DAT_1044afb8);
      if (iVar4 != 0) {
        iVar4 = thunk_FUN_100435f0(iVar2,&DAT_1044afb0);
        if (iVar4 == 0) {
          cVar1 = thunk_FUN_1014f480();
          if (cVar1 != '\0') {
            *(undefined1 *)(param_1 + 0x230) = 1;
            goto LAB_10162c79;
          }
        }
        else {
          iVar4 = thunk_FUN_100435f0(iVar2,&DAT_1044acec);
          if (iVar4 == 0) {
            cVar1 = thunk_FUN_101617c0();
            if (cVar1 != '\0') {
              *(undefined1 *)(param_1 + 0x230) = 1;
              goto LAB_10162c79;
            }
          }
          else {
            iVar2 = thunk_FUN_100435f0(iVar2,&DAT_1044ab88);
            if (iVar2 == 0) {
              cVar1 = thunk_FUN_1014f650();
              if (cVar1 != '\0') {
                *(undefined1 *)(param_1 + 0x230) = 1;
                goto LAB_10162c79;
              }
            }
          }
        }
        pcVar5 = "Wrong Argument!";
        goto LAB_10162c6c;
      }
    }
    *(undefined1 *)(param_1 + 0x230) = 0;
  }
LAB_10162c79:
  iVar2 = (**(code **)(*DAT_104ac068 + 0x130))(&DAT_10426784);
  if (iVar2 != 0) {
    uVar3 = (**(code **)(*DAT_104ac068 + 0x144))(iVar2);
    thunk_FUN_100391e0(&stack0xfffffef0,uVar3,4);
    pcVar5 = &stack0xfffffef0;
    do {
      cVar1 = *pcVar5;
      pcVar5 = pcVar5 + 1;
    } while (cVar1 != '\0');
    if (0 < (int)pcVar5 - (int)&stack0xfffffef1) {
      _DAT_104befb8 = unaff_EBX;
    }
  }
  iVar2 = (**(code **)(*DAT_104ac068 + 0x130))(&DAT_1044af68);
  if (iVar2 != 0) {
    uVar3 = (**(code **)(*DAT_104ac068 + 0x144))(iVar2);
    thunk_FUN_100391e0(&stack0xfffffee8,uVar3,4);
    pcVar5 = &stack0xfffffee8;
    do {
      cVar1 = *pcVar5;
      pcVar5 = pcVar5 + 1;
    } while (cVar1 != '\0');
    if (0 < (int)pcVar5 - (int)&stack0xfffffee9) {
      _DAT_104befbc = unaff_ESI;
    }
  }
  *(undefined4 *)(param_1 + 0xa58) = unaff_ESI;
  return;
}
```

## The same, but connection args

```
undefined4 FUN_10101ad0(int param_1,ulong *param_2)

{
  int iVar1;
  undefined4 uVar2;
  ulong uVar3;
  char unaff_DI;
  char acStack_114 [4];
  undefined1 uStack_110;
  
  if ((param_1 != 0) && (param_2 != (ulong *)0x0)) {
    thunk_FUN_1003b8a0(param_1,&DAT_10421120,0x40);
    *param_2 = 0;
    iVar1 = (**(code **)(*DAT_104ac068 + 0x130))("LoginServerIP");
    if (iVar1 == 0) {
      FUN_10370420("Program Run Command Argu - Not Exist LoginServerIP");
      return 0;
    }
    uVar2 = (**(code **)(*DAT_104ac068 + 0x144))(iVar1);
    thunk_FUN_100391e0(&stack0xfffffdf0,uVar2,0x104);
    if (unaff_DI != '\0') {
      FUN_10370420("Program Run Command Argu - LoginServerIP: %s",&stack0xfffffdf0);
      thunk_FUN_10042ff0(&stack0xfffffdf0,param_1,0x40);
      iVar1 = (**(code **)(*DAT_104ac068 + 0x130))("LoginServerPort");
      if (iVar1 == 0) {
        FUN_10370420("[Error] Program Run Command Argu - Not Exist LoginServerPort");
        return 0;
      }
      uStack_110 = 0;
      uVar2 = (**(code **)(*DAT_104ac068 + 0x144))(iVar1);
      thunk_FUN_100450a0(uVar2);
      if (acStack_114[0] != '\0') {
        FUN_10370420("Program Run Command Argu - LoginServerPort: %s",acStack_114);
        uVar3 = strtoul(acStack_114,(char **)0x0,0);
        *param_2 = uVar3;
        if (uVar3 != 0) {
          return 1;
        }
      }
      FUN_10370420("[Error] Program Run Command Argu - Invalid Info LoginServerPort");
      return 0;
    }
    FUN_10370420("[Error] Program Run Command Argu - Invalid Info LoginServerIP");
  }
  return 0;
}
```
