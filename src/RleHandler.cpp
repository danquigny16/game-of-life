#include "RleHandler.hpp"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

/**
Constructor
*/
RleHandler::RleHandler() : corner_x(0), corner_y(0), width(0), height(0), gol_rule(true), pattern(NULL){

}

/**
Destructor
*/
RleHandler::~RleHandler(){
  if (pattern == NULL){
    delete[] pattern;
  }
}


////////////////////////////////////////////////////////////////////////////////
// Getters and setters

/**
Give the pattern width
@return the pattern width
*/
int RleHandler::pattern_width() const{
  return width;
}

/**
Give the pattern height
@return the pattern height
*/
int RleHandler::pattern_height() const{
  return height;
}

/**
Give the pattern x corner coordinate
@return the pattern x corner coordinate
*/
int RleHandler::pattern_corner_x() const{
  return corner_x;
}

/**
Give the pattern y corner coordinate
@return the pattern y corner coordinate
*/
int RleHandler::pattern_corner_y() const{
  return corner_y;
}

/**
Say if the pattern is supposed to follow the game of life rules (B3/S23)
@return true if the pattern is supposed to follow the game of life rules, false otherwise
*/
bool RleHandler::have_gol_rule() const{
  return gol_rule;
}

/**
Give the cell state of cell at row "row" and column "col"
@param row Cell row (we start to count at 1)
@param col Cell column (we start to count at 1)
@return true if the cell at row "row" and column "col" is alive, false if it is empty
*/
bool RleHandler::pattern_cell(int row, int col) const{
  return pattern[(row - 1) * width + (col - 1)];
}


////////////////////////////////////////////////////////////////////////////////
// RleHandler general method

/**
Reinitialize all our variable to their initial value (constructor)
*/
void RleHandler::clear(){
  corner_x = 0;
  corner_y = 0;
  width = 0;
  height = 0;
  gol_rule = true;
  if (pattern != NULL){
    delete[] pattern;
  }
  pattern = NULL;
}


////////////////////////////////////////////////////////////////////////////////
// RleHandler reading methods

