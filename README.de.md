<div align="center">

# ⚡ MemStand

### Hochpräzisions-Systemtimer und intelligenter Speicher-Optimierer für Windows

<p align="center">
  <a href="README.md">English</a> • <a href="README.tr.md">Türkçe</a> • <a href="README.fr.md">Français</a> • <b>Deutsch</b> • <a href="README.es.md">Español</a> • <a href="README.it.md">Italiano</a> • <a href="README.pt-BR.md">Português (Brasil)</a> • <a href="README.ru.md">Русский</a>
</p>

[![Release](https://img.shields.io/github/v/release/enixyazilim/MemStand?color=3b82f6&logo=github&style=flat-square)](https://github.com/enixyazilim/MemStand/releases)
[![Build Status](https://img.shields.io/github/actions/workflow/status/enixyazilim/MemStand/ci.yml?branch=main&label=CI&logo=githubactions&logoColor=white&style=flat-square)](https://github.com/enixyazilim/MemStand/actions)
[![Language](https://img.shields.io/badge/C%2B%2B-20%20(ISO)-00599C?logo=c%2B%2B&style=flat-square)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011%20(x64)-0078D4?logo=windows&style=flat-square)](https://microsoft.com/windows)
[![Dependencies](https://img.shields.io/badge/Dependencies-Zero%20(Pure%20Win32)-10b981?style=flat-square)](#-technische-spezifikationen)
[![Binary Size](https://img.shields.io/badge/Binary%20Size-%3C%20500%20KB-success?style=flat-square)](#-technische-spezifikationen)
[![RAM Footprint](https://img.shields.io/badge/RAM%20Usage-~1.9%20MB-success?style=flat-square)](#-technische-spezifikationen)
[![License](https://img.shields.io/badge/License-MIT-8b5cf6?style=flat-square)](LICENSE)

<br/>

**MemStand** ist ein moderner, ultraleichtgewichtiger, eigenständiger Windows-Systemoptimierer, entwickelt in reinem **C++20** von **[Enix Yazılım](https://github.com/enixyazilim)**. Er fixiert die globale Kernel-Timer-Auflösung von Windows auf **0,50 ms**, um Mauseingangslatenz, Mikroruckler und Frame-Time-Schwankungen im kompetitiven Gaming zu beseitigen – und liefert gleichzeitig eine automatisierte, hardwarebewusste Speicherverwaltung.

Nach der strikten Philosophie **keiner externen Abhängigkeiten** benötigt MemStand weder .NET-Runtime noch WebView2, Electron oder Hintergrunddienste. Die Anwendung läuft still im Infobereich mit einem Arbeitsspeicher von lediglich **~1,9 MB** im Leerlauf und **0,00 %** CPU-Auslastung.

</div>

---

## ⚡ Funktions highlights

| Funktion | Beschreibung |
| :--- | :--- |
| **[▶] Hauptschalter** | Sofortiges Starten/Stoppen der Hintergrund-Engine mit einem Klick |
| **[⚡] 0,50-ms-Timer** | Sub-Millisekunden-Kernelauflösung + Watchdog |
| **[🧠] Automatische Profilanalyse** | Berechnet die optimalen Schwellwerte dynamisch aus dem physischen RAM |
| **[🎯] Standby-Bereinigung** | Sicheres Leeren des Standby-Caches, ohne Spiele einzufrieren |
| **[📊] Dynamischer Infobereich** | Live, farbcodierte RAM-Auslastung in der Windows-Taskleiste |
| **[🛡️] Start ohne UAC** | Stille Anmeldung mit Administratorrechten über die Task-Scheduler-API |
| **[🌐] Nativer Updater** | Prüft GitHub-Releases direkt über schlankes WinHTTP |
| **[🌍] Modulares i18n** | Austauschbare Community-Übersetzungen (JSON-basiert) |

---

## 🥊 Warum MemStand? (Vergleich mit klassischen Werkzeugen)

| Funktion | Klassische Tools (z. B. ISLC) | OEM-Suites (Razer usw.) | **MemStand (Enix Yazılım)** |
| :--- | :---: | :---: | :---: |
| **Programmiersprache** | C# / .NET Framework | Electron / Chromium | **Reines C++20 (ISO-Standard)** |
| **Externe Abhängigkeiten** | .NET-Runtime 4.x/6.x | Node.js, Chromium, Dienste | **Keine (natives Win32 und NT-API)** |
| **Größe der ausführbaren Datei** | ~3 - 10 MB | 150 MB - 500 MB+ | **< 500 KB** |
| **Speicher im Leerlauf** | 40 MB - 80 MB | 200 MB - 800 MB | **~1,9 MB** |
| **Hardwarebewusste Schwellwerte** | ❌ Feste hartcodierte Zahlen | ❌ Aggressives blindes Leeren | **✅ Dynamische Profilanalyse (RAM / 2)** |
| **Timer-Watchdog** | ⚠️ Einfaches Abfragen | ❌ Nicht vorhanden | **✅ Watchdog, der die Kernelauflösung neu setzt** |
| **Erhöhte Anmeldung ohne UAC** | ⚠️ Inkonsistente Task-Erstellung | ❌ Startet einen schweren Dienst | **✅ Saubere Task-Scheduler-Integration** |
| **Moderner nativer Dark Mode** | ❌ WinForms-Stil der 90er | ⚠️ Web-gerendert / Schwer | **✅ Selbst gezeichnetes Win32 Deep Slate** |
| **Integrierter Update-Prüfer** | ❌ Manuelle Websuche | ⚠️ Automatischer Updater im Hintergrund | **✅ Nativer WinHTTP-Release-Prüfer** |

---

## 🚀 Details: zentrale Funktionen

### 1. ⚡ Hardwarebewusste Speicherprofilanalyse
Anders als klassische Werkzeuge, die vom Benutzer das Raten willkürlicher Megabyte-Werte verlangen, untersucht MemStand beim Start den installierten physischen RAM und berechnet die optimalen Schwellwerte:
- **Standby-Schwelle (MB):** wird automatisch auf 50 % des installierten Speichers gesetzt (`RAM / 2`). Das bewahrt den nützlichen Dateisystem-Cache von Windows im Normalbetrieb und stellt zugleich sicher, dass überladene Standby-Seiten laufende Spiele nie aushungern.
- **Sicherheitsreserve für freien RAM:** skaliert dynamisch mit der Hardwareklasse (8 GB, 16 GB, 24 GB, 32 GB, 64 GB+), um Speichererschöpfung zu verhindern.
- **Schaltfläche `⚡ Automatisch berechnen`:** stellt die empfohlenen Hardwareparameter jederzeit per Klick wieder her oder berechnet sie neu.

### 2. 🎮 Sub-Millisekunden-Kernel-Timer (0,50 ms) + Watchdog
- Ruft native NT-Kernelroutinen (`NtSetTimerResolution`) in Verbindung mit Multimedia-Timer-Perioden auf, um die Auflösung von standardmäßigen 15,6 ms bzw. 1,00 ms auf **0,50 ms** zu senken (5.000 Einheiten bei 100-ns-Präzision).
- Flacht Frame-Time-Schwankungen deutlich ab, verringert Jitter beim Maus-Polling und verbessert die Reaktionszeit bei Trefferregistrierungen in kompetitiven Titeln (*Valorant, CS2, Apex Legends, Warzone, Fortnite*).
- **Aktiver Watchdog:** setzt die Auflösung im Hintergrund kontinuierlich erneut durch, damit Windows oder Hintergrundsoftware sie nicht stillschweigend auf den Standard zurücksetzt. Anpassbare Ziele (0,50 ms, 0,75 ms, 1,00 ms, 15,62 ms).

### 3. 🧠 Adaptives Speicherbereinigen in mehreren Stufen
- **Stufe 1 (normale Last / im Spiel):** Nur die Standby-Liste wird sicher geleert. Die Working Sets aktiver Prozesse bleiben warm im Cache – garantiert ohne Mikroruckler oder Frame-Einbrüche.
- **Stufe 2 (hoher Speicherdruck):** Überschreitet der RAM-Verbrauch des Systems den eingestellten Schwellwert (Standard: **85 %**), werden Working Sets und Systemdatei-Cache behutsam reduziert, um Out-of-Memory-Einfrierungen zu verhindern.

### 4. 📊 Live-Abzeichen im Infobereich
- Zeichnet die aktuelle RAM-Auslastung in Prozent direkt auf das Infobereich-Symbol der Taskleiste – mit eigenem GDI-Double-Buffering.
- Farbcodiert auf einen Blick:
  - 🟢 **Smaragdgrün:** normale Last (< 70 %)
  - 🟡 **Bernsteinorange:** mittlere Last (70 % - 85 %)
  - 🔴 **Karminrot:** hoher Druck (> 85 %)
- Das Kontextmenü des Infobereichs bietet manuelle Bereinigungen per Klick, das Umschalten des Timers und die Updateprüfung, ohne das Hauptfenster zu öffnen.

### 5. 🛡️ Erhöhter Start ohne UAC
- Registriert eine native Windows-Aufgabe (`schtasks.exe` mit Ausführungsstufe `HIGHEST`).
- Startet bei der Anmeldung automatisch mit Administratorrechten – störende UAC-Dialoge beim Hochfahren entfallen vollständig.

### 6. 🔄 Nativer Release- und Update-Prüfer
- Ein integrierter, schlanker Update-Prüfer fragt die GitHub-Releases-API über `winhttp.dll` ab.
- Prüft Release-Tags asynchron, ohne die Benutzeroberfläche zu blockieren oder Bibliotheken Dritter zu benötigen.
- Ein Klick führt zum Download neuer Versionen und zu den Änderungsprotokollen.

---

## 📥 Download und Installation

MemStand ist **100 % portabel**. Es benötigt weder Installationsprogramm noch Registry-Änderungen.

1. Laden Sie die aktuelle `MemStand-vX.X.X-windows-x64.zip` von [GitHub Releases](https://github.com/enixyazilim/MemStand/releases/latest) herunter.
2. Entpacken Sie das Archiv in einen Ordner Ihrer Wahl (z. B. `C:\Tools\MemStand` oder `Desktop`).
3. Starten Sie `MemStand.exe`.
4. *(Optional)* Aktivieren Sie im Tab **Einstellungen** die Optionen **Mit Windows starten (ohne UAC)** und **Minimiert im Infobereich starten** für einen unauffälligen Hintergrundbetrieb.

### Überprüfung (SHA-256)
Jede Version enthält eine Datei `checksums.txt`. So prüfen Sie die Integrität des Downloads mit PowerShell:
```powershell
Get-FileHash -Path MemStand.exe -Algorithm SHA256
```

---

## 💻 Kommandozeilen-Schnittstelle (CLI)

MemStand unterstützt Kommandozeilenparameter für Skripte, Spiele-Launcher und automatisierte Batch-Aufgaben:

| Argument | Beschreibung |
| :--- | :--- |
| `MemStand.exe -minimized` | Startet still in den Infobereich, ohne das Fenster anzuzeigen. |
| `MemStand.exe -purge` | Leert sofort die Standby-Liste und beendet sich still. |
| `MemStand.exe -clean` | Führt eine vollständige Speicherbereinigung aus (Standby, Working Sets, Cache) und beendet sich still. |

---

## ⌨️ Globale Tastenkombination

- **`Ctrl + Shift + M`**: Löst sofort eine Speicherbereinigung aus jedem Vollbild-Spiel oder -Programm heraus, ohne zum Desktop zu wechseln. Im Tab Einstellungen ein- und ausschaltbar.

### 🎹 Frei belegbare Tastenkombination

Sie sind nicht auf `Ctrl + Shift + M` festgelegt. Aktivieren Sie im Tab Einstellungen **„Globales Tastenkürzel zum Speicherbereinigen"** und klicken Sie auf die Schaltfläche daneben: Der Rekorder schaltet scharf (der Rahmen wird bernsteinfarben und die Beschriftung wechselt zu *Wird aufgezeichnet...*), dann drücken Sie die gewünschte Kombination.

- Drücken Sie während der Aufzeichnung auf **`Esc`**, um abzubrechen, oder auf **`Entf`**, um das integrierte `Ctrl + Shift + M` wiederherzustellen.
- Mindestens eine der Tasten `Ctrl`, `Alt`, `Umschalt` oder `Win` ist erforderlich, damit die Anwendung niemals eine einzelne Taste abfängt.
- Die Auswahl wird erst übernommen, wenn Sie **Einstellungen speichern** drücken, und überlebt Neustarts.
- Besitzt eine andere Anwendung die Kombination bereits, meldet MemStand das, statt still zu scheitern, und Sie können eine andere wählen.
- Die Tastenwiederholung wird unterdrückt – ein gedrückt gehaltener Shortcut löst daher keine wiederholten Bereinigungen aus.

---

## 🌐 Community-Übersetzungen (i18n)

MemStand wird mit **8 Oberflächensprachen** ausgeliefert – English, Türkçe, Deutsch, Español, Français, Italiano, Português (Brasil) und Русский – und unterstützt über modulare JSON-Dateien beliebig viele Community-Übersetzungen.

Die Liste ist alphabetisch nach dem jeweiligen Sprachnamen sortiert, und jede Datei im Ordner `languages/` wird beim Start automatisch erkannt.

1. Kopieren Sie `languages/template.json` nach `languages/<Ihre-Sprache>.json` (z. B. `languages/german.json`).
2. Übersetzen Sie die Werte im Block `"strings"`. Halten Sie die Beschriftungen kurz – der Einstellungsbildschirm verwendet feste Spaltenbreiten, ein zu langes Label ragt in das Feld daneben.
3. Starten Sie MemStand neu – Ihre Sprache erscheint sofort im Menü **Sprache**.
4. Öffnen Sie einen Pull Request, um Ihre Übersetzung mit der Community zu teilen!

---

## 📊 Technische Spezifikationen

| Parameter | Spezifikation |
| :--- | :--- |
| **Sprache und Standard** | Reines C++ (ISO C++20) |
| **Architektur** | Native 64-Bit (`x86_64`) |
| **Verwendete APIs** | Win32-API, NT Native API (`ntdll.dll`), WinHTTP (`winhttp.dll`) |
| **Externe Abhängigkeiten** | **Keine** (keine Laufzeitabhängigkeiten, eigenständige Binärdatei) |
| **Größe der Binärdatei** | **~500 KB** (`MemStand.exe`) |
| **Speicherverbrauch** | **~1,9 MB** im Leerlauf |
| **CPU-Auslastung** | **0,00 %** zwischen zwei Abfrageintervallen von einer Sekunde |
| **Unterstützte Betriebssysteme** | Windows 10 (alle Editionen ab 1809), Windows 11 (alle Editionen) |

---

## 🛠️ Aus dem Quellcode bauen

### Voraussetzungen
- Windows 10 oder Windows 11 (x64)
- Visual Studio 2022 oder Visual Studio 2026 mit der Arbeitslastung *„Desktop-Entwicklung mit C++"*
- CMake 3.20 oder neuer (optional, wenn mit CMake gebaut wird)

### Methode 1: Ein-Klick-Buildskript
Führen Sie das Buildskript im Stammverzeichnis aus:
```cmd
build.bat
```
Die kompilierte, eigenständige ausführbare Datei wird unter `bin\MemStand.exe` erzeugt.

### Methode 2: CMake
```cmd
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## 🤝 Mitwirken

Beiträge, Fehlerberichte und Vorschläge sind willkommen! Details zu Code-Stil, Architekturkonventionen und Pull-Request-Workflow finden Sie in [CONTRIBUTING.md](CONTRIBUTING.md).

---

## 📄 Lizenz

Dieses Projekt steht unter der [MIT-Lizenz](LICENSE).  
Copyright © 2026 **[Enix Yazılım](https://github.com/enixyazilim)** und die MemStand-Mitwirkenden.
