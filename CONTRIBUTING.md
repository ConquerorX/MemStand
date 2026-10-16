# Contributing to MemStand

Thank you for your interest in contributing to **MemStand**! MemStand is maintained by **Enix Yazılım** and open-source contributors worldwide.

We welcome contributions of all kinds: bug fixes, performance improvements, translations, documentation, and feature proposals.

---

## 🛠️ Development Setup & Requirements

### Prerequisites
- **Operating System:** Windows 10 (version 1809+) or Windows 11 (x64)
- **Compiler:** MSVC with ISO C++20 standard support (Visual Studio 2022 or Visual Studio 2026)
- **Build Systems:** CMake 3.20+ OR the built-in `build.bat` script
- **SDK:** Windows 10/11 SDK (version 10.0.19041.0 or newer)

### Quick Build
1. Clone the repository:
   ```cmd
   git clone https://github.com/enixyazilim/MemStand.git
   cd MemStand
   ```
2. Build with the one-click MSVC script:
   ```cmd
   build.bat
   ```
   The compiled executable will be placed at `bin\MemStand.exe`.

3. Or build with CMake:
   ```cmd
   cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release
   ```

---

## 📐 Core Engineering Principles

MemStand is built around a set of strict engineering standards. Any PR submitted to MemStand must respect these principles:

1. **Zero External Runtime Dependencies:**
   MemStand does not link against third-party UI frameworks, .NET runtime, WebView2, Electron, or external runtimes. It uses native Win32, NT Native API, and Windows system libraries exclusively.

2. **Sub-Millisecond Resource Efficiency:**
   - Memory footprint must remain minimal (~1.9 MB idle working set).
   - CPU utilization between polling cycles must remain 0.00%.
   - Polling loops must sleep in chunks to guarantee instant, responsive worker termination.

3. **Strict RAII and GDI Management:**
   Every GDI object (`HBITMAP`, `HBRUSH`, `HPEN`, `HFONT`, `HDC`, `HICON`) must be explicitly released or deleted. Leakage of GDI handles causes system degradation over multi-day continuous runtime.

4. **Unicode Everywhere:**
   All Win32 functions must use the wide (`W`) variants (`CreateWindowExW`, `SetWindowTextW`, `swprintf_s`, `std::wstring`). String literals must use `L"..."`.

5. **100% Decoupled Localization (i18n):**
   Never hardcode user-facing strings into UI logic. All labels, buttons, notifications, and status texts must query the localization subsystem via `L("key_name")`.

---

## 🌐 Adding or Updating a Translation

MemStand makes community localization straightforward and modular:

1. Locate `languages/template.json` in the root repository.
2. Make a copy named after your language (e.g., `languages/french.json`, `languages/german.json`, `languages/japanese.json`).
3. Set the `language_name` (in native characters, e.g., `"Deutsch"`, `"Français"`) and `language_code` (`"de"`, `"fr"`).
4. Translate all key-value pairs in the `"strings"` block.
5. Launch MemStand; it automatically detects new `.json` files placed in `languages/`.
6. Submit your translation via a Pull Request!

---

## 🚀 Pull Request Workflow

1. **Fork the Repository:** Create your personal fork on GitHub.
2. **Create a Feature Branch:**
   ```cmd
   git checkout -b feature/your-feature-name
   ```
3. **Commit Your Changes:** Write descriptive, imperative commit messages (e.g., `Fix timer resolution restoration on exit`, `Add French localization`).
4. **Test Thoroughly:**
   - Compile cleanly in Release mode.
   - Run on Windows 10 and/or Windows 11.
   - Verify stability over multiple sleep/wake and lock/unlock cycles.
5. **Push and Open a Pull Request:**
   Open a PR against the `main` branch and fill out the provided PR template.

---

## 💬 Code of Conduct

All contributors and participants are expected to adhere to our [Code of Conduct](CODE_OF_CONDUCT.md). Please treat fellow community members with respect and professionalism.
