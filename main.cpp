// ============================================================
// VOID WEAVER - A Narrative Puzzle Adventure Game
// Built with C++ and Raylib
// ============================================================
// This is the main entry point. The game is split across:
//   game_states.h  - All enums, structs, and constants
//   sounds.h       - Procedural sound/music generation
//   uttt.h         - Ultimate Tic-Tac-Toe game logic + AI
//   chess.h        - Chess puzzle logic with move validation
//   main_part1.h   - Globals, resource loading, helper functions
//   main_part2.h   - Title, Intro cutscenes, Module 1, Travel
//   main_part3.h   - Module 2 (TTT), Module 3 (Chess+Lift), Module 4
// ============================================================
//
// VIRTUAL RESOLUTION SYSTEM
// The game is designed at 1280x720. We render everything into a
// RenderTexture at that size, then scale it up (letterboxed) to fill
// the actual window/screen.
//
// GetMousePosition() is #define-d BEFORE including any game headers so
// that every hit-test in the game code transparently receives mouse
// coordinates in the virtual 1280x720 space.

#include "raylib.h"
#include <ctime>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// ----- Virtual resolution globals (forward-declared so the macro can use them) -----
static Vector2 gVirtualMouse = {0, 0};  // Updated each frame in virtual space
static float   gScaleX = 1.0f;
static float   gScaleY = 1.0f;
static Vector2 gOffset  = {0, 0};

// Override GetMousePosition() to return virtual-space coordinates.
// This macro is seen by all headers included below.
#undef  GetMousePosition
#define GetMousePosition() (gVirtualMouse)

// ----- Now include all game headers (they will use the macro above) -----
#include "main_part3.h"

// ===== VIRTUAL RESOLUTION HELPERS =====

static RenderTexture2D gRenderTarget;
static Rectangle       gDestRect;

// Recalculate scale & offset to fill screen and map mouse coordinates
static void UpdateVirtualScale() {
    int rw = GetScreenWidth();
    int rh = GetScreenHeight();
    if (rw <= 0) rw = SCREEN_W;
    if (rh <= 0) rh = SCREEN_H;

    gScaleX = (float)rw / (float)SCREEN_W;
    gScaleY = (float)rh / (float)SCREEN_H;
    gOffset = { 0.0f, 0.0f };
    gDestRect = { 0.0f, 0.0f, (float)rw, (float)rh };
}

// Translate real screen mouse position to virtual 1280x720 space.
// We temporarily undef our macro so we call the real raylib function.
#undef GetMousePosition
static Vector2 ComputeVirtualMouse() {
#if defined(PLATFORM_WEB)
    // On PLATFORM_WEB, Emscripten's HTML5 API converts browser DOM mouse events directly
    // into canvas pixel space (0..SCREEN_W, 0..SCREEN_H) using canvas.getBoundingClientRect().
    // Returning real GetMousePosition() directly avoids double-scaling division in WebAssembly!
    return GetMousePosition();
#else
    Vector2 real = GetMousePosition();   // real raylib call (macro is undef'd above)
    return { (real.x - gOffset.x) / gScaleX,
             (real.y - gOffset.y) / gScaleY };
#endif
}
// Re-apply the macro so nothing below this point calls the real GetMousePosition
#define GetMousePosition() (gVirtualMouse)

static bool gAudioOk = false;
void UpdateDrawFrame();

// ===== MAIN FUNCTION =====
int main() {
    // Seed random number generator for AI moves
    srand((unsigned)time(NULL));

    // Initialize window and audio.
    // FLAG_BORDERLESS_WINDOWED_MODE makes the window fill the monitor at startup.
    // The render-texture system then scales the 1280x720 virtual canvas
    // up to fit the actual screen resolution with correct aspect ratio.
#if defined(PLATFORM_WEB)
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
#else
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_BORDERLESS_WINDOWED_MODE);
#endif

    InitWindow(SCREEN_W, SCREEN_H, "VOID WEAVER - Detective Life in an AI Futuristic World");
    InitAudioDevice();
    // If audio device failed to init, wait briefly and retry once
    if (!IsAudioDeviceReady()) {
        WaitTime(0.5);
        CloseAudioDevice();
        InitAudioDevice();
    }
    gAudioOk = IsAudioDeviceReady();
    SetTargetFPS(60);

    // Load all scene images, sounds, and music
    LoadAllResources();

    // Start background music loop (only if audio initialized successfully)
    if (gAudioOk) {
        SetSoundVolume(bgMusic, 0.5f);
        PlaySound(bgMusic);
    }

    // Create the virtual 1280x720 render target
    gRenderTarget = LoadRenderTexture(SCREEN_W, SCREEN_H);
    SetTextureFilter(gRenderTarget.texture, TEXTURE_FILTER_BILINEAR);
    UpdateVirtualScale();

    // Initialize game state
    curState = STATE_TITLE;
    stateTimer = 0;
    animTime = 0;

    // ==========================================
    // MAIN GAME LOOP SETUP
    // ==========================================
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    // ===== CLEANUP =====
    UnloadRenderTexture(gRenderTarget);
    UnloadAllResources();
    CloseAudioDevice();
    CloseWindow();
