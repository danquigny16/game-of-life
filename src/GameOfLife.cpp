#include "GameOfLife.hpp"


////////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

GameOfLife::GameOfLife() : QMainWindow(), grid_height(1), grid_width(1), cell_color(Qt::black), empty_color(Qt::white){
  //////////////////////////////////////////////////////////////////////////////
  // Set windows parameters

  // Window open with its maximum size
  this->setWindowState(Qt::WindowMaximized);

  // Set a title to our window
  this->setWindowTitle("Game of life");

  //////////////////////////////////////////////////////////////////////////////
  // Initialization of our variables

  // Create a SDI (Single Document Interface) widget to contain our main layout
  sdi_widget = new QWidget();
  this->setCentralWidget(sdi_widget);

  // Set our main layout which will contain everything
  main_layout = new QGridLayout();
  sdi_widget->setLayout(main_layout);

  // Initialize our scene and our vue
  scene = new QGraphicsScene();
  vue = new QGraphicsView(scene);

  // Initialize our cell grid with 1 cell, waiting for the user to specifie other parameters
  cell_grid = new CellGrid(1, 1);

  //////////////////////////////////////////////////////////////////////////////
  // Add the different component of our GUI

  // Add our vue to our main layout
  main_layout->addWidget(vue, 0, 0, 3, 4);
  scene->addEllipse(QRect(-400, -400, 800, 800), QPen(Qt::black, 3, Qt::SolidLine));
}

GameOfLife::~GameOfLife(){
  // We don't need to delete Qt objects, Qt delete them for us
  // So we only need to delete our cell_grid which is not a Qt objects
  delete cell_grid;
}
