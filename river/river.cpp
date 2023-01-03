#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
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

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */
/* 
  result and l_boat are output parameters output parameter
  result array
    l_miss  l_can
    b_miss  b_can
    r_miss  r_can

*/
Status decipher(const char *left, const char *boat, int result[3][2], bool &l_boat) { 
  // set output parameters to default values
  l_boat = false;

  for (int i = 0 ; i < 3 ; ++i)
    for (int j = 0 ; j < 2 ; ++j)
      result[i][j] = 0;

  // decipher left and boat
  for ( ; *left ; ++left) {
    switch(*left) {
      case 'M':
        ++result[0][0];
        break;
      case 'C':
        ++result[0][1];
        break;
      case 'B':
        if (l_boat)
          return ERROR_INPUT_STREAM_ERROR;
        l_boat = true;
        break;
      default:
        return ERROR_INPUT_STREAM_ERROR;
    }
  }

  for ( ; *boat ; ++boat) {
    switch(*boat) {
      case 'M':
        ++result[1][0];
        break;
      case 'C':
        ++result[1][1];
        break;
      default:
        return ERROR_INVALID_MOVE;
    }
  }

  result[2][0] = 3 - result[0][0] - result[1][0];
  result[2][1] = 3 - result[0][1] - result[1][1];
  return VALID_NONGOAL_STATE;
}

void add_entities_to_scene(char **scene, int result[3][2], bool l_boat) {
  int i;

  // add left bank entities
  for (i = 0 ; i < result[0][0] ; ++ i)
    add_to_scene(scene, 2, 1 + (6 * i), "missionary.txt");
  for (i = 0 ; i < result[0][1] ; ++i)
    add_to_scene(scene, 11, 1 + (6 * i), "cannibal.txt");

  // add right bank entities
  for (i = 0 ; i < result[2][0] ; ++ i)
    add_to_scene(scene, 2, 54 + (6 * i), "missionary.txt");
  for (i = 0 ; i < result[2][1]; ++i)
    add_to_scene(scene, 11, 54 + (6 * i), "cannibal.txt");

  // add boat entities
  for (i = 0 ; i < result[1][0] ; ++i)
    add_to_scene(scene, 11, ((l_boat) ? 22 : 39 ) + (i * 6), "missionary.txt");
  for ( ; i < result[1][0] + result[1][1] ; ++i)
    add_to_scene(scene, 11, ((l_boat) ? 22 : 39 ) + (i * 6), "cannibal.txt");
}


char **make_river_scene(const char *left, const char *boat) {
  assert(strlen(left) < 8);
  assert(strlen(boat) < 3);

  char **scene = create_scene();
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
  add_to_scene(scene, 19, 19, "river.txt");

  // decipher strings
  int result[3][2];
  bool l_boat;

  if (int code = decipher(left, boat, result, l_boat) < 0) {
    cout << '\n' << status_description(code) << "Exiting program.\n\n";
    exit(1);
  }
  
  // add boat
  add_to_scene(scene, 17, (l_boat) ? 19 : 36, "boat.txt");

  add_entities_to_scene(scene, result, l_boat);

  return scene;
}

void reformat_string(char *str) {
  char temp[10];
  int i = 0, j = 0;

  for ( ; str[i] ; ++i) {
    if (str[i] == ' ')
      continue;
    temp[j] = str[i];
    ++j;
  }
  temp[j] = '\0';
  strcpy(str, temp);
}

Status check_state(char *left, const char *boat) {
  int result[3][2];
  bool l_boat;
  Status stat;

  if ((stat = decipher(left, boat, result, l_boat)) < 0)
    return stat;

  if (result[2][0] < 0)
    return ERROR_INVALID_MISSIONARY_COUNT;

  if (result[2][1] < 0)
    return ERROR_INVALID_CANNIBAL_COUNT;

  if (result[0][0] < result[0][1] || result[2][0] < result[2][1])
    return ERROR_MISSIONARIES_EATEN;

  if (result[2][0] == 3)
    return VALID_GOAL_STATE;

  return VALID_NONGOAL_STATE;
}

Status perform_crossing(char *left, const char* target) {
  char **scene;
  bool l_boat = false;
  int i;

  // check arguemnts
  if (strlen(left) > 7 || strlen(left) < 0)
    return ERROR_INPUT_STREAM_ERROR;

  if (strlen(target) > 2 || strlen(target) < 1)
    return ERROR_INVALID_MOVE;

  for (i = 0 ; left[i] ; ++i)
    if (left[i] == 'B') {
      if (l_boat)
        return ERROR_INPUT_STREAM_ERROR;
      l_boat = true;
    }

  // loading boat
  if (l_boat) {
    // if boat at left bank: remove entities in target from left
    for (const char *ptr = target ; *ptr ; ++ptr) {
      for (i = 0; left[i] != *ptr; ++i);
      // left[i] == target
      left[i] = ' ';
    }
    reformat_string(left);
  }
  // if boat at right, do nothing

  // print
  cout << "Loading the boat...\n";
  scene = make_river_scene(left, target);
  print_scene(scene);
  destroy_scene(scene);

  // crossing river
  if (l_boat) {
    // if boat at left: remove 'B' from left
    for (i = 0 ; left[i] != 'B' ; ++i);
    left[i] = ' ';
    reformat_string(left);
  } else {
    // if boat at right: add 'B' to left
    left[strlen(left)] = 'B';
    left[strlen(left) + 1] = '\0';
  }

  // print
  cout << "\nCrossing the river...\n";
  scene = make_river_scene(left, target);
  print_scene(scene);
  destroy_scene(scene);
  
  // unloading boat: boat is empty string
  if (!l_boat) 
    // if boat ar tight, add target to left
    strcat(left, target);
  // if boat at right, no changes

  // print
  cout << "\nUnloading the boat...\n";
  scene = make_river_scene(left, "");
  print_scene(scene);
  destroy_scene(scene);

  return check_state(left, target);
}
