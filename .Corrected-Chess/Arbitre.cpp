#include "Arbitre.hpp"

Arbitre::Arbitre(){
    srand(time(NULL));
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 12; j++){
            hashtable[i][j] = rand();
        }
    }
    h = new HashList;
}

void Arbitre::initHash(Board* b){
    for(int i = 0; i < 64; i++){
        if(b->pieces[i] != EMPTY){
            b->hash ^= hashtable[i][b->pieces[i][COLOR] * 6 + (b->pieces[i][TYPE] - 1)];
        }
    }
    h->hash = b->hash;
    h->nb++;
}

void Arbitre::updateHash(Board* b, string s){
    int caseStop = (int)(s[3] - '1') * 8 + (int)(s[2] - 'a');
    b->hash ^= hashtable[caseStop][b->pieces[caseStop][COLOR] * 6 + (b->pieces[caseStop][TYPE] - 1)];
}

void Arbitre::clearHash(Board* b, string s){
    int caseStart = (int)(s[1] - '1') * 8 + (int)(s[0] - 'a');
    int caseStop = (int)(s[3] - '1') * 8 + (int)(s[2] - 'a');
    if(b->pieces[caseStart][TYPE] != EMPTY){
        b->hash ^= hashtable[caseStart][b->pieces[caseStart][COLOR] * 6 + (b->pieces[caseStart][TYPE] - 1)];
    }
    if(b->pieces[caseStop][TYPE] != EMPTY){
        b->hash ^= hashtable[caseStop][b->pieces[caseStop][COLOR] * 6 + (b->pieces[caseStop][TYPE] - 1)];
    }
}

void Arbitre::addHash(Board* b){
    HashList* tmp = h;
    bool done = false;
    while(tmp->next != NULL && !done){
        if(tmp->hash == b->hash){
            tmp->addVisite();
            done = true;
        }else{
            tmp = tmp->next;
        }
    }
    if(!done && tmp->hash == b->hash){
        tmp->addVisite();
        done = true;
    }
    if(!done){
        tmp->addNext();
        tmp->next->hash = b->hash;
        tmp->next->nb = 1;
    }
}

