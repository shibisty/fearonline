## Launcher callbacks

```
undefined1 LoginLauncher(undefined1 *param_1,short param_2,ushort *param_3,int param_4,int param _5)

{
  ushort uVar1;
  undefined1 *puVar2;
  short sVar3;
  ushort *puVar4;
  char cVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  ushort *puVar9;
  int *piVar10;
  void *pvVar11;
  undefined4 *puVar12;
  ushort *puVar13;
  undefined4 *puVar14;
  bool bVar15;
  undefined4 local_8;
  undefined4 local_4;
  
  iVar8 = param_4;
  puVar4 = param_3;
  sVar3 = param_2;
                    /* 0x2530  3  LoginLauncher */
  puVar2 = param_1;
  if (param_1 == (undefined1 *)0x0) {
    return 0;
  }
  if (param_2 == 0) {
    return 0;
  }
  if (param_3 == (ushort *)0x0) {
    return 0;
  }
  if (param_4 == 0) {
    return 0;
  }
  iVar6 = FUN_100019d0();
  if (*(char *)(iVar6 + 0x88) == '\x01') {
    local_8 = 0x3e;
    FUN_100012b0("LoginLauncher");
    local_4 = 10;
    puVar12 = &local_8;
    puVar14 = &local_4;
    goto LAB_10002594;
  }
  puVar13 = &DAT_1016e48c;
  puVar9 = puVar4;
  do {
    uVar1 = *puVar9;
    bVar15 = uVar1 < *puVar13;
    if (uVar1 != *puVar13) {
LAB_100025e9:
      iVar6 = (1 - (uint)bVar15) - (uint)(bVar15 != 0);
      goto LAB_100025ee;
    }
    if (uVar1 == 0) break;
    uVar1 = puVar9[1];
    bVar15 = uVar1 < puVar13[1];
    if (uVar1 != puVar13[1]) goto LAB_100025e9;
    puVar9 = puVar9 + 2;
    puVar13 = puVar13 + 2;
  } while (uVar1 != 0);
  iVar6 = 0;
LAB_100025ee:
  if (iVar6 == 0) {
    iVar8 = FUN_100019d0();
    *(undefined4 *)(iVar8 + 0x6c) = 2;
    local_4 = 0x45;
    FUN_100012b0("LoginLauncher");
    local_8 = 0xc;
    uVar7 = FUN_1000f180(&local_8,&param_1,&local_4,L"Login IP: %s, Port: %d, Type: %s",puVar2,s Var3
                        );
    FUN_1000f380(uVar7);
LAB_10002803:
    FUN_10001250();
  }
  else {
    puVar13 = &DAT_1016e4dc;
    puVar9 = puVar4;
    do {
      uVar1 = *puVar9;
      bVar15 = uVar1 < *puVar13;
      if (uVar1 != *puVar13) {
LAB_10002678:
        iVar6 = (1 - (uint)bVar15) - (uint)(bVar15 != 0);
        goto LAB_1000267d;
      }
      if (uVar1 == 0) break;
      uVar1 = puVar9[1];
      bVar15 = uVar1 < puVar13[1];
      if (uVar1 != puVar13[1]) goto LAB_10002678;
      puVar9 = puVar9 + 2;
      puVar13 = puVar13 + 2;
    } while (uVar1 != 0);
    iVar6 = 0;
LAB_1000267d:
    if (iVar6 != 0) {
      puVar13 = &DAT_1016e4e4;
      puVar9 = puVar4;
      do {
        uVar1 = *puVar9;
        bVar15 = uVar1 < *puVar13;
        if (uVar1 != *puVar13) {
LAB_1000273e:
          iVar6 = (1 - (uint)bVar15) - (uint)(bVar15 != 0);
          goto LAB_10002743;
        }
        if (uVar1 == 0) break;
        uVar1 = puVar9[1];
        bVar15 = uVar1 < puVar13[1];
        if (uVar1 != puVar13[1]) goto LAB_1000273e;
        puVar9 = puVar9 + 2;
        puVar13 = puVar13 + 2;
      } while (uVar1 != 0);
      iVar6 = 0;
LAB_10002743:
      if (iVar6 == 0) {
        iVar8 = FUN_100019d0();
        *(undefined4 *)(iVar8 + 0x6c) = 5;
        local_4 = 0x54;
        FUN_100012b0("LoginLauncher");
        local_8 = 0xc;
        uVar7 = FUN_1000f180(&local_8,&param_1,&local_4,
                             L"Login IP: %s, Port: %d, Type: %s, accountID: %s",puVar2,sVar3,puVa r4)
        ;
        FUN_1000f380(uVar7);
      }
      else {
        if (param_5 == 0) {
          return 0;
        }
        iVar6 = FUN_100019d0();
        *(undefined4 *)(iVar6 + 0x6c) = 0;
        local_4 = 0x5e;
        FUN_100012b0("LoginLauncher");
        local_8 = 0xc;
        uVar7 = FUN_1000f180(&local_8,&param_1,&local_4,
                             L"Login IP: %s, Port: %d, Type: %s, accountID: %s, password: %s\n",
                             puVar2,sVar3,puVar4,iVar8);
        FUN_1000f380(uVar7);
      }
      goto LAB_10002803;
    }
    iVar6 = FUN_100019d0();
    *(undefined4 *)(iVar6 + 0x6c) = 4;
    local_4 = 0x4a;
    FUN_100012b0("LoginLauncher");
    local_8 = 0xc;
    uVar7 = FUN_1000f180(&local_8,&param_1,&local_4,L"Login IP: %s, Port: %d, Type: %s",puVar2,s Var3
                        );
    FUN_1000f380(uVar7);
    FUN_10001250();
    param_1 = &stack0xffffffe4;
    FUN_100031d0(iVar8,0);
    iVar8 = FUN_100019d0();
    cVar5 = FUN_10011bd0(iVar8 + 0xe0);
    if (cVar5 == '\0') {
      return 0;
    }
  }
  FUN_100019d0();
  FUN_100030d0();
  iVar8 = FUN_100019d0();
  *(short *)(iVar8 + 0x10) = sVar3;
  iVar8 = FUN_100019d0();
  FUN_10002110(*(undefined4 *)(iVar8 + 0x4c));
  iVar8 = FUN_100019d0();
  *(undefined1 *)(iVar8 + 0x88) = 1;
  FUN_100019d0();
  FUN_100030d0();
  FUN_100019d0();
  FUN_100030d0();
  piVar10 = (int *)FUN_100019d0();
  if (*piVar10 == 0) {
    pvVar11 = operator_new(0x48);
    if (pvVar11 == (void *)0x0) {
      uVar7 = 0;
    }
    else {
      uVar7 = FUN_100054a0();
    }
    puVar12 = (undefined4 *)FUN_100019d0();
    *puVar12 = uVar7;
  }
  FUN_100019d0();
  FUN_10001bf0();
  FUN_10005630();
  FUN_100019d0();
  iVar8 = FUN_10001bf0();
  iVar8 = **(int **)(iVar8 + 4);
  FUN_100019d0();
  cVar5 = (**(code **)(iVar8 + 0x34))();
  if (cVar5 != '\0') {
    return 1;
  }
  local_4 = 0x77;
  FUN_100012b0("LoginLauncher");
  local_8 = 10;
  puVar12 = &local_4;
  puVar14 = &local_8;
LAB_10002594:
  uVar7 = FUN_1000f180(puVar14,&param_1,puVar12);
  FUN_1000f380(uVar7);
  FUN_10001250();
  return 0;
}
```

