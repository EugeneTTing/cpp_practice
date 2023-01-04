#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' || buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char) ('A' + row) << "|";
  for (int i=0; i<9; i++) 
    cout << ( (data[i]=='.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r);
  cout <<  endl;
  cout << " +---------+" << endl;
  for (int r=0; r<9; r++) 
    print_row(board[r],r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */ 
void initialise_board(char board[9][9]) {
  for (int r=0; r<9; r++)
    for (int c=0; c<9; c++)
      board[r][c] = '?';
}

/* add your functions here */
bool is_complete(const char mine[9][9], const char revealed[9][9]) {
  for (int i = 0 ; i < 9 ; ++i)
    for (int j = 0 ; j < 9 ; ++j) 
      if (mine[i][j] != '*' && revealed[i][j] == '?')
        return false;
    
  return true;
}

int count_mines(const char pos[2], const char mines[9][9]) {
  // decode position string
  int count = 0, r_start, r_end, c_start, c_end;
  int row = pos[0] - 'A';
  int col = pos[1] - '1';

  (row == 0) ? r_start = 0 : r_start = row - 1;
  (col == 0) ? c_start = 0 : c_start = col - 1;
  (row == 8) ? r_end = 8 : r_end = row + 1;
  (col == 8) ? c_end = 8 : c_end = col + 1;

  for (int i = r_start ; i < r_end + 1 ; ++i)
    for (int j =  c_start ; j < c_end + 1 ; ++j)
      if (mines[i][j] == '*')
        ++count;

  return count;
}

void uncover(const char mines [9][9], char revealed[9][9], const char *pos) {
  int row = pos[0] - 'A';
  int col = pos[1] - '1';

  if (row < 0 || row > 8 || col < 0 || col > 8)
    return;

  if (mines[row][col] == '*')
    return;

  if (isdigit(revealed[row][col]) || revealed[row][col] == ' ')
    return;

  int count = count_mines(pos, mines);

  if (count == 0)
    revealed[row][col] = ' ';
  else {
    revealed[row][col] = '1' - 1 + count;
  }

  char new_pos[2];

  new_pos[0] = pos[0] - 1;
  new_pos[1] = pos[1];
  uncover(mines, revealed, new_pos);

  new_pos[0] = pos[0] + 1;
  uncover(mines, revealed, new_pos);

  new_pos[0] = pos[0];
  new_pos[1] = pos[1] - 1;
  uncover(mines, revealed, new_pos);

  new_pos[1] = pos[1] + 1;
  uncover(mines, revealed, new_pos);
}

MoveResult make_move(const char *pos, const char mines [9][9], char revealed[9][9]) {
  // check validity of move
  int row = pos[0] - 'A';
  int col = pos[1] - '1';

  if (row < 0 || row > 8 || col < 0 || col > 8)
    return INVALID_MOVE;
  if (pos[2] != '\0' && pos[2] != '*')
    return INVALID_MOVE;

  // check redundancy
  if (isdigit(revealed[row][col]) || revealed[row][col] == ' ' || revealed[row][col] == '*')
    return REDUNDANT_MOVE;

  if (pos[2] == '\0' && mines[row][col] == '*')
    return BLOWN_UP;

  if (pos[2] == '*') {
    revealed[row][col] = '*';
    return VALID_MOVE;
  }

  uncover(mines, revealed, pos);

  if (is_complete(mines, revealed))
    return SOLVED_BOARD;

  return VALID_MOVE;
}

int count_flag(const char revealed[9][9], const int row, const int col) {
  int count = 0, r_start, r_end, c_start, c_end;

  (row == 0) ? r_start = 0 : r_start = row - 1;
  (col == 0) ? c_start = 0 : c_start = col - 1;
  (row == 8) ? r_end = 8 : r_end = row + 1;
  (col == 8) ? c_end = 8 : c_end = col + 1;

  for (int i = r_start ; i < r_end + 1 ; ++i)
    for (int j =  c_start ; j < c_end + 1 ; ++j)
      if (revealed[i][j] == '*')
        ++count;

  return count;
}

int count_unrevealed(const char revealed[9][9], const int row, const int col) {
  int count = 0, r_start, r_end, c_start, c_end;

  (row == 0) ? r_start = 0 : r_start = row - 1;
  (col == 0) ? c_start = 0 : c_start = col - 1;
  (row == 8) ? r_end = 8 : r_end = row + 1;
  (col == 8) ? c_end = 8 : c_end = col + 1;

  for (int i = r_start ; i < r_end + 1 ; ++i)
    for (int j =  c_start ; j < c_end + 1 ; ++j)
      if (revealed[i][j] == '?')
        ++count;

  return count;
}

void closest_unrevealed(char *string, const char revealed[9][9], const int row, const int col) {
int r_start, r_end, c_start, c_end;

  (row == 0) ? r_start = 0 : r_start = row - 1;
  (col == 0) ? c_start = 0 : c_start = col - 1;
  (row == 8) ? r_end = 8 : r_end = row + 1;
  (col == 8) ? c_end = 8 : c_end = col + 1;

  for (int i = r_start ; i < r_end + 1 ; ++i)
    for (int j =  c_start ; j < c_end + 1 ; ++j)
      if (revealed[i][j] == '?') {
        string[0] = 'A' + i;
        string[1] = '1' + j;
        return;
      }
}

bool find_safe_move(const char revealed[9][9], char *move) {
  int row, col;
  move[0] = '\0';

  for (row = 0 ; row < 9 ; ++row)
    for (col = 0 ; col < 9 ; ++col) {

      if (!isdigit(revealed[row][col]))
        continue;
      
      // revealed[row][col] is a number char

      if (revealed[row][col] - '1' + 1 == count_flag(revealed, row, col) 
        && count_unrevealed(revealed, row, col) > 0) {
        // unreveal first adjacent unrevealed square
        closest_unrevealed(move, revealed, row, col);
        move[2] = '\0';
        return true;
      }

      if (revealed[row][col] - '1' + 1 == count_unrevealed(revealed, row, col) 
        + count_flag(revealed, row, col) && count_unrevealed(revealed, row, col) > 0) {
        // flag first adjacent unrevealed square
        closest_unrevealed(move, revealed, row, col);
        move[2] = '*';
        move[3] = '\0';
        return true;
      }
    }

  return false;
}