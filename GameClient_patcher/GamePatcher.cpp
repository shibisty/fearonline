// GamePatcher.cpp - Исправленная версия
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")  // <-- ДОБАВЛЕНО!

// ============ Configuration ============
struct Config {
    std::string enginePath = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\FEAR Online\\FEAR_Online\\Engine.exe";
    std::string workingDirectory = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\FEAR Online\\FEAR_Online";
    std::vector<std::string> arguments = {
        "+PB", "IN",
        "+UIPB", "none",
        "+BANNER", "0",
        "+UID", "test123",
        "+LauncherID", "1",
        "+windowed", "1",
        "+Lan", "en",
        "+LoginServerIP", "127.0.0.1",
        "+LoginServerPort", "30003"
    };
    uint32_t bad_address = 0x00340033;
    uint32_t good_address = 0x169B0000;
    const char* target_dll = "GameClient.dll";
    int max_wait_seconds = 120;
    int init_wait_ms = 8000;
};

static Config g_config;

// ============ Logging ============
void Log(const char* fmt, ...) {
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    printf("[Patcher] %s\n", buf);
    OutputDebugStringA("[Patcher] ");
    OutputDebugStringA(buf);
    OutputDebugStringA("\n");
}

// ============ Process Functions ============
DWORD GetProcessIdByName(const char* processName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;
    
    DWORD pid = 0;
    if (Process32First(snapshot, &entry)) {
        do {
            if (_stricmp(entry.szExeFile, processName) == 0) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return pid;
}

bool IsProcessRunning(DWORD pid) {
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!process) return false;
    DWORD exitCode;
    bool running = GetExitCodeProcess(process, &exitCode) && exitCode == STILL_ACTIVE;
    CloseHandle(process);
    return running;
}

// ============ Module Functions ============
HMODULE FindRemoteModule(HANDLE hProcess, const char* moduleName) {
    HMODULE hMods[1024];
    DWORD cbNeeded;
    if (!EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) return nullptr;
    
    DWORD count = cbNeeded / sizeof(HMODULE);
    char modName[MAX_PATH];
    for (DWORD i = 0; i < count; i++) {
        if (GetModuleFileNameExA(hProcess, hMods[i], modName, MAX_PATH)) {
            char* fileName = strrchr(modName, '\\');
            if (fileName) fileName++;
            else fileName = modName;
            if (_stricmp(fileName, moduleName) == 0) {
                return hMods[i];
            }
        }
    }
    return nullptr;
}

DWORD GetModuleSize(HANDLE hProcess, HMODULE module) {
    if (!module) return 0;
    IMAGE_DOS_HEADER dos;
    IMAGE_NT_HEADERS nt;
    if (!ReadProcessMemory(hProcess, module, &dos, sizeof(dos), NULL)) return 0;
    if (dos.e_magic != IMAGE_DOS_SIGNATURE) return 0;
    if (!ReadProcessMemory(hProcess, (BYTE*)module + dos.e_lfanew, &nt, sizeof(nt), NULL)) return 0;
    if (nt.Signature != IMAGE_NT_SIGNATURE) return 0;
    return nt.OptionalHeader.SizeOfImage;
}

// ============ Memory Operations ============
bool WriteRemoteMemoryEx(HANDLE hProcess, void* address, const void* data, size_t size) {
    if (!hProcess || !address || !data || size == 0) return false;
    
    DWORD protections[] = {
        PAGE_EXECUTE_READWRITE,
        PAGE_READWRITE,
        PAGE_EXECUTE_WRITECOPY,
        PAGE_WRITECOPY
    };
    
    for (int i = 0; i < sizeof(protections)/sizeof(DWORD); i++) {
        DWORD oldProtect;
        if (VirtualProtectEx(hProcess, address, size, protections[i], &oldProtect)) {
            SIZE_T bytesWritten;
            if (WriteProcessMemory(hProcess, address, data, size, &bytesWritten) && bytesWritten == size) {
                VirtualProtectEx(hProcess, address, size, oldProtect, &oldProtect);
                FlushInstructionCache(hProcess, address, size);
                return true;
            }
            VirtualProtectEx(hProcess, address, size, oldProtect, &oldProtect);
        }
    }
    
    // Try with SEH
    __try {
        DWORD oldProtect;
        if (VirtualProtectEx(hProcess, address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            SIZE_T bytesWritten;
            if (WriteProcessMemory(hProcess, address, data, size, &bytesWritten) && bytesWritten == size) {
                VirtualProtectEx(hProcess, address, size, oldProtect, &oldProtect);
                FlushInstructionCache(hProcess, address, size);
                return true;
            }
            VirtualProtectEx(hProcess, address, size, oldProtect, &oldProtect);
        }
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        Log("Exception in WriteMemory at 0x%p", address);
    }
    
    return false;
}

// ============ Global Variables ============
struct GlobalVar {
    uint32_t offset;
    const char* name;
    size_t size;
    std::vector<uint8_t> value;
};

std::map<std::string, GlobalVar> g_globalVars = {
    {"g_bMonolithMultiplayerFinalBuild", {0x248a4, "FinalBuild", 1, {0x01}}},
    {"g_bMonolithMultiplayerIsRanked", {0x23ffc, "IsRanked", 1, {0x01}}},
    {"g_nMonolithMenuNumUsers", {0x2264c, "NumUsers", 4, {0x01, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMenuUserMyIndex", {0x227cc, "MyIndex", 4, {0x00, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMenuPing2", {0x22824, "Ping", 4, {0x32, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerMyServerState", {0x2ba38, "ServerState", 4, {0x02, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerMyLevel", {0x258f8, "Level", 4, {0x64, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerMyUserGrade", {0x25928, "Grade", 4, {0x0A, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerNumPlayers", {0x241c0, "NumPlayers", 4, {0x01, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerNumTeams", {0x246cc, "NumTeams", 4, {0x02, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerHangarMyPoint", {0x2a57c, "Points", 4, {0x00, 0xE1, 0xF5, 0x05}}},
    {"g_nMonolithMultiplayerHangarMyCash", {0x2a5b0, "Cash", 4, {0x00, 0xE1, 0xF5, 0x05}}},
    {"g_nMonolithSystemLayerPlatform", {0x3892c, "Platform", 4, {0x02, 0x00, 0x00, 0x00}}},
    {"g_nMonolithGlobalLanguage", {0x2108c, "Language", 4, {0x01, 0x00, 0x00, 0x00}}},
    {"g_bMonolithMultiplayerClearTutorial", {0x2ba6c, "Tutorial", 1, {0x01}}},
    {"g_bMonolithMultiplayerClearTutorialNow", {0x2baa0, "TutorialNow", 1, {0x01}}},
    {"g_bMonolithGlobalIsCollectorsEdition", {0x21050, "Collectors", 1, {0x01}}},
    {"g_nMonolithMultiplayerLoginWin_PVP", {0x24f4c, "LoginWinPVP", 4, {0x01, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerLoginDraw_PVP", {0x24f80, "LoginDrawPVP", 4, {0x00, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerLoginLose_PVP", {0x24fb4, "LoginLosePVP", 4, {0x00, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerLoginWin_PVE", {0x24fe8, "LoginWinPVE", 4, {0x01, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerLoginLose_PVE", {0x2501c, "LoginLosePVE", 4, {0x00, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMenuUserHostIndex", {0x227f8, "HostIndex", 4, {0x00, 0x00, 0x00, 0x00}}},
    {"g_nMonolithMultiplayerCurRound", {0x24190, "CurRound", 4, {0x00, 0x00, 0x00, 0x00}}},
    {"g_bMonolithMultiplayerLanOnly", {0x2402c, "LanOnly", 1, {0x01}}},
    {"g_bMonolithMultiplayerIsAutoMove", {0x2bd84, "AutoMove", 1, {0x01}}},
    {"g_nMonolithMultiplayerAutoMoveLobbyIdx", {0x2bdb8, "AutoLobby", 4, {0x00, 0x00, 0x00, 0x00}}},
};

// ============ Apply Patches ============
void ApplyPatches(HANDLE hProcess, uint8_t* moduleStart, DWORD moduleSize) {
    Log("========================================");
    Log("Applying patches...");
    Log("========================================");
    
    // Try to enable debug privilege
    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        TOKEN_PRIVILEGES tp;
        LUID luid;
        if (LookupPrivilegeValueA(NULL, "SeDebugPrivilege", &luid)) {
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Luid = luid;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
            Log("SeDebugPrivilege enabled");
        }
        CloseHandle(hToken);
    }
    
    // Initialize global variables
    Log("Initializing global variables...");
    int varCount = 0;
    for (auto& pair : g_globalVars) {
        GlobalVar& var = pair.second;
        void* address = moduleStart + var.offset;
        
        if ((uintptr_t)address >= (uintptr_t)moduleStart && 
            (uintptr_t)address < (uintptr_t)moduleStart + moduleSize) {
            if (WriteRemoteMemoryEx(hProcess, address, var.value.data(), var.value.size())) {
                Log("  [+] %s at 0x%p", var.name, address);
                varCount++;
            }
        }
    }
    Log("Initialized %d variables", varCount);
    
    // Try NtWriteVirtualMemory if normal write failed
    if (varCount == 0) {
        Log("Trying alternative write method (NtWriteVirtualMemory)...");
        HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
        if (hNtdll) {
            typedef NTSTATUS (NTAPI *NtWriteVirtualMemory_t)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
            auto NtWriteVirtualMemory = (NtWriteVirtualMemory_t)GetProcAddress(hNtdll, "NtWriteVirtualMemory");
            
            if (NtWriteVirtualMemory) {
                for (auto& pair : g_globalVars) {
                    GlobalVar& var = pair.second;
                    void* address = moduleStart + var.offset;
                    SIZE_T bytesWritten;
                    NTSTATUS status = NtWriteVirtualMemory(hProcess, address, (PVOID)var.value.data(), var.value.size(), &bytesWritten);
                    if (status == 0 && bytesWritten == var.value.size()) {
                        Log("  [+] NtWrite: %s at 0x%p", var.name, address);
                        varCount++;
                    }
                }
                Log("NtWrite initialized %d variables", varCount);
            }
        }
    }
    
    Log("========================================");
    Log("Patch complete!");
    Log("========================================");
}

// ============ Check Admin Rights ============
bool IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin != FALSE;
}

// ============ Main ============
int main() {
    SetConsoleTitleA("Game Patcher");
    
    bool isAdmin = IsRunningAsAdmin();
    
    Log("========================================");
    Log("Game Patcher v2.0 - Standalone EXE");
    Log("========================================");
    Log("Admin rights: %s", isAdmin ? "YES" : "NO (Run as Admin!)");
    Log("Target: %s", g_config.enginePath.c_str());
    Log("Working Dir: %s", g_config.workingDirectory.c_str());
    
    if (!isAdmin) {
        Log("========================================");
        Log("WARNING: Not running as Administrator!");
        Log("Memory writes may fail!");
        Log("========================================");
    }
    
    // Check if engine exists
    if (GetFileAttributesA(g_config.enginePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        Log("ERROR: Engine.exe not found!");
        Log("Press any key to exit...");
        getchar();
        return 1;
    }
    
    // Build command line
    std::string cmdLine = "\"" + g_config.enginePath + "\"";
    for (const auto& arg : g_config.arguments) {
        cmdLine += " " + arg;
    }
    Log("Command: %s", cmdLine.c_str());
    
    // Start process
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    
    if (!CreateProcessA(
        g_config.enginePath.c_str(),
        (LPSTR)cmdLine.c_str(),
        NULL,
        NULL,
        FALSE,
        CREATE_SUSPENDED,
        NULL,
        g_config.workingDirectory.c_str(),
        &si,
        &pi
    )) {
        Log("Failed to start process! Error: %d", GetLastError());
        getchar();
        return 1;
    }
    
    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);
    
    Log("Process started (PID: %u)", pi.dwProcessId);
    Log("Waiting for GameClient.dll to load...");
    
    // Wait for DLL
    HMODULE module = nullptr;
    for (int i = 0; i < g_config.max_wait_seconds; i++) {
        if (!IsProcessRunning(pi.dwProcessId)) {
            Log("Process terminated!");
            CloseHandle(pi.hProcess);
            getchar();
            return 1;
        }
        
        module = FindRemoteModule(pi.hProcess, g_config.target_dll);
        if (module) {
            Log("%s loaded at 0x%p (after %d seconds)", g_config.target_dll, module, i + 1);
            break;
        }
        Sleep(1000);
    }
    
    if (!module) {
        Log("DLL not found after %d seconds!", g_config.max_wait_seconds);
        CloseHandle(pi.hProcess);
        getchar();
        return 1;
    }
    
    DWORD moduleSize = GetModuleSize(pi.hProcess, module);
    Log("Module size: 0x%X bytes", moduleSize);
    
    // Apply patches
    ApplyPatches(pi.hProcess, (uint8_t*)module, moduleSize);
    
    Log("========================================");
    Log("Patching complete!");
    Log("Close this console to exit.");
    Log("========================================");
    
    WaitForSingleObject(pi.hProcess, INFINITE);
    Log("Game exited.");
    
    CloseHandle(pi.hProcess);
    
    Log("Press any key to exit...");
    getchar();
    
    return 0;
}
