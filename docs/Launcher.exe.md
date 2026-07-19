## Arguments for start Engine.exe

```
void __thiscall FUN_0048f700(int param_1,undefined4 param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  LPCWSTR lpOutputString;
  int nCmdShow;
  undefined *local_1ec;
  basic_string<> *local_1e8;
  int local_1e4;
  basic_string<> local_1d0 [28];
  basic_string<> local_1b4 [28];
  basic_string<> local_198 [28];
  basic_string<> local_17c [28];
  basic_string<> local_160 [28];
  basic_string<> local_144 [28];
  basic_string<> local_128 [28];
  basic_string<> local_10c [28];
  basic_string<> local_f0 [28];
  basic_string<> *local_d4;
  basic_string<> local_d0 [28];
  int local_b4;
  WCHAR local_b0 [20];
  basic_string<> local_88 [28];
  basic_string<> local_6c [28];
  basic_string<> local_50 [28];
  undefined4 local_34;
  basic_string<> local_30 [28];
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0049f516;
  local_10 = ExceptionList;
  uVar2 = DAT_004cd518 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar2;
  iVar3 = FUN_00408490(uVar2);
  local_34 = FUN_0048e910(*(undefined4 *)(iVar3 + 0x20));
  wsprintfW(local_b0,(LPCWSTR)&PTR_DAT_004b7f34,local_34);
  FUN_004062d0();
  iVar3 = FUN_00405730(&DAT_004b7f3c);
  cVar1 = FUN_00401f70(iVar3 + 0x164);
  if (cVar1 == '\0') {
    FUN_004062d0();
    local_1e4 = FUN_00405730();
    local_1e4 = local_1e4 + 0x164;
  }
  else {
    local_1e4 = basic_string<>(&PTR_DAT_004b7f40);
    local_8 = 0;
  }
  local_b4 = local_1e4;
  FUN_00401040(local_1e4);
  local_8 = 2;
  if (cVar1 != '\0') {
    std::basic_string<>::~basic_string<>(local_d0);
  }
  basic_string<>(L"+PB ");
  local_8 = CONCAT31(local_8._1_3_,3);
  FUN_00404da0(local_88);
  FUN_004062d0();
  iVar3 = FUN_00405730(&DAT_004b7f54);
  cVar1 = FUN_00401f70(iVar3 + 0x180);
  if (cVar1 == '\0') {
    FUN_004062d0();
    iVar3 = FUN_00405730();
    local_1e8 = (basic_string<> *)(iVar3 + 0x180);
  }
  else {
    local_1e8 = local_88;
  }
  local_d4 = local_1e8;
  FUN_00401040(local_1e8);
  local_8._0_1_ = 4;
  std::basic_string<>::operator=(local_30,(char *)L" +UIPB ");
  FUN_00404da0(local_50);
  iVar3 = FUN_00408240();
  if (*(char *)(iVar3 + 0x13d8) == '\x01') {
    local_1ec = &DAT_004b7f68;
  }
  else {
    local_1ec = &DAT_004b7f6c;
  }
  basic_string<>(local_1ec);
  local_8 = CONCAT31(local_8._1_3_,5);
  std::basic_string<>::operator=(local_30,(char *)L" +BANNER ");
  std::basic_string<>::operator=(local_6c,"1");
  FUN_00404da0(local_6c);
  switch(*(undefined4 *)(param_1 + 0xf00)) {
  case 1:
  case 2:
  case 4:
    std::basic_string<>::operator=(local_30,(char *)L" +UID ");
    FUN_00404da0(param_2);
    std::basic_string<>::operator=(local_30,(char *)L" +LauncherID ");
    std::basic_string<>::operator=(local_30,(char *)local_b0);
    break;
  case 5:
    std::basic_string<>::operator=(local_30,(char *)L" +UID ");
    FUN_00404da0(param_2);
  }
  std::basic_string<>::operator=(local_30,(char *)L" +windowed 1");
  FUN_004062d0();
  iVar3 = FUN_00405730();
  if (*(int *)(iVar3 + 0x160) == 1) {
    std::basic_string<>::operator=(local_30,(char *)L" +nVersionType 1");
  }
  FUN_004062d0();
  uVar4 = FUN_00401ee0(local_f0,local_30,L" +Lan ");
  local_8._0_1_ = 6;
  uVar5 = FUN_00405730();
  uVar4 = FUN_00405460(local_10c,uVar4,uVar5);
  local_8._0_1_ = 7;
  FUN_00402e00(uVar4);
  local_8._0_1_ = 6;
  std::basic_string<>::~basic_string<>(local_10c);
  local_8._0_1_ = 5;
  std::basic_string<>::~basic_string<>(local_f0);
  FUN_004062d0();
  FUN_00405730();
  iVar3 = FUN_004028b0();
  if (iVar3 != 0) {
    FUN_004062d0();
    uVar4 = FUN_00401ee0(local_128,local_30,L" +LoginServerIP ");
    local_8._0_1_ = 8;
    iVar3 = FUN_00405730();
    uVar4 = FUN_00405460(local_144,uVar4,iVar3 + 0x19c);
    local_8._0_1_ = 9;
    FUN_00402e00(uVar4);
    local_8._0_1_ = 8;
    std::basic_string<>::~basic_string<>(local_144);
    local_8._0_1_ = 5;
    std::basic_string<>::~basic_string<>(local_128);
  }
  FUN_004062d0();
  FUN_00405730();
  iVar3 = FUN_004028b0();
  if (iVar3 != 0) {
    FUN_004062d0();
    uVar4 = FUN_00401ee0(local_160,local_30,L" +LoginServerPort ");
    local_8._0_1_ = 10;
    iVar3 = FUN_00405730();
    uVar4 = FUN_00405460(local_17c,uVar4,iVar3 + 0x1b8);
    local_8._0_1_ = 0xb;
    FUN_00402e00(uVar4);
    local_8._0_1_ = 10;
    std::basic_string<>::~basic_string<>(local_17c);
    local_8._0_1_ = 5;
    std::basic_string<>::~basic_string<>(local_160);
  }
  cVar1 = FUN_00407cc0();
  if ((cVar1 != '\0') && (cVar1 = FUN_00407ca0(), cVar1 == '\0')) {
    FUN_00483b00();
    std::basic_string<>::operator=(local_30,(char *)L" +OnSteam 1");
    uVar4 = FUN_004053d0(local_198,L" +SteamID ",param_1 + 0x13bc);
    local_8._0_1_ = 0xc;
    FUN_00404da0(uVar4);
    local_8._0_1_ = 5;
    std::basic_string<>::~basic_string<>(local_198);
  }
  nCmdShow = 6;
  iVar3 = FUN_00408490();
  ShowWindow(*(HWND *)(iVar3 + 0x20),nCmdShow);
  uVar4 = FUN_004011b0();
  FUN_0049ccc0(local_1b4,L"AppManager::StartClient GameRun -- sParam : (%s) \n",uVar4);
  lpOutputString = (LPCWSTR)FUN_004011b0();
  OutputDebugStringW(lpOutputString);
  std::basic_string<>::~basic_string<>(local_1b4);
  iVar3 = FUN_00405730();
  uVar4 = FUN_00405460(local_1d0,param_1 + 8,iVar3 + 0x118);
  local_8._0_1_ = 0xd;
  uVar4 = FUN_004011b0(uVar2,uVar4);
  uVar4 = FUN_004011b0(uVar4);
  uVar4 = FUN_0049cd60(uVar4);
  *(undefined4 *)(param_1 + 0x24) = uVar4;
  local_8._0_1_ = 5;
  std::basic_string<>::~basic_string<>(local_1d0);
  local_8._0_1_ = 4;
  std::basic_string<>::~basic_string<>((basic_string<> *)local_6c);
  local_8._0_1_ = 3;
  std::basic_string<>::~basic_string<>(local_50);
  local_8 = CONCAT31(local_8._1_3_,2);
  std::basic_string<>::~basic_string<>((basic_string<> *)local_30);
  local_8 = 0xffffffff;
  std::basic_string<>::~basic_string<>(local_88);
  ExceptionList = local_10;
  __security_check_cookie(local_14 ^ (uint)&stack0xfffffffc);
  return;
}
```

