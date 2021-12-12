#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include<iostream>
#include<vector>
using namespace std;

#include "Arbitre.hpp"
#include "Player.hpp"

#define WHITE   1
#define BLACK   2

#define PAWN    1
#define ROOK    2
#define KNIGHT  3
#define BISHOP  4
#define QUEEN   5
#define KING    6

#define EMPTY   0

#define DRAW    3

#define PAWN_WHITE 3
#define PAWN_BLACK 4

#define MAXIMIZE 1
#define MINIMIZE 0

#define COLOR   0
#define TYPE    1

class Board{

    private:
        int pieces[64][2];      // Grille à proprement parler: 64 cases contenant la couleur de la pièce et le type de celle-ci
        int roque[2][2] = {{1,1},{1,1}};// Suivi des roques
        string kings[2] = {"e1", "e8"}; // Détermine la position des rois
        int tour = 0;                   // Compteur de tours
        int noPrise = 0;                // Compteur de coups sans prises
        int currentPlayer = WHITE;      // Suivi du joueur qui doit jouer
        int nextPlayer = BLACK;         // Suivi de l'adversaire du joueur en cours
        vector<string> menaces[64][4];  // Suivi des cases menacées
        vector<string> cibles[64][4];   // Liste des cases menacées par chaque pièce
        vector<string> coups[2];        // Liste des coups jouables pour chaque couleur
        int winner = EMPTY;
        unsigned long int hash = 0;

    public:

        // Constructeur
        Board();

        // Destructeur
        ~Board(){};

        // Arbitre comme interface
        friend class Arbitre;

        // Affichage du plateau
        friend ostream& operator<<(ostream& os, const Board& b);

        vector<string> getMenaces(int caseToCheck, int color);
        vector<string> getCoups(int color);

        int getPlayer();
        int getHash();
        int getPieces(int caseToCheck, int info);

        int getWinner();

};

ostream& operator<<(ostream& os, const Board& b);

#endif
