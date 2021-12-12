#include "Arbitre.hpp"


Arbitre::Arbitre(Player* p1, Player* p2)
{
    p[WHITE] = p1;
    p[BLACK] = p2;
//     srand(time(NULL));
//     for(int i = 0; i < 64; i++){
//         for(int j = 0; j < 12; j++){
//             hashKeys[i][j] = (unsigned long) rand() << 32;
//             hashKeys[i][j] += (unsigned long) rand();
//         }
//     }
}

void Arbitre::eraseHistory()
{
    remove(fn);
}


void Arbitre::play(Board* b)
{
    moveToFile(p[b->player]->play(b, this), fn);
}


// Fonctions de Hachage
// --------------------------------

// void Arbitre::initHash(Board* b)
// {
//     for(int i = 0; i < 64; i++){
//         if(PIECE_TYPE(b->pieces[i]) != EMPTY){
//             b->hash->hash ^= hashKeys[i][PIECE_COLOR(b->pieces[i]) * 6 + PIECE_TYPE(b->pieces[i]) - 1];
//         }
//     }
//     b->hash->turn[0] = 0;
//     b->hash->nb++;
// }

void Arbitre::initHash(Board* b)
{
    for(int i = 0; i < 64; i++){
        if(PIECE_TYPE(b->pieces[i]) != EMPTY){
            b->hash->hash ^= 1ul << i;
        }
    }
    b->hash->turn[0] = 0;
    b->hash->nb++;
}

// void Arbitre::updateHash(Board* b, move m)
// {
//     byte start = MOVE_START(m);
//     byte stop = MOVE_STOP(m);
//     unsigned long hash = b->hash->hash;
//     if(PIECE_TYPE(b->pieces[stop]) != EMPTY)    // If there was a piece on the arrival case, we xor it out of the hash
//     {
//         std::cout << "Xor-ing out: ";
//         printPiece(b->pieces[stop]);
//         std::cout << std::string("At ");
//         decToOct(stop);
//         std::cout << std::endl;
//         hash ^= hashKeys[stop][PIECE_COLOR(b->pieces[stop]) * 6 + PIECE_TYPE(b->pieces[stop]) - 1];
//     }
//     std::cout << "Xor-ing out: ";
//     printPiece(b->pieces[start]);
//     std::cout << std::string("At ");
//     decToOct(start);
//     std::cout << std::endl;
//     hash ^= hashKeys[start][PIECE_COLOR(b->pieces[start]) * 6 + PIECE_TYPE(b->pieces[start]) - 1];     // We xor out the moved piece from its starting case
//     std::cout << "Xor-ing in: ";
//     printPiece(b->pieces[start]);
//     std::cout << std::string("At ");
//     decToOct(stop);
//     std::cout << std::endl;
//     hash ^= hashKeys[stop][PIECE_COLOR(b->pieces[start]) * 6 + PIECE_TYPE(b->pieces[start]) - 1];      // We xor in the moved piece on its arrival case
//     b->h = hash;
// }

void Arbitre::updateHash(Board* b, move m)
{
    byte start = MOVE_START(m);
    byte stop = MOVE_STOP(m);
    unsigned long hash = b->hash->hash;
    if(PIECE_TYPE(b->pieces[stop]) != EMPTY)    // If there was a piece on the arrival case, we xor it out of the hash
    {
        hash ^= 1ul << stop;
    }
    hash ^= 1ul << start;     // We xor out the moved piece from its starting case
    hash ^= 1ul << stop;      // We xor in the moved piece on its arrival case
    b->h = hash;
}

byte Arbitre::addHash(Board* b)
{
    HashTable* tmp = b->hash;
    byte done = 0;
    while(tmp->nextHash != NULL && !done)
    {
        if(tmp->hash == b->h){
            tmp->turn[tmp->nb] = b->getTurn();
            tmp->nb++;
            done = 1;
        }else{
            tmp = tmp->nextHash;
        }
    }
    if(!done && tmp->hash == b->h)
    {
        tmp->nb++;
        done = 1;
    }
    if(!done){
        HashTable* tmp = new HashTable;
        tmp->hash = b->h;
        tmp->turn[0] = b->getTurn();
        tmp->nb = 1;
        tmp->nextHash = b->hash;
        b->hash = tmp;
    }
    return tmp->nb;
}

