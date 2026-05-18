// ============================================================
// VOID WEAVER - Game States and Data Structures
// A Narrative Adventure Game built with C++ and Raylib
// ============================================================
#ifndef GAME_STATES_H
#define GAME_STATES_H

#include "raylib.h"
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>

// ----- Screen dimensions -----
const int SCREEN_W = 1280;
const int SCREEN_H = 720;
const int TOTAL_SCENES = 60;  // Total scene images from PDF + room_key_picked

// ----- All possible game states -----
// Each state corresponds to a specific screen/scene in the game
enum GameState {
    // Title Screen
    STATE_TITLE = 0,

    // Introduction Cutscenes (14 scenes showing the backstory)
    STATE_INTRO_1,   // President thinks about AI (scene_01)
    STATE_INTRO_2,   // President meets Ethan (scene_02)
    STATE_INTRO_3,   // Ethan typing, accepts project (scene_03)
    STATE_INTRO_4,   // Ethan celebrates AI completion (scene_04)
    STATE_INTRO_5,   // Ethan realizes AI is dangerous (scene_05)
    STATE_INTRO_6,   // Ethan tries to erase source code (scene_06)
    STATE_INTRO_7,   // Lab explodes, Ethan dies (scene_07)
    STATE_INTRO_8,   // Supreme AI description (scene_08)
    STATE_INTRO_9,   // ICJ meeting about AI threats (scene_09)
    STATE_INTRO_10,  // James tries calling Ethan (scene_10)
    STATE_INTRO_11,  // James plans to send an agent (scene_11)
    STATE_INTRO_12,  // Agent Zero drinking tea (scene_12)
    STATE_INTRO_13,  // Red indicator appears on table (scene_13)
    STATE_INTRO_14,  // Agent reads mission briefing (scene_14)

    // Module 1: Escape Room - Scientist's House
    STATE_M1_HOUSE,       // House exterior with door (scene_15)
    STATE_M1_ROOM_BASE1,  // Room interior, computer OFF (scene_16)
    STATE_M1_COMPUTER,    // Computer passcode screen (scene_17)
    STATE_M1_ROOM_BASE2,  // Room interior, computer ON (scene_18)
    STATE_M1_BOOK,        // Book "Machine Intelligence" (scene_19)
    STATE_M1_DESK_PASS,   // Desk passcode rollers (scene_20)
    STATE_M1_DESK_ITEMS,  // Desk contents floating (scene_21)
    STATE_M1_PHOTO_KEY,   // Photo fallen, key visible (scene_22)
    STATE_M1_MIRROR,      // Mirror close-up (scene_23)
    STATE_M1_MIRROR_OPEN, // Mirror opened showing shelf (scene_24)
    STATE_M1_LETTER,      // Letter from shelf (scene_25)
    STATE_M1_COMP_DESK,   // Computer desktop after login (scene_26)
    STATE_M1_MAP,         // Map with red location dot (scene_27)

    // Travel Sequence (slide animations between locations)
    STATE_TRAVEL_FLIGHT,  // Flight to North America (scene_28)
    STATE_TRAVEL_BOAT,    // Boat to Greenland (scene_29)
    STATE_TRAVEL_HARBOUR, // Arriving at harbour (scene_30)
    STATE_TRAVEL_GARAGE,  // Garage door on surface (scene_31)

    // Module 2: Garage + Ultimate Tic-Tac-Toe
    STATE_M2_GARAGE,      // Inside garage with mirror (scene_32)
    STATE_M2_MIRROR,      // Mirror reflection (scene_33)
    STATE_M2_VORTEX,      // Vortex transition (scene_34)
    STATE_M2_TTT_INTRO,   // TTT intro screen (scene_35)
    STATE_M2_TTT_GAME,    // Ultimate TTT gameplay (scene_36)
    STATE_M2_VEHICLE,     // Vehicle revealed after win (scene_38)

    // Module 3: Vehicle Journey + Chess + Lift
    STATE_M3_COCKPIT,     // Vehicle cockpit (scene_39)
    STATE_M3_WALL_SLIDE,  // Wall/door sliding up (scene_40)
    STATE_M3_FLOAT_UP,    // Vehicle floating up (scene_41)
    STATE_M3_LEV_VEH,     // Levitation vehicle text (scene_42)
    STATE_M3_RED_LINE,    // Following red line (scene_43)
    STATE_M3_LAB_DOOR,    // Lab door approach (scene_44)
    STATE_M3_LAB_ENTER,   // Lab entrance (scene_45)
    STATE_M3_NEAR_WALL,   // Agent near wall (scene_46)
    STATE_M3_WALL_ZOOM,   // Wall zoomed in (scene_47)
    STATE_M3_CHESS_INTRO, // Chess intro text (scene_48)
    STATE_M3_CHESS_GAME,  // Chess puzzle (scene_49)
    STATE_M3_LIFT_CORR,   // Corridor to lift (scene_50)
    STATE_M3_LIFT_IN,     // Inside lift (scene_51)
    STATE_M3_LIFT_DESC,   // Lift descending 28->0 (scene_52)

