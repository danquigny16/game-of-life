#include "GameOfLife.hpp"


////////////////////////////////////////////////////////////////////////////////
// Constructors and destructors (and methods they need)

/**
Constructor
*/
GameOfLife::GameOfLife() : QMainWindow(), grid_width(0), grid_height(0), cell_item_color(Qt::black), empty_item_color(Qt::white){
  this->set_window_parameters();
  this->main_variables_initialization();
  this->GUI_view();
  this->GUI_size();
  this->GUI_color();
  this->GUI_launch();
}

/**
Set the window parameters, give it a title make make it open at maximum size
*/
void GameOfLife::set_window_parameters(){
  // Window open with its maximum size
  this->setWindowState(Qt::WindowMaximized);

  // Set a title to our window
  this->setWindowTitle("Game of life");
}

/**
Initialize our main variables
*/
void GameOfLife::main_variables_initialization(){
  // Create a SDI (Single Document Interface) widget to contain our main layout
  sdi_widget = new QWidget();
  this->setCentralWidget(sdi_widget);

  // Set our main layout which will contain everything, it will be a 9 line 16 column grid layout
  main_layout = new QGridLayout();
  sdi_widget->setLayout(main_layout);

  // Initialize our scene and our view
  scene = new QGraphicsScene();
  view = new QGraphicsView(scene);

  // Initialize our cell grid with at NULL, waiting the user to enter size and cells
  cell_grid = new CellGrid(0, 0);

  // "cell_items" represent the cells in the gui, it is a tab of pointer
  // "scene->addRect()" return a pointer, and to remove a rectangle from a scene, we need to pass his address, that's why we use a tab of pointer
  // We initialize it with a not NULL value such that the "delete" in "set_size()" method will not cause a problem
  cell_items = new CellItem*[1];
}

/**
Set view and scene component in the GUI
*/
void GameOfLife::GUI_view(){
  // Add our view to our main layout
  main_layout->addWidget(view, 0, 0, 9, 14);
}

/**
Set GUI size component, allowing us to create a grid with the desired size
*/
void GameOfLife::GUI_size(){
  // Layout to contain all the elements related to the GUI size component
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
  button_size = new QPushButton("Confirm");

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

/**
Set GUI color component, allowing decide of empty and cell color
*/
void GameOfLife::GUI_color(){
  // Layout to contain all the elements related to the GUI color component
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

/**
Set GUI launch component, allowing us to launch "game of life" or clean our cell grid
*/
void GameOfLife::GUI_launch(){
  // Layout to contain all the elements related to the GUI launch component
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
  button_clear = new QPushButton("Clear");

  // Connect the button to the slot "clear_grid"
  QObject::connect(button_clear, SIGNAL(clicked()), this, SLOT(clear_grid()));

  // Button to start the game
  button_launch = new QPushButton("Launch");

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

/**
Destructor
*/
GameOfLife::~GameOfLife(){
  // We don't need to delete Qt objects, Qt delete them for us
  // So we only need to delete our cell_grid which is not a Qt objects
  delete cell_grid;
}


////////////////////////////////////////////////////////////////////////////////
// Getters and setters (related to the size)

/**
Give cell grid width, border exclude
@return the cell grid width, borders exclude
*/
int GameOfLife::width(){
  return grid_width;
}

/**
Give cell grid height, border exclude
@return the cell grid height, borders exclude
*/
int GameOfLife::height(){
  return grid_height;
}

/**
Set the size of our cell tab (borders exclude), update our cell_grid, free our previous items from the scene and allocate the new ones
*/
void GameOfLife::set_size(){
  // Free previous cell items from our scene
  scene->clear();

  // Set new size
  grid_width = grid_width_box->value();
  grid_height = grid_height_box->value();

  // Update our cell grid
  cell_grid->set_new_size(grid_width, grid_height);

  // Free and allocate memory for our new "cell_items" tab
  delete[] cell_items;
  cell_items = new CellItem*[grid_width * grid_height];

  // Compute cell length (take the min so that our grid can fill our scene)
  int cell_width = view->width() / grid_width;
  int cell_height = view->height() / grid_height;
  int cell_len = std::min(cell_width, cell_height);

  // Add all new cell items
  CellItem *rect;
  for (int i = 0; i < grid_height; i++){
    for (int j = 0; j < grid_width; j++){
      rect = new CellItem(j * cell_len, i * cell_len, cell_len, this, cell_grid, i+1, j+1);
      rect->setBrush(empty_item_color);
      cell_items[i * grid_width + j] = rect;
      scene->addItem(rect);
    }
  }

  // Allow us to center the scene on our grid
  scene->setSceneRect(scene->itemsBoundingRect());
}


////////////////////////////////////////////////////////////////////////////////
// Getters and setters (related to color)

/**
Give cell color
@return the cell color
*/
QColor GameOfLife::cell_color(){
  return cell_item_color;
}

/**
Give empty color
@return the empty color
*/
QColor GameOfLife::empty_color(){
  return empty_item_color;
}

/**
Give the QColor corresponding to the QString color
@param color_string Color of type QString
@return the QColor corresponding to the QString color
*/
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

/**
Give the QString corresponding to the QColor color
@param q_color Color of type QColor
@return the QString corresponding to the QColor color
*/
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

/**
Update all cell color (used when user change cell and/or empty color)
*/
void GameOfLife::update_cell_color(){
  for (int i = 0; i < grid_width * grid_height; i++){
    if (cell_items[i]->is_cell()){
      cell_items[i]->setBrush(cell_item_color);
    }
    else{
      cell_items[i]->setBrush(empty_item_color);
    }
  }
}

/**
Set cell color to the color the user choose, then update cell items color
*/
void GameOfLife::set_cell_color(){
  // Retrieve the color
  QColor c_color = this->which_color(cell_color_box->currentText());

  // If it is equal to the empty color, we switch
  if (c_color == empty_item_color){
    // We don't forget to temporarily disconnect our "empty_color_box" to the slot "set_empty_color)"
    // If we don't, "setCurrentIndex()" will call the slot and that will cause a problem (all cell of the same color)
    QObject::disconnect(empty_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_empty_color()));
    empty_color_box->setCurrentIndex(empty_color_box->findText(which_color(cell_item_color)));
    empty_item_color = cell_item_color;
    QObject::connect(empty_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_empty_color()));
  }

  // We set the new color
  cell_item_color = c_color;

  // Don't forget to change cell items color
  this->update_cell_color();
}

/**
Set empty color to the color the user choose, then update cell items color
*/
void GameOfLife::set_empty_color(){
  // Retrieve the color
  QColor e_color = this->which_color(empty_color_box->currentText());

  // If it is equal to the cell color, we switch
  if (e_color == cell_item_color){
    // We don't forget to temporarily disconnect our "cell_color_box" to the slot "set_cell_color()"
    // If we don't, "setCurrentIndex()" will call the slot and that will cause a problem (all cell of the same color)
    QObject::disconnect(cell_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_cell_color()));
    cell_color_box->setCurrentIndex(cell_color_box->findText(which_color(empty_item_color)));
    cell_item_color = empty_item_color;
    QObject::connect(cell_color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(set_cell_color()));
  }

  // We set the new color
  empty_item_color = e_color;

  // Don't forget to change cells color
  this->update_cell_color();
}


