@echo off
echo ============================================================
echo   VOID WEAVER - WebAssembly Compiler Guide
echo ============================================================
echo.
echo Compiling this game for the web requires the Emscripten SDK (emsdk).
echo If you haven't installed emsdk yet, follow these steps:
echo.
echo   1. git clone https://github.com/emscripten-core/emsdk.git
echo   2. cd emsdk
echo   3. .\emsdk install latest
echo   4. .\emsdk activate latest
echo   5. .\emsdk_env.bat  (adds emcc to your current terminal session)
echo.
echo Make sure you run this script in a terminal where emsdk is active!
echo.
pause

:: Check if emcc is available
where emcc >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: "emcc" compiler not found in PATH!
    echo Please run "emsdk_env.bat" inside your emsdk folder first.
    echo.
    pause
    exit /b 1
)

echo.
echo Compiling C++ game to WebAssembly...
echo.

:: Compilation command preloading all required assets
emcc -o index.html main.cpp -Wall -std=c++17 -DPLATFORM_WEB --shell-file shell.html -I raylib_wasm/include -L raylib_wasm/lib -lraylib -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap,HEAPF32,HEAPU8,HEAP32 --preload-file game_assets --preload-file front_page.png@front_page.png --preload-file system_screen.png@system_screen.png --preload-file desk_passcode.png@desk_passcode.png --preload-file uttt.png@uttt.png --preload-file vault.png@vault.png --preload-file mod_4_passcode.png@mod_4_passcode.png --preload-file room_key_picked.png@room_key_picked.png

if %ERRORLEVEL% equ 0 (
    echo.
    echo SUCCESS: WebAssembly build complete!
    echo.
    echo Generated files:
    echo   - index.html [the web page]
    echo   - index.js   [JavaScript runner]
    echo   - index.wasm [compiled WebAssembly binary]
    echo   - index.data [prepackaged game assets]
    echo.
    echo To play locally:
    echo   You cannot run index.html directly from your hard drive due to CORS security.
    echo   You must start a local web server [e.g. run "python -m http.server 8000" and visit http://localhost:8000]
    echo.
    echo To deploy to the internet:
    echo   Push these files to a GitHub repository and enable GitHub Pages in settings.
    echo.
) else (
    echo.
    echo ERROR: Compilation failed!
    echo.
)
pause
