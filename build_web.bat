@echo off
call C:\Personal\GitHub\emsdk\emsdk_env.bat
emcc -o index.html main.cpp -Wall -std=c++17 -DPLATFORM_WEB --shell-file shell.html -I raylib_wasm/include -L raylib_wasm/lib -lraylib -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap,HEAPF32,HEAPU8,HEAP32 --preload-file game_assets --preload-file front_page.png --preload-file system_screen.png --preload-file desk_passcode.png --preload-file uttt.png --preload-file vault.png --preload-file mod_4_passcode.png --preload-file room_key_picked.png
