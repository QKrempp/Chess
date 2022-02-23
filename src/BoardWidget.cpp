#include "BoardWidget.h"

#include <iostream>
#include <QMouseEvent>

#define TILE_SIZE 70

#define NORMAL 0
#define HIGHLIGHTED 1
#define MOVED 2

#define REVERSE(i) ((byte) ((i) ^ 0x38))

BoardWidget::BoardWidget(QWidget* parent) : QWidget(parent), p1(&m_b, WHITE), p2(&m_b, BLACK), m_a(&p1, &p2)
{
    setFixedSize(8 * TILE_SIZE, 8 * TILE_SIZE);

    m_dTile = new QBrush(QColor::fromRgb(209, 139, 71));
    m_lTile = new QBrush(QColor::fromRgb(255, 206, 158));
    m_highlightDTile = new QBrush(QColor::fromRgb(71, 171, 209));
    m_highlightLTile = new QBrush(QColor::fromRgb(158, 211, 255));
    m_movedDTile = new QBrush(QColor::fromRgb(209, 75, 71));
    m_movedLTile = new QBrush(QColor::fromRgb(255, 158, 168));

    m_b = Board();
    m_b.initDefault();
    m_a.initMenaces(&m_b);
    m_a.initHash(&m_b);

    resetCmap();
}

void BoardWidget::resetCmap()
{
    for(byte i = 0; i < 64; i++)
    {
        cmap[i] = NORMAL;
    }
}

void BoardWidget::setHighlight(uint64_t c)
{
    resetCmap();
    std::cout << c << std::endl;
    for(byte i = 0; i < 64; i++)
    {
        if(IS_TARGET(c, i))
        {
            cmap[REVERSE(i)] = HIGHLIGHTED;
        }
    }
}

void BoardWidget::setMoved(b_move m)
{
    resetCmap();
    cmap[REVERSE(MOVE_START(m))] = MOVED;
    cmap[REVERSE(MOVE_STOP(m))] = MOVED;
}

void BoardWidget::mousePressEvent(QMouseEvent* event)
{
    play_h(event);
}

void BoardWidget::mouseReleaseEvent(QMouseEvent* event)
{
    play_a(event);
}

void BoardWidget::play_h(QMouseEvent* event)
{
    byte i = (event->y() / TILE_SIZE) ^ 7;
    byte j = event->x() / TILE_SIZE;
    if(event->button() == Qt::LeftButton)
    {
        byte c = m_b.getPiece(INT_TO_BYTE(i, j));
        if(c && PIECE_COLOR(c) == m_b.getPlayer())
        {
            uint64_t mv = m_b.getAltMoves(i * 8 + j, m_b.getPlayer());
            for(byte k = 0; k < 64; k++)
            {
                if(IS_TARGET(mv, k) && !m_a.isMoveValid(&m_b, BYTE_TO_MOVE(i * 8 + j, k)))
                {
                    mv ^= (UINT64_C(1) << j);
                }
            }
            setHighlight(mv);
            s_i = i;
            s_j = j;
            selected = TRUE;
        }
        else
        {
            if(selected)
            {
                std::cout << "Evaluation du mouvement" << std::endl;
                std::cout << (int) INT_TO_MOVE(s_i, s_j, i, j) << std::endl;

                if(IS_TARGET(m_b.getAltMoves(INT_TO_BYTE(s_i, s_j), m_b.getPlayer()), INT_TO_BYTE(i, j)) && m_a.isMoveValid(&m_b, INT_TO_MOVE(s_i, s_j, i, j)))
                {
                    std::cout << "Mouvement valide" << std::endl;
                    m_a.playRequest(&m_b, INT_TO_MOVE(s_i, s_j, i, j));
                    setMoved(INT_TO_MOVE(s_i, s_j, i, j));
                    played = TRUE;
                }
                else
                {
                    std::cout << "Mouvement invalide" << std::endl;
                    resetCmap();
                }
                selected = FALSE;
            }
            else
            {
                resetCmap();
            }
        }
        update();
    }
}

void BoardWidget::play_a(QMouseEvent* event)
{
    if(played)
    {
        setMoved(p2.play(&m_b, &m_a));
        played = FALSE;
        update();
    }
}


