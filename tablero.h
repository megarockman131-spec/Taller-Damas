#ifndef TABLERO_H
#define TABLERO_H
#include <vector>
#include <string>

const int N = 8;
extern bool ok;
struct Move {
    int x1,y1,x2,y2;
    bool isCapture;
};
extern char Board[N][N];
void initBoard();
void printBoard();

bool inside(int x, int y);
char opponent(char p);
bool isPlayerPiece(char ch, char player);
bool isQueen(char ch);
void applyMoves(const Move &m);
bool hasAnyMoves(char player);
bool playerHasPieces(char player);
Move chooseAIMove(char ai);
bool parseHumanMove(Move& m);
std::vector<Move>getAllMoves(char player);  
void mostrarMenuRetro();
bool validarMovimiento(int& x1, int& y1, int& x2, int& y2);

#endif //TABLERO_H