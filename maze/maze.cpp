#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

bool find_marker(const char ch, char **maze, const int height,
  const int width, int &row, int &col)
{
  // loop through all chars in maze
  for (row = 0 ; row < height ; ++row)
    for (col = 0 ; col < width ; ++col)
      if (maze[row][col] == ch)
        return true;

  row = -1;
  col = -1;
  return false;
}

bool valid_solution(const char *path, char **maze, const int height, 
  const int width)
{
  int row, col;
  if (!find_marker('>', maze, height, width, row, col))
    return false;

  // start maze from position (row, col)
  for ( ; *path; ++path)
  {
    switch(*path) 
    {
      case 'E':
        ++col; 
        break;
      case 'S':
        ++row;
        break;
      case 'W':
        --col;
        break;
      case 'N':
        --row;
        break;
      default:
        return false;
    }

    // check for out of bounds
    if (row < 0 || row >= height)
      return false;

    if (col < 0 || col >= width)
      return false;

    if (!(maze[row][col] == ' ' || maze[row][col] == 'X'))
      return false;
  }

  return maze[row][col] == 'X';
}

bool solver(char **maze, int row, int col, char *string, const char end,
  const int height, const int width)
{
  if (maze[row][col] == end)
  {
    maze[row][col] = '#';
    *string = '\0';
    return true;
  }

  if (maze[row][col] != ' ')
    return false;

  if (row < 0 || row >= height)
    return false;

  if (col < 0 || col >= width)
    return false;

  // maze[row][col] is a valid tile to move to
  maze[row][col] = '#';

  if (solver(maze, row, col + 1, string + 1, end, height, width))
  {
    *string = 'E';
    return true;
  }

  if (solver(maze, row, col - 1, string + 1, end, height, width))
  {
    *string = 'W';
    return true;
  }

  if (solver(maze, row + 1, col, string + 1, end, height, width))
  {
    *string = 'S';
    return true;
  }

  if (solver(maze, row - 1, col, string + 1, end, height, width))
  {
    *string = 'N';
    return true;
  }

  maze[row][col] = ' ';
  return false;
}

char *find_path(char **maze, const int height, const int width, 
  const char start, const char end)
{
  char *solution = new char[64];

  int row, col;
  if (!find_marker(start, maze, height, width, row, col))
  {
    strcpy(solution, "no solution");
    return solution;
  }

  maze[row][col] = ' ';
  if (!solver(maze, row, col, solution, end, height, width))
    strcpy(solution, "no solution");
  
  return solution;
}