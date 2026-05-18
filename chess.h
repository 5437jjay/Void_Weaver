#ifndef CHESS_H
#define CHESS_H
#include <cstdlib>
#include <cstring>
#include <climits>

// ============================================================
// CHESS PUZZLE ENGINE - Module 3
// Complete chess logic with all rules, special moves, and AI
// ============================================================

// Chess piece constants
enum CP {
    CE=0, WP=1,WR=2,WN=3,WB=4,WQ=5,WK=6,
    BP=7,BR=8,BN=9,BB=10,BQ=11,BK=12
};

// Piece values for AI evaluation
static const int PIECE_VAL[] = {0, 100,500,320,330,900,20000, 100,500,320,330,900,20000};

struct ChessGame {
    int board[8][8];       // board[row][col], row 0=rank 1 (bottom)
    int selR,selC;         // selected piece (-1 if none)
    int result;            // 0=playing,1=white wins,2=host skip,3=black wins,4=stalemate,5=draw
    char host[6]; int hostLen; bool hostMode;
    bool highlights[8][8]; // valid move highlights
    bool whiteTurn;        // true = white's turn (human player)
    int epCol;             // en passant target column (-1 if none)
    bool wKMoved, wRA_Moved, wRH_Moved; // castling tracking
    bool bKMoved, bRA_Moved, bRH_Moved;
    int halfMoveClock;     // fifty-move rule
    int moveCount;
    bool promoting;        // waiting for promotion choice
    int promoR, promoC;
    int aiDiff;            // 0=Easy, 1=Medium, 2=Hard
    int lastFR,lastFC,lastTR,lastTC; // last move highlight
};

static bool IsWhite(int p){return p>=WP&&p<=WK;}
static bool IsBlack(int p){return p>=BP&&p<=BK;}

// ===== PUZZLE STARTING POSITION (Rule 11) =====
void ChessInit(ChessGame &g) {
    memset(g.board,0,sizeof(g.board));
    // White pieces
    g.board[0][0]=WR; g.board[0][6]=WK; g.board[0][7]=WR;
    g.board[1][1]=WP; g.board[1][3]=WQ; g.board[1][5]=WP;
    g.board[1][6]=WP; g.board[1][7]=WP;
    g.board[2][0]=WP; g.board[2][2]=WP; g.board[3][4]=WP;
    // Black pieces
    g.board[6][0]=BP; g.board[6][1]=BP; g.board[6][5]=BP;
    g.board[6][6]=BP; g.board[6][7]=BP;
    g.board[5][0]=BQ; g.board[4][3]=BP; g.board[5][5]=BR; g.board[5][6]=BK;

    g.selR=g.selC=-1; g.result=0;
    g.hostLen=0; g.hostMode=false;
    memset(g.host,0,6);
    memset(g.highlights,0,sizeof(g.highlights));
    g.whiteTurn=true; g.epCol=-1;
    g.wKMoved=true; g.wRA_Moved=false; g.wRH_Moved=false; // King moved from E1->G1
    g.bKMoved=true; g.bRA_Moved=true; g.bRH_Moved=true;   // King on G6, all moved
    g.halfMoveClock=0; g.moveCount=1;
    g.promoting=false; g.promoR=g.promoC=-1;
    g.aiDiff=1; // Medium default
    g.lastFR=g.lastFC=g.lastTR=g.lastTC=-1;
}

// ===== CORE LOGIC =====
static void FindKing(int b[8][8],int piece,int&kr,int&kc){
    for(int r=0;r<8;r++)for(int c=0;c<8;c++)
        if(b[r][c]==piece){kr=r;kc=c;return;}
    kr=kc=-1;
}