void BoardWidget::paintEvent(QPaintEvent* event)
{
//     std::cout << "Repaint!" << std::endl;
    QPainter m_painter(this);

    QImage plt(QString("../img/Chess_plt64.png"));
    QImage pdt(QString("../img/Chess_pdt64.png"));
    QImage rlt(QString("../img/Chess_rlt64.png"));
    QImage rdt(QString("../img/Chess_rdt64.png"));
    QImage nlt(QString("../img/Chess_nlt64.png"));
    QImage ndt(QString("../img/Chess_ndt64.png"));
    QImage blt(QString("../img/Chess_blt64.png"));
    QImage bdt(QString("../img/Chess_bdt64.png"));
    QImage qlt(QString("../img/Chess_qlt64.png"));
    QImage qdt(QString("../img/Chess_qdt64.png"));
    QImage klt(QString("../img/Chess_klt64.png"));
    QImage kdt(QString("../img/Chess_kdt64.png"));

    m_painter.setPen(Qt::black);

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if((i + j) % 2 == 1)
            {
                switch(cmap[i * 8 + j])
                {
                    case NORMAL:
                        m_painter.setBrush(*m_dTile);
                        break;
                    case HIGHLIGHTED:
                        m_painter.setBrush(*m_highlightDTile);
                        break;
                    case MOVED:
                        m_painter.setBrush(*m_movedDTile);
                        break;
                }
            }
            else
            {
                switch(cmap[i * 8 + j])
                {
                    case NORMAL:
                        m_painter.setBrush(*m_lTile);
                        break;
                    case HIGHLIGHTED:
                        m_painter.setBrush(*m_highlightLTile);
                        break;
                    case MOVED:
                        m_painter.setBrush(*m_movedLTile);
                        break;
                }
            }
            m_painter.drawRect(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            QPoint* currentTile = new QPoint(j * TILE_SIZE + 3, i * TILE_SIZE + 3);
            switch(m_b.getPiece(INT_TO_BYTE((7 - i), j)))
            {
                case PIECE(WHITE, PAWN):
//                     std::cout << "Pion Blanc" << std::endl;
                    m_painter.drawImage(*currentTile, plt);
                    break;
                case PIECE(BLACK, PAWN):
//                     std::cout << "Pion Noir" << std::endl;
                    m_painter.drawImage(*currentTile, pdt);
                    break;
                case PIECE(WHITE, ROOK):
//                     std::cout << "Tour Blanc" << std::endl;
                    m_painter.drawImage(*currentTile, rlt);
                    break;
                case PIECE(BLACK, ROOK):
//                     std::cout << "Tour Noir" << std::endl;
                    m_painter.drawImage(*currentTile, rdt);
                    break;
                case PIECE(WHITE, KNIGHT):
//                     std::cout << "Cavalier Blanc" << std::endl;
                    m_painter.drawImage(*currentTile, nlt);
                    break;
                case PIECE(BLACK, KNIGHT):
//                     std::cout << "Cavalier Noir" << std::endl;
                    m_painter.drawImage(*currentTile, ndt);
                    break;
                case PIECE(WHITE, BISHOP):
//                     std::cout << "Fou Blanc" << std::endl;
                    m_painter.drawImage(*currentTile, blt);
                    break;
                case PIECE(BLACK, BISHOP):
//                     std::cout << "Fou Noir" << std::endl;
                    m_painter.drawImage(*currentTile, bdt);
                    break;
                case PIECE(WHITE, QUEEN):
//                     std::cout << "Dame Blanc" << std::endl;
                    m_painter.drawImage(*currentTile, qlt);
                    break;
                case PIECE(BLACK, QUEEN):
//                     std::cout << "Dame Noir" << std::endl;
                    m_painter.drawImage(*currentTile, qdt);
                    break;
                case PIECE(WHITE, KING):
//                     std::cout << "Roi Blanc" << std::endl;
                    m_painter.drawImage(*currentTile, klt);
                    break;
                case PIECE(BLACK, KING):
//                     std::cout << "Roi Noir" << std::endl;
                    m_painter.drawImage(*currentTile, kdt);
            }
        }
    }
}
