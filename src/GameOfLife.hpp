#ifndef GAMEOFLIFE
#define GAMEOFLIFE

#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QtGui>
#include <QMainWindow>
#include <QGridLayout>

class GameOfLife : public QMainWindow{
Q_OBJECT;

private:
  QGridLayout *main_layout;

public:
  GameOfLife();
};

#endif
