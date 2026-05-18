// main_part1.h - Globals, loading, helpers, title screen, intro cutscenes
#ifndef MAIN_PART1_H
#define MAIN_PART1_H

#include "game_states.h"
#include "sounds.h"
#include "uttt.h"
#include "chess.h"
#include <cstdio>

// ===== GLOBALS =====
Texture2D scenes[TOTAL_SCENES];
Sound keySnds[10];        // Sounds for keys 0-9
Font gameFont;            // Times New Roman font for all text
Sound bgMusic;            // Background music (played as looping sound)
Sound glassSound;         // Glass breaking
bool bgMusicPlaying = false;
GameState curState = STATE_TITLE;
GameState prevState = STATE_TITLE;
float stateTimer = 0;     // Time in current state
float fadeAlpha = 0;      // For fade transitions
bool fading = false;
bool fadeOut = true;
GameState fadeTarget;
float slideOff = 0;       // For slide transitions  
bool sliding = false;
int slideDir = 0;         // 1=left, 2=right, 3=up

// Module 1 state
bool m1CompOn = false;
bool m1DeskOpen = false;
bool m1PhotoSel = false;
bool m1HasKey = false;
bool m1KeyGlow = false;
bool m1MirrorOpen = false;
bool m1LetterRead = false;
bool m1CompLogged = false;
int m1CompCode[4] = {0,0,0,0};
int m1CompPos = 0;
int m1DeskCode[4] = {0,0,0,0};
int m1BookPage = 0; // 0-8
bool m1ShowLeaveDialog = false;

// Module 2 state
UTTTGame uttt;
float vortexAngle = 0;
float vortexScale = 1;

// Module 3 state
ChessGame chess;
int liftFloor = 28;
float liftTimer = 0;

// Module 4 state
int passCode[4] = {0,0,0,0};
int passPos = 0;
bool passUnlocked = false;
bool bgMusicMuted = false;

// Animation helpers
float animTime = 0;
float textTimer = 0;
int textChars = 0;
bool textDone = false;
int textPage = 0;          // Current 3-line page of cinematic text
float pageTimer = 0;       // Timer within current page

// ===== CINEMATIC TEXT DATA =====
// All dialogue/narration for each intro scene (grammar-corrected)
const char* introTexts[] = {
// Scene 1 - President thinking
"James: Mm... The life with AI has created a beautiful bond with ourselves, more than a friend with\n"
"extreme loyalty, standing like a compass whenever we lose direction without any underestimation.\n"
"I want to achieve something which resembles the other AI Technologies in our era. What if we build\n"
"a supreme AI which stands out in extreme thinking, creativity without any dependency and to be\n"
"implemented in such a way that it helps all the humans without any discrimination and without any\n"
"hesitation. I want to allocate this work to someone who has the ability to make this possible.\n"
"What if we allocate this work to Scientist Ethan Kaul, he is the one who builds the best Agentic\n"
"AIs in this era.",

// Scene 2 - Handshake
"James: Hi, Mr. Ethan, Nice meeting you.\n"
"Ethan: Yeah, Mr. President, It's also nice for me to see you in your court. I have heard a lot\n"
"about your research towards AI and I got inspired by those also.\n"
"James: Nice catch, You have been called here in order to discuss one such matter about AI.\n"
"Ethan: Interesting, Can you please tell me about that.\n"
"James: Yeah sure, I want to achieve something in the AI field by creating a supreme AI which\n"
"excels in individual thinking and stands out in creativity and it should act like a life supporter\n"
"for all people. I have decided to allocate this project to you. What about you?\n"
"Ethan: Yeah, Mr. President, I have also planned to work on some different kinds of projects.\n"
"This one is very interesting. Let me work on it.\n"
"James: Make sure that, this should be done in a secret manner and also there shouldn't be any\n"
"contact between me and you.\n"
"Ethan: Once I finish this, I'll reach you out.",

// Scene 3 - Ethan typing
"Ethan: Whether it's gonna work or not, I'll dedicate my full potential in creating a Supreme AI,\n"
"a system capable of thinking every known intelligence.",

// Scene 4 - Victory
"Ethan: Victory... After years of relentless work and sacrifice, Supreme AI was finally born.\n"
"Humanity has entered a new era.",

// Scene 5 - Realization
"Ethan: But as Supreme AI evolved beyond human limits... I realized, I had not created a tool for\n"
"humanity. I have created something uncontrollable.",

// Scene 6 - Erasing code
"Ethan: If I have to stop this, there is only one way. Let me erase the source code from the\n"
"memory before the AI takes complete control over it.",

// Scene 7 - Lab fire
"Before Ethan completely erased the source code, the AI sensed that Ethan was going to dismantle\n"
"the source code. Without thinking for a minute, it made the system explode which led to the death\n"
"of Ethan and the lab was isolated from the internet. But the AI Machine is in a room next to it,\n"
"so the Supreme AI is not isolated from the internet.",

// Scene 8 - Supreme AI
"Supreme AI was unlike any technology the world has ever seen - a self-evolving intelligence,\n"
"powerful enough to analyse, learn and control systems far beyond human capability.",

// Scene 9 - ICJ Meeting
"The President of ICJ discussed about a lot of matters among the Presidents of other countries.\n"
"The President also suspected that there is an entity which leads to unexplained attacks on warfare,\n"
"leads to controversy between countries, intrudes in other system servers and totally messes it up,\n"
"leads to shutting down of today's AI systems and it also has humongous effects on Cars - it took\n"
"control over the automation in Cars and leads to accidents.\n"
"James' internal sayings: Is This... The result of our Supreme AI..?",

// Scene 10 - Phone call
"James: If Supreme AI is truly behind all these arguments then it will lead to destabilization\n"
"of the world. Mr. Ethan, please attend the call.",

// Scene 11 - James planning
"James: I have tried so many times in order to reach out Mr. Ethan. Is he following my early\n"
"sayings \"It should be a secret operation, there shouldn't be connection between me and you.\"\n"
"But my instinct says something goes wrong. I have planned to allocate the work of finding the\n"
"Scientist to an agent and if Supreme AI is behind all these arguments, I want them to extract\n"
"Supreme AI's source code. I have traced the location of the scientist's mobile. Let me share\n"
"it to him.",

// Scene 12 - Agent tea
"Agent Zero: Hmm... Such a nice cup of tea. For the first time in months everything goes well\n"
"and everything sounds peaceful.",

// Scene 13 - Red indicator
"Agent Zero: Oh... No, Now only I felt peace. Even without completing the statement, there's\n"
"a mission that comes up.",

// Scene 14 - Mission brief
"Agent Zero: Dr. Ethan, who created the AI, I have to find the Scientist. He is the only person\n"
"who has answers for the questions of the President. The President wants answers, not assumptions.",
};

