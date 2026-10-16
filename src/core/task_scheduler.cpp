#include "task_scheduler.hpp"

namespace MemStand {

bool TaskSchedulerHelper::ExecuteCommand(const std::wstring& cmd, int& exitCode) {
    STARTUPINFOW si{ sizeof(STARTUPINFOW) };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi{};
    std::wstring cmdMutable = cmd;

    if (!CreateProcessW(nullptr, &cmdMutable[0], nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        return false;
    }

    WaitForSingleObject(pi.hProcess, 5000);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    exitCode = (int)code;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return (exitCode == 0);
}

bool TaskSchedulerHelper::RegisterStartupTask(bool startMinimized) {
    std::wstring exePath = GetExecutablePath();
    std::wstring trArg = L"\\\"" + exePath + L"\\\"";
    if (startMinimized) {
        trArg += L" -minimized";
    }

    std::wstring cmd = L"schtasks.exe /Create /TN \"MemStand\" /TR \"" + trArg + L"\" /SC ONLOGON /RL HIGHEST /F";
    int exitCode = -1;
    return ExecuteCommand(cmd, exitCode);
}

bool TaskSchedulerHelper::UnregisterStartupTask() {
    std::wstring cmd = L"schtasks.exe /Delete /TN \"MemStand\" /F";
    int exitCode = -1;
    return ExecuteCommand(cmd, exitCode);
}

bool TaskSchedulerHelper::IsStartupTaskRegistered() {
    std::wstring cmd = L"schtasks.exe /Query /TN \"MemStand\"";
    int exitCode = -1;
    return ExecuteCommand(cmd, exitCode);
}

} // namespace MemStand
