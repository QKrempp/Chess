#ifndef __BOARDWIDGET_H__
#define __BOARDWIDGET_H__

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QImage>
#include <QString>
#include <QRect>

#include "Board.h"
#include "Arbitre.h"
#include "Player.h"

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    BoardWidget(QWidget* parent);

protected:
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;

public slots:

private:
    Human p1;
    AlphaBeta p2;
    Arbitre m_a;
    Board m_b;

    byte cmap[64];
    byte selected = FALSE;
    byte played = FALSE;
    byte s_i;
    byte s_j;
    QBrush* m_dTile;
    QBrush* m_lTile;
    QBrush* m_highlightDTile;
    QBrush* m_highlightLTile;
    QBrush* m_movedDTile;
    QBrush* m_movedLTile;

    void resetCmap();
    void setHighlight(uint64_t c);
    void setMoved(b_move m);
    void play_h(QMouseEvent * event);
    void play_a(QMouseEvent * event);
};

#endif
