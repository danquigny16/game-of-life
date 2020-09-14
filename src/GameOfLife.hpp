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
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QListView>
#include <QWidget>
#include <QDir>
#include <QFileSystemModel>
#include <QModelIndex>

#include "RleHandler.hpp"
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

  QWidget *new_window;
  QFileSystemModel *model_rle;
  QListView *view_rle;

  QWidget *sdi_widget;
  QGridLayout *main_layout;

  QGraphicsScene *scene;
  QGraphicsView *view;

  RleHandler *rle_handler;

  CellGrid *cell_grid;
  CellItem **cell_items;

protected:
  void set_window_parameters();
  void main_variables_initialization();
  void GUI_view();
  void GUI_size();
  void GUI_color();
  void GUI_launch();
  void GUI_open_rle();
  void GUI_menu();

public:
  GameOfLife();
  ~GameOfLife();

  int width() const;
  int height() const;

  QColor cell_color() const;
  QColor empty_color() const;
  QColor which_color(QString color_string) const;
  QString which_color(QColor q_color) const;
  void update_cell_color();

  bool is_cell(int row, int col) const;

  void set_item_alive(int row, int col);
  void set_item_empty(int row, int col);

  void update_cell_grid();

public slots:
  void set_size();

  void set_cell_color();
  void set_empty_color();

  void clear_grid();

  void open_rle_selected_file(const QModelIndex &index);
  void open_rle();

  void start_game_of_life();
};

#endif
