#ifndef CELLGRID
#define CELLGRID

// For debbuging with "cout"
#include <iostream>


class CellGrid{
protected:
  int grid_width;
  int grid_height;

  char *cells;
  char *cells_next;

protected:
  void set_size(int width, int height);

  void switch_cells();

public:
  CellGrid(int width, int height);
  ~CellGrid();

  int width();
  int height();
  void set_new_size(int width, int height);

  char is_cell(int row, int col);
  void set_alive(int row, int col);
  void set_empty(int row, int col);

  void clear_grid();
  
  void compute_cells();
  void compute_cells_seq();
};

#endif
