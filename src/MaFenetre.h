#ifndef MAFENETRE_H
#define MAFENETRE_H

#include "BoardWidget.h"

#include "Board.h"
#include "Arbitre.h"
#include "Player.h"

class MaFenetre : public QWidget
{
    public:
        MaFenetre();

    private:
        BoardWidget* m_board;
};

#endif // MAFENETRE_H
