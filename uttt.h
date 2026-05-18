#ifndef UTTT_H
#define UTTT_H

// ============================================================
// ULTIMATE TIC-TAC-TOE - Module 2
// Modified Rules: Both players locked to same board until
// won/drawn. Drawn boards reset. Winner picks next board.
// ============================================================

struct UTTTGame {
    int cells[9][9];    // cells[board][cell]: 0=empty,1=X(player),2=O(bot)
    int won[9];         // won[board]: 0=open,1=X won,2=O won
    int activeBoard;    // -1=pick any board, 0-8=locked to this board
    bool playerTurn;    // true=player(X), false=bot(O)
    int result;         // 0=playing,1=X wins,2=O wins,3=draw
    bool pickingBoard;  // true=player/bot must pick next board after winning
    int picker;         // who picks: 1=player, 2=bot
    char hostCode[6];
    int hostLen;
    bool hostMode;
    int score[2];       // score[0]=player wins, score[1]=bot wins (across games)
};

// Check if 3 values form a winning line
static bool Line3(int a,int b,int c,int p){return a==p&&b==p&&c==p;}

// Check winner of a small board (returns 0,1,2)
static int CheckSmall(int b[9]){
    for(int p=1;p<=2;p++){
        if(Line3(b[0],b[1],b[2],p)) return p;
        if(Line3(b[3],b[4],b[5],p)) return p;
        if(Line3(b[6],b[7],b[8],p)) return p;
        if(Line3(b[0],b[3],b[6],p)) return p;
        if(Line3(b[1],b[4],b[7],p)) return p;
        if(Line3(b[2],b[5],b[8],p)) return p;
        if(Line3(b[0],b[4],b[8],p)) return p;
        if(Line3(b[2],b[4],b[6],p)) return p;
    }
    return 0;
}

// Check if a small board is full
static bool BoardFull(int b[9]){
    for(int i=0;i<9;i++) if(b[i]==0) return false;
    return true;
}

// Check overall game winner from board wins (need 3 in a row)
static int CheckBig(int w[9]){
    for(int p=1;p<=2;p++){
        if(Line3(w[0],w[1],w[2],p)) return p;
        if(Line3(w[3],w[4],w[5],p)) return p;
        if(Line3(w[6],w[7],w[8],p)) return p;
        if(Line3(w[0],w[3],w[6],p)) return p;
        if(Line3(w[1],w[4],w[7],p)) return p;
        if(Line3(w[2],w[5],w[8],p)) return p;
        if(Line3(w[0],w[4],w[8],p)) return p;
        if(Line3(w[2],w[4],w[6],p)) return p;
    }
    return 0;
}

