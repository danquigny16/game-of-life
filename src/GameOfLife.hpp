#ifndef GAMEOFLIFE
#define GAMEOFLIFE

#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QMainWindow>
#include <QGridLayout>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "CellGrid.hpp"


class GameOfLife : public QMainWindow{
Q_OBJECT;

private:
  int grid_height;
  int grid_width;
  QColor cell_color;
  QColor empty_color;
  QWidget *sdi_widget;
  QGridLayout *main_layout;
  QGraphicsScene *scene;
  QGraphicsView *vue;
  CellGrid *cell_grid;

public:
  GameOfLife();
  ~GameOfLife();
};

#endif
