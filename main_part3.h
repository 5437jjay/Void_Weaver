// main_part3.h - Module 2 (UTTT), Module 3 (Chess+Lift), Module 4 (Passlock+End)
#ifndef MAIN_PART3_H
#define MAIN_PART3_H
#include "main_part2.h"

// ===== MODULE 2: GARAGE + ULTIMATE TIC-TAC-TOE =====
void UpdateM2() {
    switch(curState) {
    case STATE_M2_GARAGE: {
        DrawScene(32, 1.0f, true);
        // Mirror in center is selectable
        if(DrawSelectable(480,100,320,500)) ChangeState(STATE_M2_MIRROR);
    } break;
    case STATE_M2_MIRROR: {
        DrawScene(33, 1.0f, true);
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
        DrawScene(35, 1.0f, true);
        pageTimer+=GetFrameTime();
        DrawCinText("Press SPACEBAR to begin the challenge...",pageTimer,0.04f,SCREEN_W);
        DrawText("Press SPACE to start",490,50,22,(Color){255,255,100,200});
        if(IsKeyPressed(KEY_SPACE)) { UTTTInit(uttt); ChangeState(STATE_M2_TTT_GAME); }
    } break;
    case STATE_M2_TTT_GAME: {
        // ======== BACKGROUND (light lavender/grey) ========
        DrawRectangle(0,0,SCREEN_W,SCREEN_H,(Color){210,215,225,255});

        // ======== BOARD LAYOUT ========
        int gridX=170, gridY=40;
        int boardSz=180, cellSz=boardSz/3, boardGap=18;
        int totalSz=boardSz*3+boardGap*2;

        // Outer frame
        DrawRectangleRounded({(float)(gridX-12),(float)(gridY-12),
            (float)(totalSz+24),(float)(totalSz+24)},0.03f,8,(Color){195,200,212,255});

        // ======== DRAW 9 SMALL BOARDS ========
        for(int br=0;br<3;br++) for(int bc=0;bc<3;bc++){
            int bi=br*3+bc;
            int bx=gridX+bc*(boardSz+boardGap);
            int by=gridY+br*(boardSz+boardGap);

            // Board background
            Color bgCol={235,238,245,255}; // Default light
            bool isActive=(!uttt.pickingBoard && uttt.activeBoard==bi);
            bool isSelectable=(uttt.pickingBoard && uttt.won[bi]==0);
            bool isWon=(uttt.won[bi]==1||uttt.won[bi]==2);

            if(isActive) bgCol=(Color){220,235,255,255}; // Active: subtle blue
            if(isSelectable) bgCol=(Color){225,240,225,255}; // Selectable: subtle green
            if(isWon) bgCol=(Color){200,200,210,200}; // Won: greyed

            DrawRectangleRounded({(float)bx,(float)by,(float)boardSz,(float)boardSz},
                0.08f,8,bgCol);

            // Active board glow border
            if(isActive)
                DrawRectangleRoundedLinesEx({(float)(bx-2),(float)(by-2),
                    (float)(boardSz+4),(float)(boardSz+4)},0.08f,8,3,(Color){80,150,255,200});
            else if(isSelectable)
                DrawRectangleRoundedLinesEx({(float)(bx-1),(float)(by-1),
                    (float)(boardSz+2),(float)(boardSz+2)},0.08f,8,2,(Color){80,200,80,150});
            else
                DrawRectangleRoundedLinesEx({(float)bx,(float)by,
                    (float)boardSz,(float)boardSz},0.08f,8,1,(Color){190,195,210,200});

            // Grid lines inside small board
            Color lineCol={200,205,218,255};
            for(int i=1;i<3;i++){
                DrawLineEx({(float)(bx+i*cellSz),(float)(by+4)},
                    {(float)(bx+i*cellSz),(float)(by+boardSz-4)},1.5f,lineCol);
                DrawLineEx({(float)(bx+4),(float)(by+i*cellSz)},
                    {(float)(bx+boardSz-4),(float)(by+i*cellSz)},1.5f,lineCol);
            }

            // Draw cells (X and O)
            if(!isWon){
                for(int cr=0;cr<3;cr++) for(int cc=0;cc<3;cc++){
                    int ci=cr*3+cc;
                    int cx=bx+cc*cellSz, cy=by+cr*cellSz;
                    int pad=12;
                    if(uttt.cells[bi][ci]==1){ // X - coral red
                        DrawLineEx({(float)(cx+pad),(float)(cy+pad)},
                            {(float)(cx+cellSz-pad),(float)(cy+cellSz-pad)},3,(Color){220,80,80,255});
                        DrawLineEx({(float)(cx+cellSz-pad),(float)(cy+pad)},
                            {(float)(cx+pad),(float)(cy+cellSz-pad)},3,(Color){220,80,80,255});
                    } else if(uttt.cells[bi][ci]==2){ // O - blue
                        DrawCircleLinesV({(float)(cx+cellSz/2),(float)(cy+cellSz/2)},
                            (float)(cellSz/2-pad),(Color){70,100,200,255});
                        DrawCircleLinesV({(float)(cx+cellSz/2),(float)(cy+cellSz/2)},
                            (float)(cellSz/2-pad-1),(Color){70,100,200,255});
                    }
                }
            }

            // Won board: large X or O overlay
            if(uttt.won[bi]==1){
                int pad=20;
                DrawLineEx({(float)(bx+pad),(float)(by+pad)},
                    {(float)(bx+boardSz-pad),(float)(by+boardSz-pad)},6,(Color){220,60,60,200});
                DrawLineEx({(float)(bx+boardSz-pad),(float)(by+pad)},
                    {(float)(bx+pad),(float)(by+boardSz-pad)},6,(Color){220,60,60,200});
            }
            if(uttt.won[bi]==2){
                DrawCircleLinesV({(float)(bx+boardSz/2),(float)(by+boardSz/2)},
                    (float)(boardSz/2-20),(Color){60,80,200,200});
                DrawCircleLinesV({(float)(bx+boardSz/2),(float)(by+boardSz/2)},
                    (float)(boardSz/2-22),(Color){60,80,200,200});
            }
        }

        // ======== HOST CODE BUTTON (golden, top-right) ========
        {
            int hx=SCREEN_W-175,hy=10,hw=155,hh=36;
            Vector2 mp=GetMousePosition();
            bool hv=mp.x>=hx&&mp.x<=hx+hw&&mp.y>=hy&&mp.y<=hy+hh;
            Color bg=hv?(Color){225,185,55,240}:(Color){195,155,40,220};
            DrawRectangleRounded({(float)hx,(float)hy,(float)hw,(float)hh},0.4f,8,bg);
            DrawRectangleRoundedLinesEx({(float)hx,(float)hy,(float)hw,(float)hh},0.4f,8,2,
                hv?(Color){255,225,100,255}:(Color){210,170,50,200});
            int tw2=MeasureText("HOST CODE",18);
            DrawText("HOST CODE",hx+(hw-tw2)/2,hy+9,18,(Color){40,20,0,255});
            if(hv&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) uttt.hostMode=!uttt.hostMode;
        }
        if(uttt.hostMode){
            DrawRectangle(SCREEN_W-215,54,195,34,(Color){40,45,60,235});
            DrawRectangleLinesEx({(float)(SCREEN_W-215),54,195,34},1,(Color){100,150,200,180});
            DrawText("Code:",SCREEN_W-210,62,16,WHITE);
            char buf[7]; strncpy(buf,uttt.hostCode,uttt.hostLen); buf[uttt.hostLen]=0;
            DrawText(buf,SCREEN_W-150,62,16,YELLOW);
            for(int k=KEY_A;k<=KEY_Z;k++)
                if(IsKeyPressed(k)&&uttt.hostLen<5) uttt.hostCode[uttt.hostLen++]='A'+(k-KEY_A);
            if(IsKeyPressed(KEY_BACKSPACE)&&uttt.hostLen>0) uttt.hostLen--;
            if(uttt.hostLen==5&&strncmp(uttt.hostCode,"VWDAZ",5)==0) uttt.result=1;
        }

        // ======== STATUS TEXT ========
        {
            const char* status="";
            Color stCol={60,70,90,220};
            if(uttt.result==0){
                if(uttt.pickingBoard && uttt.picker==1)
                    status="Pick a board to play in";
                else if(uttt.pickingBoard && uttt.picker==2)
                    status="Bot is picking a board...";
                else if(uttt.playerTurn)
                    status="Your turn (X)";
                else
                    status="Bot thinking (O)...";
            }
            int sw=MeasureText(status,20);
            DrawText(status,(SCREEN_W-sw)/2,SCREEN_H-45,20,stCol);
        }

        // ======== HANDLE PLAYER CLICKS ========
        if(uttt.result==0 && uttt.playerTurn && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mp=GetMousePosition();
            for(int br=0;br<3;br++) for(int bc=0;bc<3;bc++){
                int bi=br*3+bc;
                int bx=gridX+bc*(boardSz+boardGap);
                int by=gridY+br*(boardSz+boardGap);
                if(mp.x>=bx&&mp.x<bx+boardSz&&mp.y>=by&&mp.y<by+boardSz){
                    if(uttt.pickingBoard && uttt.picker==1){
                        UTTTPickBoard(uttt,bi);
                    } else if(!uttt.pickingBoard && uttt.activeBoard==bi){
                        int cc2=(int)(mp.x-bx)/cellSz;
                        int cr2=(int)(mp.y-by)/cellSz;
                        if(cc2>=0&&cc2<3&&cr2>=0&&cr2<3){
                            int ci=cr2*3+cc2;
                            UTTTMove(uttt,bi,ci,1);
                        }
                    }
                }
            }
        }

        // ======== BOT TURN ========
        if(uttt.result==0 && !uttt.playerTurn){
            UTTTBotMove(uttt);
        }

        // ======== GAME END MESSAGES ========
        if(uttt.result==1){
            DrawRectangle(0,SCREEN_H/2-35,SCREEN_W,70,(Color){0,0,0,180});
            DrawText("YOU WIN!",SCREEN_W/2-70,SCREEN_H/2-14,28,(Color){0,255,120,255});
            if(stateTimer>2.0f) StartSlide(STATE_M2_VEHICLE,1);
        } else if(uttt.result==2){
            DrawRectangle(0,SCREEN_H/2-35,SCREEN_W,70,(Color){0,0,0,180});
            DrawText("BOT WINS! Resetting...",SCREEN_W/2-140,SCREEN_H/2-14,24,(Color){255,80,80,255});
            if(stateTimer>3.0f){UTTTInit(uttt);uttt.score[0]=uttt.score[0];stateTimer=0;}
        } else if(uttt.result==3){
            DrawRectangle(0,SCREEN_H/2-35,SCREEN_W,70,(Color){0,0,0,180});
            DrawText("DRAW! Resetting...",SCREEN_W/2-110,SCREEN_H/2-14,24,(Color){255,200,0,255});
            if(stateTimer>3.0f){UTTTInit(uttt);stateTimer=0;}
        }
    } break;
    case STATE_M2_VEHICLE: {
        DrawScene(38, 1.0f, true);
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
        DrawScene(39, 1.0f, true);
        if(DrawSelectable(400,100,480,520)) StartSlide(STATE_M3_WALL_SLIDE,3);
    } break;
    case STATE_M3_WALL_SLIDE: {
        DrawScene(40, 1.0f, true);
        if(stateTimer>2.0f) ChangeState(STATE_M3_FLOAT_UP);
    } break;
    case STATE_M3_FLOAT_UP: {
        DrawScene(41, 1.0f, true);
        pageTimer+=GetFrameTime();
        DrawCinText("Agent Zero: This vehicle is an Automatic Levitation Vehicle.",pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_LEV_VEH,1);
    } break;
    case STATE_M3_LEV_VEH: {
        DrawScene(42, 1.0f, true);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[0],pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_RED_LINE,1);
    } break;
    case STATE_M3_RED_LINE: {
        DrawScene(43, 1.0f, true);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[1],pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_LAB_DOOR,1);
    } break;
    case STATE_M3_LAB_DOOR: {
        DrawScene(44, 1.0f, true);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[2],pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_LAB_ENTER,1);
    } break;
    case STATE_M3_LAB_ENTER: {
        DrawScene(45, 1.0f, true);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[3],pageTimer,0.04f,SCREEN_W);
        if(stateTimer>4.0f) StartSlide(STATE_M3_NEAR_WALL,1);
    } break;
    case STATE_M3_NEAR_WALL: {
        DrawScene(46, 1.0f, true);
        pageTimer+=GetFrameTime();
        DrawCinText(m3Texts[4],pageTimer,0.04f,SCREEN_W);
        if(DrawSelectable(400,100,480,500)) StartSlide(STATE_M3_WALL_ZOOM,1);
    } break;
    case STATE_M3_WALL_ZOOM: {
        DrawScene(47);
        if(stateTimer>2.0f) ChangeState(STATE_M3_CHESS_INTRO);
    } break;
    case STATE_M3_CHESS_INTRO: {
        DrawScene(48, 1.0f, true);
        pageTimer+=GetFrameTime();
        DrawCinText("A chess challenge awaits... Solve the puzzle to proceed.",pageTimer,0.04f,SCREEN_W);
        if(stateTimer>3.0f) { ChessInit(chess); ChangeState(STATE_M3_CHESS_GAME); }
    } break;
    case STATE_M3_CHESS_GAME: {
        // ======== BACKGROUND ========
        DrawRectangle(0,0,SCREEN_W,SCREEN_H,(Color){8,14,30,255});
        // Subtle grid pattern
        for(int gx=0;gx<SCREEN_W;gx+=40) DrawLine(gx,0,gx,SCREEN_H,(Color){20,35,60,40});
        for(int gy=0;gy<SCREEN_H;gy+=40) DrawLine(0,gy,SCREEN_W,gy,(Color){20,35,60,40});

        // ======== BOARD LAYOUT ========
        int sq=70, boardPx=sq*8;
        int bx=(SCREEN_W-boardPx)/2, by=52;

        // Outer glow frame
        DrawRectangleLinesEx({(float)(bx-10),(float)(by-10),(float)(boardPx+20),(float)(boardPx+20)},2,(Color){0,180,255,120});
        DrawRectangleLinesEx({(float)(bx-6),(float)(by-6),(float)(boardPx+12),(float)(boardPx+12)},1,(Color){0,120,200,80});
        DrawRectangle(bx-4,by-4,boardPx+8,boardPx+8,(Color){15,25,50,200});

        // ======== SQUARES ========
        Color ltSq={165,190,215,255}, dkSq={50,80,120,255};
        for(int r=0;r<8;r++) for(int c=0;c<8;c++){
            int dr=7-r;
            int x=bx+c*sq, y2=by+dr*sq;
            bool light=((r+c)%2==0);
            Color col=light?ltSq:dkSq;
            // Last move highlight
            if((r==chess.lastFR&&c==chess.lastFC)||(r==chess.lastTR&&c==chess.lastTC))
                col=light?(Color){180,205,145,255}:(Color){90,130,75,255};
            // Selected piece
            if(chess.selR==r&&chess.selC==c)
                col=(Color){90,195,115,255};
            // Legal moves
            if(chess.highlights[r][c])
                col=light?(Color){130,210,130,200}:(Color){70,160,70,200};
            DrawRectangle(x,y2,sq,sq,col);
            // Legal move dot/ring
            if(chess.highlights[r][c]){
                if(chess.board[r][c]==CE)
                    DrawCircle(x+sq/2,y2+sq/2,9,(Color){0,0,0,70});
                else
                    DrawRing({(float)(x+sq/2),(float)(y2+sq/2)},(float)(sq/2-5),(float)(sq/2-1),0,360,36,(Color){0,0,0,70});
            }
            // ======== DRAW PIECE ========
            int p=chess.board[r][c];
            if(p!=CE){
                const char* sym="";
                switch(p){
                    case WP:case BP:sym="P";break; case WR:case BR:sym="R";break;
                    case WN:case BN:sym="N";break; case WB:case BB:sym="B";break;
                    case WQ:case BQ:sym="Q";break; case WK:case BK:sym="K";break;
                }
                int fs=38;
                int tw=MeasureText(sym,fs), tx=x+(sq-tw)/2, ty=y2+(sq-fs)/2;
                if(IsWhite(p)){
                    for(int dx=-2;dx<=2;dx++) for(int dy=-2;dy<=2;dy++)
                        if(dx||dy) DrawText(sym,tx+dx,ty+dy,fs,(Color){30,30,30,220});
                    DrawText(sym,tx,ty,fs,(Color){240,240,235,255});
                } else {
                    for(int dx=-2;dx<=2;dx++) for(int dy=-2;dy<=2;dy++)
                        if(dx||dy) DrawText(sym,tx+dx,ty+dy,fs,(Color){160,160,160,100});
                    DrawText(sym,tx,ty,fs,(Color){25,25,25,255});
                }
            }
        }

        // ======== FILE & RANK LABELS ========
        const char* files="ABCDEFGH";
        Color lblC={120,180,240,200};
        for(int i=0;i<8;i++){
            char fb[2]={files[i],0};
            DrawText(fb,bx+i*sq+sq/2-5,by-24,18,lblC);
            DrawText(fb,bx+i*sq+sq/2-5,by+boardPx+8,18,lblC);
            char rb[2]={(char)('1'+i),0};
            DrawText(rb,bx-24,by+(7-i)*sq+sq/2-9,18,lblC);
            DrawText(rb,bx+boardPx+12,by+(7-i)*sq+sq/2-9,18,lblC);
        }

        // ======== HOST CODE BUTTON (golden, top-right) ========
        {
            int hx=SCREEN_W-175,hy=10,hw=155,hh=36;
            Vector2 mp=GetMousePosition();
            bool hv=mp.x>=hx&&mp.x<=hx+hw&&mp.y>=hy&&mp.y<=hy+hh;
            Color bg=hv?(Color){225,185,55,240}:(Color){195,155,40,220};
            DrawRectangleRounded({(float)hx,(float)hy,(float)hw,(float)hh},0.4f,8,bg);
            DrawRectangleRoundedLinesEx({(float)hx,(float)hy,(float)hw,(float)hh},0.4f,8,2,
                hv?(Color){255,225,100,255}:(Color){210,170,50,200});
            int tw2=MeasureText("HOST CODE",18);
            DrawText("HOST CODE",hx+(hw-tw2)/2,hy+9,18,(Color){40,20,0,255});
            if(hv&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) chess.hostMode=!chess.hostMode;
        }
        // Host code input
        if(chess.hostMode){
            DrawRectangle(SCREEN_W-215,54,195,34,(Color){12,22,48,235});
            DrawRectangleLinesEx({(float)(SCREEN_W-215),54,195,34},1,(Color){0,140,255,140});
            DrawText("Code:",SCREEN_W-210,62,16,WHITE);
            char buf[7]; strncpy(buf,chess.host,chess.hostLen); buf[chess.hostLen]=0;
            DrawText(buf,SCREEN_W-150,62,16,YELLOW);
            for(int k=KEY_A;k<=KEY_Z;k++)
                if(IsKeyPressed(k)&&chess.hostLen<5) chess.host[chess.hostLen++]='A'+(k-KEY_A);
            if(IsKeyPressed(KEY_BACKSPACE)&&chess.hostLen>0) chess.hostLen--;
            if(chess.hostLen==5&&strncmp(chess.host,"VWDAZ",5)==0) chess.result=2;
        }

        // ======== RESET BUTTON (golden, bottom-center) ========
        {
            int rw=220,rh=40;
            int rx=(SCREEN_W-rw)/2, ry=by+boardPx+32;
            Vector2 mp=GetMousePosition();
            bool hv=mp.x>=rx&&mp.x<=rx+rw&&mp.y>=ry&&mp.y<=ry+rh;
            Color bg=hv?(Color){225,185,55,240}:(Color){195,155,40,220};
            DrawRectangleRounded({(float)rx,(float)ry,(float)rw,(float)rh},0.4f,8,bg);
            DrawRectangleRoundedLinesEx({(float)rx,(float)ry,(float)rw,(float)rh},0.4f,8,2,
                hv?(Color){255,225,100,255}:(Color){210,170,50,200});
            int tw3=MeasureText("RESET",22);
            DrawText("RESET",rx+(rw-tw3)/2,ry+9,22,(Color){40,20,0,255});
            if(hv&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ChessInit(chess);
        }

        // ======== AI DIFFICULTY SELECTOR ========
        {
            const char* df[]={"EASY","MED","HARD"};
            Color dc[]={(Color){80,200,80,255},(Color){220,200,50,255},(Color){255,70,70,255}};
            for(int i=0;i<3;i++){
                int dx=bx+boardPx+18, dy=by+200+i*35;
                bool sel=(chess.aiDiff==i);
                Color tc=sel?dc[i]:(Color){70,70,70,180};
                DrawText(df[i],dx,dy,14,tc);
                if(sel) DrawRectangleLinesEx({(float)(dx-3),(float)(dy-2),
                    (float)(MeasureText(df[i],14)+6),18.0f},1,tc);
                Vector2 mp=GetMousePosition();
                if(mp.x>=dx-3&&mp.x<=dx+55&&mp.y>=dy-2&&mp.y<=dy+16
                    &&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&&chess.result==0)
                    chess.aiDiff=i;
            }
            DrawText("AI",bx+boardPx+22,by+180,14,(Color){100,150,200,180});
        }

        // ======== CHECK INDICATOR ========
        if(chess.result==0 && InCheck(chess.board,chess.whiteTurn)){
            float pulse=(sinf(animTime*5)+1)*0.5f;
            unsigned char a=(unsigned char)(180+pulse*75);
            DrawText("CHECK!",(SCREEN_W-MeasureText("CHECK!",22))/2,by+boardPx+10,22,(Color){255,50,50,a});
        }

        // ======== TURN INDICATOR ========
        if(chess.result==0 && !chess.promoting){
            const char* turnTxt=chess.whiteTurn?"Your turn (White)":"AI thinking...";
            Color turnCol=chess.whiteTurn?(Color){200,220,255,200}:(Color){255,180,80,200};
            DrawText(turnTxt,bx,by+boardPx+12,16,turnCol);
        }

        // ======== PROMOTION UI ========
        if(chess.promoting){
            DrawRectangle(0,0,SCREEN_W,SCREEN_H,(Color){0,0,0,160});
            int pw=320,ph=120;
            int px2=(SCREEN_W-pw)/2, py2=(SCREEN_H-ph)/2;
            DrawRectangleRounded({(float)px2,(float)py2,(float)pw,(float)ph},0.15f,8,(Color){20,35,65,240});
            DrawRectangleRoundedLinesEx({(float)px2,(float)py2,(float)pw,(float)ph},0.15f,8,2,(Color){0,160,255,200});
            DrawText("Promote to:",px2+90,py2+10,20,(Color){200,220,255,255});
            const char* opts[]={"Q","R","B","N"};
            int pcs[]={WQ,WR,WB,WN};
            for(int i=0;i<4;i++){
                int ox=px2+20+i*75, oy=py2+45;
                Vector2 mp=GetMousePosition();
                bool hv=mp.x>=ox&&mp.x<=ox+60&&mp.y>=oy&&mp.y<=oy+60;
                DrawRectangle(ox,oy,60,60,hv?(Color){60,110,180,255}:(Color){30,55,100,255});
                DrawRectangleLinesEx({(float)ox,(float)oy,60,60},2,(Color){80,160,255,255});
                int otw=MeasureText(opts[i],32);
                DrawText(opts[i],ox+(60-otw)/2,oy+14,32,WHITE);
                if(hv&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    ChessPromote(chess,pcs[i]);
                    ChessCheckEnd(chess);
                }
            }
        }
        // ======== PLAYER INPUT ========
        else if(chess.result==0 && chess.whiteTurn && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mp=GetMousePosition();
            int mc=(int)(mp.x-bx)/sq, mdr=(int)(mp.y-by)/sq;
            if(mc>=0&&mc<8&&mdr>=0&&mdr<8){
                int mr=7-mdr;
                if(chess.selR<0){
                    if(IsWhite(chess.board[mr][mc])){
                        chess.selR=mr; chess.selC=mc;
                        memset(chess.highlights,0,sizeof(chess.highlights));
                        int mvs[64][2];
                        int nm=GetMoves(chess.board,mr,mc,mvs,true,chess.epCol,
                            chess.wKMoved,chess.wRA_Moved,chess.wRH_Moved,
                            chess.bKMoved,chess.bRA_Moved,chess.bRH_Moved);
                        for(int i=0;i<nm;i++) chess.highlights[mvs[i][0]][mvs[i][1]]=true;
                    }
                } else {
                    if(chess.highlights[mr][mc]){
                        ChessDoMove(chess,chess.selR,chess.selC,mr,mc);
                        if(!chess.promoting) ChessCheckEnd(chess);
                    } else if(IsWhite(chess.board[mr][mc])){
                        chess.selR=mr; chess.selC=mc;
                        memset(chess.highlights,0,sizeof(chess.highlights));
                        int mvs[64][2];
                        int nm=GetMoves(chess.board,mr,mc,mvs,true,chess.epCol,
                            chess.wKMoved,chess.wRA_Moved,chess.wRH_Moved,
                            chess.bKMoved,chess.bRA_Moved,chess.bRH_Moved);
                        for(int i=0;i<nm;i++) chess.highlights[mvs[i][0]][mvs[i][1]]=true;
                    } else {
                        chess.selR=chess.selC=-1;
                        memset(chess.highlights,0,sizeof(chess.highlights));
                    }
                }
            }
        }
        // ======== AI TURN ========
        else if(chess.result==0 && !chess.whiteTurn && !chess.promoting){
            ChessBotMove(chess);
            ChessCheckEnd(chess);
        }

        // ======== GAME END MESSAGES ========
        if(chess.result>=1 && chess.result<=5){
            DrawRectangle(0,SCREEN_H/2-35,SCREEN_W,70,(Color){0,0,0,190});
            const char* msg=""; Color mc=GREEN;
            if(chess.result==1){msg="CHECKMATE! You Win!";mc=(Color){0,255,120,255};}
            if(chess.result==2){msg="Puzzle Complete!";mc=(Color){0,255,120,255};}
            if(chess.result==3){msg="You Lost! Click RESET.";mc=(Color){255,80,80,255};}
            if(chess.result==4){msg="STALEMATE - Draw!";mc=(Color){255,210,0,255};}
            if(chess.result==5){msg="DRAW!";mc=(Color){255,210,0,255};}
            int mw=MeasureText(msg,28);
            DrawText(msg,(SCREEN_W-mw)/2,SCREEN_H/2-14,28,mc);
        }
        if((chess.result==1||chess.result==2) && stateTimer>4.0f) StartSlide(STATE_M3_LIFT_CORR,1);
    } break;
    case STATE_M3_LIFT_CORR: {
        // Zoom animation (Ken Burns effect)
        float zoom = 1.0f + (stateTimer / 3.0f) * 0.15f; // Zoom in 15% over 3s
        float destW = SCREEN_W * zoom;
        float destH = SCREEN_H * zoom;
        DrawTexturePro(scenes[50],
            (Rectangle){0, 0, (float)scenes[50].width, (float)scenes[50].height},
            (Rectangle){SCREEN_W/2.0f - destW/2.0f, SCREEN_H/2.0f - destH/2.0f, destW, destH},
            (Vector2){0, 0}, 0.0f, WHITE);

        // Fade out to black in the last 0.5 seconds
        if(stateTimer > 2.5f){
            float alpha = (stateTimer - 2.5f) / 0.5f;
            if(alpha > 1.0f) alpha = 1.0f;
            DrawRectangle(0, 0, SCREEN_W, SCREEN_H, (Color){0, 0, 0, (unsigned char)(255 * alpha)});
        }

        if(stateTimer > 3.0f) ChangeState(STATE_M3_LIFT_IN);
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
        DrawScene(53, 1.0f, true);
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
        DrawScene(55, 1.0f, true); // Cinematic zoom
        // AI machine selectable
        if(DrawSelectable(450,150,380,400)) StartFade(STATE_M4_BREAK);
    } break;
    case STATE_M4_BREAK: {
        DrawScene(56, 1.0f, true); // Cinematic zoom
        // Play glass breaking sound once
        if(stateTimer<0.1f) {
            StopSound(bgMusic);
            PlaySound(glassSound);
        }
        // Auto advance
        if(stateTimer>4.0f) StartFade(STATE_M4_EXPLODE);
    } break;
    case STATE_M4_EXPLODE: {
        DrawScene(57, 1.0f, true); // Cinematic zoom
        // Explosion effect - screen shake
        if(stateTimer<2.0f) {
            float shake=sinf(stateTimer*50)*5*(2.0f-stateTimer);
            DrawScene(57, 1.0f, true);
        }
        if(stateTimer>4.0f) StartFade(STATE_M4_ENDING);
    } break;
    case STATE_M4_ENDING: {
        DrawScene(58, 1.0f, true); // Cinematic zoom
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
