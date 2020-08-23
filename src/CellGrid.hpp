#ifndef CELLGRID
#define CELLGRID

#include <iostream>


class CellGrid{
protected:
  char *cells;
  char *cells_next;
  int grid_width;
  int grid_height;

protected:
  void switch_cells();
  void set_size(int width, int height);

public:
  CellGrid(int width, int height);
  ~CellGrid();

  int height();
  int width();
  void set_new_size(int width, int height);

  char is_cell(int row, int col);
  void set_alive(int row, int col);
  void set_empty(int row, int col);
  void clear_grid();

  void compute_cells();
  void compute_cells_seq();
};

#endif
