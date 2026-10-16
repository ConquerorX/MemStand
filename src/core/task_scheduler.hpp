#pragma once

#include "../common.hpp"

namespace MemStand {

class TaskSchedulerHelper {
public:
    static bool RegisterStartupTask(bool startMinimized = true);
    static bool UnregisterStartupTask();
    static bool IsStartupTaskRegistered();

private:
    static bool ExecuteCommand(const std::wstring& cmd, int& exitCode);
};

} // namespace MemStand
