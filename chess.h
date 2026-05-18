#ifndef CHESS_H
#define CHESS_H
#include <cstdlib>
#include <cstring>

// Chess piece constants
enum CP {
    CE=0, WP=1,WR=2,WN=3,WB=4,WQ=5,WK=6,
    BP=7,BR=8,BN=9,BB=10,BQ=11,BK=12
};

struct ChessGame {
    int board[8][8]; // board[row][col], row 0=rank 1 (bottom)
    int selR,selC;   // selected piece (-1 if none)
    int result;      // 0=playing,1=checkmate win,2=skip
    char host[6]; int hostLen; bool hostMode;
    bool highlights[8][8]; // valid move highlights
};

static bool IsWhite(int p){return p>=WP&&p<=WK;}
static bool IsBlack(int p){return p>=BP&&p<=BK;}

// Set up the puzzle position (partial game)
void ChessInit(ChessGame &g) {
    memset(g.board,0,sizeof(g.board));
    // White pieces (bottom)
    g.board[0][0]=WR; g.board[0][4]=WK; g.board[0][7]=WR;
    g.board[0][3]=WQ; g.board[0][2]=WB; g.board[0][5]=WB;
    g.board[1][0]=WP;g.board[1][1]=WP;g.board[1][2]=WP;
    g.board[1][5]=WP;g.board[1][6]=WP;g.board[1][7]=WP;
    g.board[2][5]=WN; g.board[3][3]=WP; g.board[3][4]=WP;
    // Black pieces (top)
    g.board[7][0]=BR; g.board[7][4]=BK; g.board[7][7]=BR;
    g.board[7][3]=BQ; g.board[7][2]=BB; g.board[7][5]=BB;
    g.board[6][0]=BP;g.board[6][1]=BP;g.board[6][5]=BP;g.board[6][6]=BP;g.board[6][7]=BP;
    g.board[5][2]=BN; g.board[5][3]=BP; g.board[4][4]=BN;
    g.selR=g.selC=-1; g.result=0;
    g.hostLen=0; g.hostMode=false;
    memset(g.host,0,6);
    memset(g.highlights,0,sizeof(g.highlights));
}

// Find king position
static void FindKing(int board[8][8],int piece,int&kr,int&kc){
    for(int r=0;r<8;r++)for(int c=0;c<8;c++)
        if(board[r][c]==piece){kr=r;kc=c;return;}
    kr=kc=-1;
}

