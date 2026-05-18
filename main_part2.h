// main_part2.h - Module update/draw logic
#ifndef MAIN_PART2_H
#define MAIN_PART2_H
#include "main_part1.h"

// ===== CUSTOM DRAWING FUNCTIONS =====
void DrawMetalInventoryBox(int x, int y, int w, int h) {
    // Main metal body
    DrawRectangle(x, y, w, h, (Color){100,110,120,255});
    DrawRectangle(x+2, y+2, w-4, h-4, (Color){80,90,100,255});
    
    // Top inner recessed square (item slot)
    int slotX = x + 10;
    int slotY = y + 10;
    int slotW = w - 20;
    int slotH = h - 35; // Leave room for bottom handle
    
    // Slot background (very dark)
    DrawRectangle(slotX, slotY, slotW, slotH, (Color){30,35,40,255});
    
    // Bevels for recessed look
    DrawLine(slotX, slotY, slotX+slotW, slotY, (Color){40,45,50,255}); // Top dark
    DrawLine(slotX, slotY, slotX, slotY+slotH, (Color){40,45,50,255}); // Left dark
    DrawLine(slotX, slotY+slotH, slotX+slotW, slotY+slotH, (Color){150,160,170,255}); // Bottom light
    DrawLine(slotX+slotW, slotY, slotX+slotW, slotY+slotH, (Color){150,160,170,255}); // Right light
    
    // Outer bevel for the slot
    DrawLine(slotX-1, slotY-1, slotX+slotW+1, slotY-1, (Color){150,160,170,255});
    DrawLine(slotX-1, slotY-1, slotX-1, slotY+slotH+1, (Color){150,160,170,255});
    
    // Bottom handle section
    int handleY = slotY + slotH + 8;
    int handleH = 15;
    DrawRectangle(slotX, handleY, slotW, handleH, (Color){90,100,110,255});
    // Handle bevels
    DrawLine(slotX, handleY, slotX+slotW, handleY, (Color){140,150,160,255}); // Top light
    DrawLine(slotX, handleY+handleH, slotX+slotW, handleY+handleH, (Color){50,60,70,255}); // Bottom dark
    
    // Downward Triangle in handle
    Vector2 p1 = { (float)x + w/2 - 6, (float)handleY + 3 };
    Vector2 p2 = { (float)x + w/2 + 6, (float)handleY + 3 };
    Vector2 p3 = { (float)x + w/2, (float)handleY + 11 };
    DrawTriangle(p1, p3, p2, (Color){60,70,80,255});
    DrawTriangleLines(p1, p3, p2, (Color){120,130,140,255});
    
    // Corner screws
    int cx1 = x + 12;
    int cy = y + h - 10;
    int cx2 = x + w - 12;
    DrawCircle(cx1, cy, 5, (Color){70,80,90,255});
    DrawCircleLines(cx1, cy, 5, (Color){40,50,60,255});
    DrawLine(cx1-3, cy-3, cx1+3, cy+3, (Color){50,60,70,255});
    
    DrawCircle(cx2, cy, 5, (Color){70,80,90,255});
    DrawCircleLines(cx2, cy, 5, (Color){40,50,60,255});
    DrawLine(cx2-3, cy+3, cx2+3, cy-3, (Color){50,60,70,255});
    
    // Edge border
    DrawRectangleLines(x, y, w, h, (Color){50,60,70,255});
}