## Config parser

```
bool __fastcall FUN_0048d820(int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  LPCWSTR pWVar4;
  BOOL BVar5;
  int iVar6;
  DWORD dwFileAttributes;
  basic_string<> local_84 [28];
  basic_string<> local_68 [28];
  basic_string<> local_4c [28];
  basic_string<> local_30 [32];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_004a0c11;
  local_10 = ExceptionList;
  uVar1 = DAT_004cd518 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  FUN_0049b190(uVar1);
  local_8 = 0;
  FUN_0049b020();
  FUN_0049afb0(0,L"LauncherConfig");
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"Language",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"LauncherUrl",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"LauncherVersion",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"LauncherDataPathRoot",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"LauncherHttpDownloadPath",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"LauncherTorrentDownloadPath",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"LauncherNoticeUrl",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"ClientName",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"ClientUrlRoot",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"ClientRoot",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"ClientExePath",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"LoginUrl",uVar2);
  FUN_0049ccc0(local_30,&PTR_DAT_004b7340,*(undefined4 *)(param_1 + 0x150));
  local_8._0_1_ = 1;
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"VolumeSound",uVar2);
  local_8._0_1_ = 0;
  std::basic_string<>::~basic_string<>(local_30);
  FUN_0049ccc0(local_4c,&PTR_DAT_004b7360,*(undefined4 *)(param_1 + 0x154));
  local_8._0_1_ = 2;
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"VolumeBGM",uVar2);
  local_8._0_1_ = 0;
  std::basic_string<>::~basic_string<>(local_4c);
  uVar2 = FUN_0049ccc0(local_68,&PTR_DAT_004b737c,*(undefined4 *)(param_1 + 0x15c));
  local_8._0_1_ = 3;
  uVar3 = FUN_004011b0();
  FUN_0049af70(L"Development",uVar3);
  local_8._0_1_ = 0;
  std::basic_string<>::~basic_string<>(local_68);
  uVar3 = FUN_0049ccc0(local_84,&PTR_DAT_004b739c,*(undefined4 *)(param_1 + 0x160));
  local_8._0_1_ = 4;
  uVar2 = FUN_004011b0(uVar1,uVar3,uVar3,uVar2);
  FUN_0049af70(L"VersionType",uVar2);
  local_8 = (uint)local_8._1_3_ << 8;
  std::basic_string<>::~basic_string<>(local_84);
  uVar2 = FUN_004011b0();
  FUN_0049af70(&DAT_004b73bc,uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"UIPB",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"DEDI",uVar2);
  dwFileAttributes = 0x80;
  pWVar4 = (LPCWSTR)FUN_004011b0();
  SetFileAttributesW(pWVar4,dwFileAttributes);
  pWVar4 = (LPCWSTR)FUN_004011b0();
  BVar5 = DeleteFileW(pWVar4);
  if (BVar5 == 0) {
    OutputDebugStringW(L"[Config] deleting old config_xml is failed\n");
  }
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"LoginServerIP",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"LoginServerPort",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"CompareDir",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"AuthUrl",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"SocialUrl",uVar2);
  uVar2 = FUN_004011b0();
  FUN_0049af70(L"SteamNoticeUrl",uVar2);
  uVar2 = FUN_004011b0();
  iVar6 = FUN_0049b110(uVar2);
  local_8 = 0xffffffff;
  FUN_0049b0c0();
  ExceptionList = local_10;
  return iVar6 != 0;
}
```

