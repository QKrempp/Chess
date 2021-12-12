#ifndef __BOARD_HPP__
#define __BOARD_HPP__

typedef unsigned char byte;
typedef unsigned short move;


#define MOVE_START(m)   ((byte) ((m >> 6) & 0x3f)) // 0x3f = 0b111111
#define MOVE_STOP(m)    ((byte) (m & 0x3f))
#define MOVE_OPT(m)     ((byte) ((m >> 12) & 0xf)) // 0xf = 0b1111
#define OPT_NONE        0
#define OPT_CASTLING    1
#define OPT_PROMOTION   2

#define PIECE_TYPE(p)   ((byte) (p & 0x7))
#define PIECE_COLOR(p)  ((byte) ((p >> 3) & 1))
#define PIECE(c, p)     ((byte) ((c << 3) + p))

#define STR_TO_BYTE(s)  ((byte) ((s[0] - 'a') + ((s[1] - '1') << 3)))
#define INT_TO_BYTE(i, j)   ((byte) ((i << 3) + j))
#define INT_TO_MOVE(i, j, k, l)     ((move) ((((i << 3) + j) << 6) + ((k << 3) + l)))
#define BYTE_TO_MOVE(i, j)  ((move) ((i << 6) + j))
#define STR_TO_MOVE(s)  ((move) (((s[0] - 'a') << 6) + ((s[1] - '1') << 9) + (s[2] - 'a') + ((s[3] - '1') << 3)))

#define GET_LINE(c)     ((byte) (c >> 3))
#define GET_COL(c)      ((byte) (c & 0x7))

#define IS_TARGET(t, c) ((byte) ((t >> c) & 1))

#define WHITE   0
#define BLACK   1
#define DRAW    2
#define RUNNING 3

#define EMPTY   0
#define PAWN    1
#define ROOK    2
#define KNIGHT  3
#define BISHOP  4
#define QUEEN   5
#define KING    6

#define QSIDE_CASTLING 0
#define KSIDE_CASTLING 1

#include <ostream>

#include "Arbitre.hpp"
class Arbitre;

typedef struct HashTable
{
    unsigned long hash = 0;
    byte nb = 0;
    byte turn[3] = {0, 0, 0};
    struct HashTable* nextHash = NULL;
}HashTable;

class Board
{
    friend class Arbitre;
private:
    byte pieces[64]                 = {0};
    unsigned long threats[2][64]    = {{0}};
    unsigned long moves[2][64]      = {{0}};
    byte kings[2]                   = {04, 074};
    byte castling[2][2]             = {{1, 1}, {1, 1}};
    HashTable* hash                 = NULL;
    byte winner                     = RUNNING;
    byte prises                     = 0;
    byte player                     = WHITE;
    unsigned long h                 = 0ul;
    byte turn                       = 0;
public:
    Board();
    ~Board(){};

    void initDefault();
    void initClear(byte p, byte c);
    void initMenaces();
    void nextTurn();

    byte getPiece(byte c);
    byte getColor(byte c);
    byte getType(byte c);
    byte getWinner();
    byte getPrises();
    byte getPlayer();
    byte getTurn();
    unsigned long getThreats(byte c, byte color);
    unsigned long getAltThreats(byte c, byte color);
    unsigned long getMoves(byte c, byte color);
    unsigned long getAltMoves(byte c, byte color);

    unsigned long getHash();

    friend std::ostream& operator<<(std::ostream& os, const Board& b);
};


#endif