void Arbitre::move(Board* b, string s){

    if(s == "gc"){
        int i = 0;
        string posK = "e1c1";
        string posR = "a1d1";
        if(b->currentPlayer == BLACK){
            i = 7;
            posK = "e8c8";
            posR = "a8d8";
        }

        // "Dé-hashage" des pièces sur les cases de départ et d'arrivée
        clearHash(b, posK);
        clearHash(b, posR);

        // Déplacement du roi
        b->pieces[i * 8 + 2][COLOR] = b->pieces[i * 8 + 4][COLOR];
        b->pieces[i * 8 + 2][TYPE] = b->pieces[i * 8 + 4][TYPE];
        b->kings[b->currentPlayer - 1] = posK.substr(2, 2);
        b->pieces[i * 8 + 4][COLOR] = EMPTY;
        b->pieces[i * 8 + 4][TYPE] = EMPTY;

        // Déplacement de la tour
        b->pieces[i * 8 + 3][COLOR] = b->pieces[i * 8][COLOR];
        b->pieces[i * 8 + 3][TYPE] = b->pieces[i * 8][TYPE];
        b->pieces[i * 8][COLOR] = EMPTY;
        b->pieces[i * 8][TYPE] = EMPTY;

        // Mise à jour du hash avec les cases d'arrivée
        updateHash(b, posK);
        updateHash(b, posR);

        // Mise à jour des menaces
        updateMenaces(b, i * 8 + 4, i * 8 + 2);
        updateMenaces(b, i * 8, i * 8 + 3);

        b->noPrise++;  // Pas de prise lors d'un roque

        // Plus de b->roque possible une fois celui-ci effectué
        b->roque[b->currentPlayer - 1][0] = 0;
        b->roque[b->currentPlayer - 1][1] = 0;

    }else if(s == "sc"){
        int i = 0;
        string posK = "e1g1";
        string posR = "h1f1";
        if(b->currentPlayer == BLACK){
            i = 7;
            posK = "e8g8";
            posR = "h8f8";
        }

        // "Dé-hashage" des pièces sur les cases de départ et d'arrivée
        clearHash(b, posK);
        clearHash(b, posR);

        // Déplacement du roi
        b->pieces[i * 8 + 6][COLOR] = b->pieces[i * 8 + 4][COLOR];
        b->pieces[i * 8 + 6][TYPE] = b->pieces[i * 8 + 4][TYPE];
        b->kings[b->currentPlayer - 1] = posK.substr(2, 2);
        b->pieces[i * 8 + 4][COLOR] = EMPTY;
        b->pieces[i * 8 + 4][TYPE] = EMPTY;

        // Déplacement de la b->tour
        b->pieces[i * 8 + 5][COLOR] = b->pieces[i * 8 + 7][COLOR];
        b->pieces[i * 8 + 5][TYPE] = b->pieces[i * 8 + 7][TYPE];
        b->pieces[i * 8 + 7][COLOR] = EMPTY;
        b->pieces[i * 8 + 7][TYPE] = EMPTY;

        // Mise à jour du hash avec les cases d'arrivée
        updateHash(b, posK);
        updateHash(b, posR);

        // Mise à jour des menaces
        updateMenaces(b, i * 8 + 4, i * 8 + 6);
        updateMenaces(b, i * 8 + 7, i * 8 + 5);

        b->noPrise++;  // Pas de prise lors d'un broque

        // Plus de roque possible une fois celui-ci effectué
        b->roque[b->currentPlayer - 1][0] = 0;
        b->roque[b->currentPlayer - 1][1] = 0;

    }else if(s[0] == 'p'){
        int caseStart = (int)(s[2] - '1') * 8 + (int)(s[1] - 'a');
        int caseStop = (int)(s[4] - '1') * 8 + (int)(s[3] - 'a');

        // "Dé-hashage" des pièces sur la case de départ et d'arrivée
        clearHash(b, s.substr(1, 4));

        // Déplacement du pion et promotion
        b->pieces[caseStop][COLOR] = b->pieces[caseStart][COLOR];
        b->pieces[caseStop][TYPE] = QUEEN;
        b->pieces[caseStart][COLOR] = EMPTY;
        b->pieces[caseStart][TYPE] = EMPTY;

        // Mise à jour du hash avec les cases de départ et d'arrivée
        updateHash(b, s.substr(1, 4));

        // Mise à jour des menaces
        updateMenaces(b, caseStart, caseStop);

    }else{
        int caseStart = (int)(s[1] - '1') * 8 + (int)(s[0] - 'a');
        int caseStop = (int)(s[3] - '1') * 8 + (int)(s[2] - 'a');

        // Décompte du nombre de b->coups sans prises
        if(b->pieces[caseStop][COLOR] != EMPTY){
            b->noPrise = 0;
        }else{
            b->noPrise++;
        }

        // Mise à jour des b->roques
        if(b->pieces[caseStart][TYPE] == KING){
            b->roque[b->currentPlayer - 1][0] = 0;
            b->roque[b->currentPlayer - 1][1] = 0;
            b->kings[b->pieces[caseStart][COLOR] - 1] = s.substr(2, 2); // Suivi de la position des rois
        }
        if(b->pieces[caseStart][TYPE] == ROOK && (caseStart % 8) == 0){
            b->roque[b->currentPlayer - 1][0] = 0;
        }
        if(b->pieces[caseStart][TYPE] == ROOK && (caseStart % 8) == 7){
            b->roque[b->currentPlayer - 1][1] = 0;
        }
        if(b->pieces[caseStop][TYPE] == ROOK && (caseStop % 8) == 0){
            b->roque[b->nextPlayer - 1][0] = 0;
        }
        if(b->pieces[caseStop][TYPE] == ROOK && (caseStop % 8) == 7){
            b->roque[b->nextPlayer - 1][1] = 0;
        }

        // "Dé-hashage" des pièces sur la case de départ et d'arrivée
        clearHash(b, s);

        // Jeu de la pièce
        b->pieces[caseStop][COLOR] = b->pieces[caseStart][COLOR];
        b->pieces[caseStop][TYPE] = b->pieces[caseStart][TYPE];
        b->pieces[caseStart][COLOR] = EMPTY;
        b->pieces[caseStart][TYPE] = EMPTY;

        // Mise à jour du hash
        updateHash(b, s);

        // Mise à jour des menaces
        updateMenaces(b, caseStart,caseStop);
    }
}

void Arbitre::requestMove(Board* b, int color, string s, bool mustAddHash){
    if(b->winner == EMPTY){
        if(b->currentPlayer == color){
            for(int i = 0; i < (int) b->coups[color - 1].size(); i++){
                if(!b->coups[color - 1][i].compare(s)){
                    move(b, s);

                    updateEndgameConditions(b);

                    // Suivi de la couleur du dernier joueur
                    if(b->currentPlayer == WHITE){
                        b->currentPlayer = BLACK;
                        b->nextPlayer = WHITE;
                    }else{
                        b->currentPlayer = WHITE;
                        b->nextPlayer = BLACK;
                    }

                    if(mustAddHash){
                        addHash(b);
                    }

                    // Décompte du nombre de tours joués
                    b->tour++;

                    break;
                }
            }
        }else{
            cout << "Ce n'est pas à vous de jouer" << endl;
        }
    }else{
        cout << "Impossible: partie terminée." << endl;
        switch(b->winner){
            case DRAW:
                cout << "Egalité!" << endl;
                break;
            case WHITE:
                cout << "Vainqueur: Blanc!" << endl;
                break;
            case BLACK:
                cout << "Vainqueur: Noir!" << endl;
                break;
        }
    }
}

