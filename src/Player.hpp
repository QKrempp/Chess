#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "Board.hpp"
class Board;

#include "Arbitre.hpp"
class Arbitre;

class Player
{
protected:
    byte color;
    byte pieces[32];
    byte pieces_nb;
public:
    Player(Board* b, byte c);
    ~Player(){};
    virtual move play(Board* b, Arbitre* a){return 0;};
};


class Human : public Player
{
private:

public:
    Human(Board*b, byte c) : Player(b, c){};
    move play(Board * b, Arbitre * a) override;
};

class AlphaBeta : public Player
{
private:
    byte depth = 6;
public:
    AlphaBeta(Board*b, byte c) : Player(b, c){};
    move play(Board * b, Arbitre * a) override;
    int digDown(Board* b, Arbitre* a, int depth, int* alpha, int* beta);
    int evaluateBoard(Board* b);
};


#endif