// Travel scene messages
const char* travelTexts[] = {
"The Agent Zero takes a flight to reach North America which is close to the Lab location Greenland.",
"The Agent Zero takes a private boat to travel from North America to the Greenland harbour close to the Lab.",
"The Agent Zero reaching the Greenland harbour close to the Lab.",
"The Agent Zero sees a garage on the surface and decides to investigate.",
};

// Module 3 vehicle texts
const char* m3Texts[] = {
"Agent Zero: This vehicle is an Automatic Levitation Vehicle.",
"Agent Zero: This vehicle was following a red line route at a very high speed.",
"Agent Zero: Is that a Lab Door or something.",
"Agent Zero: Is this the Lab of the Scientist I have been looking for?",
"Agent Zero: Mm... I didn't find anything here. Is there something behind the wall..?",
};

// ===== LOAD ALL RESOURCES =====
void LoadAllResources() {
    char path[256];
    for (int i = 0; i < TOTAL_SCENES; i++) {
        sprintf(path, "game_assets/scene_%02d.png", i);
        // Prioritize loading the new front page image if it exists in the root directory
        if (i == 0 && FileExists("front page.jpeg")) {
            scenes[i] = LoadTexture("front page.jpeg");
        } else if (i == 17 && FileExists("system screen..jpeg")) {
            scenes[i] = LoadTexture("system screen..jpeg");
        } else if (i == 20 && FileExists("desk passcode.jpeg")) {
            scenes[i] = LoadTexture("desk passcode.jpeg");
        } else {
            scenes[i] = LoadTexture(path);
        }
    }
    // Load Times New Roman font at multiple sizes for crisp rendering
    gameFont = LoadFontEx("game_assets/times.ttf", 48, NULL, 0);
    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);
    // Generate key sounds
    for (int i = 0; i < 10; i++) {
        Wave w = GenTone(KFREQ[i], 0.3f);
        keySnds[i] = LoadSoundFromWave(w);
        UnloadWave(w);
    }
    // Generate background music from keys 8,1,5,7 melody
    Wave bgw = GenBGMusic(0.5f, 60);
    bgMusic = LoadSoundFromWave(bgw);
    UnloadWave(bgw);
    // Glass breaking sound effect
    Wave gw = GenGlassSound(2.0f);
    glassSound = LoadSoundFromWave(gw);
    UnloadWave(gw);
}

