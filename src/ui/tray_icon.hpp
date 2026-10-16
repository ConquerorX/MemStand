#pragma once

#include "../common.hpp"
#include "../../resources/resource.h"

namespace MemStand {

constexpr UINT WM_MEMSTAND_TRAY = WM_USER + 101;

class TrayIcon {
public:
    TrayIcon() = default;
    ~TrayIcon() { Remove(); }

    bool Initialize(HWND hwnd);
    void Update(int memoryUsagePercent, const std::wstring& tooltip);
    void ShowBalloon(const std::wstring& title, const std::wstring& message);
    void ShowContextMenu(HWND hwnd);
    void Remove();

private:
    HICON CreateDynamicIcon(int percent);

    HWND m_hwnd = nullptr;
    NOTIFYICONDATAW m_nid{};
    HICON m_currentIcon = nullptr;
    int m_lastPercent = -1;
    std::wstring m_lastTooltip;
    bool m_added = false;
};

} // namespace MemStand
