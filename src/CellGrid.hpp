#ifndef CELLGRID
#define CELLGRID


class CellGrid{
protected:
  bool *cells;
  bool *cells_next;
  int grid_height;
  int grid_width;

protected:
  void switch_cells();
  void set_size(int height, int width);

public:
  CellGrid(int height, int width);
  ~CellGrid();

  int height();
  int width();
  void set_new_size(int height, int width);

  bool cell_state(int row, int col);
  void change_state(int row, int col);
  void clear_grid();
  void compute_cells();
};

#endif