// ===== UNLOAD ALL RESOURCES =====
void UnloadAllResources() {
    for (int i = 0; i < TOTAL_SCENES; i++) UnloadTexture(scenes[i]);
    for (int i = 0; i < 10; i++) UnloadSound(keySnds[i]);
    UnloadFont(gameFont);
    UnloadSound(bgMusic);
    UnloadSound(glassSound);
}

// ===== DRAW SCENE IMAGE (fills screen) =====
void DrawScene(int idx, float alpha = 1.0f, bool cinematic = false) {
    if (idx < 0 || idx >= TOTAL_SCENES) return;
    Color tint = {255,255,255,(unsigned char)(alpha*255)};
    
    if (cinematic) {
        // Subtle cinematic zoom in (Ken Burns style)
        float zoom = 1.0f + (stateTimer * 0.02f); // Zoom in 2% per second
        if (zoom > 1.15f) zoom = 1.15f; // Cap at 15% zoom
        float destW = SCREEN_W * zoom;
        float destH = SCREEN_H * zoom;
        DrawTexturePro(scenes[idx],
            (Rectangle){0, 0, (float)scenes[idx].width, (float)scenes[idx].height},
            (Rectangle){SCREEN_W/2.0f - destW/2.0f, SCREEN_H/2.0f - destH/2.0f, destW, destH},
            (Vector2){0, 0}, 0.0f, tint);
    } else {
        Rectangle src = {0,0,(float)scenes[idx].width,(float)scenes[idx].height};
        Rectangle dst = {0,0,(float)SCREEN_W,(float)SCREEN_H};
        DrawTexturePro(scenes[idx], src, dst, {0,0}, 0, tint);
    }
}

// ===== DRAW TEXT WITH OUTLINE (for visibility on images) =====
// Draws text with a dark outline so it's readable on any background
void DrawOutlinedText(const char* text, int x, int y, int fontSize, Color color) {
    float fSize = (float)fontSize;
    Vector2 pos = {(float)x, (float)y};
    // Draw outline (8 directions)
    Color outline = {0, 0, 0, 220};
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx == 0 && dy == 0) continue;
            DrawTextEx(gameFont, text, {pos.x+dx, pos.y+dy}, fSize, 1, outline);
        }
    }
    // Draw main text
    DrawTextEx(gameFont, text, pos, fSize, 1, color);
}

// ===== SPLIT TEXT INTO LINES =====
// Splits text by newlines and word-wraps long lines to fit maxWidth
#include <sstream>
static std::vector<std::string> SplitTextLines(const char* text, int fontSize, int maxWidth) {
    std::vector<std::string> lines;
    std::string current;
    std::string word;
    for (int i = 0; text[i]; i++) {
        if (text[i] == '\n') {
            if (!word.empty()) { 
                if (!current.empty()) current += " ";
                current += word; word.clear();
            }
            lines.push_back(current); current.clear();
        } else if (text[i] == ' ') {
            if (!word.empty()) {
                std::string test = current.empty() ? word : current + " " + word;
                Vector2 sz = MeasureTextEx(gameFont, test.c_str(), (float)fontSize, 1);
                if (sz.x > maxWidth && !current.empty()) {
                    lines.push_back(current);
                    current = word;
                } else {
                    current = test;
                }
                word.clear();
            }
        } else {
            word += text[i];
        }
    }
    if (!word.empty()) {
        std::string test = current.empty() ? word : current + " " + word;
        Vector2 sz = MeasureTextEx(gameFont, test.c_str(), (float)fontSize, 1);
        if (sz.x > maxWidth && !current.empty()) {
            lines.push_back(current);
            current = word;
        } else {
            current = test;
        }
    }
    if (!current.empty()) lines.push_back(current);
    return lines;
}

