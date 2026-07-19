## OAuth window init

```
undefined4 FUN_10001000(HINSTANCE param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  HWND pHVar5;
  undefined4 *puVar6;
  WNDCLASSW local_28;
  
  iVar1 = GetSystemMetrics(5);
  iVar1 = iVar1 * 2 + 400;
  iVar2 = GetSystemMetrics(6);
  iVar3 = GetSystemMetrics(4);
  iVar2 = iVar3 + 0x1b3 + iVar2 * 2;
  iVar3 = GetSystemMetrics(0);
  iVar4 = GetSystemMetrics(1);
  local_28.cbClsExtra = 0;
  local_28.cbWndExtra = 0;
  local_28.hbrBackground = (HBRUSH)0x6;
  local_28.hCursor = LoadCursorW((HINSTANCE)0x0,(LPCWSTR)0x7f00);
  local_28.hIcon = LoadIconW((HINSTANCE)0x0,(LPCWSTR)0x7f04);
  local_28.hInstance = param_1;
  local_28.lpfnWndProc = (WNDPROC)&DAT_10002740;
  local_28.lpszClassName = L"Aeria";
  local_28.lpszMenuName = (LPCWSTR)0x0;
  local_28.style = 3;
  RegisterClassW(&local_28);
  pHVar5 = CreateWindowExW(0,L"Aeria",L"Login to Aeria Games",0x2080000,(iVar3 - iVar1) / 2,
                           (iVar4 - iVar2) / 2,iVar1,iVar2,(HWND)0x0,(HMENU)0x0,param_1,(LPVOID)0 x0)
  ;
  if (pHVar5 == (HWND)0x0) {
    return 0;
  }
  puVar6 = (undefined4 *)FUN_10001150();
  *puVar6 = pHVar5;
  return 1;
}
```

## Make link

