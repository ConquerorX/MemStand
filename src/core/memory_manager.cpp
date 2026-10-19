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

void MemoryManager::DoPurgeStandbyList() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return;

    SYSTEM_MEMORY_LIST_COMMAND cmd = MemoryPurgeStandbyList;
    api.NtSetSystemInformation(SystemMemoryListInformation, &cmd, sizeof(cmd));

    cmd = MemoryPurgeLowPriorityStandbyList;
    api.NtSetSystemInformation(SystemMemoryListInformation, &cmd, sizeof(cmd));
}

void MemoryManager::DoEmptyWorkingSets() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return;

    SYSTEM_MEMORY_LIST_COMMAND cmd = MemoryEmptyWorkingSets;
    api.NtSetSystemInformation(SystemMemoryListInformation, &cmd, sizeof(cmd));
}

void MemoryManager::DoFlushSystemFileCache() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return;

    SYSTEM_FILECACHE_INFORMATION sfci{};
    sfci.MinimumWorkingSet = (SIZE_T)-1;
    sfci.MaximumWorkingSet = (SIZE_T)-1;
    api.NtSetSystemInformation(SystemFileCacheInformation, &sfci, sizeof(sfci));
}

void MemoryManager::DoFlushModifiedList() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return;

    SYSTEM_MEMORY_LIST_COMMAND cmd = MemoryFlushModifiedList;
    api.NtSetSystemInformation(SystemMemoryListInformation, &cmd, sizeof(cmd));
}

void MemoryManager::DoCombineMemory() {
    auto& api = NtApi::Instance();
    if (!api.NtSetSystemInformation) return;

    MEMORY_COMBINE_INFORMATION_EX combine{};
    api.NtSetSystemInformation(SystemCombinePhysicalMemoryInformation, &combine, sizeof(combine));
}

uint64_t MemoryManager::PurgeStandbyList() {
    if (!NtApi::Instance().NtSetSystemInformation) return 0;

    auto before = GetSnapshot();
    DoPurgeStandbyList();
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
    if (!NtApi::Instance().NtSetSystemInformation) return 0;

    auto before = GetSnapshot();
    DoEmptyWorkingSets();
    auto after = GetSnapshot();

    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

uint64_t MemoryManager::FlushSystemFileCache() {
    if (!NtApi::Instance().NtSetSystemInformation) return 0;

    auto before = GetSnapshot();
    DoFlushSystemFileCache();
    auto after = GetSnapshot();

    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

uint64_t MemoryManager::FlushModifiedList() {
    if (!NtApi::Instance().NtSetSystemInformation) return 0;

    auto before = GetSnapshot();
    DoFlushModifiedList();
    auto after = GetSnapshot();

    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

uint64_t MemoryManager::CombineMemory() {
    if (!NtApi::Instance().NtSetSystemInformation) return 0;

    auto before = GetSnapshot();
    DoCombineMemory();
    auto after = GetSnapshot();

    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

uint64_t MemoryManager::PurgeAll() {
    if (!NtApi::Instance().NtSetSystemInformation) return 0;

    // A single snapshot pair wraps the entire sequence, eliminating 10 redundant snapshot passes
    auto before = GetSnapshot();

    DoEmptyWorkingSets();
    DoFlushSystemFileCache();
    DoFlushModifiedList();
    DoPurgeStandbyList();
    DoCombineMemory();

    auto after = GetSnapshot();
    return (before.used_ram_bytes > after.used_ram_bytes) ? (before.used_ram_bytes - after.used_ram_bytes) : 0;
}

} // namespace MemStand
