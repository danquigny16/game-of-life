#ifndef CELLGRID
#define CELLGRID


class CellGrid{
protected:
  bool *cells;
  bool *cells_next;
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

  bool cell_state(int row, int col);
  void change_state(int row, int col);
  void clear_grid();
  void compute_cells();
};

#endif
