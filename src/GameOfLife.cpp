#include "GameOfLife.hpp"
#include <unistd.h>


////////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

GameOfLife::GameOfLife() : QMainWindow(), grid_width(0), grid_height(0), cell_color(Qt::black), empty_color(Qt::white){
  this->set_window_parameters();
  this->main_variables_initialization();
  this->GUI_vue();
  this->GUI_size();
  this->GUI_cell_color();
  this->GUI_launch();
}

void GameOfLife::set_window_parameters(){
  // Window open with its maximum size
  this->setWindowState(Qt::WindowMaximized);

  // Set a title to our window
  this->setWindowTitle("Game of life");
}

void GameOfLife::main_variables_initialization(){
  // Create a SDI (Single Document Interface) widget to contain our main layout
  sdi_widget = new QWidget();
  this->setCentralWidget(sdi_widget);

  // Set our main layout which will contain everything, it will be a 9 line 16 column grid layout
  main_layout = new QGridLayout();
  sdi_widget->setLayout(main_layout);

  // Initialize our scene and our vue
  scene = new QGraphicsScene();
  vue = new QGraphicsView(scene);

  // Initialize our cell grid with at NULL, waiting the user to enter size and cells
  cell_grid = new CellGrid(0, 0);

  // "cell_items" represent the cells in the gui, it is a tab of pointer
  // "scene->addRect()" return a pointer, and to remove a rectangle from a scene, we need to pass his address, that's why we use a tab of pointer
  // We initialize it with a not NULL value such that the "delete" in "set_size()" method will not cause a problem
  cell_items = new CellItem*[1];
  //cell_items[0] = scene->addRect(QRect(-vue->width()/2, -vue->height()/2, vue->width(), vue->height()), QPen(Qt::black), empty_color);
}

void GameOfLife::GUI_vue(){
  // Add our vue to our main layout
  main_layout->addWidget(vue, 0, 0, 9, 14);
}

void GameOfLife::GUI_size(){
  // Layout to contain all the elements related to the size GUI component
  QVBoxLayout *size_layout = new QVBoxLayout();

  // Layout to contain the field
  QFormLayout *size_form_layout = new QFormLayout();

  // Row
  grid_height_box = new QSpinBox();
  grid_height_box->setRange(1, 200);
  size_form_layout->addRow("Number of row :", grid_height_box);

  // Column
  grid_width_box = new QSpinBox();
  grid_width_box->setRange(1, 200);
  size_form_layout->addRow("Number of column :", grid_width_box);

  // Button to confirm the size and create the cell grid
  QPushButton *button_size = new QPushButton("Confirm");

  // Connect the button to the slot "set_size"
  QObject::connect(button_size, SIGNAL(clicked()), this, SLOT(set_size()));

  // Add all in the size_layout
  size_layout->addLayout(size_form_layout);
  size_layout->addWidget(button_size);

  // Align our button to the right
  size_layout->setAlignment(button_size, Qt::AlignRight);

  // Add size_layout to our main_layout
  main_layout->addLayout(size_layout, 0, 14, 1, 2);
}

void GameOfLife::GUI_cell_color(){
  // Layout to contain all the elements related to the choice of color for cell and empty color
  QFormLayout *color_layout = new QFormLayout();

  // Create our color (little set of choices because if colors are not enough different, it will be difficult to make a difference)
  QPixmap Black_Pixmap = QPixmap(15, 15);
  QPixmap White_Pixmap = QPixmap(15, 15);
  QPixmap Red_Pixmap = QPixmap(15, 15);
  QPixmap Green_Pixmap = QPixmap(15, 15);
  QPixmap Blue_Pixmap = QPixmap(15, 15);
  QPixmap Magenta_Pixmap = QPixmap(15, 15);
  QPixmap Yellow_Pixmap = QPixmap(15, 15);
  QPixmap Cyan_Pixmap = QPixmap(15, 15);
  Black_Pixmap.fill(Qt::black);
  White_Pixmap.fill(Qt::white);
  Red_Pixmap.fill(Qt::red);
  Green_Pixmap.fill(Qt::green);
  Blue_Pixmap.fill(Qt::blue);
  Magenta_Pixmap.fill(Qt::magenta);
  Yellow_Pixmap.fill(Qt::yellow);
  Cyan_Pixmap.fill(Qt::cyan);
  QIcon Black_Icon = QIcon(Black_Pixmap);
  QIcon White_Icon = QIcon(White_Pixmap);
  QIcon Red_Icon = QIcon(Red_Pixmap);
  QIcon Green_Icon = QIcon(Green_Pixmap);
  QIcon Blue_Icon = QIcon(Blue_Pixmap);
  QIcon Magenta_Icon = QIcon(Magenta_Pixmap);
  QIcon Yellow_Icon = QIcon(Yellow_Pixmap);
  QIcon Cyan_Icon = QIcon(Cyan_Pixmap);

  // Filling cell color box
  cell_color_box = new QComboBox();
  cell_color_box->addItem(Black_Icon, "Black");
  cell_color_box->addItem(White_Icon, "White");
  cell_color_box->addItem(Red_Icon, "Red");
  cell_color_box->addItem(Green_Icon, "Green");
  cell_color_box->addItem(Blue_Icon, "Blue");
  cell_color_box->addItem(Magenta_Icon, "Magenta");
  cell_color_box->addItem(Yellow_Icon, "Yellow");
  cell_color_box->addItem(Cyan_Icon, "Cyan");

  // Filling empty color box
  empty_color_box = new QComboBox();
  empty_color_box->addItem(Black_Icon, "Black");
  empty_color_box->addItem(White_Icon, "White");
  empty_color_box->addItem(Red_Icon, "Red");
  empty_color_box->addItem(Green_Icon, "Green");
  empty_color_box->addItem(Blue_Icon, "Blue");
  empty_color_box->addItem(Magenta_Icon, "Magenta");
  empty_color_box->addItem(Yellow_Icon, "Yellow");
  empty_color_box->addItem(Cyan_Icon, "Cyan");

  // Match our data with QCombox content
  cell_color_box->setCurrentIndex(cell_color_box->findText("Black"));
  empty_color_box->setCurrentIndex(empty_color_box->findText("White"));

  // Connect our boxes to color settings
  QObject::connect(cell_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_cell_color()));
  QObject::connect(empty_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_empty_color()));

  // Add our box to our color_layout
  color_layout->addRow("Cell color :", cell_color_box);
  color_layout->addRow("Empty color :", empty_color_box);

  // Add our color_layout to our main_layout
  main_layout->addLayout(color_layout, 3, 14, 1, 2);
}

