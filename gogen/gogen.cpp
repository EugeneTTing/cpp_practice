#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

#include "common.h"
#include "mask.h"
#include "gogen.h"

using namespace std;

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

/* internal helper function which removes unprintable characters like carriage returns and newlines from strings */
void filter (char *line) {
	while (*line) {
		if (!isprint(*line))
		 *line = '\0';
		line++;
	}
}

/* loads a Gogen board from a file */
char **load_board(const char *filename) {
	char **board = allocate_2D_array(5, 6);
	ifstream input(filename);
	assert(input);
	char buffer[512];
	int lines = 0;
	input.getline(buffer, 512);
	while (input && lines < HEIGHT) {
		filter(buffer);
		if (strlen(buffer) != WIDTH)
			cout << "WARNING bad input = [" << buffer << "]" << endl;
		assert(strlen(buffer) == WIDTH);
		strcpy(board[lines], buffer);
		input.getline(buffer, 512);
		lines++;
	}
	input.close();
	return board;
}

/* saves a Gogen board to a file */
bool save_board(char **board, const char *filename) {
	ofstream out(filename); 
	if (!out)
		return false;
	for (int r=0; r<HEIGHT; r++) {
		for (int c=0; c<WIDTH; c++) {
			out << board[r][c];
		}
		out << endl;
	}
	bool result = out.good();
	out.close();
	return result;
}

/* internal helper function for counting number of words in a file */
int count_words(const char *filename) {
	char word[512];
	int count = 0;
	ifstream in(filename);
	while (in >> word)
		count++;
	in.close();
	return count;
}

/* loads a word list from a file into a NULL-terminated array of char *'s */
char **load_words(const char *filename) {
	int count = count_words(filename);
	ifstream in(filename);
	assert(in);
	int n=0;
	char **buffer = new char *[count+1]; // +1 because we NULL terminate 
	char word[512];
	for (; (in >> word) && n<count; n++) {
		buffer[n] = new char[strlen(word) + 1];
		strcpy(buffer[n], word);
	}
	buffer[n] = NULL;
	in.close();
	return buffer;
}

/* prints a Gogen board in appropriate format */
void print_board(char **board) {
	for (int r=0; r<HEIGHT; r++) {
		for (int c=0; c<WIDTH; c++) {
			cout << "[" << board[r][c] << "]";
			if (c < WIDTH-1)
	cout << "--";
		}
		cout <<endl;
		if (r < HEIGHT-1) {
			cout << " | \\/ | \\/ | \\/ | \\/ |" << endl;
			cout << " | /\\ | /\\ | /\\ | /\\ |" << endl;
		}
	}
}

/* prints a NULL-terminated list of words */
void print_words(char **words) {
	for (int n=0; words[n]; n++) 
		cout << words[n] << endl;
}

/* frees up the memory allocated in load_board(...) */
void delete_board(char **board) {
	deallocate_2D_array(board, HEIGHT);
}

/* frees up the memory allocated in load_words(...) */
void delete_words(char **words) {
	int count = 0;
	for (; words[count]; count++);
	deallocate_2D_array(words, count);
}

/* add your own function definitions here */
bool get_position(char **board, const char ch, int &row, int &column)
{
	for (int r = 0 ; r < 5 ; ++r)
		for (int c = 0 ; c < 5 ; ++c)
			if (board[r][c] == ch)
			{
				row = r;
				column = c;
				return true;
			}

	row = -1;
	column = -1;
	return false;
}

bool valid_solution(char **board, char **words)
{
	// loop through all words
	for ( ; *words != NULL ; ++words)
	{
		int row, col, next_row, next_col;
		char *next_char;

		// get position of first character in word
		get_position(board, **words, row, col);

		// first character not on board
		if (row == -1)
			return false;

		// loop through each character in word
		for (next_char = *words + 1 ; *next_char != '\0' ; ++next_char)
		{
			get_position(board, *next_char, next_row, next_col);
			if (next_row == -1)
				return false;

			if (abs(next_row - row) > 1 || abs(next_col - col) > 1)
				return false;

			row = next_row;
			col = next_col;
		}
	}

	return true;
}

void update(char **board, const char ch, Mask &mask)
{
	int row, col;

	// char is on board
	if (get_position(board, ch, row, col))
	{
		mask.set_all(false);
		mask[row][col] = true;
		return;
	}

	// loop through all cells on board
	for (int r = 0 ; r < 5 ; ++r)
		for (int c = 0 ; c < 5 ; ++c)
			if (isalpha(board[r][c]) && board[r][c] != 'Z')
			{
				// character found on board
				mask[r][c] = false;
			}

	// only one cell with true
	if (mask.count() == 1)
	{
		mask.get_position(true, row, col);
		board[row][col] = ch;
	}
}

void neighbourhood_intersect(Mask &m1, Mask &m2)
{
	m1 *= (m2.neighbourhood(1));
	m2 *= (m1.neighbourhood(1));
}

bool board_solved(char **board)
{
	for (int r = 0 ; r < 5 ; ++r)
		for (int c = 0 ; c < 5 ; ++c)
			if (!isalpha(board[r][c]))
				return false;

	return true;
}

void update_all(char **board, Mask *mask_arr)
{
	for (int i = 0 ; i < 25 ; ++i)
		update(board, 'A' + i, mask_arr[i]);
}

bool recursive_solver(char **board, Mask *mask_arr, int row, int col)
{
	// reached end of board
	if (row == 4 && col == 5)
		return board_solved(board);

	// reached end of row 
	if (col == 5)
	{
		++row;
		col = 0;
	}

	// already letter in position
	if (isalpha(board[row][col]))
		return recursive_solver(board, mask_arr, row, col + 1);

	// position is empty
	// loop through all letters from A-Y
	for (int i = 0 ; i < 25 ; ++i)
	{
		// check if letter is already on board
		if (mask_arr[i].count() == 1)
			continue;

		// check if letter can be in position
		if (!mask_arr[i][row][col])
			continue;

		// letter is not on board, and can be used in position
		// make guess on board 
		board[row][col] = 'A' + i;

		// update all masks based on guess
		update_all(board, mask_arr);

		// call next recursion
		if (recursive_solver(board, mask_arr, row, col + 1))
			return true;

		// reset changes to board
		board[row][col] = '.';

		// reset all masks
		update_all(board, mask_arr);
	}

	return false;
}

bool solve_board(char **board, char **words)
{
	// mask_arr[0] = mask for 'A', mask_arr[24] = mask for 'Y'
	Mask mask_arr[25];

	// initialise all masks in arr
	for (int i = 0 ; i < 25 ; ++i)
		update(board, 'A' + i , mask_arr[i]);

	char* curr_word;
	
	for (int i = 0 ; i < 10 ; ++i)
	{
	// loop through all words
		for (char **word_ptr = words ; *word_ptr != NULL ; ++word_ptr)
		{
			curr_word = *word_ptr;
			// loop through all chars in curr_word
			for ( ; *(curr_word + 1) != '\0' ; ++curr_word)
			{
				neighbourhood_intersect(mask_arr[*curr_word - 'A'], mask_arr[*(curr_word + 1) - 'A']);
				update(board, *curr_word, mask_arr[*curr_word - 'A']);
				update(board, *(curr_word + 1), mask_arr[*(curr_word + 1) - 'A']);
			}
		}

		if (board_solved(board))
			return true;
	}

	return recursive_solver(board, mask_arr, 0, 0);
}