static bool IsAttacked(int b[8][8],int r,int c,bool byWhite){
    // Pawn attacks
    if(byWhite){
        if(r>0&&c>0&&b[r-1][c-1]==WP) return true;
        if(r>0&&c<7&&b[r-1][c+1]==WP) return true;
    } else {
        if(r<7&&c>0&&b[r+1][c-1]==BP) return true;
        if(r<7&&c<7&&b[r+1][c+1]==BP) return true;
    }
    // Knight
    int kp=byWhite?WN:BN;
    int km[8][2]={{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for(int i=0;i<8;i++){
        int nr=r+km[i][0],nc=c+km[i][1];
        if(nr>=0&&nr<8&&nc>=0&&nc<8&&b[nr][nc]==kp) return true;
    }
    // King
    int kk=byWhite?WK:BK;
    for(int dr=-1;dr<=1;dr++)for(int dc=-1;dc<=1;dc++){
        if(!dr&&!dc) continue;
        int nr=r+dr,nc=c+dc;
        if(nr>=0&&nr<8&&nc>=0&&nc<8&&b[nr][nc]==kk) return true;
    }
    // Rook/Queen lines
    int rq1=byWhite?WR:BR, rq2=byWhite?WQ:BQ;
    int dirs[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
    for(int d=0;d<4;d++) for(int s=1;s<8;s++){
        int nr=r+dirs[d][0]*s,nc=c+dirs[d][1]*s;
        if(nr<0||nr>7||nc<0||nc>7) break;
        if(b[nr][nc]!=CE){
            if(b[nr][nc]==rq1||b[nr][nc]==rq2) return true;
            break;
        }
    }
    // Bishop/Queen diags
    int bq1=byWhite?WB:BB, bq2=byWhite?WQ:BQ;
    int dgs[4][2]={{1,1},{1,-1},{-1,1},{-1,-1}};
    for(int d=0;d<4;d++) for(int s=1;s<8;s++){
        int nr=r+dgs[d][0]*s,nc=c+dgs[d][1]*s;
        if(nr<0||nr>7||nc<0||nc>7) break;
        if(b[nr][nc]!=CE){
            if(b[nr][nc]==bq1||b[nr][nc]==bq2) return true;
            break;
        }
    }
    return false;
}

static bool InCheck(int b[8][8],bool white){
    int kr,kc; FindKing(b,white?WK:BK,kr,kc);
    if(kr<0) return true;
    return IsAttacked(b,kr,kc,!white);
}

static bool IsLegalMove(int b[8][8],int fr,int fc,int tr,int tc,bool white){
    int tmp[8][8]; memcpy(tmp,b,sizeof(tmp));
    int p=b[fr][fc];
    tmp[tr][tc]=p; tmp[fr][fc]=CE;
    // En passant capture removes the adjacent pawn
    if((p==WP||p==BP)&&fc!=tc&&b[tr][tc]==CE){
        if(p==WP) tmp[tr-1][tc]=CE;
        else tmp[tr+1][tc]=CE;
    }
    // For King moves, explicitly verify destination is not attacked
    if(p==WK||p==BK){
        return !IsAttacked(tmp,tr,tc,!white);
    }
    return !InCheck(tmp,white);
}

// ===== MOVE GENERATION =====
static int GetMoves(int b[8][8],int r,int c,int mv[][2],bool white,int epCol=-1,
                    bool wKM=true,bool wRA=true,bool wRH=true,
                    bool bKM=true,bool bRA=true,bool bRH=true){
    int n=0,p=b[r][c];
    if(p==CE) return 0;
    auto Add=[&](int tr,int tc){
        if(tr<0||tr>7||tc<0||tc>7) return;
        if(white&&IsWhite(b[tr][tc])) return;
        if(!white&&IsBlack(b[tr][tc])) return;
        if(IsLegalMove(b,r,c,tr,tc,white)){mv[n][0]=tr;mv[n][1]=tc;n++;}
    };
    if(p==WP){
        if(r<7&&b[r+1][c]==CE){Add(r+1,c);if(r==1&&b[r+2][c]==CE)Add(r+2,c);}
        if(r<7&&c>0&&IsBlack(b[r+1][c-1]))Add(r+1,c-1);
        if(r<7&&c<7&&IsBlack(b[r+1][c+1]))Add(r+1,c+1);
        if(r==4&&epCol>=0){
            if(c>0&&c-1==epCol&&b[r][c-1]==BP) Add(r+1,c-1);
            if(c<7&&c+1==epCol&&b[r][c+1]==BP) Add(r+1,c+1);
        }
    } else if(p==BP){
        if(r>0&&b[r-1][c]==CE){Add(r-1,c);if(r==6&&b[r-2][c]==CE)Add(r-2,c);}
        if(r>0&&c>0&&IsWhite(b[r-1][c-1]))Add(r-1,c-1);
        if(r>0&&c<7&&IsWhite(b[r-1][c+1]))Add(r-1,c+1);
        if(r==3&&epCol>=0){
            if(c>0&&c-1==epCol&&b[r][c-1]==WP) Add(r-1,c-1);
            if(c<7&&c+1==epCol&&b[r][c+1]==WP) Add(r-1,c+1);
        }
    } else if(p==WN||p==BN){
        int km[8][2]={{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
        for(int i=0;i<8;i++) Add(r+km[i][0],c+km[i][1]);
    } else if(p==WK||p==BK){
        // King moves one square in any direction
        for(int dr=-1;dr<=1;dr++)for(int dc=-1;dc<=1;dc++){
            if(dr==0&&dc==0) continue;
            int tr=r+dr, tc=c+dc;
            if(tr<0||tr>7||tc<0||tc>7) continue;
            // Can't move to square occupied by friendly piece
            if(white&&IsWhite(b[tr][tc])) continue;
            if(!white&&IsBlack(b[tr][tc])) continue;
            // Check if destination is safe after removing king from source
            int tmp[8][8]; memcpy(tmp,b,sizeof(tmp));
            tmp[tr][tc]=p; tmp[r][c]=CE;
            if(!IsAttacked(tmp,tr,tc,!white)){
                mv[n][0]=tr; mv[n][1]=tc; n++;
            }
        }
        // Castling
        if(p==WK&&r==0&&c==4&&!wKM&&!InCheck(b,true)){
            if(!wRH&&b[0][5]==CE&&b[0][6]==CE&&!IsAttacked(b,0,5,false)&&!IsAttacked(b,0,6,false))
                {mv[n][0]=0;mv[n][1]=6;n++;}
            if(!wRA&&b[0][1]==CE&&b[0][2]==CE&&b[0][3]==CE&&!IsAttacked(b,0,3,false)&&!IsAttacked(b,0,2,false))
                {mv[n][0]=0;mv[n][1]=2;n++;}
        }
        if(p==BK&&r==7&&c==4&&!bKM&&!InCheck(b,false)){
            if(!bRH&&b[7][5]==CE&&b[7][6]==CE&&!IsAttacked(b,7,5,true)&&!IsAttacked(b,7,6,true))
                {mv[n][0]=7;mv[n][1]=6;n++;}
            if(!bRA&&b[7][1]==CE&&b[7][2]==CE&&b[7][3]==CE&&!IsAttacked(b,7,3,true)&&!IsAttacked(b,7,2,true))
                {mv[n][0]=7;mv[n][1]=2;n++;}
        }
    } else {
        // Queen, Rook, Bishop - sliding pieces
        bool diag=(p==WB||p==BB||p==WQ||p==BQ);
        bool straight=(p==WR||p==BR||p==WQ||p==BQ);
        if(straight){
            int d[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
            for(int i=0;i<4;i++){
                for(int s=1;s<8;s++){
                    int nr=r+d[i][0]*s, nc=c+d[i][1]*s;
                    if(nr<0||nr>7||nc<0||nc>7) break;
                    int dest=b[nr][nc];
                    if(white&&IsWhite(dest)) break;  // blocked by own piece
                    if(!white&&IsBlack(dest)) break;
                    Add(nr,nc);
                    if(dest!=CE) break;  // captured enemy, can't go further
                }
            }
        }
        if(diag){
            int d[4][2]={{1,1},{1,-1},{-1,1},{-1,-1}};
            for(int i=0;i<4;i++){
                for(int s=1;s<8;s++){
                    int nr=r+d[i][0]*s, nc=c+d[i][1]*s;
                    if(nr<0||nr>7||nc<0||nc>7) break;
                    int dest=b[nr][nc];
                    if(white&&IsWhite(dest)) break;
                    if(!white&&IsBlack(dest)) break;
                    Add(nr,nc);
                    if(dest!=CE) break;
                }
            }
        }
    }
    return n;
}

static bool HasLegalMoves(int b[8][8],bool w,int ep=-1){
    for(int r=0;r<8;r++)for(int c=0;c<8;c++){
        if(w&&!IsWhite(b[r][c])) continue;
        if(!w&&!IsBlack(b[r][c])) continue;
        int m[64][2];
        if(GetMoves(b,r,c,m,w,ep)>0) return true;
    }
    return false;
}

static bool IsCheckmate(int b[8][8],bool w,int ep=-1){
    return InCheck(b,w) && !HasLegalMoves(b,w,ep);
}
static bool IsStalemate(int b[8][8],bool w,int ep=-1){
    return !InCheck(b,w) && !HasLegalMoves(b,w,ep);
}

static bool IsInsufficientMaterial(int b[8][8]){
    int wN=0,wB=0,bN=0,bB=0;
    for(int r=0;r<8;r++)for(int c=0;c<8;c++){
        int p=b[r][c];
        if(p==WP||p==WR||p==WQ||p==BP||p==BR||p==BQ) return false;
        if(p==WN) wN++; if(p==WB) wB++;
        if(p==BN) bN++; if(p==BB) bB++;
    }
    int wM=wN+wB, bM=bN+bB;
    if(wM==0&&bM==0) return true;      // K vs K
    if(wM==1&&bM==0) return true;      // K+minor vs K
    if(wM==0&&bM==1) return true;
    return false;
}

// ===== EXECUTE MOVE =====
void ChessDoMove(ChessGame &g,int fr,int fc,int tr,int tc){
    int p=g.board[fr][fc];
    bool isPawn=(p==WP||p==BP);
    bool isCapture=(g.board[tr][tc]!=CE);

    // En passant capture
    if(isPawn&&fc!=tc&&g.board[tr][tc]==CE){
        if(p==WP) g.board[tr-1][tc]=CE;
        else g.board[tr+1][tc]=CE;
        isCapture=true;
    }

    // Fifty-move clock
    g.halfMoveClock = (isPawn||isCapture) ? 0 : g.halfMoveClock+1;

    // En passant target
    g.epCol=-1;
    if(p==WP&&tr-fr==2) g.epCol=fc;
    if(p==BP&&fr-tr==2) g.epCol=fc;

    // Execute move
    g.board[tr][tc]=p; g.board[fr][fc]=CE;

    // Castling rook movement
    if((p==WK||p==BK)&&abs(tc-fc)==2){
        if(tc>fc){g.board[fr][fc+1]=g.board[fr][7];g.board[fr][7]=CE;}
        else {g.board[fr][fc-1]=g.board[fr][0];g.board[fr][0]=CE;}
    }

    // Track moved pieces
    if(p==WK) g.wKMoved=true;
    if(p==BK) g.bKMoved=true;
    if(fr==0&&fc==0) g.wRA_Moved=true;
    if(fr==0&&fc==7) g.wRH_Moved=true;
    if(fr==7&&fc==0) g.bRA_Moved=true;
    if(fr==7&&fc==7) g.bRH_Moved=true;

    g.lastFR=fr;g.lastFC=fc;g.lastTR=tr;g.lastTC=tc;

    // Pawn promotion
    if(p==WP&&tr==7){g.promoting=true;g.promoR=tr;g.promoC=tc;return;}
    if(p==BP&&tr==0){g.board[tr][tc]=BQ;} // AI auto-promotes to queen

    g.whiteTurn=!g.whiteTurn;
    if(g.whiteTurn) g.moveCount++;
}

void ChessPromote(ChessGame &g,int piece){
    g.board[g.promoR][g.promoC]=piece;
    g.promoting=false;
    g.whiteTurn=!g.whiteTurn;
    if(g.whiteTurn) g.moveCount++;
}

// ===== CHECK GAME END CONDITIONS =====
void ChessCheckEnd(ChessGame &g){
    bool side=g.whiteTurn;
    if(IsCheckmate(g.board,side,g.epCol)){
        g.result=side?3:1; // If it's white's turn and checkmate, black wins (3), else white wins (1)
        return;
    }
    if(IsStalemate(g.board,side,g.epCol)){g.result=4;return;}
    if(IsInsufficientMaterial(g.board)){g.result=5;return;}
    if(g.halfMoveClock>=100){g.result=5;return;} // 50 moves = 100 half-moves
}

// ===== AI EVALUATION =====
static int EvalBoard(int b[8][8]){
    int s=0;
    // Piece-square bonus for pawns (encourages advancement)
    static const int PT[8]={0,0,5,10,20,30,50,0};
    for(int r=0;r<8;r++)for(int c=0;c<8;c++){
        int p=b[r][c]; if(p==CE) continue;
        int v=PIECE_VAL[p];
        if(IsWhite(p)){s+=v; if(p==WP) s+=PT[r];}
        else {s-=v; if(p==BP) s+=PT[7-r];}
    }
    return s;
}

// Minimax with alpha-beta (depth-limited)
static int Minimax(int b[8][8],int depth,int alpha,int beta,bool maxPlayer,int ep){
    if(depth==0) return EvalBoard(b);
    if(maxPlayer){
        int best=-999999;
        for(int r=0;r<8;r++)for(int c=0;c<8;c++){
            if(!IsWhite(b[r][c])) continue;
            int mv[64][2]; int n=GetMoves(b,r,c,mv,true,ep);
            for(int i=0;i<n;i++){
                int tmp[8][8]; memcpy(tmp,b,sizeof(int)*64);
                int p=tmp[r][c];
                // Handle en passant in copy
                if(p==WP&&c!=mv[i][1]&&tmp[mv[i][0]][mv[i][1]]==CE)
                    tmp[mv[i][0]-1][mv[i][1]]=CE;
                tmp[mv[i][0]][mv[i][1]]=p; tmp[r][c]=CE;
                if(p==WP&&mv[i][0]==7) tmp[mv[i][0]][mv[i][1]]=WQ;
                int nep=(p==WP&&mv[i][0]-r==2)?c:-1;
                int val=Minimax(tmp,depth-1,alpha,beta,false,nep);
                if(val>best) best=val;
                if(best>alpha) alpha=best;
                if(beta<=alpha) return best;
            }
        }
        return (best==-999999)?EvalBoard(b):best;
    } else {
        int best=999999;
        for(int r=0;r<8;r++)for(int c=0;c<8;c++){
            if(!IsBlack(b[r][c])) continue;
            int mv[64][2]; int n=GetMoves(b,r,c,mv,false,ep);
            for(int i=0;i<n;i++){
                int tmp[8][8]; memcpy(tmp,b,sizeof(int)*64);
                int p=tmp[r][c];
                if(p==BP&&c!=mv[i][1]&&tmp[mv[i][0]][mv[i][1]]==CE)
                    tmp[mv[i][0]+1][mv[i][1]]=CE;
                tmp[mv[i][0]][mv[i][1]]=p; tmp[r][c]=CE;
                if(p==BP&&mv[i][0]==0) tmp[mv[i][0]][mv[i][1]]=BQ;
                int nep=(p==BP&&r-mv[i][0]==2)?c:-1;
                int val=Minimax(tmp,depth-1,alpha,beta,true,nep);
                if(val<best) best=val;
                if(best<beta) beta=best;
                if(beta<=alpha) return best;
            }
        }
        return (best==999999)?EvalBoard(b):best;
    }
}

// ===== AI MOVE =====
void ChessBotMove(ChessGame &g){
    struct Mv{int fr,fc,tr,tc;int score;};
    Mv mvs[512]; int n=0;
    for(int r=0;r<8;r++)for(int c=0;c<8;c++){
        if(!IsBlack(g.board[r][c])) continue;
        int m[64][2];
        int cnt=GetMoves(g.board,r,c,m,false,g.epCol,
                         g.wKMoved,g.wRA_Moved,g.wRH_Moved,
                         g.bKMoved,g.bRA_Moved,g.bRH_Moved);
        for(int i=0;i<cnt;i++) mvs[n++]={r,c,m[i][0],m[i][1],0};
    }
    if(n==0) return;

    if(g.aiDiff==0){
        // EASY: random legal move
        Mv mv=mvs[rand()%n];
        ChessDoMove(g,mv.fr,mv.fc,mv.tr,mv.tc);
    } else if(g.aiDiff==1){
        // MEDIUM: prioritize captures, avoid losing pieces
        int bestIdx=0, bestScore=-999999;
        for(int i=0;i<n;i++){
            int sc=0;
            int cap=g.board[mvs[i].tr][mvs[i].tc];
            if(cap!=CE) sc+=PIECE_VAL[cap]*10; // Capture bonus
            // Check if destination is attacked
            int tmp[8][8]; memcpy(tmp,g.board,sizeof(int)*64);
            tmp[mvs[i].tr][mvs[i].tc]=tmp[mvs[i].fr][mvs[i].fc];
            tmp[mvs[i].fr][mvs[i].fc]=CE;
            if(IsAttacked(tmp,mvs[i].tr,mvs[i].tc,true))
                sc-=PIECE_VAL[g.board[mvs[i].fr][mvs[i].fc]]; // Penalty for moving into attack
            // Bonus for giving check
            if(InCheck(tmp,true)) sc+=200;
            sc+=rand()%50; // Small randomness
            mvs[i].score=sc;
            if(sc>bestScore){bestScore=sc;bestIdx=i;}
        }
        ChessDoMove(g,mvs[bestIdx].fr,mvs[bestIdx].fc,mvs[bestIdx].tr,mvs[bestIdx].tc);
    } else {
        // HARD: minimax depth 3
        int bestIdx=0, bestScore=999999;
        for(int i=0;i<n;i++){
            int tmp[8][8]; memcpy(tmp,g.board,sizeof(int)*64);
            int p=tmp[mvs[i].fr][mvs[i].fc];
            // Handle en passant in copy
            if(p==BP&&mvs[i].fc!=mvs[i].tc&&tmp[mvs[i].tr][mvs[i].tc]==CE)
                tmp[mvs[i].tr+1][mvs[i].tc]=CE;
            tmp[mvs[i].tr][mvs[i].tc]=p;
            tmp[mvs[i].fr][mvs[i].fc]=CE;
            if(p==BP&&mvs[i].tr==0) tmp[mvs[i].tr][mvs[i].tc]=BQ;
            int nep=(p==BP&&mvs[i].fr-mvs[i].tr==2)?mvs[i].fc:-1;
            int sc=Minimax(tmp,2,-999999,999999,true,nep);
            mvs[i].score=sc;
            if(sc<bestScore){bestScore=sc;bestIdx=i;}
        }
        ChessDoMove(g,mvs[bestIdx].fr,mvs[bestIdx].fc,mvs[bestIdx].tr,mvs[bestIdx].tc);
    }
}

#endif