// Fonctions de déplacement
// --------------------------------

byte Arbitre::movePiece(Board* b, move m)
{
    if(PIECE_TYPE(b->pieces[MOVE_STOP(m)]) == EMPTY)
    {
        b->prises++;
    }
    else
    {
        b->prises = 0;
    }
    if(MOVE_OPT(m) == OPT_PROMOTION)
    {
        b->pieces[MOVE_STOP(m)] = ((b->pieces[MOVE_START(m)] & 0xf8) | QUEEN);
    }
    else
    {
        b->pieces[MOVE_STOP(m)] = b->pieces[MOVE_START(m)];
    }
    b->pieces[MOVE_START(m)] = 0;
    return EXIT_SUCCESS;
}

void Arbitre::manageThreats(Board* b, move m)
 {
    unsigned long casesToCheck = 0;

    // Construction de la liste des cases à actualiser
    casesToCheck |= b->getThreats(MOVE_START(m), WHITE);
    casesToCheck |= b->getThreats(MOVE_START(m), BLACK);
    casesToCheck |= b->getThreats(MOVE_STOP(m), WHITE);
    casesToCheck |= b->getThreats(MOVE_STOP(m), BLACK);

    casesToCheck |= b->getMoves(MOVE_START(m), WHITE);
    casesToCheck |= b->getMoves(MOVE_START(m), BLACK);
    casesToCheck |= b->getMoves(MOVE_STOP(m), WHITE);
    casesToCheck |= b->getMoves(MOVE_STOP(m), BLACK);

    casesToCheck |= 1ul << MOVE_START(m);
    casesToCheck |= 1ul << MOVE_STOP(m);

    byte wk = IS_TARGET(casesToCheck, b->kings[WHITE]);
    byte bk = IS_TARGET(casesToCheck, b->kings[BLACK]);
    if(wk || bk)
    {
        if(wk)
        {
            updateThreat(b, b->kings[WHITE]);
        }
        if(bk)
        {
            updateThreat(b, b->kings[BLACK]);
        }
    }else{
        threatKing(b, b->kings[WHITE], MOVES_ONLY);
        threatKing(b, b->kings[BLACK], MOVES_ONLY);
    }
        // Désactivation des menaces et des coups des pièces concernées AVANT le mouvement
    for(byte i = 0; i < 64; i++)
    {
        if(IS_TARGET(casesToCheck, i) && i != b->kings[WHITE] && i != b->kings[BLACK])
        {
            updateThreat(b, i);
        }
    }

    // Jeu de la pièce
    movePiece(b, m);

    // Suivi du mouvement des rois
    if(MOVE_START(m) == b->kings[0])
    {
        b->kings[0] = MOVE_STOP(m);
    }
    if(MOVE_START(m) == b->kings[1])
    {
        b->kings[1] = MOVE_STOP(m);
    }

    // Re-calcul des menaces et des coups pour les pièces concernées
    for(byte i = 0; i < 64; i++)
    {
        if(IS_TARGET(casesToCheck, i) && i != b->kings[WHITE] && i != b->kings[BLACK])
        {
            updateThreat(b, i);
        }
    }
    if(wk || bk)
    {
        if(wk)
        {
            threatKing(b, b->kings[BLACK], MOVES_ONLY);
            updateThreat(b, b->kings[WHITE]);
            threatKing(b, b->kings[BLACK], MOVES_ONLY);
        }
        if(bk)
        {
            threatKing(b, b->kings[WHITE], MOVES_ONLY);
            updateThreat(b, b->kings[BLACK]);
            threatKing(b, b->kings[WHITE], MOVES_ONLY);
        }
    }else{
        threatKing(b, b->kings[WHITE], MOVES_ONLY);
        threatKing(b, b->kings[BLACK], MOVES_ONLY);
    }
}

