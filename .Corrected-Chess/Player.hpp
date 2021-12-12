#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "Board.hpp"

#include <vector>
#include "Arbitre.hpp"

#define DEPTH 3

class Board;
class Arbitre;

using namespace std;

class Player{

    protected:
        int color;

    public:
        Player(int color0);
        ~Player(){};
        virtual void play(Board* b, Arbitre* a) = 0; //    Cette fonction devrait être déclarée ```virtual``` mais pour une raison inconnue cela empêche la compilation
        int getColor();
};

class AlphaBeta : public Player{

    private:
        int d = DEPTH;
        int explored[DEPTH];

    public:
        AlphaBeta(int color0) : Player(color0){};
        ~AlphaBeta(){};
        void play(Board* b, Arbitre* a);
        int digDown(Board b, Arbitre* a, int depth, int* alpha, int* beta);
        int evaluateBoard(Board b);
};

class Human : public Player{

    public:
        Human(int color0) : Player(color0){};
        ~Human(){};
        void play(Board* b, Arbitre* a);
};

#endif
