#include "Board.h"

// Fonctions d'initialisation
// ---------------------------------------

Board::Board()
{
    hash = new HashTable;
    hash->hash = 0;
    hash->nb = 1;
    hash->nextHash = NULL;
}


void Board::initDefault()
{
    byte piece = 0;
    for(byte i = 0; i < 8; i++){
        for(byte j = 0; j < 8; j++){
            piece = 0;
            if(i == 1 || i == 0){
                piece += WHITE << 3;
            }
            if(i == 6 || i == 7){
                piece += BLACK << 3;
            }
            if(i == 1 || i == 6){
                piece += PAWN;
            }
            if(i == 0 || i == 7){
                if(j == 0 || j == 7){
                    piece += ROOK;
                }
                if(j == 1 || j == 6){
                    piece += KNIGHT;
                }
                if(j == 2 || j == 5){
                    piece += BISHOP;
                }
                if(j == 3){
                    piece += QUEEN;
                }
                if(j == 4){
                    piece += KING;
                }
            }
            pieces[(i << 3) + j] = piece;
        }
    }
}

void Board::initClear(unsigned char p, unsigned char c)
{
    if(PIECE_TYPE(p) == KING)
    {
        kings[PIECE_COLOR(p)] = c;
    }
    pieces[c] = p;
}



// Fonctions d'accès
// ---------------------------------------

byte Board::getPiece(byte c)
{
    return pieces[c];
}

byte Board::getType(byte c)
{
    return pieces[c] & 0x7; // 0x7 = 0b111
}

byte Board::getColor(byte c)
{
    return pieces[c] >> 3;
}

uint64_t Board::getThreats(byte c, byte color)
{
    return threats[color][c];
}

uint64_t Board::getAltThreats(byte c, byte color)
{
    uint64_t altThreats = 0ul;
    for(byte i = 0; i < 64; i ++)
    {
        altThreats |= ((threats[color][i] >> c) & 1) << i;
    }
    return altThreats;
}

uint64_t Board::getMoves(byte c, byte color)
{
    return moves[color][c];
}

uint64_t Board::getAltMoves(byte c, byte color)
{
    uint64_t altMoves = 0ul;
    for(byte i = 0; i < 64; i++)
    {
        altMoves |= ((moves[color][i] >> c) & 1) << i;
    }
    return altMoves;
}

byte Board::getWinner()
{
    return winner;
}

byte Board::getPrises()
{
    return prises;
}

byte Board::getPlayer()
{
    return player;
}

byte Board::getTurn()
{
    return turn;
}

uint64_t Board::getHash()
{
    return h;
}


void Board::nextTurn()
{
    turn++;
}






// Fonctions d'affichage
// -----------------------------------------

std::ostream& operator<<(std::ostream& os, const Board& b)
{
    os << std::string("..A  B  C  D  E  F  G  H..\n");
    os << std::string(" +--+--+--+--+--+--+--+--+\n");
    for(int i = 0; i < 8; i++){
        os << i + 1;
        for(int j = 0; j < 8; j++){
            os << '|';
            if(b.pieces[INT_TO_BYTE(i, j)] & 0x7)
            {
                switch((b.pieces[INT_TO_BYTE(i, j)] >> 3) & 1)
                {
                    case WHITE:
                        os << 'W';
                        break;
                    case BLACK:
                        os << 'B';
                        break;
                }
            }else{
                os << ' ';
            }
            switch(b.pieces[INT_TO_BYTE(i, j)] & 0x7)  // 0x7 = 0b111
            {
                case PAWN:
                    os << 'P';
                    break;
                case ROOK:
                    os << 'R';
                    break;
                case KNIGHT:
                    os << 'N';
                    break;
                case BISHOP:
                    os << 'B';
                    break;
                case QUEEN:
                    os << 'Q';
                    break;
                case KING:
                    os << 'K';
                    break;
                case EMPTY:
                    os << ' ';
                    break;
            }
        }
        os << std::string("|\n");
        os << std::string(" +--+--+--+--+--+--+--+--+\n");
    }
    return os;
}