void GameOfLife::GUI_launch(){
  // Layout to contain all the elements related to the launch GUI component
  QVBoxLayout *launch_layout = new QVBoxLayout();

  // Layout to contain the field
  QFormLayout *launch_form_layout = new QFormLayout();

  // Number of iteration box
  nb_iter_box = new QSpinBox();
  nb_iter_box->setRange(1, 1000);
  launch_form_layout->addRow("Number of iteration :", nb_iter_box);

  // Number of iteration that remain
  nb_iter_remain = new QLabel("Remaining iteration : 0");

  // Layout to contain "clear" and "launch" button
  QHBoxLayout *button_layout = new QHBoxLayout();

  // Button to clear the grid
  QPushButton *button_clear = new QPushButton("Clear");

  // Connect the button to the slot "clear_grid"
  QObject::connect(button_clear, SIGNAL(clicked()), this, SLOT(clear_grid()));

  // Button to start the game
  QPushButton *button_launch = new QPushButton("Launch");

  // Connect the button to the slot "start_game_of_life"
  QObject::connect(button_launch, SIGNAL(clicked()), this, SLOT(start_game_of_life()));

  // Add buttons to the "button_layout"
  button_layout->addWidget(button_clear);
  button_layout->addWidget(button_launch);

  // Align our buttons
  button_layout->setAlignment(button_clear, Qt::AlignLeft);
  button_layout->setAlignment(button_launch, Qt::AlignRight);

  // Add all in the launch_layout
  launch_layout->addLayout(launch_form_layout);
  launch_layout->addWidget(nb_iter_remain);
  launch_layout->addLayout(button_layout);

  // Add launch_layout to our main_layout
  main_layout->addLayout(launch_layout, 8, 14, 1, 2);
}

GameOfLife::~GameOfLife(){
  // We don't need to delete Qt objects, Qt delete them for us
  // So we only need to delete our cell_grid which is not a Qt objects
  delete cell_grid;
}

////////////////////////////////////////////////////////////////////////////////
// Getters and setters (and function they need)

int GameOfLife::get_grid_width(){
  return grid_width;
}

int GameOfLife::get_grid_height(){
  return grid_height;
}

QColor GameOfLife::get_cell_color(){
  return cell_color;
}

QColor GameOfLife::get_empty_color(){
  return empty_color;
}

/**
Set the size of our cell tab (borders exclude), update our cell_grid, free our previous items from the scene and allocate the new ones
*/
void GameOfLife::set_size(){
  // Free previous item from our scene
  scene->clear();

  // Set new size
  grid_width = grid_width_box->value();
  grid_height = grid_height_box->value();

  // Update our cell grid
  cell_grid->set_new_size(grid_width, grid_height);

  // Free and allocate memory for our new "cell_items" tab
  delete[] cell_items;
  cell_items = new CellItem*[grid_width * grid_height];

  // Compute cell length
  int cell_width = vue->width() / grid_width;
  int cell_height = vue->height() / grid_height;
  int cell_len = std::min(cell_width, cell_height);

  // Add all new cell items
  CellItem *rect;
  for (int i = 0; i < grid_height; i++){
    for (int j = 0; j < grid_width; j++){
      rect = new CellItem(j * cell_len, i * cell_len, cell_len, this, i, j);
      rect->setBrush(empty_color);
      cell_items[i * grid_width + j] = rect;
      scene->addItem(rect);
    }
  }

  // Allow us to center the scene on our grid
  scene->setSceneRect(scene->itemsBoundingRect());
}

