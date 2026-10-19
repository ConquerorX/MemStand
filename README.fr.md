<div align="center">

# ⚡ MemStand

### Timer système haute précision et optimiseur de mémoire intelligent pour Windows

<p align="center">
  <a href="README.md">English</a> • <a href="README_TR.md">Türkçe</a> • <b>Français</b> • <a href="README.de.md">Deutsch</a> • <a href="README.es.md">Español</a> • <a href="README.it.md">Italiano</a> • <a href="README.pt-BR.md">Português (Brasil)</a> • <a href="README.ru.md">Русский</a>
</p>

[![Release](https://img.shields.io/github/v/release/enixyazilim/MemStand?color=3b82f6&logo=github&style=flat-square)](https://github.com/enixyazilim/MemStand/releases)
[![Build Status](https://img.shields.io/github/actions/workflow/status/enixyazilim/MemStand/ci.yml?branch=main&label=CI&logo=githubactions&logoColor=white&style=flat-square)](https://github.com/enixyazilim/MemStand/actions)
[![Language](https://img.shields.io/badge/C%2B%2B-20%20(ISO)-00599C?logo=c%2B%2B&style=flat-square)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011%20(x64)-0078D4?logo=windows&style=flat-square)](https://microsoft.com/windows)
[![Dependencies](https://img.shields.io/badge/Dependencies-Zero%20(Pure%20Win32)-10b981?style=flat-square)](#-spécifications-techniques)
[![Binary Size](https://img.shields.io/badge/Binary%20Size-%3C%20500%20KB-success?style=flat-square)](#-spécifications-techniques)
[![RAM Footprint](https://img.shields.io/badge/RAM%20Usage-~1.9%20MB-success?style=flat-square)](#-spécifications-techniques)
[![License](https://img.shields.io/badge/License-MIT-8b5cf6?style=flat-square)](LICENSE)

<br/>

**MemStand** est un optimiseur de système Windows moderne, ultra-léger et autonome, conçu en **C++20** pur par **[Enix Yazılım](https://github.com/enixyazilim)**. Il verrouille la résolution de l'horloge noyau globale de Windows à **0,50 ms** pour supprimer la latence de la souris, les micro-à-coups et les variations de temps de frame en jeu compétitif, tout en fournissant une gestion de la mémoire automatisée et adaptée au matériel.

Construit selon une philosophie stricte de **zéro dépendance externe**, MemStand ne requiert ni runtime .NET, ni WebView2, ni Electron, ni service en arrière-plan. Il fonctionne discrètement dans la zone de notification avec une mémoire de travail au repos d'environ **1,9 Mo** et une utilisation CPU de **0,00 %**.

</div>

---

## ⚡ Points forts

| Fonctionnalité | Description |
| :--- | :--- |
| **[▶] Interrupteur principal** | Démarrage/arrêt instantané en un clic du moteur en arrière-plan |
| **[⚡] Timer 0,50 ms** | Verrouillage de la résolution noyau sous la milliseconde + chien de garde |
| **[🧠] Profilage automatique** | Calcule dynamiquement les seuils optimaux à partir de la RAM physique |
| **[🎯] Purge de la liste de veille** | Purge sûre du cache de la liste de veille sans figer les jeux |
| **[📊] Zone de notification dynamique** | Pourcentage de RAM en direct, codé par couleur, dans la barre des tâches |
| **[🛡️] Démarrage sans UAC** | Ouverture de session administrative silencieuse via l'API Planificateur de tâches |
| **[🌐] Mise à jour native** | Vérifie les versions publiées sur GitHub directement via WinHTTP |
| **[🌍] i18n modulaire** | Traductions de la communauté ajoutables à chaud (basées sur JSON) |

---

## 🥊 Pourquoi MemStand ? (comparaison avec les utilitaires historiques)

| Fonctionnalité | Outils historiques (ex. ISLC) | Suites OEM (Razer, etc.) | **MemStand (Enix Yazılım)** |
| :--- | :---: | :---: | :---: |
| **Langage de programmation** | C# / .NET Framework | Electron / Chromium | **C++20 pur (norme ISO)** |
| **Dépendances externes** | Runtime .NET 4.x/6.x | Node.js, Chromium, services | **Aucune (Win32 natif et API NT)** |
| **Taille de l'exécutable** | ~3 - 10 Mo | 150 Mo - 500 Mo+ | **< 500 Ko** |
| **Mémoire au repos** | 40 Mo - 80 Mo | 200 Mo - 800 Mo | **~1,9 Mo** |
| **Seuils adaptés au matériel** | ❌ Valeurs statiques codées en dur | ❌ Nettoyages aveugles agressifs | **✅ Profilage dynamique (RAM / 2)** |
| **Chien de garde du timer** | ⚠️ Sondage basique | ❌ Aucun | **✅ Réaffirmation continue de la résolution noyau** |
| **Ouverture élevée sans UAC** | ⚠️ Création de tâche incohérente | ❌ Lance un service lourd | **✅ Intégration propre au Planificateur de tâches** |
| **Mode sombre natif moderne** | ❌ WinForms des années 90 | ⚠️ Rendu web / Lourd | **✅ Win32 Deep Slate dessiné à la main** |
| **Vérificateur de mise à jour intégré** | ❌ Recherche manuelle sur le web | ⚠️ Mise à jour automatique en arrière-plan | **✅ Vérificateur de versions natif via WinHTTP** |

---

## 🚀 Détail : capacités clés

### 1. ⚡ Profilage mémoire adapté au matériel
Contrairement aux outils historiques qui demandent à l'utilisateur de deviner des valeurs arbitraires en mégaoctets, MemStand inspecte la RAM physique installée au démarrage et calcule les seuils optimaux :
- **Seuil de veille (Mo) :** réglé automatiquement à 50 % de la mémoire installée (`RAM / 2`). Cela préserve le cache de fichiers de Windows, utile en fonctionnement normal, tout en garantissant que les pages de veille surnuméraires ne privent jamais de mémoire les jeux en cours.
- **Marge de sécurité RAM libre :** s'adapte dynamiquement à votre matériel (8 Go, 16 Go, 24 Go, 32 Go, 64 Go+) pour éviter l'épuisement de la mémoire.
- **Bouton `⚡ Calculer automatiquement` :** restaure ou recalcule à tout moment les paramètres recommandés pour votre matériel, en un clic.

### 2. 🎮 Timer noyau sous la milliseconde à 0,50 ms + chien de garde
- Appelle les routines natives du noyau NT (`NtSetTimerResolution`) en les associant aux périodes de timer multimédia, afin de ramener la résolution de 15,6 ms ou 1,00 ms à **0,50 ms** (5 000 unités en précision de 100 ns).
- Aplatrit nettement les variations de temps de frame, réduit le jitter de sondage de la souris et améliore la réactivité d'enregistrement des tirs dans les titres compétitifs (*Valorant, CS2, Apex Legends, Warzone, Fortnite*).
- **Chien de garde actif :** réaffirme en continu la résolution en arrière-plan pour empêcher Windows ou un logiciel tiers de la remettre silencieusement à sa valeur par défaut. Cibles personnalisables (0,50 ms, 0,75 ms, 1,00 ms, 15,62 ms).

### 3. 🧠 Nettoyage mémoire adaptatif à plusieurs niveaux
- **Niveau 1 (charge normale / en jeu) :** seule la liste de veille est purgée en toute sécurité. Les espaces de travail des processus actifs restent chauds en cache, garantissant zéro micro-à-coup ni perte d'image.
- **Niveau 2 (forte pression mémoire) :** lorsque la RAM globale dépasse le seuil configuré (par défaut : **85 %**), les espaces de travail et le cache de fichiers système sont allégés en douceur pour éviter les blocages par manque de mémoire.

### 4. 📊 Badge dynamique dans la zone de notification
- Dessine le pourcentage d'utilisation de la RAM en temps réel directement sur l'icône de notification de la barre des tâches, via un double tampon GDI personnalisé.
- Codage couleur en un coup d'œil :
  - 🟢 **Vert émeraude :** charge normale (< 70 %)
  - 🟡 **Orange ambre :** charge modérée (70 % - 85 %)
  - 🔴 **Rouge carmin :** forte pression (> 85 %)
- Le menu contextuel de la zone de notification permet les purges manuelles en un clic, le basculement du timer et la vérification des mises à jour, sans ouvrir la fenêtre principale.

### 5. 🛡️ Démarrage élevé sans UAC
- Enregistre une tâche planifiée Windows native (`schtasks.exe` avec le niveau d'exécution `HIGHEST`).
- Se lance automatiquement à l'ouverture de session avec les privilèges administrateur — supprimant complètement les invites UAC de Windows au démarrage.

### 6. 🔄 Vérificateur natif de versions et de mises à jour
- Un vérificateur de mises à jour léger et intégré interroge l'API GitHub Releases via `winhttp.dll`.
- Vérifie les étiquettes de version de manière asynchrone, sans bloquer l'interface utilisateur ni nécessiter de bibliothèques tierces.
- Accès en un clic au téléchargement des nouvelles versions et consultation des notes de version.

---

## 📥 Téléchargement et installation

MemStand est **100 % portable**. Aucun installateur ni modification du registre n'est requis.

1. Téléchargez la dernière `MemStand-vX.X.X-windows-x64.zip` depuis [GitHub Releases](https://github.com/enixyazilim/MemStand/releases/latest).
2. Extrayez l'archive dans le dossier de votre choix (par exemple `C:\Tools\MemStand` ou `Bureau`).
3. Lancez `MemStand.exe`.
4. *(Facultatif)* Dans l'onglet **Paramètres**, cochez **Démarrer avec Windows (sans UAC)** et **Démarrer réduit dans la zone de notification** pour un fonctionnement en arrière-plan transparent.

### Vérification (SHA-256)
Chaque version inclut un fichier `checksums.txt`. Vous pouvez vérifier l'intégrité de votre téléchargement avec PowerShell :
```powershell
Get-FileHash -Path MemStand.exe -Algorithm SHA256
```

---

## 💻 Interface de ligne de commande (CLI)

MemStand accepte des paramètres de ligne de commande pour les scripts, les lanceurs de jeux et les tâches batch automatisées :

| Argument | Description |
| :--- | :--- |
| `MemStand.exe -minimized` | Se lance discrètement dans la zone de notification sans afficher la fenêtre. |
| `MemStand.exe -purge` | Purg immédiatement la liste de veille et se termine discrètement. |
| `MemStand.exe -clean` | Exécute un balayage mémoire complet (veille, espaces de travail, cache) et se termine discrètement. |

---

## ⌨️ Raccourci global

- **`Ctrl + Shift + M`** : déclenche une purge mémoire immédiate depuis n'importe quel jeu ou application en plein écran, sans basculer sur le bureau. Activable ou désactivable dans l'onglet Paramètres.

### 🎹 Raccourci configurable

Vous n'êtes pas obligé de garder `Ctrl + Shift + M`. Cochez **« Raccourci global de nettoyage »** dans l'onglet Paramètres et cliquez sur le bouton placé à côté : l'enregistreur s'arme (sa bordure passe à l'ambre et le libellé passe à *Enregistrement...*), puis appuyez sur la combinaison de votre choix.

- Appuyez sur **`Échap`** pendant l'enregistrement pour annuler, ou sur **`Suppr`** pour restaurer le `Ctrl + Shift + M` intégré.
- Au moins l'un de `Ctrl`, `Alt`, `Maj` ou `Win` est requis, afin que l'application ne capture jamais une touche isolée.
- Le choix n'est appliqué que lorsque vous appuyez sur **Enregistrer les paramètres**, et il survit aux redémarrages.
- Si une autre application possède déjà cette combinaison, MemStand vous le signale au lieu d'échouer en silence, et vous pouvez en choisir une autre.
- Le répétition automatique des touches est supprimée : maintenir la combinaison n'enfile donc pas de purges successives.

---

## 🌐 Traductions de la communauté (i18n)

MemStand est fourni avec **8 langues d'interface** — English, Türkçe, Deutsch, Español, Français, Italiano, Português (Brasil) et Русский — et accepte un nombre illimité de traductions de la communauté via des fichiers JSON modulaires.

La liste est triée alphabétiquement d'après le nom de la langue, et chaque fichier du dossier `languages/` est détecté automatiquement au démarrage.

1. Copiez `languages/template.json` vers `languages/<votre-langue>.json` (par exemple `languages/french.json`).
2. Traduisez les valeurs du bloc `"strings"`. Gardez les libellés courts : l'écran des paramètres utilise des colonnes de largeur fixe, un libellé trop long chevauche le champ voisin.
3. Redémarrez MemStand — votre langue apparaît immédiatement dans le menu **Langue**.
4. Ouvrez une Pull Request pour partager votre traduction avec la communauté !

---

## 📊 Spécifications techniques

| Paramètre | Spécification |
| :--- | :--- |
| **Langage et norme** | C++ pur (ISO C++20) |
| **Architecture** | Natif 64 bits (`x86_64`) |
| **API utilisées** | API Win32, API Native NT (`ntdll.dll`), WinHTTP (`winhttp.dll`) |
| **Dépendances externes** | **Aucune** (zéro dépendance à l'exécution, binaire autonome) |
| **Taille du binaire** | **~500 Ko** (`MemStand.exe`) |
| **Consommation mémoire** | **~1,9 Mo** au repos |
| **Utilisation CPU** | **0,00 %** entre deux cycles de sondage d'une seconde |
| **OS pris en charge** | Windows 10 (toutes les éditions 1809+), Windows 11 (toutes les éditions) |

---

## 🛠️ Compiler depuis les sources

### Prérequis
- Windows 10 ou Windows 11 (x64)
- Visual Studio 2022 ou Visual Studio 2026 avec la charge de travail *« Développement de bureau en C++ »*
- CMake 3.20 ou plus récent (facultatif, si compilation via CMake)

### Méthode 1 : script de compilation en un clic
Lancez le script de build du répertoire racine :
```cmd
build.bat
```
L'exécutable autonome compilé est produit dans `bin\MemStand.exe`.

### Méthode 2 : CMake
```cmd
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## 🤝 Contribuer

Contributions, rapports de bogues et suggestions sont les bienvenus ! Consultez [CONTRIBUTING.md](CONTRIBUTING.md) pour les règles détaillées sur le style de code, les conventions d'architecture et le workflow des Pull Requests.

---

## 📄 Licence

Ce projet est distribué sous la [licence MIT](LICENSE).  
Copyright © 2026 **[Enix Yazılım](https://github.com/enixyazilim)** et les contributeurs de MemStand.
