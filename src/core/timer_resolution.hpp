#pragma once

#include "ntdll_api.hpp"

namespace MemStand {

class TimerResolutionManager {
public:
    static TimerResolutionManager& Instance() {
        static TimerResolutionManager instance;
        return instance;
    }

    ~TimerResolutionManager() {
        RestoreDefault();
    }

    bool Initialize();

    bool QueryResolution(double& minMs, double& maxMs, double& currentMs);
    bool SetResolution(double targetMs);
    bool RestoreDefault();

    bool IsActive() const { return m_isActive; }
    double GetTargetMs() const { return m_targetMs; }
    double GetCurrentMs();

private:
    TimerResolutionManager() = default;

    bool m_isActive = false;
    double m_targetMs = 0.50;
    ULONG m_originalRes = 156250; // default ~15.625ms in 100ns units
    ULONG m_minRes = 5000;       // ~0.5ms
    ULONG m_maxRes = 156250;
    bool m_timeBeginPeriodCalled = false;
};

} // namespace MemStand
