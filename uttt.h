#ifndef UTTT_H
#define UTTT_H

// Ultimate Tic-Tac-Toe game logic
// Rules: 9 small 3x3 boards in a 3x3 grid
// Your move position determines which board opponent plays next
// Win 3 boards in a row to win the game

struct UTTTGame {
    int cells[9][9];    // cells[board][cell]: 0=empty,1=X(player),2=O(bot)
    int won[9];         // won[board]: 0=open,1=X won,2=O won,3=draw
    int activeBoard;    // -1 means any board is valid
    bool playerTurn;
    int result;         // 0=playing,1=X wins,2=O wins,3=draw
    char hostCode[6];
    int hostLen;
    bool hostMode;
};

// Check if 3 values form a winning line
static bool Line3(int a,int b,int c,int p){return a==p&&b==p&&c==p;}

// Check winner of a small board (returns 0,1,2)
static int CheckSmall(int b[9]) {
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

// Check if board is full
static bool BoardFull(int b[9]) {
    for(int i=0;i<9;i++) if(b[i]==0) return false;
    return true;
}

// Check overall game winner from board wins
static int CheckBig(int w[9]) {
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
    bool allDone=true;
    for(int i=0;i<9;i++) if(w[i]==0) allDone=false;
    if(allDone) return 3;
    return 0;
}

// Initialize the UTTT game
void UTTTInit(UTTTGame &g) {
    for(int b=0;b<9;b++){
        for(int c=0;c<9;c++) g.cells[b][c]=0;
        g.won[b]=0;
    }
    g.activeBoard=-1;
    g.playerTurn=true;
    g.result=0;
    g.hostLen=0;
    g.hostMode=false;
    memset(g.hostCode,0,6);
}

// Make a move. Returns true if valid.
bool UTTTMove(UTTTGame &g, int board, int cell, int player) {
    if(g.result!=0) return false;
    if(g.won[board]!=0) return false;
    if(g.cells[board][cell]!=0) return false;
    if(g.activeBoard!=-1 && g.activeBoard!=board) return false;

    g.cells[board][cell]=player;

    // Check if this small board is now won
    int w=CheckSmall(g.cells[board]);
    if(w) g.won[board]=w;
    else if(BoardFull(g.cells[board])) g.won[board]=3;

    // Check overall game
    g.result=CheckBig(g.won);

    // Determine next active board
    int nextBoard=cell; // The cell position determines next board
    if(g.won[nextBoard]!=0) g.activeBoard=-1; // If that board is done, free choice
    else g.activeBoard=nextBoard;

    g.playerTurn=!g.playerTurn;
    return true;
}

// Simple AI: pick best available move for bot (player 2)
void UTTTBotMove(UTTTGame &g) {
    if(!g.playerTurn==false || g.result!=0) return;
    // Collect all valid moves
    struct Move { int b,c; int score; };
    Move moves[81]; int n=0;
    for(int b=0;b<9;b++){
        if(g.activeBoard!=-1 && g.activeBoard!=b) continue;
        if(g.won[b]!=0) continue;
        for(int c=0;c<9;c++){
            if(g.cells[b][c]!=0) continue;
            int sc=0;
            // Prefer center cells
            if(c==4) sc+=3;
            // Prefer corners
            if(c==0||c==2||c==6||c==8) sc+=2;
            // Check if this move wins the small board
            int tmp[9]; for(int i=0;i<9;i++) tmp[i]=g.cells[b][i];
            tmp[c]=2;
            if(CheckSmall(tmp)==2) sc+=50;
            // Check if it blocks player win
            tmp[c]=1;
            if(CheckSmall(tmp)==1) sc+=30;
            // Avoid sending to a board player can win easily
            if(g.won[c]==0){
                int cnt=0;
                for(int i=0;i<9;i++) if(g.cells[c][i]==1) cnt++;
                sc-=cnt;
            }
            moves[n++]={b,c,sc};
        }
    }
    if(n==0) return;
    // Find best score
    int best=moves[0].score;
    for(int i=1;i<n;i++) if(moves[i].score>best) best=moves[i].score;
    // Collect all moves with best score
    Move top[81]; int t=0;
    for(int i=0;i<n;i++) if(moves[i].score==best) top[t++]=moves[i];
    // Pick random among best
    Move m=top[rand()%t];
    UTTTMove(g,m.b,m.c,2);
}

#endif
