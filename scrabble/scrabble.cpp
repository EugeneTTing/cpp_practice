#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

/* insert your function definitions here */
int tile_score(char tile)
{
    tile = toupper(tile);

    switch (tile)
    {
        case 'A':
        case 'E':
        case 'I':
        case 'L':
        case 'N':
        case 'O':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
            return 1;
            break;

        case 'D':
        case 'G':
            return 2;
            break;
        
        case 'B':
        case 'C':
        case 'M':
        case 'P':
            return 3;
            break;

        case 'F':
        case 'H':
        case 'V':
        case 'W':
        case 'Y':
            return 4;
            break;

        case 'K':
            return 5;
            break;

        case 'J':
        case 'X':
            return 8;
            break;

        case 'Q':
        case 'Z':
            return 10;

        case ' ':
        case '?':
            return 0;
            break;
    
        default:
            return -1;
            break;
    }
}

void add_letter_to_played(char letter, char* played_tiles)
{
    // go to end of played tiles
    while (*played_tiles != '\0')
        ++played_tiles;

    // add letter to played_tiles
    *played_tiles = letter;

    // append sentinel character
    *(played_tiles + 1) = '\0';
}

bool letter_found_in_played(char letter, char* played)
{
    // loop through all chars in played
    for (int i = 0 ; played[i] != '\0' && i < 80 ; ++i)
    {
        // letter is in played
        if (letter == played[i])
        {
            played[i] = '.';
            return true;
        }
    }

    return false;
}

bool letter_found_in_tiles(char letter, const char* tiles, char* played_tiles)
{
    bool joker_found = false;

    // create copy of player_tiles to edit in function
    char played_copy[80];
    strncpy(played_copy, played_tiles, 80);

    // loop through chars in tiles
    for ( ; *tiles != '\0' ; ++tiles)
    {
        // check if current tiles char is in played_copy
        if (letter_found_in_played(*tiles, played_copy))
            continue;

        // current tiles char does not appear in played_tiles
        // check current tiles char against letter
        if (letter == *tiles)
        {
            add_letter_to_played(letter, played_tiles);
            return true;
        }

        if (*tiles == '?')
            joker_found = true;
    }

    if (joker_found)
    {
        // player joker tile
        add_letter_to_played('?', played_tiles);
        return true;
    }

    return false;
    // played_copy deleted here.
}

bool word_recursion(const char* word, const char* tiles, char* played_tiles)
{
    // reached end of word, therefore word can be formed
    if (*word == '\0')
        return true;

    // if current letter can be found in tiles, check next letter
    if (letter_found_in_tiles(*word, tiles, played_tiles))
        if (word_recursion(word + 1, tiles, played_tiles))
            return true;

    // if current letter cannot be found, return false
    return false;
}

bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles)
{
    // reset played_tiles string
    *played_tiles = '\0';

    if (word_recursion(word, tiles, played_tiles))
        return true;

    return false;
}

int compute_score(const char* tiles, ScoreModifier* sm)
{
    int score = 0, word_multiplier = 1;

    // loop through characters in played_tiles
    for (int i = 0 ; tiles[i] != '\0' ; i++)
    {
        int points = tile_score(tiles[i]);

        if (sm[i] == DOUBLE_LETTER_SCORE)
            points *= 2;
        else if (sm[i] == TRIPLE_LETTER_SCORE)
            points *= 3;
        else if (sm[i] == DOUBLE_WORD_SCORE)
            word_multiplier *= 2;
        else if (sm[i] == TRIPLE_WORD_SCORE)
            word_multiplier *= 3;

        score += points;
    }

    score *= word_multiplier;

    if (strlen(tiles) == 7)
        score += 50;

    return score;
}

int highest_scoring_word_from_tiles(const char* tiles, ScoreModifier* sm, char* word)
{
    int highest_score = -1;
    char played_tiles[80];
    char curr_word[80];

    ifstream in;
    in.open("words.txt");

    if (in.fail())
    {
        cout << "File failed to open. Exiting program.\n";
        exit(1);
    }

    in >> curr_word;
    while (!in.eof())
    {
        if (can_form_word_from_tiles(curr_word, tiles, played_tiles))
        {
            int new_score = compute_score(played_tiles, sm);
            if (new_score > highest_score)
            {
                highest_score = new_score;
                strcpy(word, curr_word);
            }
        }
        in >> curr_word;
    }

    in.close();
    return highest_score;
}
