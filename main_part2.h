// main_part2.h - Module update/draw logic
#ifndef MAIN_PART2_H
#define MAIN_PART2_H
#include "main_part1.h"

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
    DrawScene(idx + 1); // scene_01 to scene_14
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
        // Draw inventory box bottom-right
        DrawRectangle(SCREEN_W-80,SCREEN_H-80,70,70,(Color){0,0,0,150});
        DrawRectangleLines(SCREEN_W-80,SCREEN_H-80,70,70,(Color){100,200,255,200});
        if(m1HasKey) {
            Color kc = m1KeyGlow ? YELLOW : (Color){200,200,100,255};
            DrawText("KEY",SCREEN_W-72,SCREEN_H-60,16,kc);
            if(DrawSelectable(SCREEN_W-80,SCREEN_H-80,70,70)) m1KeyGlow=!m1KeyGlow;
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
        DrawScene(GetSceneIndex(base), 0.3f);
        int bx=290,by=100,bw=700,bh=500;
        DrawRectangle(bx,by,bw,bh,(Color){60,40,20,240});
        DrawRectangleLines(bx,by,bw,bh,(Color){139,90,43,255});
        DrawText("Machine Intelligence",bx+200,by+20,28,(Color){200,180,140,255});
        DrawRectangle(bx+bw/2-1,by+50,2,bh-60,(Color){100,70,30,200});
        // Left page
        if(m1BookPage>0) DrawText("[ empty page ]",bx+80,by+230,18,(Color){150,130,100,180});
        // Right page
        if(m1BookPage<8) DrawText("[ empty page ]",bx+bw/2+80,by+230,18,(Color){150,130,100,180});
        if(m1BookPage==8) {
            DrawText("3604",bx+bw/2+120,by+bh-80,36,(Color){80,40,10,255});
        }
        // Arrow buttons
        if(m1BookPage>0 && DrawButton(bx+20,by+bh-60,80,40,"< Prev",16)) m1BookPage--;
        if(m1BookPage<8 && DrawButton(bx+bw-100,by+bh-60,80,40,"Next >",16)) m1BookPage++;
        DrawText(TextFormat("Page %d/9",m1BookPage+1),bx+bw/2-30,by+bh-40,14,(Color){180,160,120,200});
        // Click outside
        Vector2 mp=GetMousePosition();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&&!(mp.x>=bx&&mp.x<=bx+bw&&mp.y>=by&&mp.y<=by+bh))
            ChangeState(m1CompOn?STATE_M1_ROOM_BASE2:STATE_M1_ROOM_BASE1);
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
    // Inventory always visible in M1
    if(curState>=STATE_M1_ROOM_BASE1 && curState<=STATE_M1_MAP && curState!=STATE_M1_HOUSE) {
        DrawRectangle(SCREEN_W-80,SCREEN_H-80,70,70,(Color){0,0,0,150});
        DrawRectangleLines(SCREEN_W-80,SCREEN_H-80,70,70,(Color){100,200,255,200});
        if(m1HasKey) {
            Color kc=m1KeyGlow?YELLOW:(Color){200,200,100,255};
            DrawText("KEY",SCREEN_W-72,SCREEN_H-60,16,kc);
        }
    }
}

// ===== TRAVEL SEQUENCE =====
void UpdateTravel() {
    int idx=(int)curState-(int)STATE_TRAVEL_FLIGHT; // 0-3
    DrawScene(28+idx);
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