byte Arbitre::moveRequest(Board* b, move m)
{
//     updateHash(b, m);
    if(GET_LINE(MOVE_STOP(m)) == 0 || GET_LINE(MOVE_STOP(m)) == 7)
    {
        switch(PIECE_TYPE(b->pieces[MOVE_START(m)]))
        {
            case PAWN:
                m += (OPT_PROMOTION << 12);
                break;
            case KING:
                if((MOVE_START(m) == 04 && (MOVE_STOP(m) == 02 || MOVE_STOP(m) == 06)) || (MOVE_START(m) == 074 && (MOVE_STOP(m) == 072 || MOVE_STOP(m) == 076)))
                {
                    m+= (OPT_CASTLING << 12);
                }
                break;
        }
    }
    switch(MOVE_OPT(m))
    {
        case OPT_NONE:
            switch(MOVE_START(m))
            {
                case(0):
                    b->castling[WHITE][QSIDE_CASTLING] = 0;
                    break;
                case(04):
                    b->castling[WHITE][QSIDE_CASTLING] = 0;
                    b->castling[WHITE][KSIDE_CASTLING] = 0;
                    break;
                case(07):
                    b->castling[WHITE][KSIDE_CASTLING] = 0;
                    break;
                case(070):
                    b->castling[BLACK][QSIDE_CASTLING] = 0;
                    break;
                case(074):
                    b->castling[BLACK][QSIDE_CASTLING] = 0;
                    b->castling[BLACK][KSIDE_CASTLING] = 0;
                    break;
                case(077):
                    b->castling[BLACK][KSIDE_CASTLING] = 0;
                    break;
            }
            manageThreats(b, m);
            break;
        case OPT_CASTLING:
            switch(MOVE_STOP(m))
            {
                case(02):
                    manageThreats(b, 03);       // Déplacement de la tour queenside
                    manageThreats(b, m);
                    b->castling[WHITE][QSIDE_CASTLING] = 0;
                    b->castling[WHITE][KSIDE_CASTLING] = 0;
                    break;
                case(06):
                    manageThreats(b, 0705);     // Déplacement de la tour kingside
                    manageThreats(b, m);
                    b->castling[WHITE][QSIDE_CASTLING] = 0;
                    b->castling[WHITE][KSIDE_CASTLING] = 0;
                    break;
                case(072):
                    manageThreats(b, 07073);    // Déplacement de la tour queenside
                    manageThreats(b, m);
                    b->castling[BLACK][QSIDE_CASTLING] = 0;
                    b->castling[BLACK][KSIDE_CASTLING] = 0;
                    break;
                case(076):
                    manageThreats(b, 07775);    // Déplacement de la tour kingside
                    manageThreats(b, m);
                    b->castling[BLACK][QSIDE_CASTLING] = 0;
                    b->castling[BLACK][KSIDE_CASTLING] = 0;
                    break;
                default:
                    return EXIT_ERROR;
            }
            break;
        case OPT_PROMOTION:
            manageThreats(b, m);
            break;
        default:
            return EXIT_ERROR;
    }
    b->player = !b->player;
    return EXIT_SUCCESS;
}

byte Arbitre::playRequest(Board* b, move m)
{
    updateHash(b, m);
    moveRequest(b, m);
    if(b->prises == 50)
    {
        std::cout << std::string("50 turns without takes!") << std::endl;
        b->winner = DRAW;
    }
    if(addHash(b) == 3)
    {
        HashTable* tmp = b->hash;
        while(tmp != NULL)
        {
            std::cout << std::string("Hash: ") << '\t' << tmp->hash << std::endl;
            std::cout << std::string("Rencontré: ") << '\t' << (int) tmp->nb << std::endl;
            std::cout << '[';
            for(int i = 0; i < tmp->nb; i++)
            {
                std::cout << (int) tmp->turn[i] << ' ';
            }
            std::cout << ']' << std::endl;
            if(tmp->nb == 3)
            {
                showCases(tmp->hash);
            }
            tmp = tmp->nextHash;
        }
        std::cout << std::string("Three times the same situation!") << std::endl;
        b->winner = DRAW;
    }
    byte canWhiteMove = 0;
    byte canBlackMove = 0;
    for(byte i = 0; i < 64; i++)
    {
        for(byte j = 0; j < 64 && !(canWhiteMove && canBlackMove); j++)
        {
            if(IS_TARGET(b->getAltMoves(i, WHITE), j) && isMoveValid(b, BYTE_TO_MOVE(i, j)))
            {
                canWhiteMove = 1;
            }
            if(IS_TARGET(b->getAltMoves(i, BLACK), j) && isMoveValid(b, BYTE_TO_MOVE(i, j)))
            {
                canBlackMove = 1;
            }
        }
    }
    if(!canWhiteMove)
    {
        if(isCheck(b, WHITE))
        {
            b->winner = BLACK;
        }
        else
        {
            std::cout << std::string("White can't move!") << std::endl;
            b->winner = DRAW;
        }
    }
    if(!canBlackMove)
    {
        if(isCheck(b, BLACK))
        {
            b->winner = WHITE;
        }
        else
        {
            std::cout << std::string("Black can't move!") << std::endl;
            b->winner = DRAW;
        }
    }
    return EXIT_SUCCESS;
}


