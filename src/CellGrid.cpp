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
  cells = new char[size];
  cells_next = new char[size];

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

/**
Return the cell state, dead (0) or alive (1), at row "row" and column "col"
We consider we begin to count at 1 for rows and columns
*/
char CellGrid::is_cell(int row, int col){
  return cells[row * grid_width + col];
}

////////////////////////////////////////////////////////////////////////////////
// CellGrid general methods

void CellGrid::set_alive(int row, int col){
  cells[row * grid_width + col] = (char) 1;
}

void CellGrid::set_empty(int row, int col){
  cells[row * grid_width + col] = (char) 0;
}

/**
Set all cell to empty
*/
void CellGrid::clear_grid(){
  for (int i = 0; i < grid_height * grid_width; i++){
    // Make the cell empty
    cells[i] = 0;
  }
}

/**
Switch our cells tab, such that "cells" contain the cells new states compute in "cells_next"
*/
void CellGrid::switch_cells(){
  char *cells_switch = cells;
  cells = cells_next;
  cells_next = cells_switch;
}

/**
Compute all the new cells state
*/
void CellGrid::compute_cells(){
  // We only call the function we want to compute the new cells state into "cells_next"
  this->compute_cells_seq();

  // Switch cells tab
  this->switch_cells();

  // for (int i = 1; i <= grid_height - 2; i++){
  //   for (int j = 1; j <= grid_width - 2; j++){
  //     std::cout << (int) this->is_cell(i, j) << " ";
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "-----------------------------------\n";
}

////////////////////////////////////////////////////////////////////////////////
// Sequential computation of the new cell grid

void CellGrid::compute_cells_seq(){
  // First line is the top border, we skip, line below, the first cell is on the left border, we skip
  // Knowing that we begin to count at 0, k begin with the value "grid_width + 1"
  // We will increment k every time we advance of 1 cell, instead of computing every time "k = j * grid_width + i"
  int k = grid_width + 1;
  int sub_grid_height = grid_height - 2;
  int sub_grid_width = grid_width - 2;
  for (int i = 1; i <= sub_grid_height; i++){
    for (int j = 1; j <= sub_grid_width; j++){
      // Count the number of cells around the cell "k" including it
      char count = cells[k-1 - grid_width] + cells[k - grid_width] + cells[k+1 - grid_width] +
                   cells[k-1]              + cells[k]              + cells[k+1]              +
                   cells[k-1 + grid_width] + cells[k + grid_width] + cells[k+1 + grid_width];
      // Next state :
      //    -if we are on a cell : being surrounded by 2 or 3 cell
      //    -if we are on a empty cell : being surrounded by 3 cell
      cells_next[k] = (count == ((char) 3) + cells[k]) | (count == ((char) 3));

      // We advance of one cell
      k++;
    }
    // We finished a line, k is on the right border
    // k+1 -> line below, left border, k+2 -> the next non border cell
    k += 2;
  }
}