## Event callbacks from ZLauncher.dll

```
undefined4 * __fastcall FUN_004823f0(undefined4 *param_1)

{
  HMODULE pHVar1;
  FARPROC pFVar2;
  undefined4 uVar3;
  
  *(undefined1 *)(param_1 + 2) = 0;
  pHVar1 = LoadLibraryW(L"ZLauncher.dll");
  param_1[1] = pHVar1;
  if (param_1[1] != 0) {
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"init");
    param_1[3] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"destoryed");
    param_1[4] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"Tick");
    param_1[5] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"GetLauncherData");
    param_1[6] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"LoginLauncher");
    param_1[7] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"DisConnect");
    param_1[8] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"Set_OnIoConnectedCallback");
    param_1[9] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"Set_OnIoDisonnectedCallback");
    param_1[10] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"Set_On_LOGIN_DATA_Callback");
    param_1[0xb] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"Set_On_LOGIN_FAIL_Callback");
    param_1[0xc] = pFVar2;
    pFVar2 = GetProcAddress((HMODULE)param_1[1],"Set_On_BLOCK_IP_Callback");
    param_1[0xd] = pFVar2;
    uVar3 = (*(code *)param_1[6])();
    *param_1 = uVar3;
    (*(code *)param_1[3])();
    (*(code *)param_1[9])(FUN_004823d0);
    (*(code *)param_1[10])(FUN_004823c0);
    (*(code *)param_1[0xb])(FUN_004823b0);
    (*(code *)param_1[0xc])(FUN_004823a0);
    (*(code *)param_1[0xd])(FUN_00482390);
  }
  return param_1;
}
```

