#include "tablero.h"
#include <iostream>
#include <sstream>
#include <string>


char Board[N][N];

void initBoard(){
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            if ((i + j) % 2 == 0) Board[i][j] = '.';
            else if (i < 3) Board[i][j] = 'b';
            else if (i > 4) Board [i][j] = 'r';
            else Board [i][j] = ' ';
        }
    }
}

bool inside(int x, int y){
    return x>=0 &&x<N && y>=0 && y<N;
}  

void printBoard(){
    std::cout << "\033[37m" <<"\n     0   1   2   3   4   5   6   7\n" << "\033[37m";
    std::cout << "   +---+---+---+---+---+---+---+---+\n";
    for (int r = 0; r<8; r++){
        std::cout << " "<< r <<" |";
        for (int c = 0; c<8; c++){
            bool pintada = ((r + c) % 2 == 0);
            std::string fondo = pintada ? "\033[47m" : "\033[40m";
            char pieza = Board[r][c];
            std:: cout << fondo << " " << (pieza == ' '?' ' : pieza) <<  " \033[0m|";
        }
        std::cout << "\n";
        std::cout<< "   +---+---+---+---+---+---+---+---+\n";
    }
}


char opponent(char p){
    if (p == 'r') return 'b';
    if (p == 'b') return 'r';
    if (p == 'R') return 'b';
    if (p == 'B') return 'r';
    return ' ';
}

bool isPlayerPiece(char ch, char player){
    if(player == 'r') return ch =='r' || ch == 'R';
    if(player == 'b') return ch == 'b'|| ch == 'B';
    return false;
}

bool isQueen(char ch){
  return ch=='R' || ch=='B'; 
}

std::vector<Move> getAllMoves(char player){
    std::vector<Move> moves;
    int dirs[4][2] {{1,1}, {1,-1},{-1,1},{-1,-1}};
    for (int x=0; x<N; x++) for (int y = 0; y<N; y++){
        if(!isPlayerPiece(Board[x][y],player)) continue;
        bool queen = isQueen(Board[x][y]);
        for (int d = 0; d<N; d++){
            int dx = dirs[d][0], dy = dirs[d][1];
            if (!queen){    
                if((player == 'r' && dx == 1 || player == 'b' && dx == -1))continue;
            }
            int nx = x + dx, ny = y + dy;
            if (inside(nx, ny) && Board[nx][ny] == ' ') {
            moves.push_back({x, y, nx, ny, false});
        }

           int cx = x + dx, cy = y + dy;
           int nx2 = x + 2*dx, ny2 = y + 2*dy;

           if (inside(cx, cy) && inside(nx2, ny2)) {
           char medio = Board[cx][cy];
           bool esEnemigo = !isPlayerPiece(medio, player) && medio != ' ' && medio != '.';

           if (esEnemigo && Board[nx2][ny2] == ' ') {
           moves.push_back({x, y, nx2, ny2, true});
           }
        }   
  
        
       }
   }
    return moves;

}

void applyMoves(const Move &m){
    if (!inside(m.x1, m.y1) || !inside(m.x2,m.y2)) return;

    char p = Board[m.x1][m.y1];
    Board[m.x1][m.y1] = ' ';
    Board[m.x2][m.y2] = p;

    if(m.isCapture){
        int mx = (m.x1 + m.x2) /2;
        int my = (m.y1 + m.y2) /2;
        if(inside(mx,my)){
            Board[mx][my] = ' ';
        }
    }
    if (p== 'r' && m.x2 == 0) Board[m.x2][m.y2] = 'R';
    if (p == 'b' && m.x2 == N - 1)Board[m.x2][m.y2] = 'B';
    
}

bool hasAnyMoves(char player){
    auto mv = getAllMoves(player);
    return !mv.empty();
}

bool playerHasPieces(char player) {
	for(int i=0; i<N; i++) for(int j=0; j<N; j++)
			if(isPlayerPiece(Board[i][j], player)) return true;
	return false;
}

Move chooseAIMove(char ai) {
    std::vector<Move> moves = getAllMoves(ai);
    if (moves.empty()) return {-1, -1, -1, -1, false};

    std::vector<Move> caps;
    for (auto &m : moves) {
        if (m.isCapture) caps.push_back(m);
    }

    std::vector<Move> candidatos = !caps.empty() ? caps : moves;
    std::vector<Move> legales;
    for (auto &m : candidatos) {
        if (inside(m.x1, m.y1) && inside(m.x2, m.y2)) {
            legales.push_back(m);
        }
    }

    if (legales.empty()) return {-1, -1, -1, -1, false};

    return legales[rand() % legales.size()];
}


bool parseHumanMove(Move& m) {
    std::string linea;
    std::getline(std::cin, linea);
    std::istringstream iss(linea);
	int x1,y1,x2,y2, extra;

	if((iss >> x1 >> y1 >> x2 >> y2) && !(iss >> extra)){
    bool isCapture = (abs(x2 -x1) ==  2 && abs(y2-y1) == 2);    
	m = {x1,y1,x2,y2,isCapture};
    return true;
 }
 return false;
}

void mostrarMenuRetro() {
    std::cout << "-------------------------------\n";
    std::cout << "|      Damas Retro            |\n";
    std::cout << "|-----------------------------|\n";
    std::cout << "| 1. VS CPU                   |\n";
    std::cout << "| 2. 2 Jugadores              |\n";
    std::cout << "|-----------------------------|\n";
    std::cout << "Seleccione una opcion: ";
}

bool validarMovimiento(int& x1, int& y1, int& x2, int& y2){
    std:: string linea;
    std:: getline(std::cin, linea);
    std:: istringstream iss (linea);
    int extra;
    if((iss >> x1 >> y1 >> x2 >> y2) &&  !(iss  >> extra)){
        return true;
    }
    return false;

}