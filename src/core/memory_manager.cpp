#include "memory_manager.hpp"

namespace MemStand {

bool MemoryManager::Initialize() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    m_pageSize = si.dwPageSize ? si.dwPageSize : 4096;

    return NtApi::Instance().Initialize();
}

MemorySnapshot MemoryManager::GetSnapshot() {
    MemorySnapshot snapshot{};

    MEMORYSTATUSEX ms{ sizeof(MEMORYSTATUSEX) };
    if (GlobalMemoryStatusEx(&ms)) {
        snapshot.total_ram_bytes = ms.ullTotalPhys;
        snapshot.free_ram_bytes = ms.ullAvailPhys;
        snapshot.used_ram_bytes = ms.ullTotalPhys - ms.ullAvailPhys;
        snapshot.usage_percentage = (int)ms.dwMemoryLoad;
    }

    auto& api = NtApi::Instance();
    if (api.NtQuerySystemInformation) {
        SYSTEM_MEMORY_LIST_INFORMATION memList{};
        ULONG returnLength = 0;
        NTSTATUS status = api.NtQuerySystemInformation(SystemMemoryListInformation, &memList, sizeof(memList), &returnLength);
        if (NT_SUCCESS(status)) {
            uint64_t standbyPages = 0;
            for (int i = 0; i < 8; ++i) {
                standbyPages += memList.PageCountByPriority[i];
            }
            snapshot.standby_bytes = standbyPages * m_pageSize;
            snapshot.modified_bytes = memList.ModifiedPageCount * m_pageSize;
        }
    }

    PERFORMANCE_INFORMATION pi{ sizeof(PERFORMANCE_INFORMATION) };
    if (GetPerformanceInfo(&pi, sizeof(pi))) {
        snapshot.file_cache_bytes = (uint64_t)pi.SystemCache * pi.PageSize;
    }

    return snapshot;
}

uint64_t MemoryManager::PurgeStandbyList() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return 0;

    auto before = GetSnapshot();

    SYSTEM_MEMORY_LIST_COMMAND cmd = MemoryPurgeStandbyList;
    api.NtSetSystemInformation(SystemMemoryListInformation, &cmd, sizeof(cmd));

    cmd = MemoryPurgeLowPriorityStandbyList;
    api.NtSetSystemInformation(SystemMemoryListInformation, &cmd, sizeof(cmd));

    auto after = GetSnapshot();
    if (before.used_ram_bytes > after.used_ram_bytes) {
        return before.used_ram_bytes - after.used_ram_bytes;
    }
    if (before.standby_bytes > after.standby_bytes) {
        return before.standby_bytes - after.standby_bytes;
    }
    return 0;
}

uint64_t MemoryManager::EmptyWorkingSets() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return 0;

    auto before = GetSnapshot();

    SYSTEM_MEMORY_LIST_COMMAND cmd = MemoryEmptyWorkingSets;
    api.NtSetSystemInformation(SystemMemoryListInformation, &cmd, sizeof(cmd));

    auto after = GetSnapshot();
    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

uint64_t MemoryManager::FlushSystemFileCache() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return 0;

    auto before = GetSnapshot();

    SYSTEM_FILECACHE_INFORMATION sfci{};
    sfci.MinimumWorkingSet = (SIZE_T)-1;
    sfci.MaximumWorkingSet = (SIZE_T)-1;
    api.NtSetSystemInformation(SystemFileCacheInformation, &sfci, sizeof(sfci));

    auto after = GetSnapshot();
    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

uint64_t MemoryManager::FlushModifiedList() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return 0;

    auto before = GetSnapshot();

    SYSTEM_MEMORY_LIST_COMMAND cmd = MemoryFlushModifiedList;
    api.NtSetSystemInformation(SystemMemoryListInformation, &cmd, sizeof(cmd));

    auto after = GetSnapshot();
    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

uint64_t MemoryManager::CombineMemory() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return 0;

    auto before = GetSnapshot();

    MEMORY_COMBINE_INFORMATION_EX combine{};
    api.NtSetSystemInformation(SystemCombinePhysicalMemoryInformation, &combine, sizeof(combine));

    auto after = GetSnapshot();
    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

uint64_t MemoryManager::PurgeAll() {
    auto before = GetSnapshot();

    EmptyWorkingSets();
    FlushSystemFileCache();
    FlushModifiedList();
    PurgeStandbyList();
    CombineMemory();

    auto after = GetSnapshot();
    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

} // namespace MemStand