## Hardcode for news page

```
void __fastcall FUN_0047dd10(int param_1)

{
  CWnd::ShowWindow((CWnd *)(param_1 + 0x66c),5);
  ShellExecuteW((HWND)0x0,L"open",L"http://fearonline.aeriagames.com/news",(LPCWSTR)0x0,(LPCWSTR )0x0
                ,1);
  return;
}
```

## Err009 Handller (Failed to connect the login server. Please restart the game.)

```
void __fastcall FUN_00490310(int param_1)

{
  char cVar1;
  int iVar2;
  wchar_t *pwVar3;
  undefined1 auStack_2f4 [12];
  undefined4 uStack_2e8;
  uint uVar4;
  uint uVar5;
  char local_29c;
  basic_string<> local_294 [28];
  basic_string<> local_278 [28];
  basic_string<> local_25c [28];
  basic_string<> local_240 [28];
  basic_string<> local_224 [28];
  basic_string<> local_208 [28];
  undefined1 *local_1ec;
  basic_string<> local_1e8 [28];
  basic_string<> local_1cc [28];
  basic_string<> local_1b0 [28];
  basic_string<> local_194 [28];
  undefined1 *local_178;
  basic_string<> local_174 [28];
  basic_string<> local_158 [28];
  basic_string<> local_13c [144];
  char *local_ac;
  basic_string<> *local_a8;
  basic_string<> *local_a4;
  uint local_a0;
  char local_35;
  basic_string<> local_34 [28];
  uint local_18;
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_004a026d;
  local_10 = ExceptionList;
  local_18 = DAT_004cd518 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  FUN_00481950();
  local_8 = 0;
  FUN_00405730();
  FUN_00401040();
  local_8 = CONCAT31(local_8._1_3_,1);
  local_35 = 0;
  cVar1 = FUN_00407cc0();
  if ((cVar1 == '\0') || (cVar1 = FUN_00407ca0(), cVar1 != '\0')) {
    local_29c = '\0';
  }
  else {
    local_29c = '\x01';
  }
  local_35 = local_29c;
  local_a0 = -(uint)(local_29c != '\0') & 2;
  if (local_29c == '\0') {
    cVar1 = FUN_00407ca0();
    if (cVar1 != '\0') {
      FUN_004847f0();
      local_8._0_1_ = 3;
      basic_string<>();
      local_8._0_1_ = 4;
      FUN_0048d600();
      local_8._0_1_ = 5;
      uStack_2e8 = 0x4904f0;
      FUN_004053d0();
      local_8._0_1_ = 6;
      FUN_00404da0();
      local_8._0_1_ = 5;
      std::basic_string<>::~basic_string<>(local_1e8);
      local_8._0_1_ = 4;
      std::basic_string<>::~basic_string<>(local_1cc);
      local_8._0_1_ = 3;
      std::basic_string<>::~basic_string<>(local_1b0);
      uVar5 = 0;
      uVar4 = 0;
      pwVar3 = (wchar_t *)FUN_004011b0();
      uStack_2e8 = 0x49055d;
      AfxMessageBox(pwVar3,uVar4,uVar5);
      local_8 = CONCAT31(local_8._1_3_,1);
      std::basic_string<>::~basic_string<>(local_158);
    }
  }
  else {
    FUN_004062d0();
    iVar2 = FUN_00405730();
    local_178 = auStack_2f4;
    FUN_00401040(iVar2 + 0x228);
    FUN_00484210(local_194,local_a0);
    local_8._0_1_ = 2;
    FUN_00402e00();
    local_8 = CONCAT31(local_8._1_3_,1);
    std::basic_string<>::~basic_string<>(local_194);
  }
  local_11 = '\0';
  basic_string<>();
  local_8._0_1_ = 7;
  local_ac = &local_11;
  local_a8 = local_13c;
  local_a4 = local_34;
  FUN_00407de0();
  FUN_0041551b();
  if (local_11 == '\0') {
    basic_string<>();
    local_8._0_1_ = 0xb;
    if (local_35 != '\0') {
      std::basic_string<>::operator=(local_174,(char *)L"SteamErr100");
    }
    FUN_0048d600();
    local_8._0_1_ = 0xc;
    uVar5 = 0;
    uVar4 = 0;
    pwVar3 = (wchar_t *)FUN_004011b0();
    uStack_2e8 = 0x490834;
    AfxMessageBox(pwVar3,uVar4,uVar5);
    local_8 = CONCAT31(local_8._1_3_,0xb);
    std::basic_string<>::~basic_string<>(local_278);
    FUN_0048b020();
    if (local_35 != '\0') {
      basic_string<>();
      FUN_0048b0c0();
      std::basic_string<>::~basic_string<>(local_294);
    }
    *(undefined1 *)(param_1 + 0xf28) = 1;
    local_8._0_1_ = 7;
    std::basic_string<>::~basic_string<>((basic_string<> *)local_174);
  }
  else {
    if ((local_35 == '\0') || (local_a0 != 2)) {
      iVar2 = FUN_00408240();
      if (*(int *)(iVar2 + 0xf00) == 0) {
        FUN_0048f700();
      }
      else {
        *(char *)(param_1 + 0xf2a) = local_11;
        FUN_00402e00();
      }
    }
    else {
      local_a0 = 1;
      FUN_004062d0();
      iVar2 = FUN_00405730();
      local_1ec = auStack_2f4;
      FUN_00401040(iVar2 + 0x20c);
      FUN_00484210(local_208,local_a0);
      local_8._0_1_ = 8;
      FUN_00402e00();
      local_8._0_1_ = 7;
      std::basic_string<>::~basic_string<>(local_208);
      local_11 = '\0';
      FUN_0041551b();
      if (local_11 == '\0') {
        basic_string<>();
        local_8._0_1_ = 9;
        FUN_0048d600();
        local_8._0_1_ = 10;
        uVar5 = 0;
        uVar4 = 0;
        pwVar3 = (wchar_t *)FUN_004011b0();
        uStack_2e8 = 0x49071c;
        AfxMessageBox(pwVar3,uVar4,uVar5);
        local_8._0_1_ = 9;
        std::basic_string<>::~basic_string<>(local_240);
        local_8._0_1_ = 7;
        std::basic_string<>::~basic_string<>(local_224);
        FUN_0048b020();
        basic_string<>();
        FUN_0048b0c0();
        std::basic_string<>::~basic_string<>(local_25c);
      }
      else {
        iVar2 = FUN_00408240();
        if (*(int *)(iVar2 + 0xf00) == 0) {
          FUN_0048f700();
        }
        else {
          *(char *)(param_1 + 0xf2a) = local_11;
          FUN_00402e00();
        }
      }
    }
    *(undefined1 *)(param_1 + 0xf28) = 1;
  }
  local_8._0_1_ = 1;
  std::basic_string<>::~basic_string<>(local_34);
  local_8 = (uint)local_8._1_3_ << 8;
  std::basic_string<>::~basic_string<>(local_13c);
  local_8 = 0xffffffff;
  FUN_00481ae0();
  ExceptionList = local_10;
  __security_check_cookie(local_18 ^ (uint)&stack0xfffffffc);
  return;
}
```