void Arbitre::updateEndgameConditions(Board* b){
    // Vérification des conditions de victoire
    //  Echec et mat
    if(isCheck(b, b->currentPlayer)){
        if(isCheckmate(b, b->currentPlayer)){
            b->winner = b->nextPlayer;
        }
    }

    //  Egalité par (probable) retour 3 fois à la même situation
    HashList* tmp = h;
    bool found = false;
    while(tmp != NULL && !found){
        if(tmp->hash == b->hash){
            found = true;
        }else{
            tmp = tmp->next;
        }
    }
    if(found && (int) tmp->nb >= 2){
        b->winner = DRAW;
    }
    //  Egalité par absence de prise sur plus de 50 coups
    if(b->noPrise >= 50){
        b->winner = DRAW;
    }

    //  Pat (l'adversaire ne peut plus jouer)
    if(b->coups[b->nextPlayer - 1].empty()){
        cout << "Pat" <<endl;
        b->winner = DRAW;
    }
}

int Arbitre::isGameOver(Board* b){
    updateEndgameConditions(b);
    return b->winner;
}

void Arbitre::updateMenaces(Board* b, int caseStart, int caseStop){

    vector<string> toUpdate[4];
    string s = "  ";
    s[0] = (caseStop % 8) + 'a';
    s[1] = (caseStop / 8) + '1';

    // Recensement des cases à mettre à jour
    toUpdate[b->currentPlayer - 1].push_back(s);
    for(int i = 0; i < (int) b->menaces[caseStart][b->currentPlayer - 1].size(); i++){
        toUpdate[b->currentPlayer - 1].push_back(b->menaces[caseStart][b->currentPlayer - 1][i]);
    }
    for(int i = 0; i < (int) b->menaces[caseStart][b->nextPlayer - 1].size(); i++){
        toUpdate[b->nextPlayer - 1].push_back(b->menaces[caseStart][b->nextPlayer - 1][i]);
    }
    for(int i = 0; i < (int) b->menaces[caseStop][b->currentPlayer - 1].size(); i++){
        toUpdate[b->currentPlayer - 1].push_back(b->menaces[caseStop][b->currentPlayer - 1][i]);
    }
    for(int i = 0; i < (int) b->menaces[caseStop][b->nextPlayer - 1].size(); i++){
        toUpdate[b->nextPlayer - 1].push_back(b->menaces[caseStop][b->nextPlayer - 1][i]);
    }

    // Vérification des cases de mouvement des pions
    for(int i = 0; i < (int) b->menaces[caseStart][b->currentPlayer + 1].size(); i++){
        toUpdate[b->currentPlayer + 1].push_back(b->menaces[caseStart][b->currentPlayer + 1][i]);
    }
    for(int i = 0; i < (int) b->menaces[caseStart][b->nextPlayer + 1].size(); i++){
        toUpdate[b->nextPlayer + 1].push_back(b->menaces[caseStart][b->nextPlayer + 1][i]);
    }
    for(int i = 0; i < (int) b->menaces[caseStop][b->currentPlayer + 1].size(); i++){
        toUpdate[b->currentPlayer + 1].push_back(b->menaces[caseStop][b->currentPlayer + 1][i]);
    }
    for(int i = 0; i < (int) b->menaces[caseStop][b->nextPlayer + 1].size(); i++){
        toUpdate[b->nextPlayer + 1].push_back(b->menaces[caseStop][b->nextPlayer + 1][i]);
    }

    // On parcours les cases à mettre à jour pour supprimer les menaces et les coups qui y sont liées (puisqu'on va les mettre à jour)
    for(int color = 0; color < 4; color++){
        for(int i = 0; i < (int) toUpdate[color].size(); i++){
            cout << "Mise à jour de " << toUpdate[color][i] << endl;
            int x1 = toUpdate[color][i][1] - '1';
            int y1 = toUpdate[color][i][0] - 'a';
            for(int j = 0; j < (int) b->cibles[x1 * 8 + y1][color].size(); j++){
                int x2 = b->cibles[x1 * 8 + y1][color][j][1] - '1';
                int y2 = b->cibles[x1 * 8 + y1][color][j][0] - 'a';
                for(int k = 0; k < (int) b->menaces[x2 * 8 + y2][color].size(); k++){
                    if(b->menaces[x2 * 8 + y2][color][k].compare(b->cibles[x1 * 8 + y1][color][j]) != 0){
                        b->menaces[x2 * 8 + y2][color].erase(b->menaces[x2 * 8 + y2][color].begin() + k);
                        break;  // Il ne peut pas y avoir plusieurs b->menaces provenant de la même case
                    }
                }
            }
            for(int j = (int) b->coups[color % 2].size() - 1; j >= 0; j--){
                if(toUpdate[color][i] == b->coups[color % 2][j].substr(0,2)){
                    b->coups[color % 2].erase(b->coups[color % 2].begin() + j);
                }
            }
        }
        for(int j = (int) b->coups[color % 2].size() - 1; j >= 0; j--){
            if(s.substr(2, 2) == b->coups[color % 2][j].substr(0,2)){
                b->coups[color % 2].erase(b->coups[color % 2].begin() + j);
            }
        }
    }

    for(int color = 0; color < 4; color ++){
        for(int i = 0; i < (int) toUpdate[color].size(); i++){
            int x1 = toUpdate[color][i][1] - '1';
            int y1 = toUpdate[color][i][0] - 'a';
            switch(b->pieces[x1 * 8 + y1][TYPE]){
                case PAWN:
                    menacePawn(b, x1, y1, (color % 2) + 1);
                    break;
                case ROOK:
                    menaceRook(b, x1, y1, (color % 2) + 1);
                    break;
                case KNIGHT:
                    menaceKnight(b, x1, y1, (color % 2) + 1);
                    break;
                case BISHOP:
                    menaceBishop(b, x1, y1, (color % 2) + 1);
                    break;
                case QUEEN:
                    menaceQueen(b, x1, y1, (color % 2) + 1);
                    break;
                case KING:
                    menaceKing(b, x1, y1, (color % 2) + 1);
                    break;
            }
        }
    }
}

