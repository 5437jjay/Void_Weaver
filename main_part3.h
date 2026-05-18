// main_part3.h - Module 2 (UTTT), Module 3 (Chess+Lift), Module 4 (Passlock+End)
#ifndef MAIN_PART3_H
#define MAIN_PART3_H
#include "main_part2.h"

// ===== MODULE 2: GARAGE + ULTIMATE TIC-TAC-TOE =====
void UpdateM2() {
    switch(curState) {
    case STATE_M2_GARAGE: {
        DrawScene(32);
        // Mirror in center is selectable
        if(DrawSelectable(480,100,320,500)) ChangeState(STATE_M2_MIRROR);
    } break;
    case STATE_M2_MIRROR: {
        DrawScene(33);
        // Auto-advance after 3 seconds with vortex
        if(stateTimer>3.0f) ChangeState(STATE_M2_VORTEX);
    } break;
    case STATE_M2_VORTEX: {
        // Vortex effect - rotate and scale scene
        vortexAngle += GetFrameTime()*200;
        vortexScale = 1.0f - stateTimer*0.3f;
        if(vortexScale<0.1f) vortexScale=0.1f;
        BeginBlendMode(BLEND_ADDITIVE);
        DrawTexturePro(scenes[34],
            {0,0,(float)scenes[34].width,(float)scenes[34].height},
            {640,360,(float)(SCREEN_W*vortexScale),(float)(SCREEN_H*vortexScale)},
            {(float)(SCREEN_W*vortexScale/2),(float)(SCREEN_H*vortexScale/2)},
            vortexAngle, WHITE);
        EndBlendMode();
        pageTimer+=GetFrameTime();
        DrawCinText("The mirror reveals a digital realm beyond...",pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) ChangeState(STATE_M2_TTT_INTRO);
    } break;
    case STATE_M2_TTT_INTRO: {
        DrawScene(35);
        pageTimer+=GetFrameTime();
        DrawCinText("Press SPACEBAR to begin the challenge...",pageTimer,0.04f,SCREEN_W);
        DrawText("Press SPACE to start",490,50,22,(Color){255,255,100,200});
        if(IsKeyPressed(KEY_SPACE)) { UTTTInit(uttt); ChangeState(STATE_M2_TTT_GAME); }
    } break;
    case STATE_M2_TTT_GAME: {
        // Draw background
        DrawRectangle(0,0,SCREEN_W,SCREEN_H,(Color){15,20,35,255});
        DrawText("ULTIMATE TIC-TAC-TOE",420,10,28,(Color){100,200,255,255});
        
        // HOST CODE button
        if(DrawButton(SCREEN_W-200,10,180,35,"HOST CODE",18)) uttt.hostMode=!uttt.hostMode;
        
        // Host code input
        if(uttt.hostMode) {
            DrawRectangle(SCREEN_W-250,55,230,40,(Color){20,30,60,230});
            DrawText("Code:",SCREEN_W-245,63,18,WHITE);
            char buf[7]; strncpy(buf,uttt.hostCode,uttt.hostLen); buf[uttt.hostLen]=0;
            DrawText(buf,SCREEN_W-180,63,18,YELLOW);
            // Input
            for(int k=KEY_A;k<=KEY_Z;k++){
                if(IsKeyPressed(k)&&uttt.hostLen<5){
                    uttt.hostCode[uttt.hostLen++]='A'+(k-KEY_A);
                }
            }
            if(IsKeyPressed(KEY_BACKSPACE)&&uttt.hostLen>0) uttt.hostLen--;
            // Check host code
            if(uttt.hostLen==5 && strncmp(uttt.hostCode,"VWDAZ",5)==0) {
                uttt.result=1; // Skip
            }
        }
        
        // Draw the 9 small boards
        int gridX=240, gridY=60, cellSz=20, boardGap=8;
        int boardSz=cellSz*3;
        int bigSz=boardSz*3+boardGap*2;
        
        for(int br=0;br<3;br++) for(int bc=0;bc<3;bc++) {
            int bi=br*3+bc;
            int bx=gridX+bc*(boardSz+boardGap);
            int by=gridY+br*(boardSz+boardGap);
            
            // Highlight active board
            bool isActive=(uttt.activeBoard==-1&&uttt.won[bi]==0)||(uttt.activeBoard==bi);
            Color bgCol=isActive?(Color){30,50,80,255}:(Color){20,25,35,255};
            if(uttt.won[bi]==1) bgCol=(Color){60,20,20,200};
            if(uttt.won[bi]==2) bgCol=(Color){20,20,60,200};
            DrawRectangle(bx,by,boardSz,boardSz,bgCol);
            
            // Draw grid lines
            for(int i=1;i<3;i++) {
                DrawLine(bx+i*cellSz,by,bx+i*cellSz,by+boardSz,(Color){60,80,120,200});
                DrawLine(bx,by+i*cellSz,bx+boardSz,by+i*cellSz,(Color){60,80,120,200});
            }
            
            // Draw cells
            for(int cr=0;cr<3;cr++) for(int cc=0;cc<3;cc++) {
                int ci=cr*3+cc;
                int cx=bx+cc*cellSz+2, cy=by+cr*cellSz+2;
                if(uttt.cells[bi][ci]==1) { // X - red
                    DrawLine(cx,cy,cx+cellSz-4,cy+cellSz-4,RED);
                    DrawLine(cx+cellSz-4,cy,cx,cy+cellSz-4,RED);
                } else if(uttt.cells[bi][ci]==2) { // O - blue
                    DrawCircleLines(cx+cellSz/2-2,cy+cellSz/2-2,cellSz/2-4,BLUE);
                }
            }
            
            // Won board overlay
            if(uttt.won[bi]==1) DrawText("X",bx+boardSz/2-15,by+boardSz/2-20,40,(Color){255,80,80,200});
            if(uttt.won[bi]==2) DrawText("O",bx+boardSz/2-15,by+boardSz/2-20,40,(Color){80,80,255,200});
            
            DrawRectangleLines(bx,by,boardSz,boardSz,(Color){60,100,160,200});
        }
        
        // Handle player click
        if(uttt.playerTurn && uttt.result==0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mp=GetMousePosition();
            for(int br=0;br<3;br++) for(int bc=0;bc<3;bc++) {
                int bi=br*3+bc;
                int bx=gridX+bc*(boardSz+boardGap);
                int by=gridY+br*(boardSz+boardGap);
                if(mp.x>=bx&&mp.x<bx+boardSz&&mp.y>=by&&mp.y<by+boardSz) {
                    int cc=(int)(mp.x-bx)/cellSz;
                    int cr=(int)(mp.y-by)/cellSz;
                    if(cc>=0&&cc<3&&cr>=0&&cr<3) {
                        int ci=cr*3+cc;
                        if(UTTTMove(uttt,bi,ci,1)) {
                            // Bot moves after player
                            if(uttt.result==0 && !uttt.playerTurn) UTTTBotMove(uttt);
                        }
                    }
                }
            }
        }
        
        // Status text
        if(uttt.result==1) {
            DrawText("YOU WIN!",520,SCREEN_H-60,36,GREEN);
            if(stateTimer>2.0f) StartSlide(STATE_M2_VEHICLE,1);
        } else if(uttt.result==2) {
            DrawText("BOT WINS! Try again...",440,SCREEN_H-60,28,RED);
            if(stateTimer>3.0f) { UTTTInit(uttt); stateTimer=0; }
        } else if(uttt.result==3) {
            DrawText("DRAW! Try again...",470,SCREEN_H-60,28,YELLOW);
            if(stateTimer>3.0f) { UTTTInit(uttt); stateTimer=0; }
        } else {
            DrawText(uttt.playerTurn?"Your turn (X)":"Bot thinking (O)...",
                480,SCREEN_H-40,20,(Color){180,200,230,255});
        }
    } break;
    case STATE_M2_VEHICLE: {
        DrawScene(38);
        // Click levitation vehicle
        if(DrawSelectable(400,200,480,400)) StartSlide(STATE_M3_COCKPIT,1);
    } break;
    default: break;
    }
}