    // Module 4: Submarine + Passlock + Ending
    STATE_M4_SUBMARINE,   // Submarine room (scene_53)
    STATE_M4_PASSLOCK,    // Passlock system zoomed (scene_54)
    STATE_M4_AI_ROOM,     // AI machine room (scene_55)
    STATE_M4_BREAK,       // Breaking glass (scene_56)
    STATE_M4_EXPLODE,     // Machine explosion (scene_57)
    STATE_M4_ENDING,      // President congratulates (scene_58)

    STATE_COUNT
};

// ----- Map each state to its scene image index -----
inline int GetSceneIndex(GameState s) {
    switch(s) {
        case STATE_TITLE: return 0;
        case STATE_INTRO_1: return 1;   case STATE_INTRO_2: return 2;
        case STATE_INTRO_3: return 3;   case STATE_INTRO_4: return 4;
        case STATE_INTRO_5: return 5;   case STATE_INTRO_6: return 6;
        case STATE_INTRO_7: return 7;   case STATE_INTRO_8: return 8;
        case STATE_INTRO_9: return 9;   case STATE_INTRO_10: return 10;
        case STATE_INTRO_11: return 11; case STATE_INTRO_12: return 12;
        case STATE_INTRO_13: return 13; case STATE_INTRO_14: return 14;
        case STATE_M1_HOUSE: return 15; case STATE_M1_ROOM_BASE1: return 16;
        case STATE_M1_COMPUTER: return 17; case STATE_M1_ROOM_BASE2: return 18;
        case STATE_M1_BOOK: return 19; case STATE_M1_DESK_PASS: return 20;
        case STATE_M1_DESK_ITEMS: return 21; case STATE_M1_PHOTO_KEY: return 22;
        case STATE_M1_MIRROR: return 23; case STATE_M1_MIRROR_OPEN: return 24;
        case STATE_M1_LETTER: return 25; case STATE_M1_COMP_DESK: return 26;
        case STATE_M1_MAP: return 27;
        case STATE_TRAVEL_FLIGHT: return 28; case STATE_TRAVEL_BOAT: return 29;
        case STATE_TRAVEL_HARBOUR: return 30; case STATE_TRAVEL_GARAGE: return 31;
        case STATE_M2_GARAGE: return 32; case STATE_M2_MIRROR: return 33;
        case STATE_M2_VORTEX: return 34; case STATE_M2_TTT_INTRO: return 35;
        case STATE_M2_TTT_GAME: return 36; case STATE_M2_VEHICLE: return 38;
        case STATE_M3_COCKPIT: return 39; case STATE_M3_WALL_SLIDE: return 40;
        case STATE_M3_FLOAT_UP: return 41; case STATE_M3_LEV_VEH: return 42;
        case STATE_M3_RED_LINE: return 43; case STATE_M3_LAB_DOOR: return 44;
        case STATE_M3_LAB_ENTER: return 45; case STATE_M3_NEAR_WALL: return 46;
        case STATE_M3_WALL_ZOOM: return 47; case STATE_M3_CHESS_INTRO: return 48;
        case STATE_M3_CHESS_GAME: return 49; case STATE_M3_LIFT_CORR: return 50;
        case STATE_M3_LIFT_IN: return 51; case STATE_M3_LIFT_DESC: return 52;
        case STATE_M4_SUBMARINE: return 53; case STATE_M4_PASSLOCK: return 54;
        case STATE_M4_AI_ROOM: return 55; case STATE_M4_BREAK: return 56;
        case STATE_M4_EXPLODE: return 57; case STATE_M4_ENDING: return 58;
        default: return 0;
    }
}

// ----- Transition types for scene changes -----
enum TransitionType {
    TRANS_NONE,      // Instant switch
    TRANS_FADE,      // Fade to black and back
    TRANS_SLIDE_L,   // Slide left
    TRANS_SLIDE_R,   // Slide right
    TRANS_SLIDE_UP,  // Slide upward
    TRANS_VORTEX,    // Spinning vortex effect
    TRANS_CHECKER    // Checkerboard reveal
};

