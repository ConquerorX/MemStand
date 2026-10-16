#pragma once

#include "../common.hpp"

namespace MemStand {

typedef enum _SYSTEM_MEMORY_LIST_COMMAND {
    MemoryCaptureAccessedBits,
    MemoryCaptureAndResetAccessedBits,
    MemoryEmptyWorkingSets,
    MemoryFlushModifiedList,
    MemoryPurgeStandbyList,
    MemoryPurgeLowPriorityStandbyList,
    MemoryCommandMax
} SYSTEM_MEMORY_LIST_COMMAND;

typedef struct _SYSTEM_MEMORY_LIST_INFORMATION {
    ULONG_PTR ZeroPageCount;
    ULONG_PTR FreePageCount;
    ULONG_PTR ModifiedPageCount;
    ULONG_PTR ModifiedNoWritePageCount;
    ULONG_PTR BadPageCount;
    ULONG_PTR PageCountByPriority[8];
    ULONG_PTR RepurposedPagesByPriority[8];
    ULONG_PTR ModifiedPageCountPageFile;
} SYSTEM_MEMORY_LIST_INFORMATION;

typedef struct _SYSTEM_FILECACHE_INFORMATION {
    SIZE_T CurrentSize;
    SIZE_T PeakSize;
    ULONG PageFaultCount;
    SIZE_T MinimumWorkingSet;
    SIZE_T MaximumWorkingSet;
    SIZE_T CurrentSizeIncludingTransitionInPages;
    SIZE_T PeakSizeIncludingTransitionInPages;
    ULONG TransitionRePurposeCount;
    ULONG Flags;
} SYSTEM_FILECACHE_INFORMATION;

typedef struct _MEMORY_COMBINE_INFORMATION_EX {
    HANDLE Handle;
    ULONG_PTR PagesCombined;
    ULONG Flags;
} MEMORY_COMBINE_INFORMATION_EX;

// NT Information Classes
constexpr ULONG SystemFileCacheInformation = 21;
constexpr ULONG SystemMemoryListInformation = 80;
constexpr ULONG SystemCombinePhysicalMemoryInformation = 130;

using pfnNtSetSystemInformation = NTSTATUS(NTAPI*)(ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength);
using pfnNtQuerySystemInformation = NTSTATUS(NTAPI*)(ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
using pfnNtSetTimerResolution = NTSTATUS(NTAPI*)(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);
using pfnNtQueryTimerResolution = NTSTATUS(NTAPI*)(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);

class NtApi {
public:
    static NtApi& Instance() {
        static NtApi instance;
        return instance;
    }

    bool Initialize() {
        if (m_initialized) return true;

        HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
        if (!hNtdll) {
            hNtdll = LoadLibraryW(L"ntdll.dll");
        }
        if (!hNtdll) return false;

        NtSetSystemInformation = (pfnNtSetSystemInformation)GetProcAddress(hNtdll, "NtSetSystemInformation");
        NtQuerySystemInformation = (pfnNtQuerySystemInformation)GetProcAddress(hNtdll, "NtQuerySystemInformation");
        NtSetTimerResolution = (pfnNtSetTimerResolution)GetProcAddress(hNtdll, "NtSetTimerResolution");
        NtQueryTimerResolution = (pfnNtQueryTimerResolution)GetProcAddress(hNtdll, "NtQueryTimerResolution");

        EnablePrivileges();

        m_initialized = (NtSetSystemInformation != nullptr && NtQuerySystemInformation != nullptr);
        return m_initialized;
    }

    bool EnablePrivileges() {
        HANDLE hToken = nullptr;
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
            return false;
        }

        auto SetPrivilege = [&](LPCWSTR privName) {
            TOKEN_PRIVILEGES tp = {0};
            LUID luid;
            if (LookupPrivilegeValueW(nullptr, privName, &luid)) {
                tp.PrivilegeCount = 1;
                tp.Privileges[0].Luid = luid;
                tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr);
            }
        };

        SetPrivilege(L"SeIncreaseQuotaPrivilege");
        SetPrivilege(L"SeProfileSingleProcessPrivilege");
        SetPrivilege(L"SeDebugPrivilege");

        CloseHandle(hToken);
        return true;
    }

    pfnNtSetSystemInformation NtSetSystemInformation = nullptr;
    pfnNtQuerySystemInformation NtQuerySystemInformation = nullptr;
    pfnNtSetTimerResolution NtSetTimerResolution = nullptr;
    pfnNtQueryTimerResolution NtQueryTimerResolution = nullptr;

private:
    NtApi() = default;
    bool m_initialized = false;
};

} // namespace MemStand
