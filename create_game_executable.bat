@echo off
setlocal
echo ============================================================
echo   VOID WEAVER - Creating Standalone Game for Distribution
echo ============================================================
echo.

set "RELEASE_DIR=VoidWeaver_Release"

:: 1. Create Release Directory
echo [1/6] Creating Release Folder...
if exist "%RELEASE_DIR%" rmdir /s /q "%RELEASE_DIR%"
mkdir "%RELEASE_DIR%"

:: 2. Copy the AI generated icon
echo [2/6] Getting the game icon...
copy "C:\Users\acer\.gemini\antigravity\brain\4c620324-8c9f-4943-bf42-52985087bce5\void_weaver_icon_1779117281806.png" "void_weaver_icon.png" >nul

:: 3. Create python script to convert PNG to ICO
echo [3/6] Converting icon format (requires Python)...
echo import sys > convert_icon.py
echo try: >> convert_icon.py
echo     from PIL import Image >> convert_icon.py
echo except ImportError: >> convert_icon.py
echo     import subprocess >> convert_icon.py
echo     print("Installing Pillow for icon conversion...") >> convert_icon.py
echo     subprocess.check_call([sys.executable, "-m", "pip", "install", "Pillow"]) >> convert_icon.py
echo     from PIL import Image >> convert_icon.py
echo img = Image.open("void_weaver_icon.png") >> convert_icon.py
echo img.save("icon.ico", format="ICO", sizes=[(256, 256), (128, 128), (64, 64), (32, 32)]) >> convert_icon.py

python convert_icon.py
del convert_icon.py

:: 4. Create Icon Resource File
echo [4/6] Creating Icon Resource...
echo id ICON "icon.ico" > icon.rc
windres icon.rc -O coff -o icon.res

:: 5. Compile the Game directly with the icon
echo [5/6] Compiling Game Executable...
:: Ensure Raylib exists
if not exist "raylib" (
    echo ERROR: Raylib folder not found in current directory! Please run run_everything.bat first to set it up.
    pause
    exit /b 1
)

:: Compile with static flags for MinGW so it doesn't need libstdc++ dlls on other PCs
g++ -o "%RELEASE_DIR%\Void Weaver.exe" main.cpp icon.res -I raylib/include -L raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++

if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: Compilation failed!
    pause
    exit /b 1
)

:: 6. Copy Assets and DLLs to Release Folder
echo [6/6] Copying Assets and required DLLs to Release folder...
xcopy /E /I /Y "assets" "%RELEASE_DIR%\assets" >nul
xcopy /E /I /Y "game_assets" "%RELEASE_DIR%\game_assets" >nul

:: Copy raylib.dll if it exists (sometimes needed depending on how g++ links)
if exist "raylib\lib\raylib.dll" (
    copy "raylib\lib\raylib.dll" "%RELEASE_DIR%\" >nul
)

:: Copy audio if it's outside assets
if exist "bgmusic.wav" (
    copy "bgmusic.wav" "%RELEASE_DIR%\" >nul
)

:: Copy any image files from the root directory (like front page.jpeg, room_key_picked.png, etc)
copy "*.png" "%RELEASE_DIR%\" >nul 2>&1
copy "*.jpeg" "%RELEASE_DIR%\" >nul 2>&1
copy "*.jpg" "%RELEASE_DIR%\" >nul 2>&1

:: Cleanup temporary icon files
del icon.rc
del icon.res

echo.
echo ============================================================
echo SUCCESS! 
echo.
echo The game has been compiled into a standalone version!
echo You will find a new folder named "VoidWeaver_Release" in your project directory.
echo.
echo IMPORTANT: To share the game via WhatsApp, you must ZIP the ENTIRE "VoidWeaver_Release" folder.
echo Do not just send the .exe file, as it needs the assets folder to run!
echo.
echo 1. Right-click the "VoidWeaver_Release" folder
echo 2. Select "Compress to ZIP file" (or Send to -^> Compressed (zipped) folder)
echo 3. Send the resulting .zip file via WhatsApp!
echo ============================================================
pause
