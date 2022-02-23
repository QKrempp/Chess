#include "MaFenetre.h"

#include <iostream>


#define TILE_SIZE 70

MaFenetre::MaFenetre() : QWidget()
{
    m_board = new BoardWidget(this);

//     while(b.getWinner() == RUNNING)
//     {
//         a.play(&b);
//         b.nextTurn();
//     }

}
