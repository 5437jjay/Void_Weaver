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

#include "main_part3.h"

// ===== MAIN FUNCTION =====
int main() {
    // Seed random number generator for AI moves
    srand((unsigned)time(NULL));

    // Initialize window and audio - start in borderless fullscreen to fill laptop screen
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_BORDERLESS_WINDOWED_MODE);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "VOID WEAVER - Detective Life in an AI Futuristic World");
    InitAudioDevice();
    SetTargetFPS(60);

    // Load all scene images, sounds, and music
    LoadAllResources();

    // Start background music loop
    SetSoundVolume(bgMusic, 0.5f);
    PlaySound(bgMusic);

    // Initialize game state
    curState = STATE_TITLE;
    stateTimer = 0;
    animTime = 0;

    // ==========================================
    // MAIN GAME LOOP
    // ==========================================
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        stateTimer += dt;
        animTime += dt;

        // Loop background music when it finishes
        if (!IsSoundPlaying(bgMusic) && !bgMusicMuted) PlaySound(bgMusic);

        // Toggle fullscreen with F11
        if (IsKeyPressed(KEY_F11)) {
            ToggleBorderlessWindowed();
        }

        BeginDrawing();
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

        EndDrawing();
    }

    // ===== CLEANUP =====
    UnloadAllResources();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
