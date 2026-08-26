@echo off
echo ============================================================
echo   VOID WEAVER - Setting up Raylib for WebAssembly
echo ============================================================
echo.
echo This script will download and extract the precompiled 
echo WebAssembly version of Raylib (v5.0) from GitHub.
echo.

:: Download using PowerShell
echo Downloading raylib WebAssembly zip...
powershell -Command "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_webassembly.zip' -OutFile 'raylib_wasm.zip'"

if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to download Raylib WebAssembly zip!
    pause
    exit /b 1
)

:: Extract using PowerShell
echo Extracting files...
powershell -Command "Expand-Archive -Path 'raylib_wasm.zip' -DestinationPath 'raylib_wasm_temp' -Force"

:: Create directories
if not exist "raylib_wasm" mkdir "raylib_wasm"
if not exist "raylib_wasm\include" mkdir "raylib_wasm\include"
if not exist "raylib_wasm\lib" mkdir "raylib_wasm\lib"

:: Copy files (find them recursively inside the extracted folder)
echo Copying WebAssembly libraries and headers...
powershell -Command "Copy-Item (Get-ChildItem -Path raylib_wasm_temp -Recurse -Filter raylib.h).FullName -Destination raylib_wasm/include"
powershell -Command "Copy-Item (Get-ChildItem -Path raylib_wasm_temp -Recurse -Filter raymath.h).FullName -Destination raylib_wasm/include"
powershell -Command "Copy-Item (Get-ChildItem -Path raylib_wasm_temp -Recurse -Filter rlgl.h).FullName -Destination raylib_wasm/include"
powershell -Command "Copy-Item (Get-ChildItem -Path raylib_wasm_temp -Recurse -Filter libraylib.a).FullName -Destination raylib_wasm/lib"

:: Clean up temporary files
echo Cleaning up temporary files...
del "raylib_wasm.zip" 2>nul
rmdir /s /q "raylib_wasm_temp" 2>nul

echo.
echo ============================================================
echo SUCCESS: Raylib for WebAssembly is set up in 'raylib_wasm/'
echo ============================================================
pause
