#pragma once

#include "../common.hpp"

namespace MemStand {

struct LanguageInfo {
    std::string id;            // e.g. "turkish" or "english" or filename
    std::wstring displayName;  // e.g. L"Türkçe", L"English"
    std::string filePath;      // if loaded from file
};

class Localization {
public:
    static Localization& Instance() {
        static Localization instance;
        return instance;
    }

    void Initialize();
    bool LoadLanguage(const std::string& langId);
    
    std::wstring Get(const std::string& key) const;
    std::vector<LanguageInfo> GetAvailableLanguages() const;
    std::string GetCurrentLanguageId() const { return m_currentLangId; }

private:
    Localization();
    void LoadBuiltinFallbacks();
    void ScanLanguagesFolder();
    bool ParseLanguageJson(const std::string& filePath, LanguageInfo& info, std::map<std::string, std::wstring>& strings);

    std::string m_currentLangId = "turkish";
    std::vector<LanguageInfo> m_availableLanguages;
    std::map<std::string, std::wstring> m_currentStrings;
    std::map<std::string, std::map<std::string, std::wstring>> m_builtinLanguages;
};

inline std::wstring L(const std::string& key) {
    return Localization::Instance().Get(key);
}

} // namespace MemStand