// ===== MODULE 3: VEHICLE + CHESS + LIFT =====
void UpdateM3() {
    switch(curState) {
    case STATE_M3_COCKPIT: {
        DrawScene(39);
        if(DrawSelectable(400,100,480,520)) StartSlide(STATE_M3_WALL_SLIDE,3);
    } break;
    case STATE_M3_WALL_SLIDE: {
        DrawScene(40);
        if(stateTimer>2.0f) ChangeState(STATE_M3_FLOAT_UP);
    } break;
    case STATE_M3_FLOAT_UP: {
        DrawScene(41);
        pageTimer+=GetFrameTime();
        DrawCinText("Agent Zero: This vehicle is an Automatic Levitation Vehicle.",pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_LEV_VEH,1);
    } break;
    case STATE_M3_LEV_VEH: {
        DrawScene(42);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[0],pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_RED_LINE,1);
    } break;
    case STATE_M3_RED_LINE: {
        DrawScene(43);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[1],pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_LAB_DOOR,1);
    } break;
    case STATE_M3_LAB_DOOR: {
        DrawScene(44);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[2],pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_LAB_ENTER,1);
    } break;
    case STATE_M3_LAB_ENTER: {
        DrawScene(45);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[3],pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_NEAR_WALL,1);
    } break;
    case STATE_M3_NEAR_WALL: {
        DrawScene(46);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[4],pageTimer,0.04f,SCREEN_W);
        if(DrawSelectable(400,100,480,500)) StartSlide(STATE_M3_WALL_ZOOM,1);
    } break;
    case STATE_M3_WALL_ZOOM: {
        DrawScene(47);
        if(stateTimer>2.0f) ChangeState(STATE_M3_CHESS_INTRO);
    } break;
    case STATE_M3_CHESS_INTRO: {
        DrawScene(48);
        pageTimer+=GetFrameTime();
        DrawCinText("A chess challenge awaits... Solve the puzzle to proceed.",pageTimer,0.04f,SCREEN_W);
        if(stateTimer>3.0f) { ChessInit(chess); ChangeState(STATE_M3_CHESS_GAME); }
    } break;
    case STATE_M3_CHESS_GAME: {
        DrawRectangle(0,0,SCREEN_W,SCREEN_H,(Color){20,25,40,255});
        DrawText("CHESS PUZZLE",500,10,28,(Color){100,200,255,255});
        // HOST CODE + RESET buttons
        if(DrawButton(SCREEN_W-200,10,180,35,"HOST CODE",18)) chess.hostMode=!chess.hostMode;
        if(DrawButton(SCREEN_W-200,55,180,35,"RESET",18)) ChessInit(chess);
        // Host code input
        if(chess.hostMode) {
            DrawRectangle(SCREEN_W-250,100,230,40,(Color){20,30,60,230});
            DrawText("Code:",SCREEN_W-245,108,18,WHITE);
            char buf[7]; strncpy(buf,chess.host,chess.hostLen); buf[chess.hostLen]=0;
            DrawText(buf,SCREEN_W-180,108,18,YELLOW);
            for(int k=KEY_A;k<=KEY_Z;k++){
                if(IsKeyPressed(k)&&chess.hostLen<5) chess.host[chess.hostLen++]='A'+(k-KEY_A);
            }
            if(IsKeyPressed(KEY_BACKSPACE)&&chess.hostLen>0) chess.hostLen--;
            if(chess.hostLen==5&&strncmp(chess.host,"VWDAZ",5)==0) chess.result=2;
        }
        // Draw chess board
        int bx=340,by=60,sq=75;
        const char* cols="ABCDEFGH";
        const char* pieceChars="  PRNBQKprnbqk";
        Color lightSq={220,210,180,255}, darkSq={120,100,70,255};
        for(int r=0;r<8;r++) for(int c=0;c<8;c++) {
            int dr=7-r; // Display row (flip board)
            int x=bx+c*sq, y=by+dr*sq;
            bool light=((r+c)%2==0);
            Color sqCol=light?lightSq:darkSq;
            // Highlight selected piece
            if(chess.selR==r&&chess.selC==c) sqCol=(Color){100,200,100,255};
            // Highlight valid moves
            if(chess.highlights[r][c]) sqCol=(Color){100,255,100,150};
            DrawRectangle(x,y,sq,sq,sqCol);
            // Draw piece
            int p=chess.board[r][c];
            if(p!=CE) {
                Color pc=IsWhite(p)?WHITE:(Color){40,40,40,255};
                char ch[2]={pieceChars[p],0};
                DrawText(ch,x+sq/2-8,y+sq/2-12,28,pc);
            }
        }
        // Row/col labels
        for(int i=0;i<8;i++) {
            char lb[2]={cols[i],0};
            DrawText(lb,bx+i*sq+sq/2-5,by+8*sq+5,16,(Color){180,180,180,255});
            char rn[2]={(char)('1'+i),0};
            DrawText(rn,bx-20,by+(7-i)*sq+sq/2-8,16,(Color){180,180,180,255});
        }
        // Handle clicks
        if(chess.result==0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mp=GetMousePosition();
            int cc=(int)(mp.x-bx)/sq, dr=(int)(mp.y-by)/sq;
            if(cc>=0&&cc<8&&dr>=0&&dr<8) {
                int cr=7-dr;
                if(chess.selR<0) {
                    // Select a white piece
                    if(IsWhite(chess.board[cr][cc])) {
                        chess.selR=cr; chess.selC=cc;
                        memset(chess.highlights,0,sizeof(chess.highlights));
                        int mvs[64][2];
                        int n=GetMoves(chess.board,cr,cc,mvs,true);
                        for(int i=0;i<n;i++) chess.highlights[mvs[i][0]][mvs[i][1]]=true;
                    }
                } else {
                    // Try to move
                    if(chess.highlights[cr][cc]) {
                        chess.board[cr][cc]=chess.board[chess.selR][chess.selC];
                        chess.board[chess.selR][chess.selC]=CE;
                        // Check for checkmate
                        if(IsCheckmate(chess.board,false)) chess.result=1;
                        else { ChessBotMove(chess); if(IsCheckmate(chess.board,true)) chess.result=2; }
                    }
                    chess.selR=chess.selC=-1;
                    memset(chess.highlights,0,sizeof(chess.highlights));
                }
            }
        }
        if(chess.result==1) { DrawText("CHECKMATE! You Win!",450,SCREEN_H-50,28,GREEN); }
        if(chess.result==2) { DrawText("Puzzle Complete!",470,SCREEN_H-50,28,GREEN); }
        if(chess.result>0 && stateTimer>2.0f) StartSlide(STATE_M3_LIFT_CORR,1);
    } break;
    case STATE_M3_LIFT_CORR: {
        DrawScene(50);
        if(stateTimer>2.0f) ChangeState(STATE_M3_LIFT_IN);
    } break;
    case STATE_M3_LIFT_IN: {
        DrawScene(51);
        if(stateTimer>2.0f) { liftFloor=28; liftTimer=0; ChangeState(STATE_M3_LIFT_DESC); }
    } break;
    case STATE_M3_LIFT_DESC: {
        DrawScene(52);
        // Floor counter descending - 0.25 sec per floor (~7 seconds total)
        liftTimer+=GetFrameTime();
        if(liftTimer>=0.25f && liftFloor>0) { liftFloor--; liftTimer=0; }
        // Display floor number
        DrawRectangle(580,80,120,60,(Color){0,0,0,200});
        DrawRectangleLines(580,80,120,60,(Color){0,200,255,200});
        DrawText(TextFormat("Floor %d",liftFloor),590,95,24,(Color){0,255,200,255});
        // Progress bar
        float prog = 1.0f - (float)liftFloor/28.0f;
        DrawRectangle(580,150,120,10,(Color){30,30,30,200});
        DrawRectangle(580,150,(int)(120*prog),10,(Color){0,255,200,200});
        if(liftFloor<=0 && stateTimer>8.0f) StartFade(STATE_M4_SUBMARINE);
    } break;
    default: break;
    }
}

