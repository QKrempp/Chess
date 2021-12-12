#include "Player.hpp"

Player::Player(int color0){
    color = color0;
}

int Player::getColor(){
    return color;
}

void Human::play(Board* b, Arbitre* a){

    cout << *b << endl;

    vector<string> coups = b->getCoups(color);
    int c = 0;
    for(int i = 0; i < (int) coups.size(); i++){
        cout << i << ": " << coups[i] << "     ";
        if(i % 3 == 2){
            cout << endl;
        }
    }
    cout << endl;
    int valide = 0;
    while(!valide){
        cout << "Que voulez vous jouer? (0 - " << (int) coups.size() - 1 << "): " << endl;
        cin >> c;
        if (c >= 0 && c < (int) coups.size()){
            valide = 1;
        }else{
            cout << "Réponse invalide" << endl;
        }
    }
    a->requestMove(b, color, coups[c]);
}

void AlphaBeta::play(Board* b, Arbitre* a){
    string meilleurCoup;
    int meilleurScore = -10000;
    int alpha = -10000;
    int beta = 10000;
    vector<string> coupsToTest = b->getCoups(color);
    for(int i = 0; i < (int) coupsToTest.size(); i++){
        Board b0 = *b;
        a->requestMove(&b0, color, coupsToTest[i], false);
        int score = digDown(b0, a, d - 1, &alpha, &beta);
        if(score > meilleurScore){
            meilleurScore = score;
            meilleurCoup = coupsToTest[i];
        }
    }
    cout << color << " joue " << meilleurCoup << " pour " << meilleurScore << endl;
    a->requestMove(b, color, meilleurCoup);
}


int AlphaBeta::digDown(Board b, Arbitre* a, int depth, int* alpha, int* beta){
    int v = 0;
    explored[depth]++;
//     cout << b << endl;
    if(depth == 0 || a->isGameOver(&b)){
        return evaluateBoard(b);
    }else if(b.getPlayer() != color){
        v = 100000;
        vector<string> fils = b.getCoups(b.getPlayer());
        for(int i = 0; i < (int) fils.size(); i++){
            Board b0 = b;
//             for(int j = 0; j < DEPTH - depth - 1; j++){
//                 cout << "   ";
//             }
//             cout << fils[i] << " (" << i << ")" << endl;
            a->requestMove(&b0, b0.getPlayer(), fils[i], false);
            int ab = digDown(b0, a, depth - 1, alpha, beta);
            if(ab < v){
                v = ab;
            }
            if(*alpha >= v){
                // Coupure alpha
                return v;
            }
            if(v < *beta){
                *beta = v;
            }
        }
    }else{
        v = -10000;
        vector<string> fils = b.getCoups(b.getPlayer());
        for(int i = 0; i < (int) fils.size(); i++){
            Board b0 = b;
//             for(int j = 0; j < DEPTH - depth - 1; j++){
//                 cout << "   ";
//             }
//             cout << fils[i] << " (" << i << ")" << endl;
            a->requestMove(&b0, b0.getPlayer(), fils[i], false);
            int ab = digDown(b0, a, depth - 1, alpha, beta);
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
    return v;
}

int AlphaBeta::evaluateBoard(Board b){
    int score = 0;
    int mult = 0;
    int points = 0;
    int win = 0;
    for(int i = 0; i < 64; i++){
        if(b.getPieces(i, COLOR) == color){
            mult = 1;
        }else{
            mult = -1;
        }
        switch(b.getPieces(i, TYPE)){
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
        if(b.getWinner() == color){
            win = 10000;
        }else if(win == DRAW){
            win = -500;
        }else if(win == EMPTY){
            win = 0;
        }else{
            win = -10000;
        }
        score += mult * points + (int) b.getMenaces(i, color).size() + win;
    }
    return score;
}