// Check if a square is attacked by the given side
static bool IsAttacked(int board[8][8],int r,int c,bool byWhite){
    // Pawn attacks
    if(byWhite){
        if(r>0&&c>0&&board[r-1][c-1]==WP) return true;
        if(r>0&&c<7&&board[r-1][c+1]==WP) return true;
    } else {
        if(r<7&&c>0&&board[r+1][c-1]==BP) return true;
        if(r<7&&c<7&&board[r+1][c+1]==BP) return true;
    }
    // Knight attacks
    int kp=byWhite?WN:BN;
    int km[8][2]={{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for(int i=0;i<8;i++){
        int nr=r+km[i][0],nc=c+km[i][1];
        if(nr>=0&&nr<8&&nc>=0&&nc<8&&board[nr][nc]==kp) return true;
    }
    // King attacks
    int kk=byWhite?WK:BK;
    for(int dr=-1;dr<=1;dr++)for(int dc=-1;dc<=1;dc++){
        if(dr==0&&dc==0) continue;
        int nr=r+dr,nc=c+dc;
        if(nr>=0&&nr<8&&nc>=0&&nc<8&&board[nr][nc]==kk) return true;
    }
    // Rook/Queen (straight lines)
    int rq1=byWhite?WR:BR, rq2=byWhite?WQ:BQ;
    int dirs[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
    for(int d=0;d<4;d++){
        for(int s=1;s<8;s++){
            int nr=r+dirs[d][0]*s,nc=c+dirs[d][1]*s;
            if(nr<0||nr>7||nc<0||nc>7) break;
            if(board[nr][nc]!=CE){
                if(board[nr][nc]==rq1||board[nr][nc]==rq2) return true;
                break;
            }
        }
    }
    // Bishop/Queen (diagonals)
    int bq1=byWhite?WB:BB, bq2=byWhite?WQ:BQ;
    int dgs[4][2]={{1,1},{1,-1},{-1,1},{-1,-1}};
    for(int d=0;d<4;d++){
        for(int s=1;s<8;s++){
            int nr=r+dgs[d][0]*s,nc=c+dgs[d][1]*s;
            if(nr<0||nr>7||nc<0||nc>7) break;
            if(board[nr][nc]!=CE){
                if(board[nr][nc]==bq1||board[nr][nc]==bq2) return true;
                break;
            }
        }
    }
    return false;
}

// Check if the given side's king is in check
static bool InCheck(int board[8][8],bool whiteKing){
    int kr,kc; FindKing(board,whiteKing?WK:BK,kr,kc);
    if(kr<0) return true;
    return IsAttacked(board,kr,kc,!whiteKing);
}

// Try a move and see if it leaves own king in check
static bool IsLegalMove(int board[8][8],int fr,int fc,int tr,int tc,bool white){
    int tmp[8][8]; memcpy(tmp,board,sizeof(int)*64);
    tmp[tr][tc]=tmp[fr][fc]; tmp[fr][fc]=CE;
    return !InCheck(tmp,white);
}

// Get valid moves for piece at (r,c)
static int GetMoves(int board[8][8],int r,int c,int moves[][2],bool white){
    int n=0,p=board[r][c];
    if(p==CE) return 0;
    auto Add=[&](int tr,int tc){
        if(tr<0||tr>7||tc<0||tc>7) return;
        int dst=board[tr][tc];
        if(white&&IsWhite(dst)) return;
        if(!white&&IsBlack(dst)) return;
        if(IsLegalMove(board,r,c,tr,tc,white))
            {moves[n][0]=tr;moves[n][1]=tc;n++;}
    };
    if(p==WP){
        if(r<7&&board[r+1][c]==CE){Add(r+1,c);if(r==1&&board[r+2][c]==CE)Add(r+2,c);}
        if(r<7&&c>0&&IsBlack(board[r+1][c-1]))Add(r+1,c-1);
        if(r<7&&c<7&&IsBlack(board[r+1][c+1]))Add(r+1,c+1);
    } else if(p==BP){
        if(r>0&&board[r-1][c]==CE){Add(r-1,c);if(r==6&&board[r-2][c]==CE)Add(r-2,c);}
        if(r>0&&c>0&&IsWhite(board[r-1][c-1]))Add(r-1,c-1);
        if(r>0&&c<7&&IsWhite(board[r-1][c+1]))Add(r-1,c+1);
    } else if(p==WN||p==BN){
        int km[8][2]={{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
        for(int i=0;i<8;i++) Add(r+km[i][0],c+km[i][1]);
    } else if(p==WK||p==BK){
        for(int dr=-1;dr<=1;dr++)for(int dc=-1;dc<=1;dc++)
            if(dr||dc) Add(r+dr,c+dc);
    } else {
        bool diag=(p==WB||p==BB||p==WQ||p==BQ);
        bool straight=(p==WR||p==BR||p==WQ||p==BQ);
        if(straight){
            int d[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
            for(int i=0;i<4;i++)for(int s=1;s<8;s++){
                int nr=r+d[i][0]*s,nc=c+d[i][1]*s;
                if(nr<0||nr>7||nc<0||nc>7) break;
                Add(nr,nc);
                if(board[nr][nc]!=CE) break;
            }
        }
        if(diag){
            int d[4][2]={{1,1},{1,-1},{-1,1},{-1,-1}};
            for(int i=0;i<4;i++)for(int s=1;s<8;s++){
                int nr=r+d[i][0]*s,nc=c+d[i][1]*s;
                if(nr<0||nr>7||nc<0||nc>7) break;
                Add(nr,nc);
                if(board[nr][nc]!=CE) break;
            }
        }
    }
    return n;
}

// Check if side has any legal moves
static bool HasLegalMoves(int board[8][8],bool white){
    for(int r=0;r<8;r++)for(int c=0;c<8;c++){
        if(white&&!IsWhite(board[r][c])) continue;
        if(!white&&!IsBlack(board[r][c])) continue;
        int m[64][2];
        if(GetMoves(board,r,c,m,white)>0) return true;
    }
    return false;
}

// Check for checkmate
bool IsCheckmate(int board[8][8],bool whiteKing){
    if(!InCheck(board,whiteKing)) return false;
    return !HasLegalMoves(board,whiteKing);
}

// Simple bot move for black
void ChessBotMove(ChessGame &g){
    // Collect all legal moves for black
    struct Mv{int fr,fc,tr,tc;};
    Mv mvs[256]; int n=0;
    for(int r=0;r<8;r++)for(int c=0;c<8;c++){
        if(!IsBlack(g.board[r][c])) continue;
        int m[64][2]; int cnt=GetMoves(g.board,r,c,m,false);
        for(int i=0;i<cnt;i++) mvs[n++]={r,c,m[i][0],m[i][1]};
    }
    if(n==0) return;
    Mv mv=mvs[rand()%n];
    g.board[mv.tr][mv.tc]=g.board[mv.fr][mv.fc];
    g.board[mv.fr][mv.fc]=CE;
}

#endif
