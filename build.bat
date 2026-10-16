@echo off
setlocal enabledelayedexpansion

echo ==============================================
echo        MemStand C++20 Build System
echo ==============================================

rem 1. Check if cl.exe is already in PATH
where cl.exe >nul 2>&1
if %ERRORLEVEL% equ 0 goto compile

rem 2. Try VS 2026 / 18
if exist "%ProgramFiles%\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "%ProgramFiles%\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
    goto compile
)

if exist "%ProgramFiles%\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    call "%ProgramFiles%\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvars64.bat"
    goto compile
)

if exist "%ProgramFiles%\Microsoft Visual Studio\18\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
    call "%ProgramFiles%\Microsoft Visual Studio\18\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    goto compile
)

rem 3. Try VS 2022 / 17
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    goto compile
)

rem 4. Try vswhere.exe
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    if exist "%%i\VC\Auxiliary\Build\vcvars64.bat" (
        call "%%i\VC\Auxiliary\Build\vcvars64.bat"
        goto compile
    )
)

echo ERROR: Visual Studio MSVC x64 build tools not found!
exit /b 1

:compile
echo [1/3] Compiling Resources...
if not exist "bin" mkdir bin
rc.exe /nologo /fo bin\MemStand.res resources\MemStand.rc
if %ERRORLEVEL% neq 0 (
    echo Resource compilation failed!
    exit /b %ERRORLEVEL%
)

echo [2/3] Compiling C++ Sources (C++20 Release)...
cl.exe /nologo /O2 /std:c++20 /utf-8 /EHsc /MT /W3 /DUNICODE /D_UNICODE /DNDEBUG ^
    /I. /Isrc /Iresources ^
    src\main.cpp ^
    src\core\memory_manager.cpp ^
    src\core\timer_resolution.cpp ^
    src\core\automation_engine.cpp ^
    src\core\config_manager.cpp ^
    src\core\task_scheduler.cpp ^
    src\core\update_checker.cpp ^
    src\i18n\localization.cpp ^
    src\ui\tray_icon.cpp ^
    src\ui\main_window.cpp ^
    bin\MemStand.res ^
    /Fe:bin\MemStand.exe ^
    /link /SUBSYSTEM:WINDOWS ^
    user32.lib gdi32.lib comctl32.lib shell32.lib dwmapi.lib shlwapi.lib psapi.lib advapi32.lib winmm.lib winhttp.lib

if %ERRORLEVEL% neq 0 (
    echo Compilation failed!
    exit /b %ERRORLEVEL%
)

echo [3/3] Copying languages folder to bin...
if not exist "bin\languages" mkdir bin\languages
xcopy /Y /Q languages\*.json bin\languages\ >nul

rem Clean obj files
del /Q bin\*.obj >nul 2>&1
del /Q *.obj >nul 2>&1

echo ==============================================
echo  BUILD SUCCESS: bin\MemStand.exe
echo ==============================================
