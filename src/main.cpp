#include "common.hpp"
#include "resources/resource.h"
#include "core/memory_manager.hpp"
#include "core/timer_resolution.hpp"
#include "core/automation_engine.hpp"
#include "core/config_manager.hpp"
#include "i18n/localization.hpp"
#include "ui/main_window.hpp"

using namespace MemStand;

static bool IsProcessElevated() {
    HANDLE hToken = nullptr;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation{};
        DWORD dwSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            CloseHandle(hToken);
            return elevation.TokenIsElevated != 0;
        }
        CloseHandle(hToken);
    }
    return false;
}

static bool RelaunchElevated(LPCWSTR lpParameters) {
    wchar_t szPath[MAX_PATH];
    if (GetModuleFileNameW(nullptr, szPath, MAX_PATH)) {
        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        sei.lpVerb = L"runas";
        sei.lpFile = szPath;
        sei.lpParameters = lpParameters;
        sei.nShow = SW_NORMAL;
        return ShellExecuteExW(&sei) != FALSE;
    }
    return false;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Check elevation - request administrative rights so NT memory & timer APIs function properly
    bool isElevated = IsProcessElevated();
    if (!isElevated) {
        if (RelaunchElevated(pCmdLine)) {
            return 0; // successfully launched elevated process
        }
    }

    // 1. Single Instance Check
    HANDLE hMutex = CreateMutexW(nullptr, TRUE, L"Global\\MemStand_SingleInstance_Mutex");
    bool alreadyRunning = (GetLastError() == ERROR_ALREADY_EXISTS);

    // 2. Parse command line arguments
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    bool argMinimized = false;
    bool argPurge = false;
    bool argCleanAll = false;

    for (int i = 1; i < argc; ++i) {
        if (_wcsicmp(argv[i], L"-minimized") == 0) {
            argMinimized = true;
        } else if (_wcsicmp(argv[i], L"-purge") == 0) {
            argPurge = true;
        } else if (_wcsicmp(argv[i], L"-clean") == 0) {
            argCleanAll = true;
        }
    }
    LocalFree(argv);

    // If another instance is running and user requested a one-shot command
    if (alreadyRunning) {
        if (argPurge) {
            MemoryManager::Instance().Initialize();
            MemoryManager::Instance().PurgeStandbyList();
            CloseHandle(hMutex);
            return 0;
        }
        if (argCleanAll) {
            MemoryManager::Instance().Initialize();
            MemoryManager::Instance().PurgeAll();
            CloseHandle(hMutex);
            return 0;
        }

        // Otherwise bring existing instance to foreground
        HWND hWndExisting = FindWindowW(L"MemStandMainWindowClass", nullptr);
        if (hWndExisting) {
            ShowWindow(hWndExisting, SW_SHOW);
            SetForegroundWindow(hWndExisting);
        }
        CloseHandle(hMutex);
        return 0;
    }

    // 3. Initialize Common Controls
    INITCOMMONCONTROLSEX icex{};
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    // 4. Initialize Core Subsystems
    ConfigManager::Instance().Load();
    Localization::Instance().Initialize();
    Localization::Instance().LoadLanguage(ConfigManager::Instance().Get().language);

    MemoryManager::Instance().Initialize();
    TimerResolutionManager::Instance().Initialize();

    // Immediate CLI execution if requested without GUI loop
    if (argPurge) {
        MemoryManager::Instance().PurgeStandbyList();
        CloseHandle(hMutex);
        return 0;
    }
    if (argCleanAll) {
        MemoryManager::Instance().PurgeAll();
        CloseHandle(hMutex);
        return 0;
    }

    // 5. Start Background Automation Engine if configured to auto-start
    const auto& config = ConfigManager::Instance().Get();
    if (config.auto_start_monitoring) {
        AutomationEngine::Instance().Start();
        if (config.timer_resolution_enabled) {
            TimerResolutionManager::Instance().SetResolution(config.desired_timer_resolution_ms);
        }
    }

    // 6. Create UI
    if (!MainWindow::Instance().Create()) {
        AutomationEngine::Instance().Stop();
        CloseHandle(hMutex);
        return 1;
    }
    if (argMinimized || config.start_minimized) {
        MainWindow::Instance().Show(SW_HIDE);
    } else {
        MainWindow::Instance().Show(nCmdShow);
    }

    // 7. Message Loop
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    // 8. Cleanup
    AutomationEngine::Instance().Stop();
    TimerResolutionManager::Instance().RestoreDefault();

    if (hMutex) {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

    return (int)msg.wParam;
}
