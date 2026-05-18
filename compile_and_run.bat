@echo off
echo ============================================================
echo   VOID WEAVER - Compile and Play Game
echo ============================================================
echo.

:: Check if raylib folder exists in the project
if not exist "%~dp0raylib\include\raylib.h" (
    echo ERROR: Raylib path not found!
    echo Please run "setup_raylib_path.bat" first to link your Raylib files.
    echo.
    pause
    exit /b 1
)

echo Compiling game...
g++ -o game.exe main.cpp -I raylib/include -L raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm

if %ERRORLEVEL% equ 0 (
    echo.
    echo SUCCESS: Game compiled successfully!
    echo Running game...
    echo.
    game.exe
) else (
    echo.
    echo ERROR: Compilation failed! Please review the errors above.
    echo.
    pause
)
