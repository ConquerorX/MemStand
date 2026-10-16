#pragma once

#include "../common.hpp"

namespace MemStand {

namespace Theme {
    constexpr COLORREF BgColor       = RGB(15, 23, 42);    // #0f172a (Deep Slate 900)
    constexpr COLORREF CardBgColor   = RGB(30, 41, 59);    // #1e293b (Slate 800)
    constexpr COLORREF CardBorder    = RGB(51, 65, 85);    // #334155 (Slate 700)
    constexpr COLORREF TextPrimary   = RGB(248, 250, 252); // #f8fafc (Slate 50)
    constexpr COLORREF TextMuted     = RGB(148, 163, 184); // #94a3b8 (Slate 400 - 4.5:1+ contrast)
    constexpr COLORREF AccentBlue    = RGB(59, 130, 246);  // #3b82f6 (Blue 500)
    constexpr COLORREF AccentGreen   = RGB(16, 185, 129);  // #10b981 (Emerald 500)
    constexpr COLORREF AccentYellow  = RGB(245, 158, 11);  // #f59e0b (Amber 500)
    constexpr COLORREF AccentRed     = RGB(239, 68, 68);   // #ef4444 (Red 500)
    constexpr COLORREF InputBg       = RGB(15, 23, 42);    // #0f172a (Slate 900)
}

class DarkTheme {
public:
    static void EnableImmersiveDarkMode(HWND hwnd) {
        BOOL dark = TRUE;
        // DWMWA_USE_IMMERSIVE_DARK_MODE is 20 in Windows 10 build 18985+ / Windows 11, 19 in older
        if (FAILED(DwmSetWindowAttribute(hwnd, 20, &dark, sizeof(dark)))) {
            DwmSetWindowAttribute(hwnd, 19, &dark, sizeof(dark));
        }
    }

    static HBRUSH GetBgBrush() {
        static HBRUSH hbr = CreateSolidBrush(Theme::BgColor);
        return hbr;
    }

    static HBRUSH GetCardBrush() {
        static HBRUSH hbr = CreateSolidBrush(Theme::CardBgColor);
        return hbr;
    }

    static HBRUSH GetInputBrush() {
        static HBRUSH hbr = CreateSolidBrush(Theme::InputBg);
        return hbr;
    }

    static HFONT GetMainFont(int pointSize = 9, bool bold = false) {
        static std::map<std::pair<int, bool>, HFONT> fontCache;
        auto key = std::make_pair(pointSize, bold);
        if (fontCache.find(key) != fontCache.end()) {
            return fontCache[key];
        }

        HDC hdc = GetDC(nullptr);
        int logPixelsY = GetDeviceCaps(hdc, LOGPIXELSY);
        ReleaseDC(nullptr, hdc);

        int height = -MulDiv(pointSize, logPixelsY, 72);
        HFONT hFont = CreateFontW(
            height, 0, 0, 0,
            bold ? FW_BOLD : FW_NORMAL,
            FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            L"Segoe UI"
        );
        fontCache[key] = hFont;
        return hFont;
    }
};

} // namespace MemStand