void DrawMetalKey(int x, int y, float scale, bool glow) {
    Color baseGold = glow ? (Color){255, 230, 120, 255} : (Color){180, 150, 70, 255};
    Color darkGold = glow ? (Color){210, 170, 60, 255}  : (Color){120, 90, 40, 255};
    Color highlight = glow ? (Color){255, 255, 230, 255} : (Color){220, 200, 120, 255};
    Color shadow = (Color){40, 30, 15, 255};
    
    float rOuter = 8 * scale;
    float rInner = 4.5f * scale;
    float shaftW = 5 * scale;
    float shaftH = 26 * scale;
    
    float cyRing = y + rOuter;
    
    // Top Ring
    DrawCircle(x, cyRing, rOuter, darkGold);
    DrawCircle(x, cyRing, rOuter - 1*scale, baseGold);
    DrawCircle(x, cyRing, rInner, (Color){30,35,40,255}); // Inner hole matches slot bg
    DrawCircleLines(x, cyRing, rInner, darkGold); // Inner bevel
    
    float shaftY = cyRing + rOuter - 1*scale;
    
    // Shaft
    DrawRectangle(x - shaftW/2, shaftY, shaftW, shaftH, baseGold);
    DrawLine(x - shaftW/2, shaftY, x - shaftW/2, shaftY + shaftH, highlight);
    DrawLine(x + shaftW/2, shaftY, x + shaftW/2, shaftY + shaftH, shadow);
    
    // Upper band
    float uBandY = shaftY + 3*scale;
    DrawRectangle(x - shaftW/2 - 1.5f*scale, uBandY, shaftW + 3*scale, 2.5f*scale, darkGold);
    DrawLine(x - shaftW/2 - 1.5f*scale, uBandY, x + shaftW/2 + 1.5f*scale, uBandY, highlight);
    
    // Grip
    float gripY = uBandY + 3.5f*scale;
    float gripH = 7*scale;
    DrawRectangle(x - shaftW/2 - 1*scale, gripY, shaftW + 2*scale, gripH, darkGold);
    // Criss-cross
    DrawLine(x - shaftW/2, gripY, x + shaftW/2, gripY + gripH, shadow);
    DrawLine(x + shaftW/2, gripY, x - shaftW/2, gripY + gripH, shadow);
    DrawLine(x - shaftW/2, gripY + gripH/2, x + shaftW/2, gripY + gripH, shadow);
    
    // Lower band
    float lBandY = gripY + gripH + 1.5f*scale;
    DrawRectangle(x - shaftW/2 - 1.5f*scale, lBandY, shaftW + 3*scale, 2.5f*scale, darkGold);
    DrawLine(x - shaftW/2 - 1.5f*scale, lBandY, x + shaftW/2 + 1.5f*scale, lBandY, highlight);
    
    // Tip
    DrawCircle(x, shaftY + shaftH, shaftW/2, baseGold);
    
    // Bit
    float bitY = shaftY + shaftH - 7*scale;
    float bitW = 9 * scale;
    float bitH = 7 * scale;
    
    DrawRectangle(x + shaftW/2, bitY, bitW, bitH, baseGold);
    DrawRectangleLines(x + shaftW/2, bitY, bitW, bitH, shadow);
    
    // Notch
    float notchY = bitY + 2*scale;
    float notchW = 4 * scale;
    float notchH = 3 * scale;
    DrawRectangle(x + shaftW/2 + bitW - notchW, notchY, notchW+1, notchH, (Color){30,35,40,255});
    
    DrawLine(x + shaftW/2, bitY, x + shaftW/2 + bitW, bitY, highlight);
}

// ===== TITLE SCREEN =====
void UpdateTitle() {
    DrawScene(0);
    // START button
    if(DrawButton(540,420,200,50,"START",30)) StartFade(STATE_INTRO_1);
    // EXIT button  
    if(DrawButton(540,490,200,50,"EXIT",30)) CloseWindow();
}

// ===== INTRO CUTSCENES (14 scenes) =====
void UpdateIntro() {
    int idx = (int)curState - (int)STATE_INTRO_1; // 0-13
    DrawScene(idx + 1, 1.0f, true); // scene_01 to scene_14
    pageTimer += GetFrameTime();
    
    // Skip intro feature
    DrawOutlinedText("Press SPACE to skip intro", 20, 20, 20, (Color){200,200,200,150});
    if (IsKeyPressed(KEY_SPACE)) {
        StartFade(STATE_M1_HOUSE);
        return;
    }

    // Draw cinematic text (3 lines at a time, on image, typewriter effect)
    bool allDone = DrawCinText(introTexts[idx], pageTimer, 0.03f, SCREEN_W-40);
    if(allDone) {
        float pulse = (sinf(animTime*2)+1)*0.5f;
        DrawOutlinedText("Click to continue...", SCREEN_W/2-100, 20, 20,
            (Color){200,230,255,(unsigned char)(100+pulse*155)});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(curState < STATE_INTRO_14) StartFade((GameState)(curState+1));
            else StartFade(STATE_M1_HOUSE);
        }
    }
}

