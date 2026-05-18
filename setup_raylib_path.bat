@echo off
echo ============================================================
echo   VOID WEAVER - Raylib Link Setup Script
echo ============================================================
echo.
echo This script will create a link (Junction) from your existing
echo Raylib folder in "C:\OOP Project\raylib" to this project folder.
echo This will fix the "raylib.h not found" compilation error.
echo.

set "TARGET_LINK=%~dp0raylib"
set "SOURCE_DIR=C:\OOP Project\raylib"

:: Check if source folder exists
if not exist "%SOURCE_DIR%" (
    echo ERROR: Could not find Raylib at "%SOURCE_DIR%"
    echo Please make sure the folder "C:\OOP Project\raylib" exists.
    pause
    exit /b 1
)

:: If a link already exists, remove it first
if exist "%TARGET_LINK%" (
    echo A link or folder named "raylib" already exists here. Removing it first...
    rmdir "%TARGET_LINK%" 2>nul
)

echo Creating link...
mklink /J "%TARGET_LINK%" "%SOURCE_DIR%"

if %ERRORLEVEL% equ 0 (
    echo.
    echo SUCCESS: Linked Raylib files successfully!
    echo You can now compile the game using your exact command:
    echo   g++ -o game.exe main.cpp -I raylib/include -L raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
) else (
    echo.
    echo FAILED to create link. Please make sure you are running as Administrator if needed.
)
echo.
pause
