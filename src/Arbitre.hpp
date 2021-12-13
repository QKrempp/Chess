#ifndef __ARBITRE_HPP__
#define __ARBITRE_HPP__

#define EXIT_SUCCESS    0
#define EXIT_ERROR      1

#define THREATS_ONLY    1
#define MOVES_ONLY      2

#include "Board.hpp"
class Board;

#include "Player.hpp"
class Player;

#include <iostream>
#include <fstream>
#include <cstdio>
#include <time.h>

class Arbitre
{
private:
    unsigned long hashKeys[64][12];
    byte movePiece(Board* b, move m);
    Player* p[2];
    const char* fn = "data/game.txt";

    //  Calcul les menaces appliquées à chaque cases, et les coups jouables pour chaque pièce
    void threatPawn(Board* b, byte c);
    void threatRook(Board* b, byte c);
    void threatKnight(Board* b, byte c);
    void threatBishop(Board* b, byte c);
    void threatQueen(Board* b, byte c);
    void threatKing(Board* b, byte c, byte t = 0);

    void swapThreat(Board* b, move m);
    void swapMove(Board* b, move m);
    void updateThreat(Board* b, byte c);
    void manageThreats(Board* b, move m);

public:
    Arbitre(Player* p1, Player* p2);
    void initMenaces(Board* b);
    void eraseHistory();
    void play(Board* b);

    void initHash(Board* b);
    void updateHash(Board* b, move m);
    byte addHash(Board* b);

    byte moveRequest(Board* b, move m);
    byte playRequest(Board* b, move m);

    byte isCheck(Board* b, byte color);
    byte isMoveValid(Board* b, move m);
    byte isCheckmate(Board* b, byte color);
    byte isKingAlive(Board* b, byte c);
    byte isConfig3TimesMet(Board* b, unsigned long hash);

    void printMoves(Board* b);
};

void decToOct(long i);
void printMove(move m);
void printPiece(byte p);
void moveToFile(move m, const char* fn);

void showCases(unsigned long c);
void showCases(byte c);

#endif
