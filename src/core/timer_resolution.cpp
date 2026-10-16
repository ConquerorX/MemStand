#include "timer_resolution.hpp"

namespace MemStand {

bool TimerResolutionManager::Initialize() {
    auto& api = NtApi::Instance();
    if (!api.Initialize() || !api.NtQueryTimerResolution || !api.NtSetTimerResolution) {
        return false;
    }

    ULONG currentRes = 0;
    if (NT_SUCCESS(api.NtQueryTimerResolution(&m_minRes, &m_maxRes, &currentRes))) {
        m_originalRes = currentRes;
        return true;
    }
    return false;
}

bool TimerResolutionManager::QueryResolution(double& minMs, double& maxMs, double& currentMs) {
    auto& api = NtApi::Instance();
    if (!api.NtQueryTimerResolution) return false;

    ULONG minRes = 0, maxRes = 0, curRes = 0;
    NTSTATUS status = api.NtQueryTimerResolution(&minRes, &maxRes, &curRes);
    if (NT_SUCCESS(status)) {
        minMs = (double)minRes / 10000.0;
        maxMs = (double)maxRes / 10000.0;
        currentMs = (double)curRes / 10000.0;
        return true;
    }
    return false;
}

double TimerResolutionManager::GetCurrentMs() {
    double minMs = 0, maxMs = 0, curMs = 15.625;
    QueryResolution(minMs, maxMs, curMs);
    return curMs;
}

bool TimerResolutionManager::SetResolution(double targetMs) {
    auto& api = NtApi::Instance();
    if (!api.NtSetTimerResolution) return false;

    if (targetMs < 0.5) targetMs = 0.5;
    if (targetMs > 15.625) targetMs = 15.625;

    ULONG desiredUnits = (ULONG)(targetMs * 10000.0);
    ULONG currentUnits = 0;

    NTSTATUS status = api.NtSetTimerResolution(desiredUnits, TRUE, &currentUnits);
    if (NT_SUCCESS(status)) {
        m_isActive = true;
        m_targetMs = targetMs;

        if (!m_timeBeginPeriodCalled) {
            timeBeginPeriod(1);
            m_timeBeginPeriodCalled = true;
        }
        return true;
    }
    return false;
}

bool TimerResolutionManager::RestoreDefault() {
    auto& api = NtApi::Instance();
    if (!m_isActive && !m_timeBeginPeriodCalled) return true;

    if (api.NtSetTimerResolution) {
        ULONG curRes = 0;
        api.NtSetTimerResolution(m_originalRes, FALSE, &curRes);
    }

    if (m_timeBeginPeriodCalled) {
        timeEndPeriod(1);
        m_timeBeginPeriodCalled = false;
    }

    m_isActive = false;
    return true;
}

} // namespace MemStand
