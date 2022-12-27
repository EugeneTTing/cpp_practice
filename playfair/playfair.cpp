#include<iostream>
#include<cctype>
#include"playfair.h"

using namespace std;

void prepare(const char *input, char *output)
{
    int count = 0;

    // loop through all chars in input string
    for ( ; *input ; ++input)
    {
        // if not alphanumeric char, continue to next loop
        if (!isalnum(*input))
            continue;

        // place character in output string, increment count and output pointer
        *output = toupper(*input);
        ++output;
        ++count;
    }

    // num chars in output is odd
    if (count % 2 != 0)
    {
        *output = 'X';
        ++output;
    }

    // NULL teminate output string
    *output = '\0';
}

bool occurs_before(int pos, char ch, const char* word)
{
    for (int i = 0 ; i < pos ; ++i)
        if (word[i] == ch)
            return true;

    return false;
}

void grid(const char *codeword, char square[6][6])
{
    int row = 0, col = 0, i = 0;
    char ch = 'A', num = '0';

    while (row < 6 && col < 6)
    {
        if (codeword[i])
        {
            if (occurs_before(i, codeword[i], codeword))
            {
                ++i;
                continue;
            }
            
            square[row][col] = codeword[i];
            ++i;
        }
        else if (ch <= 'Z')
        {
            if (occurs_before(i, ch, codeword))
            {
                ++ch;
                continue;
            }

            square[row][col] = ch;
            ++ch;
        }
        else
        {
            if (occurs_before(i, num, codeword))
            {
                ++num;
                continue;
            }

            square[row][col] = num;
            ++num;
        }

        ++col;
        if (col > 5)
        {
            col = 0;
            ++row;
        }
    }
}

int find_row(char square[6][6], const char ch)
{
    for (int r = 0 ; r < 6 ; ++r)
        for (int c = 0 ; c < 6 ; ++c)
            if (square[r][c] == ch)
                return r;

    return -1;
}

int find_col(char square[6][6], const char ch)
{
    for (int r = 0 ; r < 6 ; ++r)
        for (int c = 0 ; c < 6 ; ++c)
            if (square[r][c] == ch)
                return c;

    return -1;
}

void bigram(char square[6][6], const char in1, const char in2, char &out1, char &out2)
{
    out1 = square[find_row(square, in1)][find_col(square, in2)];
    out2 = square[find_row(square, in2)][find_col(square, in1)];
}

void encode(char square[6][6], const char *prepared, char *encoded)
{
    if (*prepared == '\0')
    {
        *encoded = '\0';
        return;
    }

    bigram(square, *prepared, *(prepared + 1), *encoded, *(encoded + 1));
    encode(square, prepared + 2, encoded + 2);
}

void decode(char square[6][6], const char *code, char *decoded)
{
    if (!isalnum(*code) && *(code - 1) == '\0')
    {
        *(decoded - 1) = '\0';
        return;
    }

    if (*code == '\0')
    {
        *decoded = '\0';
        return;
    }

    bigram(square, *code, *(code + 1), *decoded, *(decoded + 1));
    decode(square, code + 2, decoded + 2);
}