```
undefined1 FUN_10001410(int param_1)

{
  undefined *puVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  int iVar8;
  int *piVar9;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  piVar9 = (int *)(param_1 + 0x14);
  iVar3 = ATL::CSimpleStringT<wchar_t,0>::StringLength(L"http://");
  puVar1 = (undefined *)*piVar9;
  if (puVar1 == (undefined *)0x0) {
    iVar8 = 0;
  }
  else if (puVar1 == PTR_DAT_1000907c) {
    iVar8 = 0;
  }
  else {
    iVar8 = *(int *)(puVar1 + -8);
  }
  iVar4 = FUN_10001640(iVar8 + iVar3);
  FUN_10003240(iVar4 + iVar8 * 2,L"http://",iVar3);
  FUN_10001880(iVar8 + iVar3);
  iVar3 = ATL::CSimpleStringT<wchar_t,0>::StringLength(L"test.aeriagames.com");
  puVar1 = (undefined *)*piVar9;
  if (puVar1 == (undefined *)0x0) {
    iVar8 = 0;
  }
  else if (puVar1 == PTR_DAT_1000907c) {
    iVar8 = 0;
  }
  else {
    iVar8 = *(int *)(puVar1 + -8);
  }
  iVar4 = FUN_10001640(iVar3 + iVar8);
  FUN_10003240(iVar4 + iVar8 * 2,L"test.aeriagames.com",iVar3);
  FUN_10001880(iVar3 + iVar8);
  iVar3 = ATL::CSimpleStringT<wchar_t,0>::StringLength
                    (
                    L"/dialog/oauth?response_type=code&state=xyz&scope=scope_general,scope_billing&l ang=en&"
                    );
  puVar1 = (undefined *)*piVar9;
  if (puVar1 == (undefined *)0x0) {
    iVar8 = 0;
  }
  else if (puVar1 == PTR_DAT_1000907c) {
    iVar8 = 0;
  }
  else {
    iVar8 = *(int *)(puVar1 + -8);
  }
  iVar4 = FUN_10001640(iVar3 + iVar8);
  FUN_10003240(iVar4 + iVar8 * 2,
               L"/dialog/oauth?response_type=code&state=xyz&scope=scope_general,scope_billing&lang=e n&"
               ,iVar3);
  FUN_10001880(iVar3 + iVar8);
  iVar3 = ATL::CSimpleStringT<wchar_t,0>::StringLength
                    (L"client_id=4cac1c17c173da5e17b03249b43c9dfe0519e6e4b&");
  puVar1 = (undefined *)*piVar9;
  if (puVar1 == (undefined *)0x0) {
    iVar8 = 0;
  }
  else if (puVar1 == PTR_DAT_1000907c) {
    iVar8 = 0;
  }
  else {
    iVar8 = *(int *)(puVar1 + -8);
  }
  iVar4 = FUN_10001640(iVar3 + iVar8);
  FUN_10003240(iVar4 + iVar8 * 2,L"client_id=4cac1c17c173da5e17b03249b43c9dfe0519e6e4b&",iVar3);
  FUN_10001880(iVar3 + iVar8);
  iVar3 = ATL::CSimpleStringT<wchar_t,0>::StringLength
                    (L"redirect_uri=http://www.aeriagames.com/content_only_launcher&");
  puVar1 = (undefined *)*piVar9;
  if (puVar1 == (undefined *)0x0) {
    iVar8 = 0;
  }
  else if (puVar1 == PTR_DAT_1000907c) {
    iVar8 = 0;
  }
  else {
    iVar8 = *(int *)(puVar1 + -8);
  }
  iVar4 = FUN_10001640(iVar3 + iVar8);
  FUN_10003240(iVar4 + iVar8 * 2,L"redirect_uri=http://www.aeriagames.com/content_only_launcher&" ,
               iVar3);
  FUN_10001880(iVar3 + iVar8);
  local_10 = 0;
  local_c = 0;
  local_8 = 400;
  local_4 = 0x1b3;
  puVar5 = (undefined4 *)FUN_10001150();
  puVar7 = &local_10;
  uVar6 = FUN_10001640(0);
  cVar2 = FUN_10001950(*puVar5,uVar6,puVar7);
  if (cVar2 == '\0') {
    return 0;
  }
  puVar7 = (undefined4 *)FUN_10001150();
  ShowWindow((HWND)*puVar7,5);
  return 1;
}
```

## For export

```
undefined4 Get_AccessCode(LPWSTR param_1)

{
  int iVar1;
  LPCWSTR psz2;
  
                    /* 0x26f0  1  Get_AccessCode */
  if (param_1 != (LPWSTR)0x0) {
    iVar1 = FUN_10001150();
    if (*(int *)(iVar1 + 8) == 2) {
      memset(param_1,0,0x40);
      iVar1 = FUN_10001150();
      psz2 = *(LPCWSTR *)(iVar1 + 0xc);
      if (*(LPCWSTR *)(iVar1 + 0xc) == (LPCWSTR)0x0) {
        psz2 = (LPCWSTR)PTR_DAT_1000907c;
      }
      StrCpyW(param_1,psz2);
      return 1;
    }
  }
  return 0;
}
```

```
undefined4 Get_State(void)

{
  int iVar1;
  
                    /* 0x26c0  2  Get_State */
  iVar1 = FUN_10001150();
  if (*(int *)(iVar1 + 0x10) != 0) {
    FUN_10001bc0(*(int *)(iVar1 + 0x10));
  }
  iVar1 = FUN_10001150();
  return *(undefined4 *)(iVar1 + 8);
}

```

```
void Send_Authentication(void)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  
                    /* 0x2680  3  Send_Authentication */
  iVar2 = FUN_10001150();
  *(undefined4 *)(iVar2 + 8) = 1;
  uVar3 = FUN_10001150();
  cVar1 = FUN_10001410(uVar3);
  if (cVar1 == '\0') {
    iVar2 = FUN_10001150();
    *(undefined4 *)(iVar2 + 8) = 0xffffffff;
  }
  return;
}

```