// ----- Transition state tracking -----
struct Transition {
    bool active;           // Is a transition currently playing?
    TransitionType type;
    float progress;        // 0.0 to 1.0
    float speed;           // How fast the transition plays
    GameState nextState;   // State to switch to after transition
    Texture2D fromTex;     // Screenshot of current scene (for slide)
};

// ----- Cinematic text display (typewriter effect) -----
struct CinematicText {
    std::string fullText;     // Complete text to display
    int charsRevealed;        // How many characters shown so far
    float charTimer;          // Timer for typewriter effect
    float charSpeed;          // Seconds per character
    bool complete;            // All text revealed?
    float displayTimer;       // How long text has been fully shown
    bool waitingForClick;     // Waiting for player click to proceed?
};

// ----- Inventory system for Module 1 -----
struct Inventory {
    bool hasKey;         // Player picked up the key
    bool keyGlowing;     // Key is selected/active in inventory
};

// ----- Module 1 puzzle state -----
struct Module1State {
    bool computerOn;        // Computer has been turned on
    bool deskUnlocked;      // Desk passcode solved (3604)
    bool computerUnlocked;  // Computer passcode solved (5819)
    bool photoSelectable;   // Photo on wall becomes selectable after desk
    bool keyPickedUp;       // Key collected from behind photo
    bool mirrorOpened;      // Mirror has been opened with key
    bool letterRead;        // Letter has been read
    int deskDigits[4];      // Current desk passcode digits [0-9 each]
    int compDigits[4];      // Current computer passcode digits
    int compDigitPos;       // Which digit being entered on computer
    int bookPage;           // Current book page (0-8, page 8 has clue)
    Inventory inventory;
};

// ----- Ultimate Tic-Tac-Toe board -----
// 9 small boards, each 3x3 = 81 cells total
struct UTTTState {
    int cells[9][9];       // 0=empty, 1=player(X), 2=bot(O)
    int boardWon[9];       // 0=open, 1=player won, 2=bot won, 3=draw
    int activeBoard;       // Which small board must be played (-1=any)
    bool playerTurn;       // true=player's turn
    int gameResult;        // 0=playing, 1=player wins, 2=bot wins, 3=draw
    bool hostCodeActive;   // Host code input mode
    char hostCodeInput[6]; // Buffer for host code
    int hostCodeLen;       // Characters entered
};

// ----- Chess piece types -----
enum ChessPiece {
    EMPTY = 0,
    W_PAWN = 1, W_ROOK = 2, W_KNIGHT = 3, W_BISHOP = 4, W_QUEEN = 5, W_KING = 6,
    B_PAWN = 7, B_ROOK = 8, B_KNIGHT = 9, B_BISHOP = 10, B_QUEEN = 11, B_KING = 12
};

// ----- Chess puzzle state -----
struct ChessState {
    int board[8][8];       // 8x8 board with ChessPiece values
    int selectedR, selectedC; // Currently selected piece (-1 if none)
    bool playerTurn;       // Player is white
    int gameResult;        // 0=playing, 1=checkmate(win), 2=lost
    bool hostCodeActive;
    char hostCodeInput[6];
    int hostCodeLen;
};

// ----- Passlock state for Module 4 -----
struct PasslockState {
    int digits[4];         // Current entered digits
    int digitCount;        // How many digits entered
    bool unlocked;         // Correct code entered (8157)
};

// ----- Lift state -----
struct LiftState {
    int currentFloor;      // Starts at 28, goes to 0
    float floorTimer;      // Timer for floor changes (1 sec each)
    bool descending;       // Currently descending?
    bool doorOpen;         // Door animation state
    float doorProgress;    // 0=closed, 1=open
};

// ----- Key sound frequencies (Hz) for keys 0-9 -----
// Each key produces a unique tone
const float KEY_FREQS[10] = {
    220.0f,  // Key 0: A3
    262.0f,  // Key 1: C4
    294.0f,  // Key 2: D4
    330.0f,  // Key 3: E4
    370.0f,  // Key 4: F#4
    415.0f,  // Key 5: Ab4
    466.0f,  // Key 6: Bb4
    523.0f,  // Key 7: C5
    587.0f,  // Key 8: D5
    659.0f   // Key 9: E5
};

// Background music uses keys 8,1,5,7 = D5, C4, Ab4, C5
const int BG_MUSIC_KEYS[4] = {8, 1, 5, 7};

#endif // GAME_STATES_H
