@echo off
echo ============================================================
echo   VOID WEAVER - Full Setup, Asset Process and Run Script
echo ============================================================
echo.

set "TARGET_LINK=%~dp0raylib"
set "SOURCE_DIR=C:\OOP Project\raylib"

:: 1. Setup Raylib Path Link if it doesn't exist
if not exist "%TARGET_LINK%" (
    echo [1/3] Creating virtual link to Raylib...
    if not exist "%SOURCE_DIR%" (
        echo ERROR: Could not find Raylib at "%SOURCE_DIR%"
        echo Please ensure you have Raylib in "C:\OOP Project\raylib"
        echo.
        pause
        exit /b 1
    )
    mklink /J "%TARGET_LINK%" "%SOURCE_DIR%"
    if %ERRORLEVEL% neq 0 (
        echo ERROR: Failed to create virtual link. Please try running as Administrator.
        echo.
        pause
        exit /b 1
    )
    echo SUCCESS: Linked Raylib folder!
) else (
    echo [1/3] Raylib link already exists. Skipping link creation.
)
echo.

:: 2. Replace Front Page Image
echo [2/3] Replacing Title Screen Image in high quality...
if exist "%~dp0replace_front_page.py" (
    python "%~dp0replace_front_page.py"
) else (
    echo WARNING: replace_front_page.py not found! Image replacement skipped.
)
echo.

:: 3. Compile the Game
echo [3/3] Compiling C++ game using linked Raylib...
if exist "%~dp0game.exe" (
    del "%~dp0game.exe" 2>nul
)

g++ -o game.exe main.cpp -I raylib/include -L raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm

if %ERRORLEVEL% equ 0 (
    echo.
    echo SUCCESS: Game compiled successfully with the new title image!
    echo Launching Void Weaver...
    echo.
    game.exe
) else (
    echo.
    echo ERROR: Compilation failed! Please read the C++ compiler errors above.
    echo.
    pause
)