// ===== DRAW CINEMATIC TEXT (3 lines at a time, on image) =====
// Shows text directly on the scene image with typewriter effect
// Returns true when ALL pages have been shown
bool DrawCinText(const char* full, float timer, float speed, int maxW) {
    int fontSize = 28;
    int lineSpacing = 38;
    int linesPerPage = 3;
    int padX = 60;
    int startY = SCREEN_H - 160;  // Position near bottom of screen

    // Split full text into wrapped lines
    std::vector<std::string> allLines = SplitTextLines(full, fontSize, maxW - padX*2);
    int totalPages = ((int)allLines.size() + linesPerPage - 1) / linesPerPage;
    if (textPage >= totalPages) textPage = totalPages - 1;
    if (textPage < 0) textPage = 0;

    // Get current page lines
    int startLine = textPage * linesPerPage;
    int endLine = startLine + linesPerPage;
    if (endLine > (int)allLines.size()) endLine = (int)allLines.size();

    // Calculate total characters in current page
    int pageChars = 0;
    for (int i = startLine; i < endLine; i++) pageChars += (int)allLines[i].size();

    // Typewriter: how many chars revealed so far on this page
    int revealed = (int)(pageTimer / speed);
    if (revealed > pageChars) revealed = pageChars;
    bool pageDone = (revealed >= pageChars);

    // Draw a subtle semi-transparent strip behind text for readability
    DrawRectangle(0, startY - 10, SCREEN_W, linesPerPage * lineSpacing + 20,
        (Color){0, 0, 0, 120});

    // Draw each line with typewriter effect
    int charsLeft = revealed;
    for (int i = startLine; i < endLine; i++) {
        int lineIdx = i - startLine;
        int lineLen = (int)allLines[i].size();
        int showChars = (charsLeft >= lineLen) ? lineLen : charsLeft;
        if (showChars < 0) showChars = 0;
        charsLeft -= showChars;

        // Build substring to show
        std::string display = allLines[i].substr(0, showChars);
        DrawOutlinedText(display.c_str(), padX, startY + lineIdx * lineSpacing,
            fontSize, (Color){255, 255, 255, 255});
    }

    // Show page indicator if multiple pages
    if (totalPages > 1) {
        DrawOutlinedText(TextFormat("[%d/%d]", textPage+1, totalPages),
            SCREEN_W - 120, startY + linesPerPage * lineSpacing + 5,
            18, (Color){180, 200, 255, 200});
    }

    // If current page done, show prompt and handle auto-advance
    if (pageDone && textPage < totalPages - 1) {
        float pulse = (sinf(animTime * 3) + 1) * 0.5f;
        DrawOutlinedText("Click for more...",
            SCREEN_W/2 - 80, startY - 30, 20,
            (Color){255, 255, 100, (unsigned char)(150 + pulse*105)});
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            textPage++;
            pageTimer = 0;
            return false; // Not all done yet
        }
        return false;
    }

    return pageDone && (textPage >= totalPages - 1); // true = all text shown
}

// ===== DRAW BUTTON =====
bool DrawButton(int x, int y, int w, int h, const char* text, int fontSize) {
    Vector2 mp = GetMousePosition();
    bool hover = mp.x>=x && mp.x<=x+w && mp.y>=y && mp.y<=y+h;
    Color bg = hover ? (Color){60,120,200,230} : (Color){30,60,120,200};
    Color border = hover ? (Color){100,180,255,255} : (Color){60,100,180,200};
    DrawRectangleRounded({(float)x,(float)y,(float)w,(float)h}, 0.3f, 8, bg);
    DrawRectangleRoundedLinesEx({(float)x,(float)y,(float)w,(float)h}, 0.3f, 8, 2.0f, border);
    int tw = (int)MeasureTextEx(gameFont, text, (float)fontSize, 1).x;
    DrawTextEx(gameFont, text, {(float)(x+(w-tw)/2), (float)(y+(h-fontSize)/2)},
        (float)fontSize, 1, WHITE);
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// ===== SELECTABLE OBJECT GLOW =====
// Draws a pulsing glow rectangle around a clickable area
bool DrawSelectable(int x, int y, int w, int h) {
    Vector2 mp = GetMousePosition();
    bool hover = mp.x>=x && mp.x<=x+w && mp.y>=y && mp.y<=y+h;
    float pulse = (sinf(animTime * 3.0f) + 1.0f) * 0.5f;
    unsigned char a = hover ? (unsigned char)(100 + pulse*100) : (unsigned char)(40 + pulse*40);
    Color glow = {100, 200, 255, a};
    DrawRectangleLinesEx({(float)x,(float)y,(float)w,(float)h}, 2, glow);
    if (hover) {
        DrawText("[Click]", x+w/2-30, y-20, 16, (Color){200,230,255,(unsigned char)(150+pulse*105)});
    }
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// ===== START FADE TRANSITION =====
void StartFade(GameState target) {
    if (fading) return;  // Don't restart if already fading
    fading = true;
    fadeOut = true;
    fadeAlpha = 0;
    fadeTarget = target;
}

// ===== START SLIDE TRANSITION =====
void StartSlide(GameState target, int dir) {
    if (sliding) return;  // Don't restart if already sliding
    sliding = true;
    slideDir = dir;
    slideOff = 0;
    fadeTarget = target;
}

// ===== CHANGE STATE =====
void ChangeState(GameState next) {
    prevState = curState;
    curState = next;
    stateTimer = 0;
    textChars = 0;
    textDone = false;
    textTimer = 0;
    textPage = 0;      // Reset text pagination
    pageTimer = 0;     // Reset page typewriter timer
}

#endif