```
void DisConnect(void)

{
  undefined *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined *puVar4;
  LONG LVar5;
  undefined *local_14;
  undefined4 local_10;
  undefined4 local_c [2];
  
                    /* 0x2910  1  DisConnect */
  local_10 = 0x80;
  local_14 = PTR_DAT_1017b000;
  uVar2 = FUN_10012ed0("DisConnect");
  uVar3 = FUN_10001310(uVar2);
  FUN_10012ef0(uVar3,"DisConnect",uVar2);
  puVar1 = local_14;
  puVar4 = PTR_DAT_1017b000;
  if (local_14 != (undefined *)0x0) {
    puVar4 = local_14;
  }
  uVar2 = FUN_10012ed0(puVar4);
  FUN_10001640(uVar2);
  local_c[0] = 0xc;
  uVar2 = FUN_1000f180(local_c,&local_14,&local_10,&DAT_1016e5f0);
  FUN_1000f1f0(uVar2);
  if (((puVar1 != (undefined *)0x0) && (puVar1 != PTR_DAT_1017b000)) &&
     (puVar1 + -8 != (undefined *)0x0)) {
    LVar5 = InterlockedDecrement((LONG *)(puVar1 + -4));
    if (LVar5 == 0) {
      FUN_10012d80(puVar1 + -8);
    }
  }
  FUN_100019d0();
  FUN_10001c10();
  FUN_100019d0();
  FUN_10001a40();
  return;
}

```

```
int Set_On_BLOCK_IP_Callback(undefined *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined *puVar5;
  undefined4 local_8;
  undefined4 local_4;
  
                    /* 0x2f70  6  Set_On_BLOCK_IP_Callback */
  iVar3 = (int)param_1;
  if (param_1 == (undefined *)0x0) {
    local_8 = 0xcf;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_On_BLOCK_IP_Callback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_On_BLOCK_IP_Callback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_4 = 9;
    uVar1 = FUN_1000f180(&local_4,&param_1,&local_8,"fn is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  if (*(int *)(iVar4 + 0x68) == 0) {
    local_4 = 0xd5;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_On_BLOCK_IP_Callback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_On_BLOCK_IP_Callback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_8 = 9;
    uVar1 = FUN_1000f180(&local_8,&param_1,&local_4,"m_pLauncherData is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  *(int *)(*(int *)(iVar4 + 0x68) + 0x14) = iVar3;
  return iVar4 + 0x68;
}
```