// void Arbitre::eraseMenaces(Board* b, int caseStart){
//     for(int color = 0; color < 4; color ++){
//         for(int i = 0; i < b->cibles[caseStart][color].size(); i++){
//
//         }
//     }
// }


void Arbitre::updateMenacesFull(Board* b){
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j++){
            int color = b->pieces[i * 8 + j][COLOR];
            switch(b->pieces[i * 8 + j][TYPE]){
                case PAWN:
                    menacePawn(b, i, j, color);
                    break;
                case ROOK:
                    menaceRook(b, i, j, color);
                    break;
                case KNIGHT:
                    menaceKnight(b, i, j, color);
                    break;
                case BISHOP:
                    menaceBishop(b, i, j, color);
                    break;
                case QUEEN:
                    menaceQueen(b, i, j, color);
                    break;
                case KING:
                    menaceKing(b, i, j, color);
                    break;
            }
        }
    }
}

void Arbitre::printMenaces(Board* b){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            cout << "(";
            for(int color = 0; color < 2; color++){
                cout << (int) b->menaces[i * 8 + j][color].size();
                if(color == 0){cout << ", ";}
            }
            cout << ") ";
        }
        cout << endl;
    }
}

void Arbitre::printCoups(Board* b){
    for(int color = 0; color < 2; color++){
        cout << "Couleur " << color << ": ";
        for(int i = 0; i < (int) b->coups[color].size(); i++){
            cout << b->coups[color][i] << "    ";
        }
        cout << endl;
    }
}

