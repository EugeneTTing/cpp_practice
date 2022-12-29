#ifndef MAZE_H
#define MAZE_H

/* You are pre-supplied with the functions below. Add your own 
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);

bool find_marker(const char ch, char **maze, const int height, 
   const int width, int &row, int &col);

bool valid_solution(const char *path, char **maze, const int height, 
   const int width);

char *find_path(char **maze, const int height, const int width, 
   const char start, const char end);

#endif