#endif

    return 0;
}

// ===== FRAME UPDATE AND DRAWING =====
void UpdateDrawFrame() {
    float dt = GetFrameTime();
    stateTimer += dt;
    animTime += dt;

    // Loop background music when it finishes
    if (gAudioOk && !IsSoundPlaying(bgMusic) && !bgMusicMuted) PlaySound(bgMusic);

    // Recalculate scale on every frame (handles resize / fullscreen toggle)
    UpdateVirtualScale();

    // Update virtual mouse so all game-code calls to GetMousePosition()
    // (which expand to gVirtualMouse) return the correct virtual coords
    gVirtualMouse = ComputeVirtualMouse();

#if !defined(PLATFORM_WEB)
    // Toggle fullscreen with F11
    if (IsKeyPressed(KEY_F11)) {
        ToggleBorderlessWindowed();
    }
#endif

    // ===== RENDER GAME INTO VIRTUAL CANVAS (1280x720) =====
    BeginTextureMode(gRenderTarget);
    ClearBackground(BLACK);

    // ===== HANDLE FADE TRANSITION =====
    if (fading) {
        if (fadeOut) {
            // Fading to black
            fadeAlpha += dt * 2.0f;
            if (fadeAlpha >= 1.0f) {
                fadeAlpha = 1.0f;
                fadeOut = false;
                ChangeState(fadeTarget);
            }
        } else {
            // Fading back in
            fadeAlpha -= dt * 2.0f;
            if (fadeAlpha <= 0) {
                fadeAlpha = 0;
                fading = false;
            }
        }
    }

    // ===== HANDLE SLIDE TRANSITION =====
    if (sliding) {
        slideOff += dt * SCREEN_W * 1.5f;
        if (slideOff >= SCREEN_W) {
            slideOff = 0;
            sliding = false;
            ChangeState(fadeTarget);
        }
    }

    // ===== UPDATE AND DRAW CURRENT STATE =====
    // Route to the appropriate module handler based on current state

    if (curState == STATE_TITLE) {
        // Title screen with START and EXIT buttons
        UpdateTitle();
    }
    else if (curState >= STATE_INTRO_1 && curState <= STATE_INTRO_14) {
        // Introduction cutscenes - backstory narration
        UpdateIntro();
    }
    else if (curState >= STATE_M1_HOUSE && curState <= STATE_M1_MAP) {
        // Module 1: Escape room in scientist's house
        UpdateM1();
    }
    else if (curState >= STATE_TRAVEL_FLIGHT && curState <= STATE_TRAVEL_GARAGE) {
        // Travel sequence between modules
        UpdateTravel();
    }
    else if (curState >= STATE_M2_GARAGE && curState <= STATE_M2_VEHICLE) {
        // Module 2: Garage and Ultimate Tic-Tac-Toe
        UpdateM2();
    }
    else if (curState >= STATE_M3_COCKPIT && curState <= STATE_M3_LIFT_DESC) {
        // Module 3: Vehicle journey, Chess puzzle, and Lift
        UpdateM3();
    }
    else if (curState >= STATE_M4_SUBMARINE && curState <= STATE_M4_ENDING) {
        // Module 4: Passlock vault, AI machine, and Ending
        UpdateM4();
    }

    // ===== DRAW SLIDE TRANSITION OVERLAY =====
    if (sliding) {
        // Black bar sliding across screen
        if (slideDir == 1) // Slide left
            DrawRectangle(SCREEN_W - (int)slideOff, 0, (int)slideOff, SCREEN_H, BLACK);
        else if (slideDir == 3) // Slide up
            DrawRectangle(0, SCREEN_H - (int)slideOff, SCREEN_W, (int)slideOff, BLACK);
    }

    // ===== DRAW FADE TRANSITION OVERLAY =====
    if (fading) {
        DrawRectangle(0, 0, SCREEN_W, SCREEN_H,
            (Color){0, 0, 0, (unsigned char)(fadeAlpha * 255)});
    }

    EndTextureMode();

    // ===== BLIT VIRTUAL CANVAS ONTO REAL SCREEN =====
    BeginDrawing();
    ClearBackground(BLACK);
    // RenderTexture is stored upside-down in OpenGL convention: negate height to flip
    Rectangle srcRect = { 0, 0, (float)SCREEN_W, -(float)SCREEN_H };
    DrawTexturePro(gRenderTarget.texture, srcRect, gDestRect, {0,0}, 0.0f, WHITE);
    EndDrawing();
}