void Arbitre::menacePawn(Board* b, int i, int j, int color){
    b->cibles[i * 8 + j][color - 1].clear();
    if(color == WHITE){
        if(i + 1 <= 7 && b->pieces[(i + 1) * 8 + j][TYPE] == EMPTY){
            if(i == 6){
                addCoup(b, color, i, j, i + 1, j, PROMOTION);
            }else{
                addCoup(b, color, i, j, i + 1, j);
            }
            addMenace(b, PAWN_WHITE, i, j, i + 1, j);
            if(i == 1 && b->pieces[(i + 2) * 8 + j][TYPE] == EMPTY){
                addCoup(b, color, i, j, i + 2, j);
                addMenace(b, PAWN_WHITE, i, j, i + 2, j);
            }
        }
        if(i + 1 <= 7 && j + 1 <= 7){
            addMenace(b, color, i, j, i + 1, j + 1);
            if(b->pieces[(i + 1) * 8 + (j + 1)][COLOR] == BLACK){
                if(i == 6){
                    addCoup(b, color, i, j, i + 1, j + 1, PROMOTION);
                }else{
                    addCoup(b, color, i, j, i + 1, j + 1);
                }
            }
        }
        if(i + 1 <= 7 && j - 1 >= 0){
            addMenace(b, color, i, j, i + 1, j - 1);
            if(b->pieces[(i + 1) * 8 + (j - 1)][COLOR] == BLACK){
                if(i == 6){
                    addCoup(b, color, i, j, i + 1, j - 1, PROMOTION);
                }else{
                    addCoup(b, color, i, j, i + 1, j - 1);
                }
            }
        }
    }
    if(color == BLACK){
        if(i - 1 >= 0 && b->pieces[(i - 1) * 8 + j][TYPE] == EMPTY){
            if(i == 1){
                addCoup(b, color, i, j, i - 1, j, PROMOTION);
            }else{
                addCoup(b, color, i, j, i - 1, j);
            }
            addMenace(b, PAWN_BLACK, i, j, i - 1, j);
            if(i == 6 && b->pieces[(i - 2) * 8 + j][TYPE] == EMPTY){
                addCoup(b, color, i, j, i - 2, j);
                addMenace(b, PAWN_BLACK, i, j, i - 2, j);
            }
        }
        if(i - 1 >= 0 && j + 1 <= 7){
            addMenace(b, color, i, j, i - 1, j + 1);
            if(b->pieces[(i - 1) * 8 + (j + 1)][COLOR] == WHITE){
                if(i == 1){
                    addCoup(b, color, i, j, i - 1, j + 1, PROMOTION);
                }else{
                    addCoup(b, color, i, j, i - 1, j + 1);
                }
            }
        }
        if(i - 1 >= 0 && j - 1 >= 0){
            addMenace(b, color, i, j, i - 1, j - 1);
            if(b->pieces[(i - 1) * 8 + (j - 1)][COLOR] == WHITE){
                if(i == 1){
                    addCoup(b, color, i, j, i - 1, j - 1, PROMOTION);
                }else{
                    addCoup(b, color, i, j, i - 1, j - 1);
                }
            }
        }
    }
}

