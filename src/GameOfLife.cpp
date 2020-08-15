#include "GameOfLife.hpp"


GameOfLife::GameOfLife() : QMainWindow(){
  // Window open with its maximum size
  this->setWindowState(Qt::WindowMaximized);

  //////////////////////////////////////////////////////////////////////////////

  // Set our main layout which will contain everything
  this->main_layout = new QGridLayout();
  this->setLayout(main_layout);
}
