#include "CellGrid.hpp"


////////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

/**
Constructeur
@param width Grid width, border exclude
@param height Grid Height, border exclude
*/
CellGrid::CellGrid(int width, int height){
  // "set_size()" method already allocate memory and initialize our class variables
  set_size(width, height);
}

/**
Destructor
*/
CellGrid::~CellGrid(){
  delete[] cells;
  delete[] cells_next;
}


////////////////////////////////////////////////////////////////////////////////
// Getters and setters (related to the size)

/**
Give cell grid width, border exclude
@return the cell grid width, borders exclude
*/
int CellGrid::width(){
  // Don't forget to substract borders size
  return grid_width - 2;
}

/**
Give cell grid height, border exclude
@return the cell grid height, borders exclude
*/
int CellGrid::height(){
  // Don't forget to substract borders size
  return grid_height - 2;
}

/**
Set the size of our cell tab (borders include) and allocate the necessary memory
Have to only be used in the constructor and in "set_new_size()", which free previous memory for cell tabs additionaly
@param width Grid width, border exclude
@param height Grid Height, border exclude
*/
void CellGrid::set_size(int width, int height){
  // 2 extra lines and 2 extra column for borders, where all cells are empty forever
  // We will have to start to count at 1 when we will use the grid, border exclude
  grid_width = width + 2;
  grid_height = height + 2;

  // Allocate memory for our cell grids
  int size = grid_width * grid_height;
  cells = new char[size];
  cells_next = new char[size];

  // Make sure that all cells are initialize to empty
  clear_grid();
}

/**
Set the new size of our cell tab (borders include), allocate the necessary memory, and delete previous cell tabs
@param width Grid width, border exclude
@param height Grid Height, border exclude
*/
void CellGrid::set_new_size(int width, int height){
  // Delete previously allocated memory for our cell grids
  delete[] cells;
  delete[] cells_next;

  // Call set_size which already do the job except for the delete part
  set_size(width, height);
}


////////////////////////////////////////////////////////////////////////////////
// Getters and setters (related to the cell state)

/**
Return the cell state, 0 for empty, 1 for alive, at row "row" and column "col"
We consider we begin to count at 1 for rows and columns
@param row Cell row, starting to count at 1
@param col Cell column, starting to count at 1
@return the cell state, 0 for empty, 1 for alive
*/
char CellGrid::is_cell(int row, int col){
  return cells[row * grid_width + col];
}

/**
Set the cell status of cell at row "row" and column "col" to alive (1)
@param row Cell row, starting to count at 1
@param col Cell column, starting to count at 1
*/
void CellGrid::set_alive(int row, int col){
  cells[row * grid_width + col] = (char) 1;
}

/**
Set the cell status of cell at row "row" and column "col" to empty (0)
@param row Cell row, starting to count at 1
@param col Cell column, starting to count at 1
*/
void CellGrid::set_empty(int row, int col){
  cells[row * grid_width + col] = (char) 0;
}


////////////////////////////////////////////////////////////////////////////////
// CellGrid general methods

/**
Set all cells to empty
*/
void CellGrid::clear_grid(){
  for (int i = 0; i < grid_height * grid_width; i++){
    // Make the cell empty
    cells[i] = (char) 0;
  }
}

/**
Switch our cell grids, such that "cells" contain the cells new state compute in "cells_next"
*/
void CellGrid::switch_cells(){
  char *cells_switch = cells;
  cells = cells_next;
  cells_next = cells_switch;
}


////////////////////////////////////////////////////////////////////////////////
// Computation of the new cell grid

/**
Compute all the new cells state, calling the function we want to make the computation, then switch "cells" and "cells_next" tabs
*/
void CellGrid::compute_cells(){
  // We only call the function we want to compute the cells new state into "cells_next"
  compute_cells_seq();

  // Switch cells tab
  switch_cells();

  // For debugging
  // for (int i = 1; i <= grid_height - 2; i++){
  //   for (int j = 1; j <= grid_width - 2; j++){
  //     std::cout << (int) is_cell(i, j) << " ";
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "-----------------------------------\n";
}


////////////////////////////////////////////////////////////////////////////////
// Sequential computation

/**
Compute all the new cells state, sequential version
*/
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
      //    -if we are on a cell : being surrounded by 2 or 3 cell => cell, empty otherwise
      //    -if we are on a empty cell : being surrounded by 3 cell => cell, empty otherwise
      cells_next[k] = (count == ((char) 3) + cells[k]) | (count == ((char) 3));

      // We advance of one cell
      k++;
    }
    // We finished a line, k is on the right border
    // k+1 -> line below, left border, k+2 -> the next non border cell on the line below
    k += 2;
  }
}