void Arbitre::menaceRook(Board* b, int i, int j, int color){
    b->cibles[i * 8 + j][color - 1].clear();
    int k = 1;
    while(i - k >= 0 && b->pieces[(i - k) * 8 + j][TYPE] == EMPTY){
        addMenace(b, color, i, j, i - k, j);
        addCoup(b, color, i, j, i - k, j);
        k++;
    }
    if(i - k >= 0){
        if(b->pieces[(i - k) * 8 + j][COLOR] != color){
            addCoup(b, color, i, j, i - k, j);
        }
        addMenace(b, color, i, j, i - k, j);
    }
    k = 1;
    while(i + k <= 7 && b->pieces[(i + k) * 8 + j][TYPE] == EMPTY){
        addMenace(b, color, i, j, i + k, j);
        addCoup(b, color, i, j, i + k, j);
        k++;
    }
    if(i + k <= 7){
        if(b->pieces[(i + k) * 8 + j][COLOR] != color){
            addCoup(b, color, i, j, i + k, j);
        }
        addMenace(b, color, i, j, i + k, j);
    }
    k = 1;
    while(j - k >= 0 && b->pieces[i * 8 + (j - k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i, j - k);
        addCoup(b, color, i, j, i, j - k);
        k++;
    }
    if(j - k >= 0){
        if(b->pieces[i * 8 + (j - k)][COLOR] != color){
            addCoup(b, color, i, j, i, j - k);
        }
        addMenace(b, color, i, j, i, j - k);
    }
    k = 1;
    while(j + k <= 7 && b->pieces[i * 8 + (j + k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i, j + k);
        addCoup(b, color, i, j, i, j + k);
        k++;
    }
    if(j + k <= 7){
        if(b->pieces[i * 8 + (j + k)][COLOR] != color){
            addCoup(b, color, i, j, i, j + k);
        }
        addMenace(b, color, i, j, i, j + k);
    }
}

void Arbitre::menaceKnight(Board* b, int i, int j, int color){
    b->cibles[i * 8 + j][color - 1].clear();
    if(i - 2 >= 0){
        if(j - 1 >= 0){
            addMenace(b, color, i, j, i - 2, j - 1);
            if(b->pieces[(i - 2) * 8 + (j - 1)][COLOR] != color){
                addCoup(b, color, i, j, i - 2, j - 1);
            }
        }
        if(j + 1 <= 7){
            addMenace(b, color, i, j, i - 2, j + 1);
            if(b->pieces[(i - 2) * 8 + (j + 1)][COLOR] != color){
                addCoup(b, color, i, j, i - 2, j + 1);
            }
        }
    }
    if(i - 1 >= 0){
        if(j - 2 >= 0){
            addMenace(b, color, i, j, i - 1, j - 2);
            if(b->pieces[(i - 1) * 8 + (j - 2)][COLOR] != color){
                addCoup(b, color, i, j, i - 1, j - 2);
            }
        }
        if(j + 2 <= 7){
            addMenace(b, color, i, j, i - 1, j + 2);
            if(b->pieces[(i - 1) * 8 + (j + 2)][COLOR] != color){
                addCoup(b, color, i, j, i - 1, j + 2);
            }
        }
    }
    if(i + 2 <= 7){
        if(j - 1 >= 0){
            addMenace(b, color, i, j, i + 2, j - 1);
            if(b->pieces[(i + 2) * 8 + (j - 1)][COLOR] != color){
                addCoup(b, color, i, j, i + 2, j - 1);
            }
        }
        if(j + 1 <= 7){
            addMenace(b, color, i, j, i + 2, j + 1);
            if(b->pieces[(i + 2) * 8 + (j + 1)][COLOR] != color){
                addCoup(b, color, i, j, i + 2, j + 1);
            }
        }
    }
    if(i + 1 <= 7){
        if(j - 2 >= 0){
            addMenace(b, color, i, j, i + 1, j - 2);
            if(b->pieces[(i + 1) * 8 + (j - 2)][COLOR] != color){
                addCoup(b, color, i, j, i + 1, j - 2);
            }
        }
        if(j + 2 <= 7){
            addMenace(b, color, i, j, i + 1, j + 2);
            if(b->pieces[(i + 1) * 8 + (j + 2)][COLOR] != color){
                addCoup(b, color, i, j, i + 1, j + 2);
            }
        }
    }
}

void Arbitre::menaceBishop(Board* b, int i, int j, int color){
    b->cibles[i * 8 + j][color - 1].clear();
    int k = 1;
    while(i - k >= 0 && j - k >= 0 && b->pieces[(i - k) * 8 + (j - k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i - k, j - k);
        addCoup(b, color, i, j, i - k, j - k);
        k++;
    }
    if(i - k >= 0 && j - k >= 0){
        if(b->pieces[(i - k) * 8 + (j - k)][COLOR] != color){
            addCoup(b, color, i, j, i - k, j - k);
        }
        addMenace(b, color, i, j, i - k, j - k);
    }
    k = 1;
    while(i + k <= 7 && j + k <= 7 && b->pieces[(i + k) * 8 + (j + k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i + k, j + k);
        addCoup(b, color, i, j, i + k, j + k);
        k++;
    }
    if(i + k <= 7 && j + k <= 7){
        if(b->pieces[(i + k) * 8 + (j + k)][COLOR] != color){
            addCoup(b, color, i, j, i + k, j + k);
        }
        addMenace(b, color, i, j, i + k, j + k);
    }
    k = 1;
    while(i + k <= 7 && j - k >= 0 && b->pieces[(i + k) * 8 + (j - k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i + k, j - k);
        addCoup(b, color, i, j, i + k, j - k);
        k++;
    }
    if(i + k <= 7 && j - k >= 0){
        if(b->pieces[(i + k) * 8 + (j - k)][COLOR] != color){
            addCoup(b, color, i, j, i + k, j - k);
        }
        addMenace(b, color, i, j, i + k, j - k);
    }
    k = 1;
    while(i - k >= 0 && j + k <= 7 && b->pieces[(i - k) * 8 + (j + k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i - k, j + k);
        addCoup(b, color, i, j, i - k, j + k);
        k++;
    }
    if(i - k >= 0 && j + k <= 7){
        if(b->pieces[(i - k) * 8 + (j + k)][COLOR] != color){
            addCoup(b, color, i, j, i - k, j + k);
        }
        addMenace(b, color, i, j, i - k, j + k);
    }
}

void Arbitre::menaceQueen(Board* b, int i, int j, int color){
    b->cibles[i * 8 + j][color - 1].clear();
    int k = 1;
    while(i - k >= 0 && b->pieces[(i - k) * 8 + j][TYPE] == EMPTY){
        addMenace(b, color, i, j, i - k, j);
        addCoup(b, color, i, j, i - k, j);
        k++;
    }
    if(i - k >= 0){
        if(b->pieces[(i - k) * 8 + j][COLOR] != color){
            addCoup(b, color, i, j, i - k, j);
        }
        addMenace(b, color, i, j, i - k, j);
    }
    k = 1;
    while(i + k <= 7 && b->pieces[(i + k) * 8 + j][TYPE] == EMPTY){
        addMenace(b, color, i, j, i + k, j);
        addCoup(b, color, i, j, i + k, j);
        k++;
    }
    if(i + k <= 7){
        if(b->pieces[(i + k) * 8 + j][COLOR] != color){
            addCoup(b, color, i, j, i + k, j);
        }
        addMenace(b, color, i, j, i + k, j);
    }
    k = 1;
    while(j - k >= 0 && b->pieces[i * 8 + (j - k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i, j - k);
        addCoup(b, color, i, j, i, j - k);
        k++;
    }
    if(j - k >= 0){
        if(b->pieces[i * 8 + (j - k)][COLOR] != color){
            addCoup(b, color, i, j, i, j - k);
        }
        addMenace(b, color, i, j, i, j - k);
    }
    k = 1;
    while(j + k <= 7 && b->pieces[i * 8 + (j + k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i, j + k);
        addCoup(b, color, i, j, i, j + k);
        k++;
    }
    if(j + k <= 7){
        if(b->pieces[i * 8 + (j + k)][COLOR] != color){
            addCoup(b, color, i, j, i, j + k);
        }
        addMenace(b, color, i, j, i, j + k);
    }
    k = 1;
    while(i - k >= 0 && j - k >= 0 && b->pieces[(i - k) * 8 + (j - k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i - k, j - k);
        addCoup(b, color, i, j, i - k, j - k);
        k++;
    }
    if(i - k >= 0 && j - k >= 0){
        if(b->pieces[(i - k) * 8 + (j - k)][COLOR] != color){
            addCoup(b, color, i, j, i - k, j - k);
        }
        addMenace(b, color, i, j, i - k, j - k);
    }
    k = 1;
    while(i + k <= 7 && j + k <= 7 && b->pieces[(i + k) * 8 + (j + k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i + k, j + k);
        addCoup(b, color, i, j, i + k, j + k);
        k++;
    }
    if(i + k <= 7 && j + k <= 7){
        if(b->pieces[(i + k) * 8 + (j + k)][COLOR] != color){
            addCoup(b, color, i, j, i + k, j + k);
        }
        addMenace(b, color, i, j, i + k, j + k);
    }
    k = 1;
    while(i + k <= 7 && j - k >= 0 && b->pieces[(i + k) * 8 + (j - k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i + k, j - k);
        addCoup(b, color, i, j, i + k, j - k);
        k++;
    }
    if(i + k <= 7 && j - k >= 0){
        if(b->pieces[(i + k) * 8 + (j - k)][COLOR] != color){
            addCoup(b, color, i, j, i + k, j - k);
        }
        addMenace(b, color, i, j, i + k, j - k);
    }
    k = 1;
    while(i - k >= 0 && j + k <= 7 && b->pieces[(i - k) * 8 + (j + k)][TYPE] == EMPTY){
        addMenace(b, color, i, j, i - k, j + k);
        addCoup(b, color, i, j, i - k, j + k);
        k++;
    }
    if(i - k >= 0 && j + k <= 7){
        if(b->pieces[(i - k) * 8 + (j + k)][COLOR] != color){
            addCoup(b, color, i, j, i - k, j + k);
        }
        addMenace(b, color, i, j, i - k, j + k);
    }
}

void Arbitre::menaceKing(Board* b, int i, int j, int color){
    b->cibles[i * 8 + j][color - 1].clear();
    if(i - 1 >= 0){
        addMenace(b, color, i, j, i - 1, j);
        if(b->menaces[(i - 1) * 8 + j][b->nextPlayer - 1].empty() && b->pieces[(i - 1) * 8 + j][COLOR] != color){
            addCoup(b, color, i, j, i - 1, j);
        }
    }
    if(i + 1 <= 7){
        addMenace(b, color, i, j, i + 1, j);
        if(b->menaces[(i + 1) * 8 + j][b->nextPlayer - 1].empty() && b->pieces[(i + 1) * 8 + j][COLOR] != color){
            addCoup(b, color, i, j, i + 1, j);
        }
    }
    if(j - 1 >= 0){
        addMenace(b, color, i, j, i, j - 1);
        if(b->menaces[i * 8 + (j - 1)][b->nextPlayer - 1].empty() && b->pieces[i * 8 + (j - 1)][COLOR] != color){
            addCoup(b, color, i, j, i, j - 1);
        }
    }
    if(j + 1 <= 7){
        addMenace(b, color, i, j, i, j + 1);
        if(b->menaces[i * 8 + (j + 1)][b->nextPlayer - 1].empty() && b->pieces[i * 8 + (j + 1)][COLOR] != color){
            addCoup(b, color, i, j, i, j + 1);
        }
    }
    if(i - 1 >= 0 && j - 1 >= 0){
        addMenace(b, color, i, j, i - 1, j - 1);
        if(b->menaces[(i - 1) * 8 + (j - 1)][b->nextPlayer - 1].empty() && b->pieces[(i - 1) * 8 + (j - 1)][COLOR] != color){
            addCoup(b, color, i, j, i - 1, j - 1);
        }
    }
    if(i + 1 <= 7 && j + 1 <= 7){
        addMenace(b, color, i, j, i + 1, j + 1);
        if(b->menaces[(i + 1) * 8 + (j + 1)][b->nextPlayer - 1].empty() && b->pieces[(i + 1) * 8 + (j + 1)][COLOR] != color){
            addCoup(b, color, i, j, i + 1, j + 1);
        }
    }
    if(i + 1 <= 7 && j - 1 >= 0){
        addMenace(b, color, i, j, i + 1, j - 1);
        if(b->menaces[(i + 1) * 8 + (j - 1)][b->nextPlayer - 1].empty() && b->pieces[(i + 1) * 8 + (j - 1)][COLOR] != color){
            addCoup(b, color, i, j, i + 1, j - 1);
        }
    }
    if(i - 1 >= 0 && j + 1 <= 7){
        addMenace(b, color, i, j, i - 1, j + 1);
        if(b->menaces[(i - 1) * 8 + (j + 1)][b->nextPlayer - 1].empty() && b->pieces[(i - 1) * 8 + (j + 1)][COLOR] != color){
            addCoup(b, color, i, j, i - 1, j + 1);
        }
    }

    // Vérifie si les roques (gr = great roque, sr = small roque) sont valables
    if(b->roque[color - 1][0] &&
    b->pieces[(i - 1) * 8 + j] == EMPTY &&
    b->pieces[(i - 2) * 8 + j] == EMPTY &&
    b->pieces[(i - 3) * 8 + j] == EMPTY &&
    b->menaces[i * 8 + j][b->nextPlayer - 1].empty() &&
    b->menaces[(i - 1) * 8 + j][b->nextPlayer - 1].empty() &&
    b->menaces[(i - 2) * 8 + j][b->nextPlayer - 1].empty()){
        addCoup(b, i, j, i - 2, j, CASTLING_LONG);
    }
    if(b->roque[color - 1][0] &&
    b->pieces[(i + 1) * 8 + j] == EMPTY &&
    b->pieces[(i + 2) * 8 + j] == EMPTY &&
    b->menaces[i * 8 + j][b->nextPlayer - 1].empty() &&
    b->menaces[(i + 1) * 8 + j][b->nextPlayer - 1].empty() &&
    b->menaces[(i + 2) * 8 + j][b->nextPlayer - 1].empty()){
        addCoup(b, i, j, i + 2, j, CASTLING_SMALL);
    }
}

void Arbitre::addCoup(Board* b, int color, int x1, int y1, int x2, int y2, int opt){
    string s = "    ";
    s[0] = (char)(y1 + 'a');
    s[1] = (char)(x1 + '1');
    s[2] = (char)(y2 + 'a');
    s[3] = (char)(x2 + '1');
    switch(opt){
        case NONE:
            b->coups[color - 1].push_back(s);
            break;
        case CASTLING_LONG:
            b->coups[color - 1].push_back("gc");
            break;
        case CASTLING_SMALL:
            b->coups[color - 1].push_back("sc");
            break;
        case PROMOTION:
            b->coups[color - 1].push_back("p" + s);
            break;
    }
}

void Arbitre::addMenace(Board* b, int color, int x1, int y1, int x2, int y2){
    string s1 = "  ";
    s1[0] = (char)(y1 + 'a');
    s1[1] = (char)(x1 + '1');
    string s2 = "  ";
    s2[0] = (char)(y2 + 'a');
    s2[1] = (char)(x2 + '1');
    b->menaces[x2 * 8 + y2][color - 1].push_back(s1);
    b->cibles[x1 * 8 + y1][color - 1].push_back(s2);
}

int Arbitre::isCheck(Board* b, int color){
    int x = b->kings[color - 1][1] - '1';
    int y = b->kings[color - 1][0] - 'a';
    int oppCol = WHITE;
    if(color == WHITE){
        oppCol = BLACK;
    }
    if(b->menaces[x * 8 + y][oppCol - 1].empty()){
        return NOT_CHECK;
    }else{
        return CHECK;
    }
}

int Arbitre::isCheckmate(Board* b, int color){
    if(!isCheck(b, color)){
        return NOT_CHECK;
    }else{
        int x = b->kings[color - 1][1] - '1';
        int y = b->kings[color - 1][0] - 'a';
        if(!b->cibles[x * 8 + y][color - 1].empty()){
            return NOT_CHECK;
        }else{
            reviseCoups(b, color);
            if(!b->coups[color - 1].empty()){
                return NOT_CHECK;
            }else{
                return CHECK;
            }
        }
    }
}

void Arbitre::reviseCoups(Board* b, int color){
    for(int i = (int) b->coups[color - 1].size() - 1; i >= 0; i--){
        Board b0 = *b;
        move(&b0, b->coups[color - 1][i]);
        if(isCheck(&b0, color)){
            b->coups[color - 1].erase(b->coups[color - 1].begin() + i);
        }
    }
}
