#pragma once

#include "../common.hpp"
#include "../version.hpp"

namespace MemStand {

struct UpdateInfo {
    bool updateAvailable = false;
    std::wstring currentVersion = MEMSTAND_VERSION_WSTRING;
    std::wstring latestVersion;
    std::wstring releaseUrl;
    std::wstring errorMessage;
};

using UpdateCallback = std::function<void(const UpdateInfo& info)>;

class UpdateChecker {
public:
    static UpdateChecker& Instance() {
        static UpdateChecker instance;
        return instance;
    }

    // Initiates an asynchronous update check in a background worker thread
    void CheckForUpdatesAsync(UpdateCallback callback);

    // Synchronous update check
    UpdateInfo CheckForUpdates();

private:
    UpdateChecker() = default;
    static int CompareVersions(const std::string& v1, const std::string& v2);
    static std::string ExtractJsonString(const std::string& json, const std::string& key);
};

} // namespace MemStand
