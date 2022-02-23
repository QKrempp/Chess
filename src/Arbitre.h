#ifndef __ARBITRE_HPP__
#define __ARBITRE_HPP__

#define EXIT_SUCCESS    0
#define EXIT_ERROR      1

#define THREATS_ONLY    1
#define MOVES_ONLY      2

#define DEBUG           1

#include "Board.h"
class Board;

#include "Player.h"
class Player;

#include <iostream>
#include <fstream>
#include <cstdio>
#include <time.h>

class Arbitre
{
private:
    uint64_t hashKeys[64][12];
    byte movePiece(Board* b, b_move m);
    Player* p[2];
    const char* fn = "data/game.txt";

    //  Calcul les menaces appliquées à chaque cases, et les coups jouables pour chaque pièce
    void threatPawn(Board* b, byte c);
    void threatRook(Board* b, byte c);
    void threatKnight(Board* b, byte c);
    void threatBishop(Board* b, byte c);
    void threatQueen(Board* b, byte c);
    void threatKing(Board* b, byte c, byte t = 0);

    void swapThreat(Board* b, b_move m);
    void swapMove(Board* b, b_move m);
    void updateThreat(Board* b, byte c);
    void manageThreats(Board* b, b_move m, byte d = 0);

public:
    Arbitre(Player* p1, Player* p2);
    void initMenaces(Board* b);
    void eraseHistory();
    void play(Board* b);

    void initHash(Board* b);
    void updateHash(Board* b, b_move m);
    byte addHash(Board* b);

    byte moveRequest(Board* b, b_move m, byte d = 0);
    byte playRequest(Board* b, b_move m);

    byte isCheck(Board* b, byte color);
    byte isMoveValid(Board* b, b_move m);
    byte isCheckmate(Board* b, byte color);
    byte isKingAlive(Board* b, byte c);
    byte isConfig3TimesMet(Board* b, uint64_t hash);

    void printMoves(Board* b);
};

void decToOct(long i);
void printMove(b_move m);
void printPiece(byte p);
void moveToFile(b_move m, const char* fn);

void showCases(uint64_t c);
void showCases(byte c);

#endif
