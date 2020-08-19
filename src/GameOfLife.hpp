#ifndef GAMEOFLIFE
#define GAMEOFLIFE

#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QMainWindow>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QPixmap>
#include <QIcon>

#include "CellGrid.hpp"


class GameOfLife : public QMainWindow{
Q_OBJECT;

protected:
  int grid_width;
  int grid_height;
  QColor cell_color;
  QColor empty_color;
  QWidget *sdi_widget;
  QGridLayout *main_layout;
  QGraphicsScene *scene;
  QGraphicsView *vue;
  CellGrid *cell_grid;
  QGraphicsRectItem **cell_items;

protected:
  void set_window_parameters();
  void main_variables_initialization();
  void GUI_vue();
  void GUI_size();
  void GUI_cell_color();
  void GUI_launch();

public:
  GameOfLife();
  ~GameOfLife();

public slots:
  void set_size(int width, int height);
};

#endif