// ===== MODULE 1: ESCAPE ROOM =====
void UpdateM1() {
    GameState base = m1CompOn ? STATE_M1_ROOM_BASE2 : STATE_M1_ROOM_BASE1;
    switch(curState) {
    case STATE_M1_HOUSE: {
        DrawScene(15);
        // Door selectable - center of image
        if(DrawSelectable(500,200,280,350)) StartFade(STATE_M1_ROOM_BASE1);
    } break;
    case STATE_M1_ROOM_BASE1:
    case STATE_M1_ROOM_BASE2: {
        // Show appropriate room base: key picked (scene 59), computer on (18), or default (16)
        int roomScene = m1HasKey ? 59 : (m1CompOn ? 18 : 16);
        DrawScene(roomScene);
        // Key logic (selection is only possible in base room)
        if(m1HasKey) {
            if(DrawSelectable(SCREEN_W-80,SCREEN_H-90,70,90)) m1KeyGlow=!m1KeyGlow;
        }
        // Selectable objects - positions matched to actual room image
        if(DrawSelectable(30,265,175,165)) { // Computer screen (left monitor)
            if(!m1CompOn) { m1CompOn=true; ChangeState(STATE_M1_ROOM_BASE2); }
            else if(!m1CompLogged) ChangeState(STATE_M1_COMPUTER);
            else ChangeState(STATE_M1_COMP_DESK);
        }
        if(DrawSelectable(1095,175,125,100)) ChangeState(STATE_M1_BOOK); // Book (Machine Intelligence on top-right shelf)
        if(DrawSelectable(30,430,200,150)) ChangeState(STATE_M1_DESK_PASS); // Desk (left desk drawers)
        if(DrawSelectable(720,95,190,220)) { // Mirror (wall, center-right)
            if(m1KeyGlow && m1HasKey) ChangeState(STATE_M1_MIRROR_OPEN);
            else ChangeState(STATE_M1_MIRROR);
        }
        if(m1PhotoSel && DrawSelectable(370,55,240,270)) ChangeState(STATE_M1_PHOTO_KEY); // Photo (wall frame, center-left)
    } break;
    case STATE_M1_COMPUTER: {
        DrawScene(17, 0.3f); // Dim background
        // Passcode entry screen overlay
        int bx=390,by=200,bw=500,bh=300;
        DrawRectangle(bx,by,bw,bh,(Color){10,20,40,240});
        DrawRectangleLines(bx,by,bw,bh,(Color){0,150,255,255});
        DrawText("ENTER PASSCODE",bx+150,by+30,24,(Color){0,200,255,255});
        // Show entered digits
        for(int i=0;i<4;i++) {
            int dx=bx+120+i*70;
            DrawRectangle(dx,by+100,50,60,(Color){20,40,80,255});
            DrawRectangleLines(dx,by+100,50,60,(Color){0,150,255,200});
            if(i<m1CompPos) {
                char d[2]={(char)('0'+m1CompCode[i]),0};
                DrawText(d,dx+15,by+110,40,WHITE);
            }
        }
        if(m1CompPos>=4) {
            bool correct=(m1CompCode[0]==5&&m1CompCode[1]==8&&m1CompCode[2]==1&&m1CompCode[3]==9);
            if(correct) {
                DrawText("ACCESS GRANTED",bx+160,by+200,24,GREEN);
                if(stateTimer>1.5f) { m1CompLogged=true; ChangeState(STATE_M1_COMP_DESK); }
            } else {
                DrawText("ACCESS DENIED",bx+170,by+200,24,RED);
                if(stateTimer>1.5f) { m1CompPos=0; stateTimer=0; }
            }
        }
        // Number key input
        for(int k=0;k<=9;k++) {
            if(IsKeyPressed(KEY_ZERO+k) && m1CompPos<4) {
                m1CompCode[m1CompPos++]=k;
                PlaySound(keySnds[k]);
                stateTimer=0;
            }
        }
        // Click outside to go back
        Vector2 mp=GetMousePosition();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           !(mp.x>=bx&&mp.x<=bx+bw&&mp.y>=by&&mp.y<=by+bh)) {
            m1CompPos=0; ChangeState(m1CompOn?STATE_M1_ROOM_BASE2:STATE_M1_ROOM_BASE1);
        }
    } break;
    case STATE_M1_BOOK: {
        // Draw the actual book image from pdf_page12_img1 (scene 19)
        DrawScene(19);

        // Book layout coordinates (matched to the open book in the image)
        int bkL = 210, bkT = 80, bkR = 1070, bkB = 640;
        int spineX = 640;
        int pgW = spineX - bkL;   // ~430
        int pgH = bkB - bkT;      // ~560

        // --- Update curl animation timer ---
        if(m1BookCurlTimer > 0) {
            m1BookCurlTimer -= GetFrameTime();
            if(m1BookCurlTimer <= 0) {
                m1BookCurlTimer = 0;
                m1BookPage = m1BookTargetPage;
                m1BookCurlDir = 0;
            }
        }

        // Animation progress (0 = just started, 1 = complete)
        float t = (m1BookCurlDir != 0 && m1BookCurlTimer > 0) ?
                  1.0f - (m1BookCurlTimer / 0.5f) : -1.0f;

        // --- PAGE CURL ANIMATION ---
        if(t >= 0 && t <= 1.0f) {
            if(m1BookCurlDir == 1) {
                // NEXT: right page curls toward the left (spine)
                float foldEdge = spineX + pgW * (1.0f - t);
                float remainW = foldEdge - spineX;
                // Remaining visible right page
                if(remainW > 2)
                    DrawRectangle(spineX, bkT, (int)remainW, pgH, (Color){225,215,190,235});
                // Curled fold-back strip (the lifted part of the page)
                float foldW = (t < 0.5f) ? pgW * t * 0.6f : pgW * (1.0f - t) * 0.6f;
                if(foldW > 2) {
                    DrawRectangle((int)foldEdge, bkT, (int)foldW, pgH, (Color){200,188,162,210});
                    // Fold shadow line
                    DrawRectangle((int)foldEdge - 4, bkT, 8, pgH,
                        (Color){0,0,0,(unsigned char)(70 * (1.0f - t))});
                }
                // Gradient shadow on left page (page landing)
                unsigned char shAlpha = (unsigned char)(40 * (t > 0.5f ? (t - 0.5f) * 2 : 0));
                if(shAlpha > 0)
                    DrawRectangle(spineX - (int)(pgW * (t - 0.5f) * 0.4f), bkT,
                        (int)(pgW * (t - 0.5f) * 0.4f), pgH, (Color){0,0,0,shAlpha});
            }
            else if(m1BookCurlDir == -1) {
                // PREV: left page curls toward the right (spine)
                float foldEdge = bkL + pgW * t;
                float remainW = spineX - foldEdge;
                if(remainW > 2)
                    DrawRectangle((int)foldEdge, bkT, (int)remainW, pgH, (Color){225,215,190,235});
                float foldW = (t < 0.5f) ? pgW * t * 0.6f : pgW * (1.0f - t) * 0.6f;
                if(foldW > 2) {
                    DrawRectangle((int)foldEdge - (int)foldW, bkT, (int)foldW, pgH,
                        (Color){200,188,162,210});
                    DrawRectangle((int)foldEdge - 4, bkT, 8, pgH,
                        (Color){0,0,0,(unsigned char)(70 * (1.0f - t))});
                }
                unsigned char shAlpha = (unsigned char)(40 * (t > 0.5f ? (t - 0.5f) * 2 : 0));
                if(shAlpha > 0)
                    DrawRectangle(spineX, bkT,
                        (int)(pgW * (t - 0.5f) * 0.4f), pgH, (Color){0,0,0,shAlpha});
            }
        }

        // --- PAGE CONTENT (only when not animating) ---
        if(m1BookCurlDir == 0) {
            // Left page text
            if(m1BookPage > 0)
                DrawText("[ empty page ]", bkL + 100, bkT + 250, 18, (Color){120,100,70,140});
            // Right page text
            if(m1BookPage < 8)
                DrawText("[ empty page ]", spineX + 80, bkT + 250, 18, (Color){120,100,70,140});
            // Passcode on last page (page 9)
            if(m1BookPage == 8)
                DrawText("3604", spineX + 130, bkB - 130, 36, (Color){80,40,10,255});
        }

        // Page indicator
        DrawText(TextFormat("Page %d / 9", m1BookPage + 1), spineX - 35, bkB + 15, 16,
            (Color){180,160,120,200});

        // Navigation arrows (use the arrow areas visible in the book image)
        if(m1BookCurlDir == 0) {
            // Left arrow area (◄) - previous page
            if(m1BookPage > 0 && DrawSelectable(195, 320, 55, 55)) {
                m1BookTargetPage = m1BookPage - 1;
                m1BookCurlDir = -1;
                m1BookCurlTimer = 0.5f;
            }
            // Right arrow area (►) - next page
            if(m1BookPage < 8 && DrawSelectable(1020, 320, 55, 55)) {
                m1BookTargetPage = m1BookPage + 1;
                m1BookCurlDir = 1;
                m1BookCurlTimer = 0.5f;
            }
        }

        // Click outside book to close
        Vector2 mp = GetMousePosition();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           !(mp.x >= bkL - 20 && mp.x <= bkR + 20 && mp.y >= bkT - 20 && mp.y <= bkB + 20))
            ChangeState(m1CompOn ? STATE_M1_ROOM_BASE2 : STATE_M1_ROOM_BASE1);
    } break;
    case STATE_M1_DESK_PASS: {
        DrawScene(20);
        int bx=440,by=250,bw=400,bh2=200;
        DrawRectangle(bx-20,by-20,bw,bh2,(Color){20,30,50,230});
        DrawText("DESK LOCK",bx+120,by-10,22,(Color){200,200,200,255});
        for(int i=0;i<4;i++) {
            int dx=bx+i*90;
            DrawRectangle(dx,by+40,60,80,(Color){30,50,80,255});
            DrawRectangleLines(dx,by+40,60,80,(Color){0,150,255,200});
            char d[2]={(char)('0'+m1DeskCode[i]),0};
            DrawText(d,dx+18,by+50,50,WHITE);
            // Click to increment digit
            if(DrawSelectable(dx,by+40,60,80)) {
                m1DeskCode[i]=(m1DeskCode[i]+1)%10;
                PlaySound(keySnds[m1DeskCode[i]]);
            }
        }
        // Check code
        if(m1DeskCode[0]==3&&m1DeskCode[1]==6&&m1DeskCode[2]==0&&m1DeskCode[3]==4) {
            DrawText("UNLOCKED!",bx+110,by+150,24,GREEN);
            if(stateTimer>1.0f) { m1DeskOpen=true; m1PhotoSel=true; ChangeState(STATE_M1_DESK_ITEMS); }
        }
        // Click outside the passcode box OR right-click to go back to room
        Vector2 mp=GetMousePosition();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           !(mp.x>=bx-20&&mp.x<=bx-20+bw&&mp.y>=by-20&&mp.y<=by-20+bh2)) {
            ChangeState(m1CompOn?STATE_M1_ROOM_BASE2:STATE_M1_ROOM_BASE1);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            ChangeState(m1CompOn?STATE_M1_ROOM_BASE2:STATE_M1_ROOM_BASE1);
    } break;
    case STATE_M1_DESK_ITEMS: {
        DrawScene(21);
        // Items float up and down
        float bob=sinf(animTime*2)*10;
        DrawText("Photo",500,(int)(300+bob),20,(Color){255,255,100,255});
        // Photo is selectable - clicking it makes wall photo selectable
        if(DrawSelectable(460,280+(int)bob,120,60)) {
            m1PhotoSel=true;
            ChangeState(m1CompOn?STATE_M1_ROOM_BASE2:STATE_M1_ROOM_BASE1);
        }
    } break;
    case STATE_M1_PHOTO_KEY: {
        DrawScene(22);
        // Key visible - selectable
        if(!m1HasKey && DrawSelectable(600,400,80,40)) {
            m1HasKey=true;
            ChangeState(m1CompOn?STATE_M1_ROOM_BASE2:STATE_M1_ROOM_BASE1);
        }
        if(m1HasKey) ChangeState(m1CompOn?STATE_M1_ROOM_BASE2:STATE_M1_ROOM_BASE1);
    } break;
    case STATE_M1_MIRROR: {
        DrawScene(23);
        DrawText("The mirror is locked.",500,600,20,(Color){200,200,200,220});
        if(stateTimer>2.0f || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            ChangeState(m1CompOn?STATE_M1_ROOM_BASE2:STATE_M1_ROOM_BASE1);
    } break;
    case STATE_M1_MIRROR_OPEN: {
        DrawScene(24);
        // Letter in shelf is selectable
        if(DrawSelectable(550,250,180,120)) ChangeState(STATE_M1_LETTER);
    } break;
    case STATE_M1_LETTER: {
        DrawScene(25);
        pageTimer+=GetFrameTime();
        // Show leave dialog
        DrawText("Leave the letter behind?",450,600,22,WHITE);
        if(DrawButton(450,640,100,35,"Yes",18)) {
            m1LetterRead=true;
            ChangeState(m1CompOn?STATE_M1_ROOM_BASE2:STATE_M1_ROOM_BASE1);
        }
        if(DrawButton(600,640,100,35,"No",18)) {} // Stay
    } break;
    case STATE_M1_COMP_DESK: {
        DrawScene(26);
        // Location of L icon selectable (center of desktop)
        if(DrawSelectable(555,240,90,90)) ChangeState(STATE_M1_MAP);
    } break;
    case STATE_M1_MAP: {
        DrawScene(27);
        // Red dot/crosshair on Greenland - starts travel sequence
        if(DrawSelectable(555,260,55,55)) StartSlide(STATE_TRAVEL_FLIGHT,1);
    } break;
    default: break;
    }
    // Inventory always visible in M1 (Metal panel design)
    if(curState>=STATE_M1_ROOM_BASE1 && curState<=STATE_M1_MAP && curState!=STATE_M1_HOUSE) {
        int invX = SCREEN_W-80;
        int invY = SCREEN_H-90; // Taller box
        DrawMetalInventoryBox(invX, invY, 70, 90);
        
        // Draw key inside the inner slot
        if(m1HasKey) {
            DrawMetalKey(invX + 35, invY + 12, 1.0f, m1KeyGlow);
        }
    }
}

// ===== TRAVEL SEQUENCE =====
void UpdateTravel() {
    int idx=(int)curState-(int)STATE_TRAVEL_FLIGHT; // 0-3
    DrawScene(28+idx, 1.0f, true);
    pageTimer+=GetFrameTime();
    bool done = DrawCinText(travelTexts[idx],pageTimer,0.04f,SCREEN_W-40);
    if(idx<3) { // Auto advance for first 3
        if(done && stateTimer>5.0f) StartSlide((GameState)(curState+1),1);
    } else { // Garage - wait for click on door
        if(done && DrawSelectable(500,200,280,300))
            StartSlide(STATE_M2_GARAGE,1);
    }
}

#endif