////////////////////////////////////////////////////////////////////////////////
// Getters and setters (related to cell grid)

/**
Say if the cell at row "row" and at column "col" is alive or empty
We start to count at 1 for rox and column
@param row Cell row
@param col Cell column
@return true if the cell at row "row" and at column "col" is alive, false otherwise
*/
bool GameOfLife::is_cell(int row, int col){
  return cell_grid->is_cell(row, col);
}


////////////////////////////////////////////////////////////////////////////////
// GameOfLife general methods

/**
Clear our cell grid (regular cell grid and cell item grid), all cell are set to empty
*/
void GameOfLife::clear_grid(){
  // Clear our cell item grid
  for (int i = 0; i < grid_width * grid_height; i++){
    cell_items[i]->set_empty();
  }

  // Clear our "cell_grid"
  cell_grid->clear_grid();
}

////////////////////////////////////////////////////////////////////////////////
// Method for computing the new "cell_grid"

/**
Classic delay function
NO SLEEP FUNCTION, IT WILL FREEZE THE GUI
we use processEvents, it will moreover allow us to handle events during the sleep
@param time Time to sleep in millisecond
*/
void delay(int time){
  QTime dieTime= QTime::currentTime().addMSecs(time);
  while (QTime::currentTime() < dieTime){
    QCoreApplication::processEvents(QEventLoop::AllEvents, time);
  }
}

/**
Start the game of life
*/
void GameOfLife::start_game_of_life(){
  // Retrieve the number of iteration wanted
  int nb_iter = nb_iter_box->value();

  // Start the game of life
  for (int i = nb_iter; i > 0; i--){
    delay(50);

    // Compute the new cell grid
    cell_grid->compute_cells();

    // Update cell item grid for the GUI
    this->update_cell_grid();

    // Update the number of iteration remaining
    nb_iter_remain->setText("Remaining iteration : " + QString::number(i));
  }

  // Don't forget the last one, when all iteration are done
  nb_iter_remain->setText("Remaining iteration : 0");
}

/**
Do one step of the game of life
*/
void GameOfLife::update_cell_grid(){
  for (int i = 0; i < grid_height; i++){
    for (int j = 0; j < grid_width; j++){
      // Cell number
      int cell_num = i * grid_width + j;

      // Update cell items (in our regular cell grid, we begin to count at 1)
      if (cell_grid->is_cell(i+1, j+1)){
        cell_items[cell_num]->set_alive();
      }
      else{
        cell_items[cell_num]->set_empty();
      }
    }
  }
}
