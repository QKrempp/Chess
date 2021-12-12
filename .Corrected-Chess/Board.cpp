#include "Board.hpp"

Board::Board(){
    int type = 0;
    int color = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            type = EMPTY;
            color = EMPTY;
            if(i == 1 || i == 0){
                color = WHITE;
            }
            if(i == 6 || i == 7){
                color = BLACK;
            }
            if(i == 1 || i == 6){
                type = PAWN;
            }
            if(i == 0 || i == 7){
                if(j == 0 || j == 7){
                    type = ROOK;
                }
                if(j == 1 || j == 6){
                    type = KNIGHT;
                }
                if(j == 2 || j == 5){
                    type = BISHOP;
                }
                if(j == 3){
                    type = QUEEN;
                }
                if(j == 4){
                    type = KING;
                }
            }
            pieces[i * 8 + j][0] = color;
            pieces[i * 8 + j][1] = type;
        }
    }
}

ostream& operator<<(ostream& os, const Board& b){
    cout << "..A  B  C  D  E  F  G  H..\n";
    cout << " +--+--+--+--+--+--+--+--+\n";
    for(int i = 0; i < 8; i++){
        cout << i + 1;
        for(int j = 0; j < 8; j++){
            cout << "|";
            switch(b.pieces[i * 8 + j][0]){
                case WHITE:
                    cout << "W";
                    break;
                case BLACK:
                    cout << "B";
                    break;
                case EMPTY:
                    cout << " ";
            }
            switch(b.pieces[i * 8 + j][1]){
                case PAWN:
                    cout << "P";
                    break;
                case ROOK:
                    cout << "R";
                    break;
                case KNIGHT:
                    cout << "N";
                    break;
                case BISHOP:
                    cout << "B";
                    break;
                case QUEEN:
                    cout << "Q";
                    break;
                case KING:
                    cout << "K";
                    break;
                case EMPTY:
                    if(!b.menaces[i * 8 + j][BLACK - 1].empty()){
                        cout << "x";
                    }else{
                        cout << " ";
                    }
                    break;
            }
        }
        cout << "|\n";
        cout << " +--+--+--+--+--+--+--+--+\n";
    }
    cout << endl << "Hash:" << b.hash << endl;
    return os;
}

vector<string> Board::getMenaces(int caseToCheck, int color){
    return menaces[caseToCheck][color - 1];
}

vector<string> Board::getCoups(int color){
    return coups[color - 1];
}

int Board::getPlayer(){
    return currentPlayer;
}

int Board::getHash(){
    return hash;
}

int Board::getPieces(int caseToCheck, int info)
{
    return pieces[caseToCheck][info];
}

int Board::getWinner(){
    return winner;
}
