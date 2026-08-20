// GamePatcher.cpp - Упрощенная версия (только запуск)
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstdio>
#include <string>
#include <vector>

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
};

static Config g_config;

int main() {
    printf("[Patcher] Starting FEAR Online...\n");
    
    std::string cmdLine = "\"" + g_config.enginePath + "\"";
    for (const auto& arg : g_config.arguments) {
        cmdLine += " " + arg;
    }
    
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    
    if (!CreateProcessA(
        g_config.enginePath.c_str(),
        (LPSTR)cmdLine.c_str(),
        NULL, NULL, FALSE, 0, NULL,
        g_config.workingDirectory.c_str(),
        &si, &pi
    )) {
        printf("[Patcher] Failed to start process! Error: %d\n", GetLastError());
        getchar();
        return 1;
    }
    
    printf("[Patcher] Process started (PID: %u)\n", pi.dwProcessId);
    printf("[Patcher] ZNetwork.dll will handle all patches internally.\n");
    printf("[Patcher] Close this console to exit.\n");
    
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    
    return 0;
}