byte Arbitre::isCheck(Board* b, byte color)
{
    if(b->threats[!color][b->kings[color]])
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

byte Arbitre::isMoveValid(Board* b, move m)
{
    Board b0 = *b;
    byte color = PIECE_COLOR(b0.pieces[MOVE_START(m)]);
    moveRequest(&b0, m);
    if(isCheck(&b0, color))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

byte Arbitre::isCheckmate(Board* b, byte color)
{
    if(isCheck(b, color))
    {
        for(byte i = 0; i < 64; i++)
        {
            for(byte j = 0; j < 64; j++)
            {
                if(IS_TARGET(b->getAltMoves(i, color), j) && isMoveValid(b, BYTE_TO_MOVE(i, j)))
                {
                    return 0;
                }
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

byte Arbitre::isKingAlive(Board* b, byte c)
{
    return (b->pieces[b->kings[c]] == PIECE(c, KING));
}

byte Arbitre::isConfig3TimesMet(Board* b, unsigned long hash)
{
    HashTable* tmp = b->hash;
    while(tmp->nextHash != NULL)
    {
        tmp = tmp->nextHash;
        if(tmp->hash == hash)
        {
            if(tmp->nb == 2)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    return 0;
}




// Fonctions de menaces
// -----------------------------

void Arbitre::initMenaces(Board* b)
{
    for(byte i = 0; i < 64; i++)
    {
        if(PIECE_TYPE(b->pieces[i]) != KING)
        {
            updateThreat(b, i);
        }
    }
    updateThreat(b, b->kings[WHITE]);
    updateThreat(b, b->kings[BLACK]);
}

void Arbitre::updateThreat(Board* b, byte c)
{
    switch(PIECE_TYPE(b->pieces[c]))
        {
            case PAWN:
                threatPawn(b, c);
                break;
            case ROOK:
                threatRook(b, c);
                break;
            case KNIGHT:
                threatKnight(b, c);
                break;
            case BISHOP:
                threatBishop(b, c);
                break;
            case QUEEN:
                threatQueen(b, c);
                break;
            case KING:
                threatKing(b, c);
                break;
        }
}


void Arbitre::swapMove(Board* b, move m)
{
    byte color = PIECE_COLOR(b->pieces[MOVE_START(m)]);
    b->moves[color][MOVE_STOP(m)] ^= (1ul << MOVE_START(m));
}

void Arbitre::swapThreat(Board* b, move m)
{
    byte color = PIECE_COLOR(b->pieces[MOVE_START(m)]);
    b->threats[color][MOVE_STOP(m)] ^= (1ul << MOVE_START(m));
}

void Arbitre::threatPawn(Board* b, byte c)
{
    byte i = GET_LINE(c);
    byte j = GET_COL(c);
    switch(PIECE_COLOR(b->pieces[c]))
    {
        case WHITE:
            if(i + 1 <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + 1), j)]) == EMPTY)
            {
                swapMove(b, INT_TO_MOVE(i, j, (i + 1), j));
                if(i == 1 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + 2), j)]) == EMPTY)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i + 2), j));
                }
            }
            if(i + 1 <= 7 && j + 1 <= 7)
            {
                swapThreat(b, INT_TO_MOVE(i, j, (i + 1), (j + 1)));
                if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i + 1), (j + 1))]) == BLACK && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + 1), (j + 1))]) != EMPTY)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j + 1)));
                }
            }
            if(i + 1 <= 7 && j - 1 >= 0){
                swapThreat(b, INT_TO_MOVE(i, j, (i + 1), (j - 1)));
                if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i + 1), (j - 1))]) == BLACK && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + 1), (j - 1))]) != EMPTY)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j - 1)));
                }
            }
            break;
        case BLACK:
            if(i - 1 >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - 1), j)]) == EMPTY)
            {
                swapMove(b, INT_TO_MOVE(i, j, (i - 1), j));
                if(i == 6 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - 2), j)]) == EMPTY)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i - 2), j));
                }
            }
            if(i - 1 >= 0 && j + 1 <= 7)
            {
                swapThreat(b, INT_TO_MOVE(i, j, (i - 1), (j + 1)));
                if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i - 1), (j + 1))]) == WHITE && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - 1), (j + 1))]) != EMPTY)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j + 1)));
                }
            }
            if(i - 1 >= 0 && j - 1 >= 0){
                swapThreat(b, INT_TO_MOVE(i, j, (i - 1), (j - 1)));
                if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i - 1), (j - 1))]) == WHITE && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - 1), (j - 1))]) != EMPTY)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j - 1)));
                }
            }
            break;
    }
}