// ===== MODULE 4: PASSLOCK + AI + ENDING =====
void UpdateM4() {
    switch(curState) {
    case STATE_M4_SUBMARINE: {
        DrawScene(53);
        // Passlock system selectable
        if(DrawSelectable(550,300,180,200)) ChangeState(STATE_M4_PASSLOCK);
    } break;
    case STATE_M4_PASSLOCK: {
        DrawScene(54);
        // Mute background music during passlock
        if(!bgMusicMuted) { SetSoundVolume(bgMusic,0); bgMusicMuted=true; }
        // Draw keypad
        int kx=440,ky=200,ksz=70;
        DrawRectangle(kx-20,ky-60,360,420,(Color){10,15,30,230});
        DrawText("ENTER PASSCODE",kx+60,ky-50,22,(Color){0,200,255,255});
        // Show entered digits
        for(int i=0;i<4;i++) {
            int dx=kx+40+i*70;
            DrawRectangle(dx,ky,55,55,(Color){20,40,70,255});
            DrawRectangleLines(dx,ky,55,55,(Color){0,150,255,200});
            if(i<passPos) {
                char d[2]={(char)('0'+passCode[i]),0};
                DrawText(d,dx+18,ky+10,36,WHITE);
            }
        }
        // Number keypad grid (1-9, 0)
        for(int r=0;r<3;r++) for(int c=0;c<3;c++) {
            int num=r*3+c+1;
            int bx2=kx+c*ksz+40, by2=ky+80+r*ksz;
            if(DrawButton(bx2,by2,ksz-5,ksz-5,TextFormat("%d",num),28)) {
                if(passPos<4) { passCode[passPos++]=num; PlaySound(keySnds[num]); stateTimer=0; }
            }
        }
        // Zero button
        if(DrawButton(kx+ksz+40,ky+80+3*ksz,ksz-5,ksz-5,"0",28)) {
            if(passPos<4) { passCode[passPos++]=0; PlaySound(keySnds[0]); stateTimer=0; }
        }
        // Check code when 4 digits entered
        if(passPos>=4) {
            bool ok=(passCode[0]==8&&passCode[1]==1&&passCode[2]==5&&passCode[3]==7);
            if(ok) {
                DrawText("VAULT OPENING...",kx+70,ky+380,24,GREEN);
                if(stateTimer>2.0f) {
                    passUnlocked=true;
                    SetSoundVolume(bgMusic,0.5f); bgMusicMuted=false;
                    StartFade(STATE_M4_AI_ROOM);
                }
            } else {
                DrawText("WRONG CODE",kx+100,ky+380,24,RED);
                if(stateTimer>1.5f) { passPos=0; stateTimer=0; }
            }
        }
    } break;
    case STATE_M4_AI_ROOM: {
        DrawScene(55);
        // AI machine selectable
        if(DrawSelectable(450,150,380,400)) StartFade(STATE_M4_BREAK);
    } break;
    case STATE_M4_BREAK: {
        DrawScene(56);
        // Play glass breaking sound once
        if(stateTimer<0.1f) {
            StopSound(bgMusic);
            PlaySound(glassSound);
        }
        // Auto advance
        if(stateTimer>4.0f) StartFade(STATE_M4_EXPLODE);
    } break;
    case STATE_M4_EXPLODE: {
        DrawScene(57);
        // Explosion effect - screen shake
        if(stateTimer<2.0f) {
            float shake=sinf(stateTimer*50)*5*(2.0f-stateTimer);
            DrawScene(57);
        }
        if(stateTimer>4.0f) StartFade(STATE_M4_ENDING);
    } break;
    case STATE_M4_ENDING: {
        DrawScene(58);
        pageTimer+=GetFrameTime();
        DrawCinText("James: Nice man, you faced the impossible, carried the fate of millions in\n"
            "your hands, and returned victorious. The world owes you its tomorrow.",
            pageTimer,0.04f,SCREEN_W);
        bool done=(int)(pageTimer/0.04f)>=150;
        if(done) {
            DrawText("THE END",550,50,40,(Color){255,215,0,255});
            DrawText("Click to return to title...",490,100,20,(Color){200,200,200,200});
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                // Reset everything
                m1CompOn=m1DeskOpen=m1PhotoSel=m1HasKey=m1KeyGlow=false;
                m1MirrorOpen=m1LetterRead=m1CompLogged=false;
                m1CompPos=0; m1BookPage=0; passPos=0; passUnlocked=false;
                memset(m1CompCode,0,16); memset(m1DeskCode,0,16);
                ChangeState(STATE_TITLE);
            }
        }
    } break;
    default: break;
    }
}

#endif
