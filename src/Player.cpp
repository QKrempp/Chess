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
    for(int i = 0; i < (int) s.size(); i++)
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
    move bestMove = 0;
    int bestScore = -__INT_MAX__;
    int alpha = -__INT_MAX__;
    int beta = __INT_MAX__;
    calculated[depth]++;
    for(byte i = 0; i < 64; i++)
    {
        std::cout << "#";
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
                    int score = digDown(&b0, a, depth - 1, alpha, beta) - ((int) a->isConfig3TimesMet(b, b0.getHash())) * 3000;
                    if(score > alpha)
                    {
                        alpha = score;
                    }
                    if(score > bestScore){
                        bestScore = score;
                        bestMove = BYTE_TO_MOVE(i, j);
                    }
                }
            }
        }
    }
    std::cout << std::endl;
    for(byte i = 0; i < 8; i++)
    {
        std::cout << calculated[i] << ' ';
    }
    std::cout << std::endl;
    a->playRequest(b, bestMove);
    return bestMove;
}

int AlphaBeta::digDown(Board* b, Arbitre* a, int depth, int alpha, int beta){
    calculated[depth]++;
    int v = 0;
    if(depth == 0 || !a->isKingAlive(b, WHITE) || !a->isKingAlive(b, BLACK) || b->getPrises() > 50){
        int sc = evaluateBoard(b);
        return sc;
    }else if(b->getPlayer() != color){
        v = __INT_MAX__;
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
                        if(alpha >= v){
                            // Coupure alpha
//                             std::cout << "Coupure alpha (" << alpha << ", " << beta << ", " << v << ")" << std::endl;
                            return v;
                        }
                        if(v < beta){
                            beta = v;
                        }
                    }
                }
            }
        }
    }else{
        v = -__INT_MAX__;
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
                        if(v >= beta){
                            // Coupure beta
//                             std::cout << "Coupure beta (" << alpha << ", " << beta << ", " << v << ")" << std::endl;
                            return v;
                        }
                        if(v > alpha){
                            alpha = v;
                        }
                    }
                }
            }
        }
    }
    return v;
}

// fonction alphabeta(nœud, i)
//          /* i représente la valeur minimale du résultat attendu.
//          Si on s'aperçoit que le résultat sera inférieur à i, autant renvoyer directement la valeur en cours (inférieure à i)
//          et ne pas perdre de temps à calculer la vraie valeur du nœud qui sera, de toute façon, inférieure à i également.
//          À l'initialisation, on prendra i = -∞ */
//    si nœud est une feuille alors
//        retourner la valeur de nœud
//    sinon
//        j = -∞
//        pour tout fils de nœud faire
//            j = max(j, alphabeta(fils, j))
//            si -j ⩽ i alors
//                retourner -j
//        retourner -j


int AlphaBeta::evaluateBoard(Board* b){
    int score = 0;
    int mult = 0;
    int points = 0;
    byte c = 0;
    for(byte i = 0; i < 64; i++){
        if(PIECE_COLOR(b->getPiece(i)) == color)
        {
            mult = 1;
        }
        else
        {
            mult = -1;
        }
        if(PIECE_COLOR(b->getPiece(i) == WHITE))
        {
            c = i;
        }
        else
        {
            c = i ^ 56;
        }
        switch(PIECE_TYPE(b->getPiece(i))){
            case PAWN:
                points = 1000 + pieces_sqv[0][c];
                break;
            case ROOK:
                points = 5000 + pieces_sqv[1][c];
                break;
            case KNIGHT:
                points = 3200 + pieces_sqv[2][c];
                break;
            case BISHOP:
                points = 3300 + pieces_sqv[3][c];
                break;
            case QUEEN:
                points = 9000 + pieces_sqv[4][c];
                break;
            case KING:
                points = 200000 + pieces_sqv[5][c];
                break;
            default:
                points = 0;
        }
//         switch(PIECE_TYPE(b->getPiece(i))){
//             case PAWN:
//                 points = 1;
//                 break;
//             case ROOK:
//                 points = 1;
//                 break;
//             case KNIGHT:
//                 points = 1;
//                 break;
//             case BISHOP:
//                 points = 1;
//                 break;
//             case QUEEN:
//                 points = 1;
//                 break;
//             case KING:
//                 points = 1;
//                 break;
//             default:
//                 points = 0;
//         }
        if(b->getMoves(i, color))
        {
            score++;
        }
        score += mult * points;
    }
    return score;
}

