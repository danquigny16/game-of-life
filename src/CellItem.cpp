#include "GameOfLife.hpp"
#include "CellItem.hpp"


CellItem::CellItem(int x, int y, int len, GameOfLife *game_of_life) :
QGraphicsRectItem(x, y, len, len), game_of_life(game_of_life), is_alive(false){

}

void CellItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
  // Call its mother's class method
  QGraphicsRectItem::mousePressEvent(event);

  // Change the cell status
  is_alive = !is_alive;

  // Set the new color
  if (is_alive){
    this->setBrush(game_of_life->get_cell_color());
  }
  else{
    this->setBrush(game_of_life->get_empty_color());
  }
}

bool CellItem::is_cell(){
  return is_alive;
}