QColor GameOfLife::which_color(QString color_string){
  if (color_string == "Black"){
    return Qt::black;
  }
  else if (color_string == "White"){
    return Qt::white;
  }
  else if (color_string == "Red"){
    return Qt::red;
  }
  else if (color_string == "Green"){
    return Qt::green;
  }
  else if (color_string == "Blue"){
    return Qt::blue;
  }
  else if (color_string == "Magenta"){
    return Qt::magenta;
  }
  else if (color_string == "Yellow"){
    return Qt::yellow;
  }
  else if (color_string == "Cyan"){
    return Qt::cyan;
  }
  // Black by default
  return Qt::black;
}

QString GameOfLife::which_color(QColor q_color){
  if (q_color == Qt::black){
    return "Black";
  }
  else if (q_color == Qt::white){
    return "White";
  }
  else if (q_color == Qt::red){
    return "Red";
  }
  else if (q_color == Qt::green){
    return "Green";
  }
  else if (q_color == Qt::blue){
    return "Blue";
  }
  else if (q_color == Qt::magenta){
    return "Magenta";
  }
  else if (q_color == Qt::yellow){
    return "Yellow";
  }
  else if (q_color == Qt::cyan){
    return "Cyan";
  }
  // Black by default
  return "Black";
}

void GameOfLife::update_cell_color(){
  // update all cell color
  for (int i = 0; i < grid_width * grid_height; i++){
    if (cell_items[i]->is_cell()){
      cell_items[i]->setBrush(cell_color);
    }
    else{
      cell_items[i]->setBrush(empty_color);
    }
  }
}

void GameOfLife::set_cell_color(){
  // Retrieve the color
  QColor c_color = this->which_color(cell_color_box->currentText());

  // If it is equal to the empty color, we switch
  if (c_color == empty_color){
    // We don't forget to temporarily disconnect our "empty_color_box" to the slot "set_empty_color()"
    // If we don't, "setCurrentIndex()" will call the slot and that will cause a problem (all cell of the same color)
    QObject::disconnect(empty_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_empty_color()));
    empty_color_box->setCurrentIndex(empty_color_box->findText(which_color(cell_color)));
    empty_color = cell_color;
    QObject::connect(empty_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_empty_color()));
  }

  // We set the new color
  cell_color = c_color;

  // Don't forget to change cells color
  this->update_cell_color();
}

void GameOfLife::set_empty_color(){
  // Retrieve the color
  QColor e_color = this->which_color(empty_color_box->currentText());

  // If it is equal to the cell color, we switch
  if (e_color == cell_color){
    // We don't forget to temporarily disconnect our "cell_color_box" to the slot "set_cell_color()"
    // If we don't, "setCurrentIndex()" will call the slot and that will cause a problem (all cell of the same color)
    QObject::disconnect(cell_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_cell_color()));
    cell_color_box->setCurrentIndex(cell_color_box->findText(which_color(empty_color)));
    cell_color = empty_color;
    QObject::connect(cell_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_cell_color()));
  }

  // We set the new color
  empty_color = e_color;

  // Don't forget to change cells color
  this->update_cell_color();
}

////////////////////////////////////////////////////////////////////////////////
// Method for the computing of new "cell_grid"

void GameOfLife::set_grid_cell_alive(int row, int col){
  cell_grid->set_alive(row+1, col+1);
}

void GameOfLife::set_grid_cell_empty(int row, int col){
  cell_grid->set_empty(row+1, col+1);
}

void GameOfLife::update_cell_grid(){
  for (int i = 0; i < grid_height; i++){
    for (int j = 0; j < grid_width; j++){
      // Cell number
      int cell_num = i * grid_width + j;

      // Update "cell_item" (in "cell_grid", we begin to count at 1)
      if (cell_grid->is_cell(i+1, j+1)){
        cell_items[cell_num]->set_alive();
      }
      else{
        cell_items[cell_num]->set_empty();
      }

      // Update the cell times color
      if (cell_items[cell_num]->is_cell()){
        cell_items[cell_num]->setBrush(cell_color);
      }
      else{
        cell_items[cell_num]->setBrush(empty_color);
      }
    }
  }
}

void GameOfLife::clear_grid(){
  // Clear our GUI grid
  for (int i = 0; i < grid_width * grid_height; i++){
    cell_items[i]->setBrush(empty_color);
    cell_items[i]->set_empty();
  }

  // Clear our "cell_grid"
  cell_grid->clear_grid();
}

/**
NO SLEEP FUNCTION, IT WILL FREEZE THE GUI
*/
void delay(int time){
  QTime dieTime= QTime::currentTime().addMSecs(time);
  while (QTime::currentTime() < dieTime){
    QCoreApplication::processEvents(QEventLoop::AllEvents, time);
  }
}

void GameOfLife::start_game_of_life(){
  int nb_iter = nb_iter_box->value();
  for (int i = nb_iter; i > 0; i--){
    delay(50);
    cell_grid->compute_cells();
    this->update_cell_grid();
    nb_iter_remain->setText("Remaining iteration : " + QString::number(i));
  }
  nb_iter_remain->setText("Remaining iteration : 0");
}
