#ifndef __ARBITRE_HPP__
#define __ARBITRE_HPP__

#define NOT_CHECK 0
#define CHECK 1

#define NONE            0
#define CASTLING_LONG   1
#define CASTLING_SMALL  2
#define PROMOTION       3

#include "Board.hpp"

#include <time.h>

class Board;

class HashList{
    public:
        unsigned long int hash;
        char nb;
        HashList* next;
        HashList(){
            hash = 0;
            nb = 0;
            next = NULL;
        }
        void addNext(){
            next = new HashList;
        }
        void addVisite(){
            nb++;
        }
        void printHash(){
            if(next != NULL){
                next->printHash();
            }else{
                cout << endl;
            }
        }
};

class Arbitre{

    private:
        unsigned long int hashtable[64][12];
        void move(Board* b, string s);
        HashList* h;

    public:

        Arbitre();
        ~Arbitre(){};

        // Permet de jouer un coup sous la forme "a1b2" sur le plateau. Suppose que le coup est correct
        void requestMove(Board* b, int color, string s, bool mustAddHash = true);

        // Calcul les menaces appliquées à chaque cases, et les coups jouables pour chaque pièce
        void menacePawn(Board* b, int i, int j, int color);
        void menaceRook(Board* b, int i, int j, int color);
        void menaceKnight(Board* b, int i, int j, int color);
        void menaceBishop(Board* b, int i, int j, int color);
        void menaceQueen(Board* b, int i, int j, int color);
        void menaceKing(Board* b, int i, int j, int color);

        // Met à jour les menaces concernées par le mouvement "s"
        void updateMenaces(Board* b, int caseStart, int caseStop);
        void eraseMenaces(Board* b, int caseStart);

        // Met à jour toutes les menaces sans tenir compte de ce qui existait déjà
        void updateMenacesFull(Board* b);

        // Affiche les menaces et les coups jouables
        void printMenaces(Board* b);
        void printCoups(Board* b);

        // Ajoute une menace ou un coup jouable
        void addCoup(Board* b, int color, int x1, int y1, int x2, int y2, int opt = NONE);
        void addMenace(Board* b, int color, int x1, int y1, int x2, int y2);

        // Gestion des échecs et correction des coups si nécessaire
        int isCheck(Board* b, int color);
        int isCheckmate(Board* b, int color);
        void reviseCoups(Board* b, int color);
        void updateEndgameConditions(Board* b);
        int isGameOver(Board* b);

        // Gestion du hash des plateaux pour s'assurer que l'on ne retourne pas trois fois à la même situation
        void initHash(Board* b);
        void updateHash(Board* b, string s);
        void addHash(Board* b);
        void clearHash(Board* b, string s);
};

#endif
