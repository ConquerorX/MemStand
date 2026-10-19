#pragma once

#include "ntdll_api.hpp"

namespace MemStand {

struct MemorySnapshot {
    uint64_t total_ram_bytes = 0;
    uint64_t used_ram_bytes = 0;
    uint64_t free_ram_bytes = 0;
    uint64_t standby_bytes = 0;
    uint64_t modified_bytes = 0;
    uint64_t file_cache_bytes = 0;
    int usage_percentage = 0;
};

class MemoryManager {
public:
    static MemoryManager& Instance() {
        static MemoryManager instance;
        return instance;
    }

    bool Initialize();

    MemorySnapshot GetSnapshot();

    // Cleaning actions: return bytes freed
    uint64_t PurgeStandbyList();
    uint64_t EmptyWorkingSets();
    uint64_t FlushSystemFileCache();
    uint64_t FlushModifiedList();
    uint64_t CombineMemory();
    uint64_t PurgeAll();

private:
    MemoryManager() = default;
    size_t m_pageSize = 4096;

    void DoPurgeStandbyList();
    void DoEmptyWorkingSets();
    void DoFlushSystemFileCache();
    void DoFlushModifiedList();
    void DoCombineMemory();
};

} // namespace MemStand