void Arbitre::threatRook(Board* b, byte c)
{
    byte i = GET_LINE(c);
    byte j = GET_COL(c);
    byte color = PIECE_COLOR(b->pieces[c]);
    byte k = 1;
    while(i - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), j)]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), j));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), j));
        k++;
    }
    if(i - k >= 0){
        if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i - k), j)]) != color){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), j));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), j));
    }
    k = 1;
    while(i + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), j)]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), j));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), j));
        k++;
    }
    if(i + k <= 7){
        if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i + k), j)]) != color){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), j));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), j));
    }
    k = 1;
    while(j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE(i, (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, i, (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, i, (j - k)));
        k++;
    }
    if(j - k >= 0){
        if(PIECE_COLOR(b->pieces[INT_TO_BYTE(i, (j - k))]) != color){
            swapMove(b, INT_TO_MOVE(i, j, i, (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, i, (j - k)));
    }
    k = 1;
    while(j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE(i, (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, i, (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, i, (j + k)));
        k++;
    }
    if(j + k <= 7){
        if(PIECE_COLOR(b->pieces[INT_TO_BYTE(i, (j + k))]) != color){
            swapMove(b, INT_TO_MOVE(i, j, i, (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, i, (j + k)));
    }
}

void Arbitre::threatKnight(Board* b, byte c)
{
    byte i = GET_LINE(c);
    byte j = GET_COL(c);
    byte p = 0;
    byte color = PIECE_COLOR(b->pieces[c]);
    if(i - 2 >= 0){
        if(j - 1 >= 0){
            swapThreat(b, INT_TO_MOVE(i, j, (i - 2), (j - 1)));
            p = b->pieces[INT_TO_BYTE((i - 2), (j - 1))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i - 2), (j - 1)));
            }
        }
        if(j + 1 <= 7){
            swapThreat(b, INT_TO_MOVE(i, j, (i - 2), (j + 1)));
            p = b->pieces[INT_TO_BYTE((i - 2), (j + 1))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i - 2), (j + 1)));
            }
        }
    }
    if(i - 1 >= 0){
        if(j - 2 >= 0){
            swapThreat(b, INT_TO_MOVE(i, j, (i - 1), (j - 2)));
            p = b->pieces[INT_TO_BYTE((i - 1), (j - 2))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j - 2)));
            }
        }
        if(j + 2 <= 7){
            swapThreat(b, INT_TO_MOVE(i, j, (i - 1), (j + 2)));
            p = b->pieces[INT_TO_BYTE((i - 1), (j + 2))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j + 2)));
            }
        }
    }
    if(i + 2 <= 7){
        if(j - 1 >= 0){
            swapThreat(b, INT_TO_MOVE(i, j, (i + 2), (j - 1)));
            p = b->pieces[INT_TO_BYTE((i + 2), (j - 1))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i + 2), (j - 1)));
            }
        }
        if(j + 1 <= 7){
            swapThreat(b, INT_TO_MOVE(i, j, (i + 2), (j + 1)));
            p = b->pieces[INT_TO_BYTE((i + 2), (j + 1))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i + 2), (j + 1)));
            }
        }
    }
    if(i + 1 <= 7){
        if(j - 2 >= 0){
            swapThreat(b, INT_TO_MOVE(i, j, (i + 1), (j - 2)));
            p = b->pieces[INT_TO_BYTE((i + 1), (j - 2))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j - 2)));
            }
        }
        if(j + 2 <= 7){
            swapThreat(b, INT_TO_MOVE(i, j, (i + 1), (j + 2)));
            p = b->pieces[INT_TO_BYTE((i + 1), (j + 2))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j + 2)));
            }
        }
    }
}

