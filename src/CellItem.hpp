#ifndef CELLITEM
#define CELLITEM

#include <QtGui>
#include <QGraphicsItem>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsRectItem>

class GameOfLife;


class CellItem : public QGraphicsRectItem{
protected:
  GameOfLife *game_of_life;
  int row;
  int col;
  bool is_alive;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
  CellItem(int x, int y, int len, GameOfLife *game_of_life, int row, int col);
  bool is_cell();
  void set_alive();
  void set_empty();
};

#endif
