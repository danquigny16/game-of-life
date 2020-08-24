#include "GameOfLife.hpp"
#include "CellItem.hpp"


////////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

/**
Constructor
@param x top-left corner "x coordinate"
@param y top-left corner "y coordinate"
@param Cell item length
@param game_of_life game_of_life address to communicate with it ans so with the GUI
@param row Cell item row (start to count at 1)
@param col Cell item column (start to count at 1)
*/
CellItem::CellItem(int x, int y, int len, GameOfLife *game_of_life, CellGrid *cell_grid, int row, int col) :
QGraphicsRectItem(x, y, len, len), game_of_life(game_of_life), cell_grid(cell_grid), row(row), col(col), is_alive(false){

}


////////////////////////////////////////////////////////////////////////////////
// Mouse event handler

/**
If a cell item is clicked, it change its status (empty/alive), and update its color and corresponding cell in the cell grid
@param event Handle the QGraphicsScene events
*/
void CellItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
  // Call its mother's class method
  QGraphicsRectItem::mousePressEvent(event);

  // Change the cell item status
  is_alive = !is_alive;

  // Update color and cell grid
  if (is_alive){
    setBrush(game_of_life->cell_color());
    cell_grid->set_alive(row, col);
  }
  else{
    setBrush(game_of_life->empty_color());
    cell_grid->set_empty(row, col);
  }
}


////////////////////////////////////////////////////////////////////////////////
// Getters and setters

/**
Tell us if the cell item is alive
@return true if the cell is alive, false otherwise
*/
bool CellItem::is_cell(){
  return is_alive;
}

/**
Set the cell status of cell item to alive and change his color
*/
void CellItem::set_alive(){
  is_alive = true;
  setBrush(game_of_life->cell_color());
}

/**
Set the cell status of cell item to empty and change his color
*/
void CellItem::set_empty(){
  is_alive = false;
  setBrush(game_of_life->empty_color());
}
