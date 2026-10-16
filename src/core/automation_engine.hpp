#pragma once

#include "memory_manager.hpp"
#include "config_manager.hpp"

namespace MemStand {

using CleanCallback = std::function<void(const std::wstring& reason, uint64_t freedBytes)>;

class AutomationEngine {
public:
    static AutomationEngine& Instance() {
        static AutomationEngine instance;
        return instance;
    }

    void Start();
    void Stop();
    bool IsRunning() const { return m_running; }

    void SetCallback(CleanCallback cb) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_callback = cb;
    }

private:
    AutomationEngine() = default;
    void WorkerLoop();

    std::atomic<bool> m_running{ false };
    std::thread m_workerThread;
    std::mutex m_mutex;
    CleanCallback m_callback;

    std::chrono::steady_clock::time_point m_lastStandbyClean{};
    std::chrono::steady_clock::time_point m_lastHighRamClean{};
};

} // namespace MemStand
