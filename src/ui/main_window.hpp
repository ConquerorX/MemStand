#pragma once

#include "../common.hpp"
#include "../../resources/resource.h"
#include "tray_icon.hpp"
#include "../core/memory_manager.hpp"
#include "../core/timer_resolution.hpp"
#include "../core/automation_engine.hpp"
#include "../core/config_manager.hpp"
#include "../core/update_checker.hpp"
#include "../i18n/localization.hpp"

namespace MemStand {

// Internal messages used to hand background work and its results back to the UI thread.
constexpr UINT WM_MEMSTAND_CLEAN_DONE = WM_USER + 102; // wParam = CleanupKind, lParam = bytes freed
constexpr UINT WM_MEMSTAND_AUTO_CLEAN = WM_USER + 103; // automation engine finished a background clean
constexpr UINT WM_MEMSTAND_TASK_DONE = WM_USER + 104; // lParam != 0 when schtasks.exe succeeded
constexpr UINT WM_MEMSTAND_UPDATE_READY = WM_USER + 105; // background update check finished
constexpr UINT WM_MEMSTAND_HOTKEY_CAPTURED = WM_USER + 106; // wParam = modifiers, lParam = virtual key
constexpr UINT WM_MEMSTAND_HOTKEY_RECORD_STATE = WM_USER + 107; // wParam = 1 while recording, 0 when done

enum class CleanupKind {
    PurgeStandby,
    CleanAll
};

enum class ActionButtonKind : DWORD_PTR {
    MasterStart = 1,
    MasterStop = 2,
    PurgeStandby = 3,
    CleanAll = 4,
    SaveSettings = 5,
    AutoDetect = 6,
    CheckUpdates = 7
};

struct HotkeyCombination {
    UINT modifiers = MOD_CONTROL | MOD_SHIFT;
    UINT key = 'M';
};

struct HotkeyRecorderState {
    bool hovered = false;
    bool recording = false;
    bool enabled = true;

    // Slate 700/600 body, amber while recording, slate 800 when switched off.
    void ResolveColors(COLORREF& fill, COLORREF& outline, COLORREF& text) const {
        if (!enabled)       { fill = RGB(30, 41, 59); outline = RGB(71, 85, 105); text = RGB(100, 116, 139); }
        else if (recording) { fill = RGB(51, 65, 85); outline = RGB(250, 204, 21); text = RGB(250, 204, 21); }
        else if (hovered)   { fill = RGB(71, 85, 105); outline = RGB(96, 165, 250); text = RGB(255, 255, 255); }
        else                { fill = RGB(51, 65, 85); outline = RGB(59, 130, 246); text = RGB(255, 255, 255); }
    }
};

class MainWindow {
public:
    static MainWindow& Instance() {
        static MainWindow instance;
        return instance;
    }

    bool Create();
    void Show(int cmdShow = SW_SHOW);
    HWND GetHwnd() const { return m_hwnd; }
    bool IsMonitoringActive() const { return m_isMonitoringActive; }

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    MainWindow() = default;

    LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void CreateControls(HWND hwnd);
    void UpdateMetrics();
    void RefreshLanguage();
    void ApplySettingsFromUI();
    void PopulateSettingsToUI();
    void RegisterGlobalHotkey();
    void UnregisterGlobalHotkey();
    void UpdateHotkeyUi();
    void SetHotkeyRecorderRecording(bool recording);
    void SwitchTab(int tabIndex);
    void ToggleMonitoring();
    void StartCleanupAsync(CleanupKind kind, const std::wstring& reason);
    void SyncStartupTaskAsync(bool enable, bool startMinimized);
    void AutoDetectOptimalSettings();
    void CheckForUpdates(bool silentIfLatest = false);
    void ShowUpdateResult();
    void OnCleanupFinished(const std::wstring& reason, uint64_t freedBytes);

    void DrawDashboard(HDC hdc, const RECT& rcClient);
    void DrawSettings(HDC hdc, const RECT& rcClient);

    HWND m_hwnd = nullptr;
    int m_currentTab = 0; // 0 = Dashboard, 1 = Settings

    // Navigation Tabs
    HWND m_hTabDashboard = nullptr;
    HWND m_hTabSettings = nullptr;

    // Dashboard Controls
    HWND m_hBtnStartStop = nullptr;
    HWND m_hBtnPurgeStandby = nullptr;
    HWND m_hBtnCleanAll = nullptr;

    // Settings Controls
    HWND m_hChkAutoStandby = nullptr;
    HWND m_hEditStandbyThresh = nullptr;
    HWND m_hEditFreeMemThresh = nullptr;
    HWND m_hChkAutoRam = nullptr;
    HWND m_hEditRamThresh = nullptr;
    HWND m_hEditPolling = nullptr;

    HWND m_hComboTimer = nullptr;
    HWND m_hChkAutoStartMonitoring = nullptr;
    HWND m_hChkStartWindows = nullptr;
    HWND m_hChkStartMinimized = nullptr;
    HWND m_hChkMinOnClose = nullptr;
    HWND m_hChkHotkey = nullptr;
    HWND m_hBtnHotkeyRecorder = nullptr;
    HWND m_hChkShowNotifications = nullptr;
    HWND m_hComboLanguage = nullptr;
    HWND m_hBtnSaveSettings = nullptr;
    HWND m_hBtnAutoDetect = nullptr;
    HWND m_hBtnCheckUpdates = nullptr;

    TrayIcon m_trayIcon;
    MemorySnapshot m_lastSnapshot{};
    std::wstring m_statusText;
    UINT_PTR m_timerId = 1;
    bool m_hotkeyRegistered = false;
    bool m_isMonitoringActive = false;

    // Combination captured in the recorder, committed to AppConfig only on "Save Settings".
    HotkeyCombination m_pendingHotkey{ MOD_CONTROL | MOD_SHIFT, 'M' };
    bool m_hotkeyRecording = false;
    HotkeyRecorderState m_hotkeyRecorderState{};

    // Memory cleaning is a synchronous, system-wide kernel operation (MemoryEmptyWorkingSets
    // walks every process working set) and can take several seconds. It therefore never runs on
    // the UI thread: the work happens on m_cleanupThread and reports back through
    // WM_MEMSTAND_CLEAN_DONE, so the window keeps pumping messages and stays responsive.
    std::thread m_cleanupThread;
    std::atomic<bool> m_cleanupRunning{ false };
    std::wstring m_pendingReason; // status prefix of the in-flight manual clean (UI thread only)

    // Payload published by the automation engine worker thread, consumed on the UI thread.
    std::mutex m_pendingMutex;
    std::wstring m_pendingAutoReason;
    uint64_t m_pendingAutoFreed = 0;
    UpdateInfo m_pendingUpdateInfo{};
    bool m_pendingUpdateSilent = false;

    // schtasks.exe is spawned and waited on (up to 5s), so startup task updates run here too.
    std::thread m_taskSyncThread;
    std::atomic<bool> m_taskSyncRunning{ false };
};

} // namespace MemStand
