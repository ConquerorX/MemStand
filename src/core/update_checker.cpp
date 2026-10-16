#include "update_checker.hpp"
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

namespace MemStand {

std::string UpdateChecker::ExtractJsonString(const std::string& json, const std::string& key) {
    std::string needle = "\"" + key + "\"";
    size_t pos = json.find(needle);
    if (pos == std::string::npos) return "";

    pos = json.find(':', pos + needle.length());
    if (pos == std::string::npos) return "";

    size_t valStart = json.find('\"', pos);
    if (valStart == std::string::npos) return "";

    size_t valEnd = json.find('\"', valStart + 1);
    if (valEnd == std::string::npos) return "";

    return json.substr(valStart + 1, valEnd - valStart - 1);
}

int UpdateChecker::CompareVersions(const std::string& v1, const std::string& v2) {
    auto parseVersion = [](const std::string& v) -> std::vector<int> {
        std::vector<int> parts;
        std::string s = v;
        if (!s.empty() && (s[0] == 'v' || s[0] == 'V')) {
            s = s.substr(1);
        }
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, '.')) {
            try {
                parts.push_back(std::stoi(item));
            } catch (...) {
                parts.push_back(0);
            }
        }
        while (parts.size() < 3) parts.push_back(0);
        return parts;
    };

    auto p1 = parseVersion(v1);
    auto p2 = parseVersion(v2);

    for (size_t i = 0; i < 3; ++i) {
        if (p1[i] < p2[i]) return -1;
        if (p1[i] > p2[i]) return 1;
    }
    return 0;
}

void UpdateChecker::CheckForUpdatesAsync(UpdateCallback callback) {
    std::thread([this, callback]() {
        UpdateInfo info = CheckForUpdates();
        if (callback) {
            callback(info);
        }
    }).detach();
}

UpdateInfo UpdateChecker::CheckForUpdates() {
    UpdateInfo result{};
    result.currentVersion = MEMSTAND_VERSION_WSTRING;
    result.releaseUrl = MEMSTAND_RELEASES_URL;

    HINTERNET hSession = WinHttpOpen(
        L"MemStand-UpdateChecker/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    );

    if (!hSession) {
        result.errorMessage = L"Failed to initialize network session.";
        return result;
    }

    // Set network timeouts (5 seconds connect, 10 seconds send/receive)
    WinHttpSetTimeouts(hSession, 5000, 5000, 10000, 10000);

    HINTERNET hConnect = WinHttpConnect(
        hSession,
        L"api.github.com",
        INTERNET_DEFAULT_HTTPS_PORT,
        0
    );

    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        result.errorMessage = L"Failed to connect to update server.";
        return result;
    }

    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"GET",
        MEMSTAND_API_LATEST_RELEASE,
        nullptr,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE
    );

    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        result.errorMessage = L"Failed to open request.";
        return result;
    }

    LPCWSTR headers = L"User-Agent: MemStand-App\r\nAccept: application/vnd.github.v3+json\r\n";
    BOOL sent = WinHttpSendRequest(
        hRequest,
        headers,
        (DWORD)-1,
        WINHTTP_NO_REQUEST_DATA,
        0,
        0,
        0
    );

    if (!sent || !WinHttpReceiveResponse(hRequest, nullptr)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        result.errorMessage = L"Unable to receive update response. Check internet connection.";
        return result;
    }

    DWORD statusCode = 0;
    DWORD statusSize = sizeof(statusCode);
    WinHttpQueryHeaders(
        hRequest,
        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
        WINHTTP_HEADER_NAME_BY_INDEX,
        &statusCode,
        &statusSize,
        WINHTTP_NO_HEADER_INDEX
    );

    if (statusCode != 200) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        // Note: 404 indicates no releases have been published to this repo yet
        if (statusCode == 404) {
            result.errorMessage = L"No releases found on GitHub repository yet.";
        } else {
            result.errorMessage = L"Server returned status code: " + std::to_wstring(statusCode);
        }
        return result;
    }

    std::string responseBody;
    DWORD bytesAvailable = 0;
    while (WinHttpQueryDataAvailable(hRequest, &bytesAvailable) && bytesAvailable > 0) {
        std::vector<char> buffer(bytesAvailable + 1, 0);
        DWORD bytesRead = 0;
        if (WinHttpReadData(hRequest, buffer.data(), bytesAvailable, &bytesRead) && bytesRead > 0) {
            responseBody.append(buffer.data(), bytesRead);
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    std::string tagName = ExtractJsonString(responseBody, "tag_name");
    std::string htmlUrl = ExtractJsonString(responseBody, "html_url");

    if (tagName.empty()) {
        result.errorMessage = L"Malformed release response.";
        return result;
    }

    result.latestVersion = Utf8ToWide(tagName);
    if (!htmlUrl.empty()) {
        result.releaseUrl = Utf8ToWide(htmlUrl);
    }

    std::string currentVer = MEMSTAND_VERSION_STRING;
    if (CompareVersions(currentVer, tagName) < 0) {
        result.updateAvailable = true;
    }

    return result;
}

} // namespace MemStand
