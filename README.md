<div align="center">

# ⚡ MemStand

### High-Precision System Timer & Intelligent Memory Optimizer for Windows

<p align="center">
  <b>English</b> • <a href="README_TR.md">Türkçe</a>
</p>

[![Release](https://img.shields.io/github/v/release/enixyazilim/MemStand?color=3b82f6&logo=github&style=flat-square)](https://github.com/enixyazilim/MemStand/releases)
[![Build Status](https://img.shields.io/github/actions/workflow/status/enixyazilim/MemStand/ci.yml?branch=main&label=CI&logo=githubactions&logoColor=white&style=flat-square)](https://github.com/enixyazilim/MemStand/actions)
[![Language](https://img.shields.io/badge/C%2B%2B-20%20(ISO)-00599C?logo=c%2B%2B&style=flat-square)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011%20(x64)-0078D4?logo=windows&style=flat-square)](https://microsoft.com/windows)
[![Dependencies](https://img.shields.io/badge/Dependencies-Zero%20(Pure%20Win32)-10b981?style=flat-square)](#-technical-specifications)
[![Binary Size](https://img.shields.io/badge/Binary%20Size-%3C%20500%20KB-success?style=flat-square)](#-technical-specifications)
[![RAM Footprint](https://img.shields.io/badge/RAM%20Usage-~1.9%20MB-success?style=flat-square)](#-technical-specifications)
[![License](https://img.shields.io/badge/License-MIT-8b5cf6?style=flat-square)](LICENSE)

<br/>

**MemStand** is a modern, ultra-lightweight, standalone Windows system optimizer engineered in pure **C++20** by **[Enix Yazılım](https://github.com/enixyazilim)**. It locks the Windows global kernel timer resolution to **0.50 ms** to eradicate mouse input latency, micro-stutters, and frame-time variance in competitive gaming while delivering automated, hardware-aware memory management.

Built with a strict **zero external dependency** philosophy, MemStand requires no .NET runtime, no WebView2, no Electron, and no background services. It runs quietly in the system tray with an idle working set of only **~1.9 MB** and **0.00%** CPU usage.

</div>

---

## ⚡ Feature Highlights

| Feature | Description |
| :--- | :--- |
| **[▶] Master Toggle** | Instant single-click start/stop for background engine |
| **[⚡] 0.50 ms Timer** | Sub-millisecond kernel resolution lock + watchdog |
| **[🧠] Auto-Profiling** | Dynamically calculates optimal thresholds from physical RAM |
| **[🎯] Standby Purge** | Safe standby list cache purge without freezing games |
| **[📊] Dynamic Tray** | Live color-coded RAM percentage on Windows taskbar |
| **[🛡️] UAC-less Startup** | Silent administrative logon via Task Scheduler API |
| **[🌐] Native Updater** | Checks GitHub releases directly via lightweight WinHTTP |
| **[🌍] Modular i18n** | Hot-pluggable community translations (JSON-based) |

---

## 🥊 Why MemStand? (Comparison with Legacy Utilities)

| Feature | Legacy Tools (e.g. ISLC) | OEM Suites (Razer, etc.) | **MemStand (Enix Yazılım)** |
| :--- | :---: | :---: | :---: |
| **Programming Language** | C# / .NET Framework | Electron / Chromium | **Pure C++20 (ISO Standard)** |
| **External Dependencies** | .NET Runtime 4.x/6.x | Node.js, Chromium, Services | **None (Native Win32 & NT API)** |
| **Binary Executable Size** | ~3 - 10 MB | 150 MB - 500 MB+ | **< 500 KB** |
| **Idle Memory Footprint** | 40 MB - 80 MB | 200 MB - 800 MB | **~1.9 MB** |
| **Hardware-Aware Thresholds**| ❌ Hardcoded static numbers | ❌ Aggressive blind wipes | **✅ Dynamic profile (RAM / 2)** |
| **Timer Watchdog** | ⚠️ Basic polling | ❌ None | **✅ Kernel re-assertion watchdog** |
| **UAC-less Elevated Logon** | ⚠️ Inconsistent task creation| ❌ Runs heavyweight service | **✅ Clean Task Scheduler integration** |
| **Modern Native Dark Mode** | ❌ WinForms 90s style | ⚠️ Web-rendered / Heavy | **✅ Custom-drawn Win32 Deep Slate** |
| **In-App Update Checker** | ❌ Manual web lookup | ⚠️ Background autoupdater | **✅ Native WinHTTP release checker** |

---

## 🚀 Deep Dive: Key Capabilities

### 1. ⚡ Hardware-Aware Memory Profiling
Unlike legacy tools that expect users to guess arbitrary megabyte values, MemStand inspects installed physical RAM on launch and computes optimal thresholds:
- **Standby Threshold (MB):** Automatically set to 50% of installed memory (`RAM / 2`). This preserves beneficial Windows filesystem caching during normal operation while ensuring bloated standby pages never starve running games.
- **Free RAM Safety Headroom:** Dynamically scales with your hardware tier (8 GB, 16 GB, 24 GB, 32 GB, 64 GB+) to prevent memory exhaustion.
- **`⚡ Auto-Calculate` Button:** Restore or recalculate recommended hardware parameters at any time with a single click.

### 2. 🎮 0.50 ms Sub-Millisecond Kernel Timer + Watchdog
- Invokes native NT kernel routines (`NtSetTimerResolution`) paired with multimedia timer periods to reduce timer resolution from standard 15.6 ms or 1.00 ms down to **0.50 ms** (5,000 units in 100ns precision).
- Significantly flattens frame-time variance, reduces mouse polling jitter, and improves hit-registration responsiveness in competitive titles (*Valorant, CS2, Apex Legends, Warzone, Fortnite*).
- **Active Watchdog:** Continuously re-asserts the resolution in the background to prevent Windows or background software from silently resetting it to default. Supports customizable targets (0.50 ms, 0.75 ms, 1.00 ms, 15.62 ms).

### 3. 🧠 Multi-Tier Adaptive Memory Cleaning
- **Tier 1 (Normal / In-Game Load):** Only the Standby List is safely purged. Active process working sets remain warm in cache to guarantee zero micro-stutters or frame drops.
- **Tier 2 (High Memory Pressure):** When overall system RAM exceeds the configured threshold (default: **85%**), working sets and system file cache are gently streamlined to prevent out-of-memory lockups.

### 4. 📊 Live Dynamic System Tray Badge
- Draws the real-time RAM usage percentage directly onto the taskbar notification icon using custom GDI double buffering.
- Color-coded at a glance:
  - 🟢 **Emerald Green:** Normal load (< 70%)
  - 🟡 **Amber Orange:** Moderate load (70% - 85%)
  - 🔴 **Crimson Red:** High pressure (> 85%)
- Right-click tray menu provides one-click manual purges, timer toggling, and update checking without bringing up the main window.

### 5. 🛡️ UAC-less Elevated Startup
- Registers a native Windows Scheduled Task (`schtasks.exe` with `HIGHEST` run level).
- Launches automatically at user logon with administrative privileges—completely eliminating disruptive Windows UAC prompts on boot.

### 6. 🔄 Native Release & Update Checker
- Integrated lightweight update checker queries the GitHub Releases API via Windows `winhttp.dll`.
- Checks release tags asynchronously without blocking the user interface or requiring third-party libraries.
- One-click access to download new releases and inspect changelogs.

---

## 📥 Download & Installation

MemStand is **100% portable**. It does not require an installer or registry modifications.

1. Download the latest `MemStand-vX.X.X-windows-x64.zip` from [GitHub Releases](https://github.com/enixyazilim/MemStand/releases/latest).
2. Extract the archive to your preferred folder (e.g. `C:\Tools\MemStand` or `Desktop`).
3. Run `MemStand.exe`.
4. *(Optional)* In the **Settings** tab, tick **Start with Windows (UAC-less)** and **Start Minimized to System Tray** for seamless set-and-forget background operation.

### Verification (SHA-256)
Every release includes a `checksums.txt` file. You can verify the integrity of your download using PowerShell:
```powershell
Get-FileHash -Path MemStand.exe -Algorithm SHA256
```

---

## 💻 Command Line Interface (CLI)

MemStand supports CLI parameters for scripting, game launchers, and automated batch jobs:

| Argument | Description |
| :--- | :--- |
| `MemStand.exe -minimized` | Launches silently into the system tray without displaying the GUI window. |
| `MemStand.exe -purge` | Immediately purges the Standby List and exits silently. |
| `MemStand.exe -clean` | Executes a complete memory sweep (Standby, Working Sets, Cache) and exits silently. |

---

## ⌨️ Global Hotkey

- **`Ctrl + Shift + M`**: Triggers an immediate memory purge from within any full-screen game or application without switching to desktop. Can be toggled on/off in the Settings tab.

---

## 🌐 Community Localization (i18n)

MemStand includes built-in English and Turkish support, and supports unlimited community translations via modular JSON files:

1. Copy `languages/template.json` to `languages/<your-language>.json` (e.g. `languages/german.json`).
2. Translate the values in the `"strings"` block.
3. Restart MemStand—your language will appear immediately in the **Interface Language** menu.
4. Open a Pull Request to share your translation with the community!

---

## 📊 Technical Specifications

| Parameter | Specification |
| :--- | :--- |
| **Language & Standard** | Pure C++ (ISO C++20) |
| **Architecture** | Native 64-bit (`x86_64`) |
| **APIs Used** | Win32 API, NT Native API (`ntdll.dll`), WinHTTP (`winhttp.dll`) |
| **External Dependencies** | **None** (Zero runtime dependencies, standalone binary) |
| **Binary Size** | **~500 KB** (`MemStand.exe`) |
| **Memory Consumption** | **~1.9 MB** idle working set |
| **CPU Utilization** | **0.00%** between 1-second polling cycles |
| **Supported OS** | Windows 10 (all editions 1809+), Windows 11 (all editions) |

---

## 🛠️ Building from Source

### Prerequisites
- Windows 10 or Windows 11 (x64)
- Visual Studio 2022 or Visual Studio 2026 with the *"Desktop development with C++"* workload
- CMake 3.20 or newer (optional, if building via CMake)

### Method 1: One-Click Build Script
Run the automated build script in the root directory:
```cmd
build.bat
```
The compiled, standalone executable is output to `bin\MemStand.exe`.

### Method 2: CMake
```cmd
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## 🤝 Contributing

Contributions, bug reports, and suggestions are welcome! Please check out [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines on code style, architecture conventions, and Pull Request workflows.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).  
Copyright © 2026 **[Enix Yazılım](https://github.com/enixyazilim)** & MemStand Contributors.