void Arbitre::threatBishop(Board* b, byte c)
{
    byte i = GET_LINE(c);
	byte j = GET_COL(c);
	byte color = PIECE_COLOR(b->pieces[c]);
    byte p = 0;
    byte k = 1;
    while(i - k >= 0 && j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        k++;
    }
    if(i - k >= 0 && j - k >= 0){
        p = b->pieces[INT_TO_BYTE((i - k), (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
    }
    k = 1;
    while(i + k <= 7 && j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        k++;
    }
    if(i + k <= 7 && j + k <= 7){
        p = b->pieces[INT_TO_BYTE((i + k), (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
    }
    k = 1;
    while(i + k <= 7 && j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        k++;
    }
    if(i + k <= 7 && j - k >= 0){
        p = b->pieces[INT_TO_BYTE((i + k), (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
    }
    k = 1;
    while(i - k >= 0 && j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        k++;
    }
    if(i - k >= 0 && j + k <= 7){
        p = b->pieces[INT_TO_BYTE((i - k), (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
    }
}

void Arbitre::threatQueen(Board* b, byte c)
{
    byte i = GET_LINE(c);
	byte j = GET_COL(c);
	byte color = PIECE_COLOR(b->pieces[c]);
    byte p =0;
    byte k = 1;
    while(i - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), j)]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), j));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), j));
        k++;
    }
    if(i - k >= 0){
        p = b->pieces[INT_TO_BYTE((i - k), j)];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), j));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), j));
    }
    k = 1;
    while(i + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), j)]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), j));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), j));
        k++;
    }
    if(i + k <= 7){
        p = b->pieces[INT_TO_BYTE((i + k), j)];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), j));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), j));
    }
    k = 1;
    while(j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE(i, (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, i, (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, i, (j - k)));
        k++;
    }
    if(j - k >= 0){
        p = b->pieces[INT_TO_BYTE(i, (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, i, (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, i, (j - k)));
    }
    k = 1;
    while(j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE(i, (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, i, (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, i, (j + k)));
        k++;
    }
    if(j + k <= 7){
        p = b->pieces[INT_TO_BYTE(i, (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, i, (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, i, (j + k)));
    }
    k = 1;
    while(i - k >= 0 && j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        k++;
    }
    if(i - k >= 0 && j - k >= 0){
        p = b->pieces[INT_TO_BYTE((i - k), (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
    }
    k = 1;
    while(i + k <= 7 && j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        k++;
    }
    if(i + k <= 7 && j + k <= 7){
        p = b->pieces[INT_TO_BYTE((i + k), (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
    }
    k = 1;
    while(i + k <= 7 && j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        k++;
    }
    if(i + k <= 7 && j - k >= 0){
        p = b->pieces[INT_TO_BYTE((i + k), (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
    }
    k = 1;
    while(i - k >= 0 && j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        k++;
    }
    if(i - k >= 0 && j + k <= 7){
        p = b->pieces[INT_TO_BYTE((i - k), (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
    }
}

void Arbitre::threatKing(Board* b, byte c, byte t)
{
    byte i = GET_LINE(c);
	byte j = GET_COL(c);
	byte color = PIECE_COLOR(b->pieces[c]);
	byte p = 0;
    if(i - 1 >= 0){
        if(t != MOVES_ONLY)
		{
			swapThreat(b, INT_TO_MOVE( i, j, (i - 1), j));
		}
        p = b->pieces[INT_TO_BYTE((i - 1), j)];
		if(!b->threats[!color][INT_TO_BYTE((i - 1), j)] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            if(t != THREATS_ONLY)
			{
				swapMove(b, INT_TO_MOVE(i, j, (i - 1), j));
			}
        }
    }
    if(i + 1 <= 7){
        if(t != MOVES_ONLY)
		{
			swapThreat(b, INT_TO_MOVE( i, j, (i + 1), j));
		}
        p = b->pieces[INT_TO_BYTE((i + 1), j)];
		if(!b->threats[!color][INT_TO_BYTE((i + 1), j)] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            if(t != THREATS_ONLY)
			{
				swapMove(b, INT_TO_MOVE(i, j, (i + 1), j));
			}
        }
    }
    if(j - 1 >= 0){
        if(t != MOVES_ONLY)
		{
			swapThreat(b, INT_TO_MOVE( i, j, i, (j - 1)));
		}
        p = b->pieces[INT_TO_BYTE(i, (j - 1))];
		if(!b->threats[!color][INT_TO_BYTE(i, (j - 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            if(t != THREATS_ONLY)
			{
				swapMove(b, INT_TO_MOVE(i, j, i, (j - 1)));
			}
        }
    }
    if(j + 1 <= 7){
        if(t != MOVES_ONLY)
		{
			swapThreat(b, INT_TO_MOVE( i, j, i, (j + 1)));
		}
        p = b->pieces[INT_TO_BYTE(i, (j + 1))];
		if(!b->threats[!color][INT_TO_BYTE(i, (j + 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            if(t != THREATS_ONLY)
			{
				swapMove(b, INT_TO_MOVE(i, j, i, (j + 1)));
			}
        }
    }
    if(i - 1 >= 0 && j - 1 >= 0){
        if(t != MOVES_ONLY)
		{
			swapThreat(b, INT_TO_MOVE( i, j, (i - 1), (j - 1)));
		}
        p = b->pieces[INT_TO_BYTE((i - 1), (j - 1))];
		if(!b->threats[!color][INT_TO_BYTE((i - 1), (j - 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            if(t != THREATS_ONLY)
			{
				swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j - 1)));
			}
        }
    }
    if(i + 1 <= 7 && j + 1 <= 7){
        if(t != MOVES_ONLY)
		{
			swapThreat(b, INT_TO_MOVE( i, j, (i + 1), (j + 1)));
		}
        p = b->pieces[INT_TO_BYTE((i + 1), (j + 1))];
		if(!b->threats[!color][INT_TO_BYTE((i + 1), (j + 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            if(t != THREATS_ONLY)
			{
				swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j + 1)));
			}
        }
    }
    if(i + 1 <= 7 && j - 1 >= 0){
        if(t != MOVES_ONLY)
		{
			swapThreat(b, INT_TO_MOVE( i, j, (i + 1), (j - 1)));
		}
        p = b->pieces[INT_TO_BYTE((i + 1), (j - 1))];
		if(!b->threats[!color][INT_TO_BYTE((i + 1), (j - 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            if(t != THREATS_ONLY)
			{
				swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j - 1)));
			}
        }
    }
    if(i - 1 >= 0 && j + 1 <= 7){
        if(t != MOVES_ONLY)
		{
			swapThreat(b, INT_TO_MOVE( i, j, (i - 1), (j + 1)));
		}
        p = b->pieces[INT_TO_BYTE((i - 1), (j + 1))];
		if(!b->threats[!color][INT_TO_BYTE((i - 1), (j + 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            if(t != THREATS_ONLY)
			{
				swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j + 1)));
			}
        }
    }
    if(b->castling[color][QSIDE_CASTLING]
        && !b->pieces[INT_TO_BYTE(i, (j - 3))]
        && !b->pieces[INT_TO_BYTE(i, (j - 2))]
        && !b->pieces[INT_TO_BYTE(i, (j - 1))]
        && !b->threats[!color][INT_TO_BYTE(i, j - 2)]
        && !b->threats[!color][INT_TO_BYTE(i, j - 1)]
        && !b->threats[!color][INT_TO_BYTE(i, j)]
        && t != THREATS_ONLY)
    {
        swapMove(b, INT_TO_MOVE(i, j, i, (j - 2)));
    }
    if(b->castling[color][KSIDE_CASTLING]
        && !b->pieces[INT_TO_BYTE(i, (j + 2))]
        && !b->pieces[INT_TO_BYTE(i, (j + 1))]
        && !b->threats[!color][INT_TO_BYTE(i, j + 2)]
        && !b->threats[!color][INT_TO_BYTE(i, j + 1)]
        && !b->threats[!color][INT_TO_BYTE(i, j)]
        && t != THREATS_ONLY)
    {
        swapMove(b, INT_TO_MOVE(i, j, i, (j + 2)));
    }
}


// Fonctions d'affichage
// -----------------------

void Arbitre::printMoves(Board* b)
{
    for(byte color = 0; color < 2; color ++)
    {
        switch(color)
        {
            case WHITE:
                std::cout << std::string("         Menaces Blanches      Mouvements Blancs") << std::endl;
                break;
            case BLACK:
                std::cout << std::string("         Menaces Noires        Mouvements Noires") << std::endl;
        }
        for(byte i = 0; i < 8; i++)
        {
            std::cout << std::string("+--+--+--+--+--+--+--+--+      +--+--+--+--+--+--+--+--+") << std::endl;
            for(byte j = 0; j < 8; j++)
            {
                std::cout << '|';
                if(b->threats[color][INT_TO_BYTE(i, j)])
                {
                    std::cout << std::string("x ");
                }
                else
                {
                    std::cout << std::string("  ");
                }
            }
            std::cout << std::string("|      ");
            for(byte j = 0; j < 8; j++)
            {
                std::cout << '|';
                if(b->moves[color][INT_TO_BYTE(i, j)])
                {
                    std::cout << std::string("x ");
                }
                else
                {
                    std::cout << std::string("  ");
                }
            }
            std::cout << '|' << std::endl;
        }
        std::cout << std::string("+--+--+--+--+--+--+--+--+      +--+--+--+--+--+--+--+--+") << std::endl;
    }
}

void decToOct(long i)
{
    long d = 0;
    long p = 1;
    while(i > 0)
    {
        d +=((i & 07) * p);
        i >>= 3;
        p *= 10;
    }
    std::cout << d;
}

void printMove(move m)
{
    std::string s = std::string("    ");
    s[2] = (char) (m & 07) + 'a';
    s[3] = (char) ((m >> 3) & 07) + '1';
    s[0] = (char) ((m >> 6) & 07) + 'a';
    s[1] = (char) ((m >> 9) & 07) + '1';
    std::cout << s;
}

void moveToFile(move m, const char* fn)
{
    std::ofstream f;
    f.open(fn, std::ios::app);
    f << (char) (((m >> 6) & 07) + 'a');
    f << (char) (((m >> 9) & 07) + '1');
    f << (char) ((m & 07) + 'a');
    f << (char) (((m >> 3) & 07) + '1');
    f << std::endl;
    f.close();
}

void printPiece(byte p)
{
    switch(PIECE_TYPE(p))
    {
        case PAWN:
            std::cout << std::string("Pion ");
            break;
        case ROOK:
            std::cout << std::string("Tour ");
            break;
        case KNIGHT:
            std::cout << std::string("Cavalier ");
            break;
        case BISHOP:
            std::cout << std::string("Fou ");
            break;
        case QUEEN:
            std::cout << std::string("Dame ");
            break;
        case KING:
            std::cout << std::string("Roi ");
            break;
        default:
            std::cout << std::string("Vide ");
    }
    if(PIECE_TYPE(p) != EMPTY)
    {
        switch(PIECE_COLOR(p))
        {
            case WHITE:
                std::cout << std::string("blanc");
                break;
            case BLACK:
                std::cout << std::string("noir");
                break;
        }
    }
    std::cout << std::endl;
}

void showCases(unsigned long c)
{
        for(byte i = 0; i < 8; i++)
        {
            std::cout << std::string("+--+--+--+--+--+--+--+--+") << std::endl;
            for(byte j = 0; j < 8; j++)
            {
                std::cout << '|';
                if(IS_TARGET(c, INT_TO_BYTE(i, j)))
                {
                    std::cout << std::string("x ");
                }
                else
                {
                    std::cout << std::string("  ");
                }
            }
            std::cout << '|' << std::endl;
        }
        std::cout << std::string("+--+--+--+--+--+--+--+--+") << std::endl;
}

void showCases(byte c)
{
        for(byte i = 0; i < 8; i++)
        {
            std::cout << std::string("+--+--+--+--+--+--+--+--+") << std::endl;
            for(byte j = 0; j < 8; j++)
            {
                std::cout << '|';
                if(c == INT_TO_BYTE(i, j))
                {
                    std::cout << std::string("x ");
                }
                else
                {
                    std::cout << std::string("  ");
                }
            }
            std::cout << '|' << std::endl;
        }
        std::cout << std::string("+--+--+--+--+--+--+--+--+") << std::endl;
}
