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
#include <QHBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QPixmap>
#include <QIcon>
#include <QColor>
#include <QLabel>

#include "CellItem.hpp"
#include "CellGrid.hpp"


class GameOfLife : public QMainWindow{
Q_OBJECT;

protected:
  int grid_width;
  int grid_height;
  QSpinBox *grid_width_box;
  QSpinBox *grid_height_box;
  QPushButton *button_size;

  QColor cell_item_color;
  QColor empty_item_color;
  QComboBox *cell_color_box;
  QComboBox *empty_color_box;

  QSpinBox *nb_iter_box;
  QLabel *nb_iter_remain;
  QPushButton *button_clear;
  QPushButton *button_launch;

  QWidget *sdi_widget;
  QGridLayout *main_layout;

  QGraphicsScene *scene;
  QGraphicsView *view;

  CellGrid *cell_grid;
  CellItem **cell_items;

protected:
  void set_window_parameters();
  void main_variables_initialization();
  void GUI_view();
  void GUI_size();
  void GUI_color();
  void GUI_launch();

public:
  GameOfLife();
  ~GameOfLife();

  int width();
  int height();

  QColor cell_color();
  QColor empty_color();
  QColor which_color(QString color_string);
  QString which_color(QColor q_color);
  void update_cell_color();

  bool is_cell(int row, int col);

  void update_cell_grid();

public slots:
  void set_size();

  void set_cell_color();
  void set_empty_color();

  void clear_grid();

  void start_game_of_life();
};

#endif
