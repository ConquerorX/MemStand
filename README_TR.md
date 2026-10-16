<div align="center">

# ⚡ MemStand

### Windows İçin Yüksek Hassasiyetli Sistem Zamanlayıcısı ve Akıllı Bellek Yöneticisi

<p align="center">
  <a href="README.md">English</a> • <b>Türkçe</b>
</p>

[![Release](https://img.shields.io/github/v/release/enixyazilim/MemStand?color=3b82f6&logo=github&style=flat-square)](https://github.com/enixyazilim/MemStand/releases)
[![Build Status](https://img.shields.io/github/actions/workflow/status/enixyazilim/MemStand/ci.yml?branch=main&label=CI&logo=githubactions&logoColor=white&style=flat-square)](https://github.com/enixyazilim/MemStand/actions)
[![Programlama Dili](https://img.shields.io/badge/C%2B%2B-20%20(ISO)-00599C?logo=c%2B%2B&style=flat-square)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011%20(x64)-0078D4?logo=windows&style=flat-square)](https://microsoft.com/windows)
[![Bağımlılık](https://img.shields.io/badge/Ba%C4%9F%C4%B1ml%C4%B1l%C4%B1k-S%C4%B1f%C4%B1r%20(Saf%20Win32)-10b981?style=flat-square)](#-teknik-özellikler)
[![Boyut](https://img.shields.io/badge/Dosya%20Boyutu-%3C%20500%20KB-success?style=flat-square)](#-teknik-özellikler)
[![RAM Tüketimi](https://img.shields.io/badge/RAM%20T%C3%BCketimi-~1.9%20MB-success?style=flat-square)](#-teknik-özellikler)
[![Lisans](https://img.shields.io/github/license/enixyazilim/MemStand?color=8b5cf6&style=flat-square)](LICENSE)

<br/>

**MemStand**, **[Enix Yazılım](https://github.com/enixyazilim)** tarafından saf ve modern **C++20** standartlarıyla sıfırdan geliştirilmiş, donanıma duyarlı akıllı bellek yönetimi sunan ve Windows sistem zamanlayıcısını **0.50 ms** seviyesine sabitleyerek rekabetçi oyunlarda giriş gecikmesini (input lag) ve mikro takılmaları (micro-stutter) ortadan kaldıran bağımsız, ultra hafif bir Windows optimizasyon aracıdır.

Sıkı bir **sıfır dış bağımlılık** ilkesiyle inşa edilen MemStand; .NET çalışma zamanı, WebView2, Electron veya üçüncü parti servislere ihtiyaç duymaz. Sistem tepsisinde arka planda çalışırken yalnızca **~1.9 MB RAM** ve **%0.00** CPU tüketir.

</div>

---

## ⚡ Öne Çıkan Özellikler

```
 ┌────────────────────────────────────────────────────────────────────────┐
 │                              MEMSTAND                                  │
 │                                                                        │
 │  [▶] BAŞLAT / DURDUR    Tek tıkla arka plan motorunu açıp kapatma      │
 │  [⚡] 0.50 ms Zamanlayıcı Milisaniye altı çekirdek hassasiyeti + bekçi │
 │  [🧠] Donanım Profili   RAM kapasitesine göre dinamik eşik hesabı      │
 │  [🎯] 2 Kademeli Temizlik Oyunları dondurmadan güvenli önbellek silme  │
 │  [📊] Canlı Tepsi İkonu Canlı renkli RAM yüzdesi (saat yanında)        │
 │  [🛡️] UAC'siz Başlatma   Görev Zamanlayıcısı ile sessiz yönetici yetkisi│
 │  [🌐] Yerel Güncelleyici Doğrudan GitHub sürümlerini sorgulama         │
 │  [🌍] Modüler Dil       JSON tabanlı tak-çalıştır topluluk çevirisi    │
 └────────────────────────────────────────────────────────────────────────┘
```

---

## 🥊 Neden MemStand? (Geleneksel Araçlarla Karşılaştırma)

| Özellik | Eski Nesil Araçlar (örn: ISLC) | OEM Programları (Razer vb.) | **MemStand (Enix Yazılım)** |
| :--- | :---: | :---: | :---: |
| **Yazılım Dili** | C# / .NET Framework | Electron / Chromium | **Saf C++20 (ISO Standardı)** |
| **Dış Bağımlılıklar** | .NET Runtime 4.x/6.x | Node.js, Chromium, Servisler | **Yok (Yerel Win32 & NT API)** |
| **Dosya Boyutu** | ~3 - 10 MB | 150 MB - 500 MB+ | **< 500 KB** |
| **RAM Tüketimi** | 40 MB - 80 MB | 200 MB - 800 MB | **~1.9 MB** |
| **Donanıma Duyarlı Eşikler** | ❌ Ezbere sabit sayılar | ❌ Agresif rastgele silme | **✅ Dinamik profil (RAM / 2)** |
| **Zamanlayıcı Bekçisi** | ⚠️ Basit döngü | ❌ Yok | **✅ Arka plan çekirdek koruması** |
| **UAC'siz Otomatik Başlatma**| ⚠️ Tutarsız görev ayarı | ❌ Ağır Windows servisi | **✅ Temiz Görev Zamanlayıcı** |
| **Özel Çizim Koyu Tema** | ❌ 90'lar WinForms stili | ⚠️ Web arayüzü / Hantal | **✅ Saf Win32 Deep Slate UI** |
| **Yerel Güncelleme Kontrolü** | ❌ Manuel web araması | ⚠️ Arka plan servis yükü | **✅ Yerel WinHTTP ile kontrol** |

---

## 🚀 Detaylı Yetenekler

### 1. ⚡ Donanıma Duyarlı Akıllı Bellek Profili
Kullanıcıyı ne yazacağını bilmediği sabit sayılarla baş başa bırakan eski araçların aksine, MemStand sistemdeki takılı RAM miktarını sorgular ve donanım kademenize (8 GB, 16 GB, 24 GB, 32 GB, 64 GB+) en uygun eşikleri otomatik oluşturur:
- **Standby Eşiği (MB):** Takılı belleğin %50'si olarak hesaplanır (`RAM / 2`). Böylece Windows'un faydalı dosya önbelleklemesi gereksiz yere bozulmaz; ancak önbellek şişip oyunları ve aktif uygulamaları sıkıştırmaya başladığı an devreye girer.
- **Boş RAM Eşiği (MB):** Sistemde her an nefes alacak geniş bir boş alanın kalmasını garanti eder.
- **`⚡ Önerilen Değerler` Butonu:** Ayarları kurcalasanız bile tek tıkla donanımınıza en uygun değerleri yeniden hesaplayıp kutulara yazdırabilirsiniz.

### 2. 🎮 0.50 ms Milisaniye Altı Zamanlayıcı + Canlı Koruma (Watchdog)
- Doğrudan NT çekirdek rutinlerini (`NtSetTimerResolution`) çağırarak sistem zamanlayıcısını standart 15.6 ms veya 1.00 ms yerine **0.50 ms** (100ns hassasiyetinde 5000) değerine kilitler.
- Rekabetçi espor oyunlarında (*Valorant, CS2, Apex Legends, Warzone, Fortnite*) kare süresi dalgalanmalarını (frametime variance), fare giriş gecikmesini ve mermi kayıt sorunlarını belirgin şekilde iyileştirir.
- **Canlı Koruma (Watchdog):** Windows'un veya arkada çalışan diğer yazılımların zamanlayıcıyı sessizce varsayılana çekmesini engellemek için periyodik olarak kontrol eder ve kilitli tutar. İsteğe bağlı özel hedefleri de destekler (0.50 ms, 0.75 ms, 1.00 ms, 15.62 ms).

### 3. 🧠 Çok Kademeli Uyarlanabilir Bellek Temizliği
- **1. Aşama (Normal / Oyun Yükü):** Yalnızca Standby List (Bekleme Listesi) güvenle temizlenir. Çalışan oyun ve programların bellekleri yerinde tutularak ani FPS düşüşleri ve takılmalar engellenir.
- **2. Aşama (Yüksek Bellek Baskısı):** Toplam RAM kullanımı ayarlanan sınırı (varsayılan: **%85**) aştığında, çalışma kümeleri ve sistem dosya önbelleği de hafifletilerek sistemin kilitlenmesi önlenir.

### 4. 📊 Canlı Renkli Sistem Tepsisi (Tray) Simgesi
- O anki gerçek zamanlı RAM kullanım yüzdesini doğrudan saat yanındaki sistem tepsisi simgesi üzerine canlı çizer.
- Renk kodlaması sayesinde bir bakışta sistem durumunu gösterir:
  - 🟢 **Zümrüt Yeşili:** Normal yük (< %70)
  - 🟡 **Kehribar Sarısı:** Orta yük (%70 - %85)
  - 🔴 **Kırmızı:** Yüksek bellek baskısı (> %85)
- Tepsi simgesine sağ tıklayarak ana pencereyi açmadan tek tıkla anında temizlik yapılabilir, zamanlayıcı durumu değiştirilebilir ve güncellemeler denetlenebilir.

### 5. 🛡️ UAC'siz Sessiz Otomatik Başlatma
- Windows Görev Zamanlayıcısı (`schtasks.exe` ve `HIGHEST` yetkisi) ile entegre çalışır.
- Her Windows açılışında rahatsız edici UAC (Kullanıcı Hesabı Denetimi) onay kutusu çıkarmadan yönetici haklarıyla sessizce başlar.

### 6. 🔄 Yerel Güncelleme Kontrolü
- Windows `winhttp.dll` kütüphanesi üzerinden GitHub Releases API'sini doğrudan sorgulayan yerel bir güncelleme denetleyicisi içerir.
- Arayüzü dondurmadan arka planda çalışır; yeni bir sürüm çıktığında tek tıkla indirme sayfasını açmanızı sağlar.

---

## 📥 İndirme ve Kurulum

MemStand **%100 taşınabilir (portable)** bir uygulamadır; kurulum veya kayıt defteri (registry) kaydı gerektirmez.

1. En güncel `MemStand-vX.X.X-windows-x64.zip` paketini [GitHub Sürümleri (Releases)](https://github.com/enixyazilim/MemStand/releases/latest) sayfasından indirin.
2. Arşivi dilediğiniz bir klasöre (örn: `C:\Araclar\MemStand` veya `Masaüstü`) çıkartın.
3. `MemStand.exe` dosyasını çalıştırın.
4. *(İsteğe bağlı)* **Ayarlar** sekmesinden **Windows Açılışında Başlat (UAC'siz)** ve **Sistem Tepsisine Küçültülmüş Başlat** seçeneklerini işaretleyerek sistemi tamamen otomatik hale getirebilirsiniz.

### Doğrulama (SHA-256)
Yayınlanan her sürümde bir `checksums.txt` dosyası yer alır. İndirdiğiniz dosyanın bütünlüğünü PowerShell ile doğrulayabilirsiniz:
```powershell
Get-FileHash -Path MemStand.exe -Algorithm SHA256
```

---

## 💻 Komut Satırı Arayüzü (CLI)

MemStand, oyun başlatıcıları veya toplu işlem (batch) betikleriyle entegre edilebilir:

| Parametre | Açıklama |
| :--- | :--- |
| `MemStand.exe -minimized` | Ana pencereyi açmadan doğrudan sistem tepsisine küçültülmüş olarak başlatır. |
| `MemStand.exe -purge` | Standby Listesini (Bekleme Listesini) anında temizler ve sessizce kapanır. |
| `MemStand.exe -clean` | Tüm bellek bölgelerini (Standby, Çalışma Kümeleri, Önbellek) temizler ve sessizce kapanır. |

---

## ⌨️ Global Kısayol Tuşu

- **`Ctrl + Shift + M`**: Tam ekran oyunların içindeyken bile oyundan çıkmadan anında bellek temizliği tetikler. Ayarlar sekmesinden açılıp kapatılabilir.

---

## 🌐 Topluluk Çevirileri (i18n)

MemStand Türkçe ve İngilizce dilleriyle hazır gelir; modüler JSON mimarisi sayesinde yeni diller kod derlemesi gerektirmeden eklenebilir:

1. `languages/template.json` dosyasını `languages/<dil-adiniz>.json` olarak kopyalayın (örn: `languages/german.json`).
2. Dosya içerisindeki metinleri kendi dilinize çevirin.
3. MemStand'i başlattığınızda yeni dil otomatik olarak **Arayüz Dili** menüsünde görünecektir.
4. Çevirinizi bir Pull Request açarak projeye kazandırabilirsiniz!

---

## 📊 Teknik Özellikler

| Parametre | Değer |
| :--- | :--- |
| **Yazılım Dili & Standart** | Saf C++ (ISO C++20) |
| **Mimari** | Yerel 64-bit (`x86_64`) |
| **Kullanılan API'ler** | Win32 API, NT Native API (`ntdll.dll`), WinHTTP (`winhttp.dll`) |
| **Dış Bağımlılıklar** | **Yok** (.NET, WebView2, Electron veya ek DLL gerekmez) |
| **Dosya Boyutu** | **~500 KB** (`MemStand.exe`) |
| **Bellek Tüketimi (RAM)** | **~1.9 MB** arka plan bellek kullanımı |
| **İşlemci Tüketimi (CPU)** | 1 saniyelik kontroller arasında **%0.00** |
| **Desteklenen İşletim Sistemleri** | Windows 10 (1809+ tüm sürümler), Windows 11 (tüm sürümler) |

---

## 🛠️ Kaynak Koddan Derleme

### Gereksinimler
- Windows 10 veya Windows 11 (x64)
- Visual Studio 2022 veya Visual Studio 2026 (*"C++ ile masaüstü geliştirme"* iş yükü kurulu)
- CMake 3.20 veya daha yenisi (isteğe bağlı)

### 1. Yöntem: Tek Tıkla Derleme Betiği
Proje kök dizininde şu komutu çalıştırın:
```cmd
build.bat
```
Derlenen bağımsız çalıştırılabilir dosya doğrudan `bin\MemStand.exe` konumunda oluşturulur.

### 2. Yöntem: CMake ile Derleme
```cmd
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## 🤝 Katkıda Bulunma

Hata bildirimleri, yeni özellik önerileri ve katkılarınız memnuniyetle karşılanır! Kod standartları ve PR işleyişi hakkında ayrıntılı bilgi için [CONTRIBUTING.md](CONTRIBUTING.md) dosyasını inceleyebilirsiniz.

---

## 📄 Lisans

Bu proje [MIT Lisansı](LICENSE) altında sunulmaktadır.  
Copyright © 2026 **[Enix Yazılım](https://github.com/enixyazilim)** ve MemStand Katkıda Bulunanları.
