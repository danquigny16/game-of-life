#include "CellGrid.hpp"


////////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

CellGrid::CellGrid(int width, int height){
  this->set_size(width, height);
}

CellGrid::~CellGrid(){
  delete[] cells;
  delete[] cells_next;
}

////////////////////////////////////////////////////////////////////////////////
// Getters and setters

/**
Return the grid cells width, borders exclude
*/
int CellGrid::width(){
  // Don't forget to substract borders size
  return grid_width - 2;
}

/**
Return the grid cells height, borders exclude
*/
int CellGrid::height(){
  // Don't forget to substract borders size
  return grid_height - 2;
}

/**
Set the size of our cell tab (borders include) and allocate the necessary memory
Have to only be used in the constructor and in "set_new_size()", which free previous memory for cell tabs additionaly
*/
void CellGrid::set_size(int width, int height){
  // 2 extra lines and 2 extra column for the borders, where all cells are empty
  // Moreover, it will allow us to start to count at 1
  grid_width = width + 2;
  grid_height = height + 2;

  int size = grid_width * grid_height;
  cells = new bool[size];
  cells_next = new bool[size];

  this->clear_grid();
}

/**
Set the new size of our cell tab (borders include) and allocate the necessary memory, and delete previous cell tabs
*/
void CellGrid::set_new_size(int width, int height){
  delete[] cells;
  delete[] cells_next;

  this->set_size(width, height);
}

////////////////////////////////////////////////////////////////////////////////
// CellGrid general methods

/**
Return the cell state, dead (false) or alive (true), at row "row" and column "col"
We consider we begin to count at 1 for rows and columns
*/
bool CellGrid::cell_state(int row, int col){
  return cells[row * grid_width + col];
}

/**
Change the cell state (empty/alive)
*/
void CellGrid::change_state(int row, int col){
  int cell = row * grid_width + col;
  cells[cell] = !cells[cell];
}

/**
Set all cell to empty
*/
void CellGrid::clear_grid(){
  for (int i = 0; i < grid_height * grid_width; i++){
    cells[i] = false;
  }
}

/**
Switch our cells tab, such that "cells" contain the cells new states compute in "cells_next"
*/
void CellGrid::switch_cells(){
  bool *cells_switch = cells;
  cells = cells_next;
  cells_next = cells_switch;
}

/**
Compute all the new cells state
*/
void CellGrid::compute_cells(){
  // We only call the function we want to compute the new cells state into "cells_next"


  // Switch cells tab
  this->switch_cells();
}

////////////////////////////////////////////////////////////////////////////////
// Sequential computation of the new cell grid
