#ifndef RLEREADER
#define RLEREADER

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <ctype.h>

using namespace std;


class RleHandler{
protected:
  int corner_x;
  int corner_y;
  int width;
  int height;
  bool gol_rule;
  bool *pattern;

public:
  static const int OK = 0;
  static const int ERR_RLE = 1;
  static const int ERR_OPEN = 2;

protected:
  void clear();

  int read_rle_comment_corner_x(string &line, int &size, int &ind_beg, int &ind_end);
  int read_rle_comment_corner_y(string &line, int &size, int &ind_beg, int &ind_end);
  int read_rle_comment_corner(string &line);
  int read_rle_comment_rule(string &line);
  int read_rle_comment(ifstream &file, string &line);
  int read_rle_size_line_width(string &line, int &size, int &ind_beg, int &ind_end);
  int read_rle_size_line_height(string &line, int &size, int &ind_beg, int &ind_end);
  int read_rle_size_line_rule(string &line, int &size, int &ind_beg, int &ind_end);
  int read_rle_size_line(string &line);
  int read_rle_pattern_extraction(ifstream &file, string &line, string &pattern_rle);
  int read_rle_pattern_filling_tag(int &run_count, int &nb_cell_line, int &ind_pattern, bool &cell);
  int read_rle_pattern_filling(string &pattern_rle);
  int read_rle_pattern(ifstream &file, string &line);

public:
  RleHandler();
  ~RleHandler();

  int pattern_width() const;
  int pattern_height() const;
  int pattern_corner_x() const;
  int pattern_corner_y() const;
  bool have_gol_rule() const;
  bool pattern_cell(int row, int col) const;

  int read_rle_file(string file_name);
};

#endif