/**
Read the x coordinate part of corner comment
@param line Current readed line
@param size Line size
@param ind_beg Beginning index, used when we have to retrieve a number
@param ind_end Ending index, used when we have to retrieve a number
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_comment_corner_x(string &line, int &size, int &ind_beg, int &ind_end){
  //////////////////////////////////////////////////////////////////////////////
  // Retrieve x coordinate of top-left corner, if negative, we put it at 0
  if (line.at(3) == '-'){
    corner_x = 0;
    ind_beg++;
    ind_end++;
    while (size > ind_end && isdigit(line.at(ind_end))){
      ind_end++;
    }
  }
  else{
    while (size > ind_end && isdigit(line.at(ind_end))){
      ind_end++;
    }
    if (ind_end - ind_beg == 0){
      return RleHandler::ERR_RLE;
    }
    corner_x = stoi(line.substr(ind_beg, ind_end - ind_beg));
  }
  ind_beg = ind_end;

  //////////////////////////////////////////////////////////////////////////////
  // Skip space
  if (size <= ind_end || line.at(ind_end) != ' '){
    return RleHandler::ERR_RLE;
  }
  else{
    ind_end++;
    ind_beg = ind_end;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Check if size is big enough (for y coordinate part)
  if (size <= ind_end){
    return RleHandler::ERR_RLE;
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no error has occured
  return RleHandler::OK;
}

/**
Read the y coordinate part of corner comment
@param line Current readed line
@param size Line size
@param ind_beg Beginning index, used when we have to retrieve a number
@param ind_end Ending index, used when we have to retrieve a number
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_comment_corner_y(string &line, int &size, int &ind_beg, int &ind_end){
  //////////////////////////////////////////////////////////////////////////////
  // Retrieve y coordinate of top-left corner, if negative, we put it at 0
  if (line.at(3) == '-'){
    corner_y = 0;
    ind_beg++;
    ind_end++;
    while (size > ind_end && isdigit(line.at(ind_end))){
      ind_beg++;
      ind_end++;
    }
  }
  else{
    while (size > ind_end && isdigit(line.at(ind_end))){
      ind_end++;
    }
    if (ind_end - ind_beg == 0){
      return RleHandler::ERR_RLE;
    }
    corner_x = stoi(line.substr(ind_beg, ind_end - ind_beg));
  }

  //////////////////////////////////////////////////////////////////////////////
  //Check this is the end of line, if not, rle format is not respected
  if (size > ind_end){
    return RleHandler::ERR_RLE;
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no error has occured
  return RleHandler::OK;
}

/**
Read the corner comment
@param line Current readed line
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_comment_corner(string &line){
  //////////////////////////////////////////////////////////////////////////////
  // We retrieve top-left corner coordinates
  int size = line.size();
  int ind_beg = 3;
  int ind_end = 3;

  // Check if comment is not empty
  if (size < 4){
    return RleHandler::ERR_RLE;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Retrieve x coordinate of top-left corner
  if (read_rle_comment_corner_x(line, size, ind_beg, ind_end) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  // Retrieve y coordinate of top-left corner
  if (read_rle_comment_corner_y(line, size, ind_beg, ind_end) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no error has occured
  return RleHandler::OK;
}

/**
Read the rule comment
@param line Current readed line
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_comment_rule(string &line){
  //////////////////////////////////////////////////////////////////////////////
  // Only check the format
  int size = line.size();
  int ind_beg = 3;
  int ind_end = 3;

  // First number
  while (size > ind_end && isdigit(line.at(ind_end))){
    ind_end++;
  }
  if (ind_end - ind_beg == 0){
    return RleHandler::ERR_RLE;
  }

  // Slash between the two number
  if (size > ind_end && line.at(ind_end) == '/'){
    ind_end++;
  }
  else{
    return RleHandler::ERR_RLE;
  }
  ind_beg = ind_end;

  // Second number
  while (size > ind_end && isdigit(line.at(ind_end))){
    ind_end++;
  }
  if (ind_end - ind_beg == 0){
    return RleHandler::ERR_RLE;
  }

  //////////////////////////////////////////////////////////////////////////////
  // We search if it is game of life rules
  if (line == "#r 23/3"){
    gol_rule = true;
  }
  else{
    gol_rule = false;
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no error has occured
  return RleHandler::OK;
}

/**
Read the comment part of a rle file
@param file Rle file we are reading
@param line Current readed line
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_comment(ifstream &file, string &line){
  //////////////////////////////////////////////////////////////////////////////
  // Some comments can't be present more than once
  bool N_encountered = false;
  bool O_encountered = false;
  bool P_R_encountered = false;
  bool r_encountered = false;

  /*
  Comments begin by '#', and is follow by a letter that indicate which type of comment it is :
      -'C' : Indicates that a line of comment follows
      -'c' : Same as C, but not recommended
      -'N' : The name of the pattern
      -'O' : Says when and by whom the file was created
      -'P' : Essentially the same as R, below
      -'R' : Gives the coordinates of the top-left corner of the pattern, can be negative
             (we don't use negative here, we consider that the top-left corner is at the coordinates (0, 0))
      -'r' : Gives the rules for the pattern in the form survival_counts/birth_counts, e.g. 23/3 for Life
             (if there is another rule int the size line of the rle file, it is this one which will be considered)
  */

  //////////////////////////////////////////////////////////////////////////////
  while (getline(file, line)){
    // We check if the line is a comment
    if (line.size() < 3 || line.at(0) != '#' || line.at(2) != ' '){
      break;
    }

    // Simple comment, go to next line
    else if (line.at(1) == 'C' || line.at(1) == 'c'){
      // We do nothing, it is just a comment
    }

    // Should be unique
    else if (line.at(1) == 'N'){
      if (N_encountered){
        return RleHandler::ERR_RLE;
      }
      N_encountered = true;
    }

    // Should be unique
    else if (line.at(1) == 'O'){
      if (O_encountered){
        return RleHandler::ERR_RLE;
      }
      O_encountered = true;
    }

    // Should be unique (only one of them, or none)
    else if (line.at(1) == 'P' || line.at(1) == 'R'){
      if (P_R_encountered){
        return RleHandler::ERR_RLE;
      }
      P_R_encountered = true;

      // Call the method which read corner comment
      if (read_rle_comment_corner(line) == RleHandler::ERR_RLE){
        return RleHandler::ERR_RLE;
      }
    }

    // Should be unique
    else if (line.at(1) == 'r'){
      if (r_encountered){
        return RleHandler::ERR_RLE;
      }
      r_encountered = true;

      // Call the method which read rule comment
      if (read_rle_comment_rule(line) == RleHandler::ERR_RLE){
        return RleHandler::ERR_RLE;
      }
    }

    // No other type of comment, so if there is something else, we consider it as an error
    else{
      return RleHandler::ERR_RLE;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no error has occured
  return RleHandler::OK;
}

/**
Read width part of the size line of a rle file
@param line Current readed line
@param size Line size
@param ind_beg Beginning index, used when we have to retrieve a number
@param ind_end Ending index, used when we have to retrieve a number
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_size_line_width(string &line, int &size, int &ind_beg, int &ind_end){
  //////////////////////////////////////////////////////////////////////////////
  // "x = " recognition
  ind_end = strlen("x = ");
  if (size <= ind_end || line.substr(ind_beg, ind_end - ind_beg) != "x = "){
    return RleHandler::ERR_RLE;
  }
  ind_beg = ind_end;

  //////////////////////////////////////////////////////////////////////////////
  // Width retrieving
  while (size > ind_end && isdigit(line.at(ind_end))){
    ind_end++;
  }
  width = stoi(line.substr(ind_beg, ind_end - ind_beg));
  if (width == 0){
    return RleHandler::ERR_RLE;
  }
  ind_beg = ind_end;

  //////////////////////////////////////////////////////////////////////////////
  // If no error has occured
  return RleHandler::OK;
}

/**
Read height part of the size line of a rle file
@param line Current readed line
@param size Line size
@param ind_beg Beginning index, used when we have to retrieve a number
@param ind_end Ending index, used when we have to retrieve a number
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_size_line_height(string &line, int &size, int &ind_beg, int &ind_end){
  //////////////////////////////////////////////////////////////////////////////
  // ", y = " recognition
  ind_end += strlen(", y = ");
  if (size <= ind_end || line.substr(ind_beg, ind_end - ind_beg) != ", y = "){
    return RleHandler::ERR_RLE;
  }
  ind_beg = ind_end;

  //////////////////////////////////////////////////////////////////////////////
  // Height retrieving
  while (size > ind_end && isdigit(line.at(ind_end))){
    ind_end++;
  }
  height = stoi(line.substr(ind_beg, ind_end - ind_beg));
  if (height == 0){
    return RleHandler::ERR_RLE;
  }
  ind_beg = ind_end;

  //////////////////////////////////////////////////////////////////////////////
  // If no error has occured
  return RleHandler::OK;
}

/**
Read rule part of the size line of a rle file
@param line Current readed line
@param size Line size
@param ind_beg Beginning index, used when we have to retrieve a number
@param ind_end Ending index, used when we have to retrieve a number
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_size_line_rule(string &line, int &size, int &ind_beg, int &ind_end){
  //////////////////////////////////////////////////////////////////////////////
  // rule part is not necessarily present
  if (size <= ind_end){
    return RleHandler::OK;
  }

  //////////////////////////////////////////////////////////////////////////////
  // ", rule = B" recognition
  ind_end += strlen(", rule = B");
  if (size <= ind_end || line.substr(ind_beg, ind_end - ind_beg) != ", rule = B"){
    return RleHandler::ERR_RLE;
  }
  ind_beg = ind_end;

  // "B" digit
  while (size > ind_end && isdigit(line.at(ind_end))){
    ind_end++;
  }
  if (ind_end - ind_beg == 0){
    return RleHandler::ERR_RLE;
  }
  int B_digit = stoi(line.substr(ind_beg, ind_end - ind_beg));
  // Check if that correspond to the gol rules
  if (B_digit != 3){
    gol_rule = false;
  }
  ind_beg = ind_end;

  //////////////////////////////////////////////////////////////////////////////
  // "/S" recognition
  ind_end += strlen("/S");
  if (size <= ind_end || line.substr(ind_beg, ind_end - ind_beg) != "/S"){
    return RleHandler::ERR_RLE;
  }
  ind_beg = ind_end;


  // "S" digit
  while (size > ind_end && isdigit(line.at(ind_end))){
    ind_end++;
  }
  if (ind_end - ind_beg == 0){
    return RleHandler::ERR_RLE;
  }
  int S_digit = stoi(line.substr(ind_beg, ind_end - ind_beg));
  // Check if that correspond to the gol rules
  if (S_digit != 23){
    gol_rule = false;
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no error has occured
  return RleHandler::OK;
}

/**
Read the size line of a rle file
@param line Current readed line
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_size_line(string &line){
  //////////////////////////////////////////////////////////////////////////////
  // The first line should be of this type "x = m, y = n", or potentially "x = m, y = n, rule = abc"
  int size = line.size();
  int ind_beg = 0;
  int ind_end = 0;

  //////////////////////////////////////////////////////////////////////////////
  // Width part
  if (read_rle_size_line_width(line, size, ind_beg, ind_end) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  // Height part
  if (read_rle_size_line_height(line, size, ind_beg, ind_end) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  // Rule part
  if (read_rle_size_line_rule(line, size, ind_beg, ind_end) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  // Allocate memory for the pattern
  pattern = new bool[width * height];

  //////////////////////////////////////////////////////////////////////////////
  // Everything is ok
  return RleHandler::OK;
}

/**
Extract and compress the rle file pattern into "pattern_rle"
@param file Rle file we are reading
@param line Current readed line
@param pattern_rle string to contain the pattern without space and carriage returns
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_pattern_extraction(ifstream &file, string &line, string &pattern_rle){
  while (getline(file, line)){
    ////////////////////////////////////////////////////////////////////////////
    // Pattern lines must not exceed 70 characters
    if (line.size() == 0 || line.size() > 70){
      return RleHandler::ERR_RLE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Spaces are allowed except between digits and between a digit and a letter :
    // Whitespace is permitted between <run_count><tag> items (and between the last <run_count><tag> item and the following !)
    // The last <run_count><tag> item is followed by a ! character
    // <run_count> can be omitted if it is equal to 1
    // <tag> is one of the following three characters : 'b', 'o', '$'
    bool last_nb = true;
    for (unsigned int i = 0; i < line.size(); i++){
      // If it is a <run_count>
      if (isdigit(line.at(i))){
        last_nb = true;
        pattern_rle += line.at(i);
      }

      // If it is a <tag>
      else if (line.at(i) == 'b' || line.at(i) == 'o' || line.at(i) == '$'){
        last_nb = false;
        pattern_rle += line.at(i);
      }

      // If it is the end of the pattern
      else if (line.at(i) == '!'){
        // There must be nothing else after the '!' on the line and '!' must not have been preceded by digits
        if (i != line.size() - 1 || last_nb){
          return RleHandler::ERR_RLE;
        }
        pattern_rle += line.at(i);
        // Everything after the '!' is ignored, we can exit the for loop
        break;
      }

      // If we are on a space
      else if (line.at(i) == ' '){
        if (last_nb){
          return RleHandler::ERR_RLE;
        }
      }

      // Otherwise there is an error
      else{
        return RleHandler::ERR_RLE;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Check that the final '!' is here and it is not the first character
  if (pattern_rle.at(pattern_rle.size() - 1) != '!' || pattern_rle.size() == 1){
    return RleHandler::ERR_RLE;
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no error has occured
  return RleHandler::OK;
}

/**
Fill our pattern with the content of pattern_rle, tag by tag
@param run_count Number of occurrences of alive/empty cell contiguous (<run_count>)
@param nb_cell_line Number of alive and empty cells on the current line (to check if it does not exceed width)
@param ind_pattern Current index in our "pattern"
@param cell Boolean to say if we have to put alive or empty cells
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_pattern_filling_tag(int &run_count, int &nb_cell_line, int &ind_pattern, bool &cell){
  //////////////////////////////////////////////////////////////////////////////
  // Filling tag (or tags)

  // Last character was '$' <=> nb_cell_line == 0, because on a new line, nb_cell_line is 0 again
  if (nb_cell_line == 0){
    // Empty cells at the end of a pattern line do not need to be encoded, so we fill what remain
    int rest = ind_pattern % width;
    for (int i = 0; i < rest; i++){
      pattern[ind_pattern] = cell;
      ind_pattern++;
    }
    run_count -= 1;
    // If there was several '$', we fill the remaining lines with empty cells
    for (int i = 0; i < run_count; i++){
      for (int j = 0; j < width; j++){
        pattern[ind_pattern] = cell;
        ind_pattern++;
      }
    }
  }
  // We fill the patern with "run_count" empty or alive cell
  else{
    for (int i = 0; i < run_count; i++){
      pattern[ind_pattern] = cell;
      ind_pattern++;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no errors has occured
  return RleHandler::OK;
}

/**
Fill our pattern with the content of pattern_rle
@param pattern_rle string to contain the pattern without space, carriage returns and '!'
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_pattern_filling(string &pattern_rle){
  //////////////////////////////////////////////////////////////////////////////
  // Create our variables

  // Number of occurrences of alive/empty cell contiguous (<run_count>)
  int run_count = 1;

  // Number of alive and empty cells on the current line (to check if it does not exceed width)
  int nb_cell_line = 0;

  // Number of line (to check if it does not exceed height)
  int nb_lines = 1;

  // Current index in our "pattern"
  int ind_pattern = 0;

  // Index to retrieve data from the "pattern_rle"
  int ind_beg = 0;
  int ind_end = 0;

  // Boolean to say if we have to put alive or empty cells
  bool cell = false;

  //////////////////////////////////////////////////////////////////////////////
  // Go all over the "pattern_rle"
  while (pattern_rle.at(ind_end) != '!'){
    // Retrieve the <run_count>, if there is (if not, run_count is equal to 1)
    if (isdigit(pattern_rle.at(ind_end))){
      ind_end++;
      while (isdigit(pattern_rle.at(ind_end))){
        ind_end++;
      }
      run_count = stoi(pattern_rle.substr(ind_beg, ind_end - ind_beg));
      if (run_count == 0){
        return RleHandler::ERR_RLE;
      }
    }
    else{
      run_count = 1;
    }

    // Retrieve the <tag>
    if (pattern_rle.at(ind_end) == 'b'){
      cell = false;
      nb_cell_line += run_count;
    }
    else if (pattern_rle.at(ind_end) == 'o'){
      cell = true;
      nb_cell_line += run_count;
    }
    else if (pattern_rle.at(ind_end) == '$'){
      if (nb_cell_line > width){
        return RleHandler::ERR_RLE;
      }
      nb_cell_line = 0;
      nb_lines += run_count;
    }

    // Update our index
    ind_end++;
    ind_beg = ind_end;
    if (read_rle_pattern_filling_tag(run_count, nb_cell_line, ind_pattern, cell) == RleHandler::ERR_RLE){
      return RleHandler::ERR_RLE;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no errors has occured
  return RleHandler::OK;
}

/**
Read the pattern of a rle file
@param file Rle file we are reading
@param line Current readed line
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_pattern(ifstream &file, string &line){
  //////////////////////////////////////////////////////////////////////////////
  // Create a string to contain the pattern
  string pattern_rle = "";

  //////////////////////////////////////////////////////////////////////////////
  // Extraction
  if (read_rle_pattern_extraction(file, line, pattern_rle) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  // Filling
  if (read_rle_pattern_filling(pattern_rle) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  //////////////////////////////////////////////////////////////////////////////
  // If no errors has occured
  return RleHandler::OK;
}

/**
Read a rle file for game of life and memorize the pattern
@param file_name Name of the rle file
@return the status, to know if an error has occured
*/
int RleHandler::read_rle_file(string file_name){
  // Clear precedent rle pattern if there was one
  clear();

  // Open the file "file_name"
  ifstream file("patterns/" + file_name);

  // We check if there was no error while opening the file
  if(!file){
    return RleHandler::ERR_OPEN;
  }

  //////////////////////////////////////////////////////////////////////////////
  //variable to contain lines of the rle file
  string line = "";

  // Comment part
  if (read_rle_comment(file, line) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  // Size line part
  if (read_rle_size_line(line) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  // Pattern part
  if (read_rle_pattern(file, line) == RleHandler::ERR_RLE){
    return RleHandler::ERR_RLE;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Don't forget to close the file
  file.close();

  // If no errors has occured, we can return the OK status
  return RleHandler::OK;
}


////////////////////////////////////////////////////////////////////////////////
// RleHandler writing methods
