#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <dwmapi.h>
#include <shlwapi.h>
#include <psapi.h>
#include <timeapi.h>

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <algorithm>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")

#ifndef NTSTATUS
typedef LONG NTSTATUS;
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

namespace MemStand {

inline std::wstring Utf8ToWide(const std::string& str) {
    if (str.empty()) return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &result[0], size);
    return result;
}

inline std::string WideToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &result[0], size, nullptr, nullptr);
    return result;
}

inline std::wstring FormatMemoryBytes(uint64_t bytes) {
    constexpr double KB = 1024.0;
    constexpr double MB = KB * 1024.0;
    constexpr double GB = MB * 1024.0;

    wchar_t buf[64];
    if (bytes >= (uint64_t)GB) {
        swprintf_s(buf, L"%.2f GB", (double)bytes / GB);
    } else if (bytes >= (uint64_t)MB) {
        swprintf_s(buf, L"%.1f MB", (double)bytes / MB);
    } else if (bytes >= (uint64_t)KB) {
        swprintf_s(buf, L"%.1f KB", (double)bytes / KB);
    } else {
        swprintf_s(buf, L"%llu B", bytes);
    }
    return buf;
}

inline std::wstring GetExecutablePath() {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    return path;
}

inline std::wstring GetExecutableDir() {
    std::wstring path = GetExecutablePath();
    size_t pos = path.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        return path.substr(0, pos);
    }
    return L".";
}

} // namespace MemStand
