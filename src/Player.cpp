#include "Player.hpp"

Player::Player(Board* b, byte c)
{
    color = c;
    pieces_nb = 0;
    byte p = 0;
    for(byte i = 0; i < 64; i++)
    {
        p = b->getPiece(i);
        if(PIECE_TYPE(p) != EMPTY && PIECE_COLOR(p) == color)
        {
            pieces[pieces_nb] = i;
            pieces_nb++;
        }
    }
}


move Human::play(Board* b, Arbitre* a)
{
    std::cout << *b << std::endl;
    for(byte i = 0; i < pieces_nb; i++)
    {
        byte p = b->getPiece(pieces[i]);
        if(PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)
        {
            pieces[i] = pieces[pieces_nb - 1];
            pieces_nb--;
        }
    }
    byte l = 0;
    move ml[100];
    for(byte i = 0; i < pieces_nb; i++)
    {
        uint64_t mv = b->getAltMoves(pieces[i], color);
        for(byte j = 0; j < 64; j++)
        {
            if(IS_TARGET(mv, j) && a->isMoveValid(b, BYTE_TO_MOVE(pieces[i], j)))
            {
                if(l % 5 == 0)
                {
                    std::cout << std::endl;
                }
                ml[l] = BYTE_TO_MOVE(pieces[i], j);
                std::cout << '|' <<  (int) l << ':' << '\t';
                printMove(BYTE_TO_MOVE(pieces[i], j));
                std::cout << '\t';
                l++;
            }
        }
    }
    std::string s = std::string();
    std::cout << std::endl;
    std::cin >> s;
    l = 0;
    for(int i = 0; i < s.size(); i++)
    {
        l = (move)(s[i] - '0') + 10 * l;
    }
    byte i = 0;
    while(pieces[i] != MOVE_START(ml[l]) && i < pieces_nb)
    {
        i++;
    }
    pieces[i] = MOVE_STOP(ml[l]);
    a->moveRequest(b, ml[l]);
    return ml[l];
}

move AlphaBeta::play(Board* b, Arbitre* a)
{
    for(byte i = 0; i < pieces_nb; i++)
    {
        byte p = b->getPiece(pieces[i]);
        if(PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)
        {
            pieces[i] = pieces[pieces_nb - 1];
            pieces_nb--;
        }
    }
    move bestMove;
    int bestScore = -10000;
    int alpha = -10000;
    int beta = 10000;
    for(byte i = 0; i < 64; i++){
        std::cout << (int) i << "/64" << std::endl;
        byte p = b->getPiece(i);
        if(PIECE_COLOR(p) == color && PIECE_TYPE(p) != EMPTY)
        {

            uint64_t mv = b->getAltMoves(i, color);
            for(byte j = 0; j < 64; j++)
            {
                if(IS_TARGET(mv, j) && a->isMoveValid(b, BYTE_TO_MOVE(i, j)))
                {
                    Board b0 = *b;
                    a->moveRequest(&b0, BYTE_TO_MOVE(i, j));
                    int score = digDown(&b0, a, depth - 1, &alpha, &beta) - ((int) a->isConfig3TimesMet(b, b0.getHash())) * 3000;
                    if(score > bestScore){
                        bestScore = score;
                        bestMove = BYTE_TO_MOVE(i, j);
                    }
                }
            }
        }
    }
    a->playRequest(b, bestMove);
    return bestMove;
}

int AlphaBeta::digDown(Board* b, Arbitre* a, int depth, int* alpha, int* beta){
    int v = 0;
    if(depth == 0 || !a->isKingAlive(b, WHITE) || !a->isKingAlive(b, BLACK) || b->getPrises() > 40){
        return evaluateBoard(b);
    }else if(b->getPlayer() != color){
        v = 100000;
        for(byte i = 0; i < 64; i++)
        {
            byte p = b->getPiece(i);
            if(PIECE_COLOR(p) != color && PIECE_TYPE(p) != EMPTY)
            {
                uint64_t mv = b->getAltMoves(i, b->getPlayer());
                for(byte j = 0; j < 64; j++){
                    if(IS_TARGET(mv, j))
                    {
                        Board b0 = *b;
                        a->moveRequest(&b0, BYTE_TO_MOVE(i, j));
                        int ab = digDown(&b0, a, depth - 1, alpha, beta);
                        if(ab < v){
                            v = ab;
                        }
                        if(*alpha >= v){
                            return v;
                        }
                        if(v < *beta){
                            *beta = v;
                        }
                    }
                }
            }
        }
    }else{
        v = -100000;
        for(byte i = 0; i < 64; i++)
        {
            byte p = b->getPiece(i);
            if(PIECE_COLOR(p) == color && PIECE_TYPE(p) != EMPTY)
            {
                uint64_t mv = b->getAltMoves(i, b->getPlayer());
                for(byte j = 0; j < 64; j++){
                    if(IS_TARGET(mv, j))
                    {
                        Board b0 = *b;
                        a->moveRequest(&b0, BYTE_TO_MOVE(i, j));
                        int ab = digDown(&b0, a, depth - 1, alpha, beta);
                        if(ab > v){
                            v = ab;
                        }
                        if(v >= *beta){
                            // Coupure beta
                            return v;
                        }
                        if(v > *alpha){
                            *alpha = v;
                        }
                    }
                }
            }
        }
    }
    return v;
}

int AlphaBeta::evaluateBoard(Board* b){
    int score = 0;
    int mult = 0;
    int points = 0;
    for(int i = 0; i < 64; i++){
        if(PIECE_COLOR(b->getPiece(i)) == color){
            mult = 1;
        }else{
            mult = -1;
        }
        switch(PIECE_TYPE(b->getPiece(i))){
            case PAWN:
                points = 10;
                break;
            case ROOK:
                points = 70;
                break;
            case KNIGHT:
                points = 50;
                break;
            case BISHOP:
                points = 50;
                break;
            case QUEEN:
                points = 100;
                break;
            case KING:
                points = 10000;
                break;
            default:
                points = 0;
        }
        if(b->getThreats(i, color))
        {
            score++;
        }
        if(b->getThreats(i, !color))
        {
            score--;
        }
        score += mult * points;
    }
    return score;
}

