#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
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

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}

bool get_symbol_position(char **map, const int height, const int width, const char target, int &row, int &col) {
  for (int i = 0 ; i < height ; ++i)
    for (int j = 0 ; j < width ; ++j)
      if (map[i][j] == target) {
        row = i;
        col = j;
        return true;
      }

  row = -1;
  col = -1;
  return false;
}

char get_symbol_for_station_or_line(const char *name) {
  ifstream in;
  char symbol, line_from_file[128];
  const char *txt_files[] = {"lines.txt", "stations.txt"};

  for (int i = 0 ; i < 2 ; ++i) {
    in.open(txt_files[i]);

    while (!in.eof()) {
      in >> symbol;
      in.ignore(1);
      in.getline(line_from_file, 128);
      if (!strcmp(line_from_file, name))
        return symbol;
    }

    in.close();
  }

  symbol = ' ';
  return symbol;
}

char *get_step_from_route(char *step, char *route) {
  for ( ; *route != ',' ; ++route) {

    if (*route == '\0') {
      *step = '\0';
      return NULL;
    }

    *step = *route;
    ++step;
  }

  // *route == ','
  *step = '\0';
  ++route;

  // *route is the first char of the nest step
  return route;
}

bool is_backtracking(Direction prev, Direction curr) {
  switch (curr) {
    case N:
      if (prev == S) {
        return true; 
      }
        break;
    case S:
      if (prev == N) {
        return true; 
      }
        break;
    case E:
      if (prev == W) {
        return true; 
      }
        break;
    case W:
      if (prev == E) {
        return true; 
      }
        break;
    case NE:
      if (prev == SW) {
        return true; 
      }
        break;
    case NW:
      if (prev == SE) {
        return true; 
      }
        break;
    case SE:
      if (prev == NW) {
        return true; 
      }
        break;
    case SW:
      if (prev == NE) {
        return true; 
      }
        break;
  }

  return false;
}

char *get_station_from_symbol(char symbol) {
  ifstream in;
  char sym_from_file = ' ';
  char *station = new char [128];

  in.open("stations.txt");
  while (sym_from_file != symbol) {
    in >> sym_from_file;
    in.ignore(1);
    in.getline(station, 128);
  }

  return station;
}

int validate_route(char **map, const int height, const int width, const char *start, char *route, char *destination) {
  char step[10], symbol = get_symbol_for_station_or_line(start), 
  prev_symbol = ' ', prev_line = ' ';
  int row, col, changes = 0;
  Direction prev_direction, curr_direction = INVALID_DIRECTION;

  // check start
  if (!isalnum(symbol))
    return ERROR_START_STATION_INVALID;

  // get position for start
  get_symbol_position(map, height, width, symbol, row, col);

  // get steps from route
  while (route != NULL) {
    route = get_step_from_route(step, route);

    if (curr_direction != INVALID_DIRECTION)
      prev_direction = curr_direction;
    curr_direction = string_to_direction(step);

    // check each step is valid
    if (curr_direction == INVALID_DIRECTION)
      return ERROR_INVALID_DIRECTION;

    // check backtracing between stations
    // back tracking allowed if at a station
    if (!isalnum(symbol))
      if (is_backtracking(prev_direction, curr_direction))
        return ERROR_BACKTRACKING_BETWEEN_STATIONS;

    // change row col based on direction
    switch (curr_direction) {
      case N:
        --row; 
        break;
      case E:
        ++col;
        break;
      case S:
        ++row;
        break;
      case W:
        --col;
        break;
      case NE:
        --row;
        ++col;
        break;
      case NW:
        --row;
        --col;
        break;
      case SE:
        ++row;
        ++col;
        break;
      case SW:
        ++row;
        --col;
        break;
    }

    // check out of bounds
    if (row > height || row < 0 || col > width || col < 0)
      return ERROR_OUT_OF_BOUNDS;

    // change symbol and prev_symbol
    if (prev_symbol != ' ') 
      prev_symbol = symbol;
    symbol = map[row][col];

    // check off track
    if (symbol == ' ')
      return ERROR_OFF_TRACK;


    // check if line change occurs
    if (prev_line == ' ')
      prev_line = symbol;

    if (symbol != prev_line && !isalnum(symbol))
      ++changes;

    // update prev_line
    if (!isalnum(symbol) && prev_line != symbol)
      prev_line = symbol;
      
    // check line hopping
    if (prev_line != symbol && !isalnum(prev_symbol))
      return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
  }

  // symbol is end point of route
  if (!isalnum(symbol))
    return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;

  // get station from symbol
  char *station = get_station_from_symbol(symbol);
  strcpy(destination, station);
  delete [] station;

  return changes;
}