// Check if overall draw (impossible for either to win)
static bool CheckOverallDraw(int w[9]){
    // Check if either player can still get 3 in a row
    int lines[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(int p=1;p<=2;p++){
        for(int i=0;i<8;i++){
            bool possible=true;
            for(int j=0;j<3;j++){
                if(w[lines[i][j]]!=0 && w[lines[i][j]]!=p) possible=false;
            }
            if(possible) return false; // At least one line is still possible
        }
    }
    return true; // No line possible for either player
}

// Count available (unlocked) boards
static int CountAvailable(int w[9]){
    int c=0;
    for(int i=0;i<9;i++) if(w[i]==0) c++;
    return c;
}

// Initialize the UTTT game
void UTTTInit(UTTTGame &g){
    for(int b=0;b<9;b++){
        for(int c=0;c<9;c++) g.cells[b][c]=0;
        g.won[b]=0;
    }
    g.activeBoard=-1;    // Player picks first board freely (Rule 6)
    g.playerTurn=true;
    g.result=0;
    g.pickingBoard=true; // Start by picking a board
    g.picker=1;          // Player picks first
    g.hostLen=0;
    g.hostMode=false;
    memset(g.hostCode,0,6);
    // Don't reset scores across games (Rule 54)
}

// Select a board to play in (for board picking phase)
bool UTTTPickBoard(UTTTGame &g, int board){
    if(!g.pickingBoard) return false;
    if(board<0||board>8) return false;
    if(g.won[board]!=0) return false; // Can't pick locked board (Rule 37)
    g.activeBoard=board;
    g.pickingBoard=false;
    return true;
}

// Make a move within the active board
bool UTTTMove(UTTTGame &g, int board, int cell, int player){
    if(g.result!=0) return false;
    if(g.pickingBoard) return false;
    if(board!=g.activeBoard) return false;
    if(g.won[board]!=0) return false;
    if(cell<0||cell>8) return false;
    if(g.cells[board][cell]!=0) return false;

    g.cells[board][cell]=player;

    // Check if this small board is now won (Rules 15-17)
    int w=CheckSmall(g.cells[board]);
    if(w){
        g.won[board]=w;
        // Check overall game win (Rule 26-28)
        g.result=CheckBig(g.won);
        if(g.result!=0) return true;
        // Check overall draw (Rules 30-32)
        if(CheckOverallDraw(g.won)){g.result=3;return true;}
        // Winner picks next board freely (Rule 18)
        g.pickingBoard=true;
        g.picker=w;
        g.playerTurn=(w==1); // Winner gets to pick
        return true;
    }

    // Check if board is full = draw (Rules 20-25)
    if(BoardFull(g.cells[board])){
        // Reset the drawn board to empty (Rule 22)
        for(int i=0;i<9;i++) g.cells[board][i]=0;
        // Opponent of player who caused draw plays there next (Rule 23)
        g.playerTurn=(player==2); // If player caused draw, bot plays next; vice versa
        // Stay in the same reset board
        g.activeBoard=board;
        return true;
    }

    // Switch turns, stay in same board (Rules 8-10)
    g.playerTurn=!g.playerTurn;
    return true;
}

// ===== STRATEGIC BOT (Rules 34-38) =====

// Bot picks a cell in the active board (Rule 35 priority)
static int BotPickCell(UTTTGame &g, int board){
    int *b=g.cells[board];
    // Priority 1: Win immediately
    for(int c=0;c<9;c++){
        if(b[c]!=0) continue;
        int tmp[9]; for(int i=0;i<9;i++) tmp[i]=b[i];
        tmp[c]=2;
        if(CheckSmall(tmp)==2) return c;
    }
    // Priority 2: Block player win
    for(int c=0;c<9;c++){
        if(b[c]!=0) continue;
        int tmp[9]; for(int i=0;i<9;i++) tmp[i]=b[i];
        tmp[c]=1;
        if(CheckSmall(tmp)==1) return c;
    }
    // Priority 3: Center
    if(b[4]==0) return 4;
    // Priority 4: Corners
    int corners[]={0,2,6,8};
    for(int i=0;i<4;i++) if(b[corners[i]]==0) return corners[i];
    // Priority 5: Any empty
    for(int c=0;c<9;c++) if(b[c]==0) return c;
    return -1;
}

// Bot picks a board to play in (Rule 36 priority)
static int BotPickBoard(UTTTGame &g){
    int *w=g.won;
    int lines[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    // Priority 1: Pick board that completes 3 in a row for bot
    for(int i=0;i<8;i++){
        int botCount=0, emptyIdx=-1, emptyCount=0;
        for(int j=0;j<3;j++){
            if(w[lines[i][j]]==2) botCount++;
            else if(w[lines[i][j]]==0){emptyIdx=lines[i][j];emptyCount++;}
        }
        if(botCount==2&&emptyCount==1&&emptyIdx>=0) return emptyIdx;
    }
    // Priority 2: Block player from getting 3 in a row
    for(int i=0;i<8;i++){
        int plCount=0, emptyIdx=-1, emptyCount=0;
        for(int j=0;j<3;j++){
            if(w[lines[i][j]]==1) plCount++;
            else if(w[lines[i][j]]==0){emptyIdx=lines[i][j];emptyCount++;}
        }
        if(plCount==2&&emptyCount==1&&emptyIdx>=0) return emptyIdx;
    }
    // Priority 3: Board where bot has 1 win in a line direction
    for(int i=0;i<8;i++){
        int botCount=0;
        for(int j=0;j<3;j++) if(w[lines[i][j]]==2) botCount++;
        if(botCount==1){
            for(int j=0;j<3;j++) if(w[lines[i][j]]==0) return lines[i][j];
        }
    }
    // Priority 4: Center board
    if(w[4]==0) return 4;
    // Priority 5: Corner boards
    int corners[]={0,2,6,8};
    for(int i=0;i<4;i++) if(w[corners[i]]==0) return corners[i];
    // Priority 6: Any available
    for(int i=0;i<9;i++) if(w[i]==0) return i;
    return -1;
}

// Full bot turn: either pick board or make move
void UTTTBotMove(UTTTGame &g){
    if(g.playerTurn || g.result!=0) return;

    if(g.pickingBoard && g.picker==2){
        int board=BotPickBoard(g);
        if(board>=0) UTTTPickBoard(g,board);
        return;
    }

    if(!g.pickingBoard && g.activeBoard>=0){
        int cell=BotPickCell(g,g.activeBoard);
        if(cell>=0) UTTTMove(g,g.activeBoard,cell,2);
    }
}

#endif