```
int Set_On_LOGIN_DATA_Callback(undefined *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined *puVar5;
  undefined4 local_8;
  undefined4 local_4;
  
                    /* 0x2cb0  7  Set_On_LOGIN_DATA_Callback */
  iVar3 = (int)param_1;
  if (param_1 == (undefined *)0x0) {
    local_8 = 0xad;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_On_LOGIN_DATA_Callback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_On_LOGIN_DATA_Callback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_4 = 9;
    uVar1 = FUN_1000f180(&local_4,&param_1,&local_8,"fn is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  if (*(int *)(iVar4 + 0x68) == 0) {
    local_4 = 0xb3;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_On_LOGIN_DATA_Callback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_On_LOGIN_DATA_Callback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_8 = 9;
    uVar1 = FUN_1000f180(&local_8,&param_1,&local_4,"m_pLauncherData is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  *(int *)(*(int *)(iVar4 + 0x68) + 0xc) = iVar3;
  return iVar4 + 0x68;
}
```

```
int Set_On_LOGIN_FAIL_Callback(undefined *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined *puVar5;
  undefined4 local_8;
  undefined4 local_4;
  
                    /* 0x2e10  8  Set_On_LOGIN_FAIL_Callback */
  iVar3 = (int)param_1;
  if (param_1 == (undefined *)0x0) {
    local_8 = 0xbe;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_On_LOGIN_FAIL_Callback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_On_LOGIN_FAIL_Callback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_4 = 9;
    uVar1 = FUN_1000f180(&local_4,&param_1,&local_8,"fn is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  if (*(int *)(iVar4 + 0x68) == 0) {
    local_4 = 0xc4;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_On_LOGIN_FAIL_Callback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_On_LOGIN_FAIL_Callback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_8 = 9;
    uVar1 = FUN_1000f180(&local_8,&param_1,&local_4,"m_pLauncherData is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  *(int *)(*(int *)(iVar4 + 0x68) + 0x10) = iVar3;
  return iVar4 + 0x68;
}
```

```
int Set_OnIoConnectedCallback(undefined *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined *puVar5;
  undefined4 local_8;
  undefined4 local_4;
  
                    /* 0x29f0  4  Set_OnIoConnectedCallback */
  iVar3 = (int)param_1;
  if (param_1 == (undefined *)0x0) {
    local_8 = 0x8b;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_OnIoConnectedCallback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_OnIoConnectedCallback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_4 = 9;
    uVar1 = FUN_1000f180(&local_4,&param_1,&local_8,"fn is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  if (*(int *)(iVar4 + 0x68) == 0) {
    local_4 = 0x91;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_OnIoConnectedCallback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_OnIoConnectedCallback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_8 = 9;
    uVar1 = FUN_1000f180(&local_8,&param_1,&local_4,"m_pLauncherData is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  *(int *)(*(int *)(iVar4 + 0x68) + 4) = iVar3;
  return iVar4 + 0x68;
}
```

```
int Set_OnIoDisonnectedCallback(undefined *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined *puVar5;
  undefined4 local_8;
  undefined4 local_4;
  
                    /* 0x2b50  5  Set_OnIoDisonnectedCallback */
  iVar3 = (int)param_1;
  if (param_1 == (undefined *)0x0) {
    local_8 = 0x9c;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_OnIoDisonnectedCallback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_OnIoDisonnectedCallback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_4 = 9;
    uVar1 = FUN_1000f180(&local_4,&param_1,&local_8,"fn is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  if (*(int *)(iVar4 + 0x68) == 0) {
    local_4 = 0xa2;
    param_1 = PTR_DAT_1017b000;
    uVar1 = FUN_10012ed0("Set_OnIoDisonnectedCallback");
    uVar2 = FUN_10001310(uVar1);
    FUN_10012ef0(uVar2,"Set_OnIoDisonnectedCallback",uVar1);
    puVar5 = param_1;
    if (param_1 == (undefined *)0x0) {
      puVar5 = PTR_DAT_1017b000;
    }
    uVar1 = FUN_10012ed0(puVar5);
    FUN_10001640(uVar1);
    local_8 = 9;
    uVar1 = FUN_1000f180(&local_8,&param_1,&local_4,"m_pLauncherData is NULL");
    FUN_1000f1f0(uVar1);
    iVar3 = FUN_10001250();
    return iVar3;
  }
  iVar4 = FUN_100019d0();
  *(int *)(*(int *)(iVar4 + 0x68) + 8) = iVar3;
  return iVar4 + 0x68;
}
```
