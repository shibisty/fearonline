@echo off
setlocal
cd /d "%~dp0"

rem The game is 32-bit: ZNetwork.dll MUST be built with the x86 toolchain
rem (inline __asm, 32-bit CONTEXT, 4-byte pointers in the struct layouts).
rem If this shell is not already an x86 VS prompt, load vcvars32 ourselves.
if /I "%VSCMD_ARG_TGT_ARCH%"=="x86" goto :build

set "VCVARS32="
for %%E in (Community Professional Enterprise BuildTools) do (
    if not defined VCVARS32 if exist "C:\Program Files\Microsoft Visual Studio\2022\%%E\VC\Auxiliary\Build\vcvars32.bat" set "VCVARS32=C:\Program Files\Microsoft Visual Studio\2022\%%E\VC\Auxiliary\Build\vcvars32.bat"
)
for %%E in (Community Professional Enterprise BuildTools) do (
    if not defined VCVARS32 if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\%%E\VC\Auxiliary\Build\vcvars32.bat" set "VCVARS32=C:\Program Files (x86)\Microsoft Visual Studio\2019\%%E\VC\Auxiliary\Build\vcvars32.bat"
)
if not defined VCVARS32 (
    echo [build] vcvars32.bat not found -- open an "x86 Native Tools Command Prompt" and run build.bat there.
    exit /b 1
)
echo [build] loading x86 toolchain: "%VCVARS32%"
call "%VCVARS32%" >nul
if /I not "%VSCMD_ARG_TGT_ARCH%"=="x86" (
    echo [build] failed to switch to the x86 toolchain
    exit /b 1
)

:build
echo [build] target arch: %VSCMD_ARG_TGT_ARCH%
cl /LD /MT znetwork_proxy.cpp /link /DEF:znetwork_proxy.def /OUT:ZNetwork.dll
if errorlevel 1 (
    echo [build] FAILED
    exit /b 1
)
for %%F in (ZNetwork.dll) do echo [build] OK: ZNetwork.dll %%~zF bytes
exit /b 0
