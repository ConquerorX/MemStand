<div align="center">

# ⚡ MemStand

### Timer di sistema ad alta precisione e ottimizzatore intelligente della memoria per Windows

<p align="center">
  <a href="README.md">English</a> • <a href="README_TR.md">Türkçe</a> • <a href="README.fr.md">Français</a> • <a href="README.de.md">Deutsch</a> • <a href="README.es.md">Español</a> • <b>Italiano</b> • <a href="README.pt-BR.md">Português (Brasil)</a> • <a href="README.ru.md">Русский</a>
</p>

[![Release](https://img.shields.io/github/v/release/enixyazilim/MemStand?color=3b82f6&logo=github&style=flat-square)](https://github.com/enixyazilim/MemStand/releases)
[![Build Status](https://img.shields.io/github/actions/workflow/status/enixyazilim/MemStand/ci.yml?branch=main&label=CI&logo=githubactions&logoColor=white&style=flat-square)](https://github.com/enixyazilim/MemStand/actions)
[![Language](https://img.shields.io/badge/C%2B%2B-20%20(ISO)-00599C?logo=c%2B%2B&style=flat-square)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011%20(x64)-0078D4?logo=windows&style=flat-square)](https://microsoft.com/windows)
[![Dependencies](https://img.shields.io/badge/Dependencies-Zero%20(Pure%20Win32)-10b981?style=flat-square)](#-specifiche-tecniche)
[![Binary Size](https://img.shields.io/badge/Binary%20Size-%3C%20500%20KB-success?style=flat-square)](#-specifiche-tecniche)
[![RAM Footprint](https://img.shields.io/badge/RAM%20Usage-~1.9%20MB-success?style=flat-square)](#-specifiche-tecniche)
[![License](https://img.shields.io/badge/License-MIT-8b5cf6?style=flat-square)](LICENSE)

<br/>

**MemStand** è un ottimizzatore di sistema Windows moderno, ultraleggero e autonomo, progettato in puro **C++20** da **[Enix Yazılım](https://github.com/enixyazilim)**. Blocca la risoluzione globale del timer del kernel di Windows a **0,50 ms** per eliminare la latenza del mouse, i micro-stutter e le variazioni del frame time nel gaming competitivo, offrendo al contempo una gestione della memoria automatica e adattata all'hardware.

Costruito secondo una rigorosa filosofia di **zero dipendenze esterne**, MemStand non richiede runtime .NET, né WebView2, né Electron, né servizi in background. Funziona silenziosamente nell'area di notifica con un working set inattivo di appena **~1,9 MB** e un utilizzo CPU dello **0,00%**.

</div>

---

## ⚡ Caratteristiche principali

| Funzione | Descrizione |
| :--- | :--- |
| **[▶] Interruttore principale** | Avvio/arresto immediato del motore in background con un clic |
| **[⚡] Timer a 0,50 ms** | Blocco della risoluzione del kernel sotto il millisecondo + watchdog |
| **[🧠] Profilatura automatica** | Calcola dinamicamente le soglie ottimali a partire dalla RAM fisica |
| **[🎯] Svuotamento della standby list** | Svuotamento sicuro della cache standby senza bloccare i giochi |
| **[📊] Area di notifica dinamica** | Percentuale di RAM live e codificata a colori nella barra delle applicazioni |
| **[🛡️] Avvio senza UAC** | Accesso amministrativo silenzioso tramite API di Task Scheduler |
| **[🌐] Aggiornatore nativo** | Controlla le release di GitHub direttamente via WinHTTP |
| **[🌍] i18n modulare** | Traduzioni della community inseribili a caldo (basate su JSON) |

---

## 🥊 Perché MemStand? (confronto con gli strumenti tradizionali)

| Funzione | Strumenti tradizionali (es. ISLC) | Suite OEM (Razer, ecc.) | **MemStand (Enix Yazılım)** |
| :--- | :---: | :---: | :---: |
| **Linguaggio di programmazione** | C# / .NET Framework | Electron / Chromium | **C++20 puro (standard ISO)** |
| **Dipendenze esterne** | Runtime .NET 4.x/6.x | Node.js, Chromium, servizi | **Nessuna (Win32 nativo e API NT)** |
| **Dimensione dell'eseguibile** | ~3 - 10 MB | 150 MB - 500 MB+ | **< 500 KB** |
| **Memoria a riposo** | 40 MB - 80 MB | 200 MB - 800 MB | **~1,9 MB** |
| **Soglie adatte all'hardware** | ❌ Valori statici codificati a mano | ❌ Pulizie cieche e aggressive | **✅ Profilo dinamico (RAM / 2)** |
| **Watchdog del timer** | ⚠️ Polling di base | ❌ Assente | **✅ Watchdog che riafferma la risoluzione del kernel** |
| **Accesso elevato senza UAC** | ⚠️ Creazione di task incoerente | ❌ Avvia un servizio pesante | **✅ Integrazione pulita con Task Scheduler** |
| **Dark mode nativo moderno** | ❌ WinForms stile anni '90 | ⚠️ Renderizzato via web / Pesante | **✅ Win32 Deep Slate disegnato a mano** |
| **Verificatore aggiornamenti integrato** | ❌ Ricerca manuale sul web | ⚠️ Aggiornamento automatico in background | **✅ Verificatore di release nativo via WinHTTP** |

---

## 🚀 Approfondimento: funzionalità chiave

### 1. ⚡ Profilazione della memoria adattata all'hardware
A differenza degli strumenti tradizionali che chiedono all'utente di indovinare valori arbitrari in megabyte, MemStand esamina la RAM fisica installata all'avvio e calcola le soglie ottimali:
- **Soglia standby (MB):** impostata automaticamente al 50% della memoria installata (`RAM / 2`). In questo modo la cache del file system di Windows, utile durante il normale funzionamento, viene conservata, garantendo al contempo che pagine standby gonfie non sottraghiano mai la memoria ai giochi in esecuzione.
- **Margine di sicurezza per la RAM libera:** si adatta dinamicamente alla fascia hardware (8 GB, 16 GB, 24 GB, 32 GB, 64 GB+) per evitare l'esaurimento della memoria.
- **Pulsante `⚡ Calcola automaticamente`:** ripristina o ricalcola in qualsiasi momento i parametri consigliati per il tuo hardware, con un solo clic.

### 2. 🎮 Timer del kernel sub-millisecondo a 0,50 ms + watchdog
- Invoca le routine native del kernel NT (`NtSetTimerResolution`) insieme ai periodi dei timer multimediali, riducendo la risoluzione dagli standard 15,6 ms o 1,00 ms a **0,50 ms** (5.000 unità con precisione a 100 ns).
- Appiattisce nettamente le variazioni del frame time, riduce il jitter del polling del mouse e migliora la reattività nella registrazione dei colpi nei titoli competitivi (*Valorant, CS2, Apex Legends, Warzone, Fortnite*).
- **Watchdog attivo:** riafferma continuamente la risoluzione in background per impedire a Windows o al software in background di reimpostarla silenziosamente al valore predefinito. Obiettivi personalizzabili (0,50 ms, 0,75 ms, 1,00 ms, 15,62 ms).

### 3. 🧠 Pulizia della memoria adattiva su più livelli
- **Livello 1 (carico normale / in gioco):** viene svuotata in sicurezza solo la standby list. I working set dei processi attivi restano caldi in cache, garantendo zero micro-stutter o cali di frame.
- **Livello 2 (forte pressione sulla memoria):** quando la RAM complessiva supera la soglia configurata (predefinito: **85%**), working set e cache di sistema vengono alleggeriti in modo graduale per evitare blocchi dovuti alla memoria esaurita.

### 4. 📊 Badge dinamico nell'area di notifica
- Disegna la percentuale di utilizzo della RAM in tempo reale direttamente sull'icona di notifica della barra delle applicazioni, con doppio buffering GDI personalizzato.
- Codifica a colori a colpo d'occhio:
  - 🟢 **Verde smeraldo:** carico normale (< 70%)
  - 🟡 **Arancio ambra:** carico moderato (70% - 85%)
  - 🔴 **Rosso carminio:** forte pressione (> 85%)
- Il menu contestuale dell'area di notifica consente svuotamenti manuali con un clic, l'attivazione/disattivazione del timer e il controllo degli aggiornamenti, senza aprire la finestra principale.

### 5. 🛡️ Avvio elevato senza UAC
- Registra un'attività pianificata nativa di Windows (`schtasks.exe` con livello di esecuzione `HIGHEST`).
- Si avvia automaticamente all'accesso con privilegi di amministrazione, eliminando completamente le richieste UAC di Windows all'avvio.

### 6. 🔄 Verificatore nativo di release e aggiornamenti
- Un verificatore di aggiornamenti leggero e integrato interroga l'API GitHub Releases tramite `winhttp.dll`.
- Controlla i tag delle release in modo asincrono, senza bloccare l'interfaccia utente e senza richiedere librerie di terze parti.
- Accesso con un clic al download delle nuove release e alla consultazione dei changelog.

---

## 📥 Download e installazione

MemStand è **100% portatile**: non richiede installer né modifiche al registro.

1. Scarica l'ultimo `MemStand-vX.X.X-windows-x64.zip` da [GitHub Releases](https://github.com/enixyazilim/MemStand/releases/latest).
2. Estrai l'archivio nella cartella che preferisci (ad esempio `C:\Tools\MemStand` o `Desktop`).
3. Avvia `MemStand.exe`.
4. *(Facoltativo)* Nella scheda **Impostazioni**, seleziona **Avvia con Windows (senza UAC)** e **Avvia ridotto nell'area di notifica** per un funzionamento silenzioso in background.

### Verifica (SHA-256)
Ogni release include un file `checksums.txt`. Puoi verificare l'integrità del download con PowerShell:
```powershell
Get-FileHash -Path MemStand.exe -Algorithm SHA256
```

---

## 💻 Interfaccia a riga di comando (CLI)

MemStand supporta parametri da riga di comando per script, launcher di giochi e processi batch automatizzati:

| Argomento | Descrizione |
| :--- | :--- |
| `MemStand.exe -minimized` | Si avvia silenziosamente nell'area di notifica senza mostrare la finestra. |
| `MemStand.exe -purge` | Svuota immediatamente la standby list ed esce silenziosamente. |
| `MemStand.exe -clean` | Esegue una pulizia completa della memoria (standby, working set, cache) ed esce silenziosamente. |

---

## ⌨️ Scorciatoia globale

- **`Ctrl + Shift + M`**: attiva uno svuotamento immediato della memoria da qualsiasi gioco o applicazione a schermo intero, senza passare al desktop. Attivabile e disattivabile nella scheda Impostazioni.

### 🎹 Scorciatoia riassegnabile

Non sei obbligato a usare `Ctrl + Shift + M`. Seleziona **"Scorciatoia globale di pulizia"** nella scheda Impostazioni e fai clic sul pulsante accanto: il registratore si attiva (il bordo diventa ambra e l'etichetta passa a *Registrazione...*), poi premi la combinazione che preferisci.

- Premi **`Esc`** durante la registrazione per annullare, oppure **`Canc`** per ripristinare il `Ctrl + Shift + M` integrato.
- È richiesto almeno uno tra `Ctrl`, `Alt`, `Shift` o `Win`, così l'applicazione non cattura mai un tasto singolo.
- La scelta viene applicata solo quando premi **Salva le impostazioni** e sopravvive ai riavvii.
- Se un'altra applicazione possiede già quella combinazione, MemStand lo segnala invece di fallire in silenzio, e puoi sceglierne un'altra.
- La ripetizione automatica dei tasti è soppressa, quindi tenere premuta la combinazione non accoda svuotamenti ripetuti.

---

## 🌐 Traduzioni della community (i18n)

MemStand viene distribuito con **8 lingue dell'interfaccia** — English, Türkçe, Deutsch, Español, Français, Italiano, Português (Brasil) e Русский — e supporta un numero illimitato di traduzioni della community tramite file JSON modulari.

L'elenco è ordinato alfabeticamente in base al nome della lingua e ogni file nella cartella `languages/` viene rilevato automaticamente all'avvio.

1. Copia `languages/template.json` in `languages/<la-tua-lingua>.json` (ad esempio `languages/french.json`).
2. Traduci i valori nel blocco `"strings"`. Tieni brevi le etichette: la schermata delle impostazioni usa colonne a larghezza fissa, quindi un'etichetta troppo lunga si sovrappone al campo accanto.
3. Riavvia MemStand: la tua lingua comparirà subito nel menu **Lingua**.
4. Apri una Pull Request per condividere la tua traduzione con la community!

---

## 📊 Specifiche tecniche

| Parametro | Specifica |
| :--- | :--- |
| **Linguaggio e standard** | C++ puro (ISO C++20) |
| **Architettura** | Nativa a 64 bit (`x86_64`) |
| **API utilizzate** | API Win32, API Native NT (`ntdll.dll`), WinHTTP (`winhttp.dll`) |
| **Dipendenze esterne** | **Nessuna** (zero dipendenze a runtime, binario autonomo) |
| **Dimensione del binario** | **~500 KB** (`MemStand.exe`) |
| **Consumo di memoria** | **~1,9 MB** di working set inattivo |
| **Utilizzo CPU** | **0,00%** tra due cicli di polling da un secondo |
| **OS supportati** | Windows 10 (tutte le edizioni dalla 1809), Windows 11 (tutte le edizioni) |

---

## 🛠️ Compilare dal sorgente

### Prerequisiti
- Windows 10 o Windows 11 (x64)
- Visual Studio 2022 o Visual Studio 2026 con il carico di lavoro *«Sviluppo desktop con C++»*
- CMake 3.20 o successivo (facoltativo, se si compila con CMake)

### Metodo 1: script di compilazione con un clic
Esegui lo script di build nella cartella principale:
```cmd
build.bat
```
L'eseguibile autonomo compilato viene prodotto in `bin\MemStand.exe`.

### Metodo 2: CMake
```cmd
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## 🤝 Contribuire

Contributi, segnalazioni di bug e suggerimenti sono benvenuti! Consulta [CONTRIBUTING.md](CONTRIBUTING.md) per le linee guida dettagliate su stile del codice, convenzioni architetturali e flusso di lavoro delle Pull Request.

---

## 📄 Licenza

Questo progetto è distribuito con licenza [MIT](LICENSE).  
Copyright © 2026 **[Enix Yazılım](https://github.com/enixyazilim)** e i contributori di MemStand.
