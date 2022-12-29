#include<iostream>
#include<cctype>

using namespace std;

int findFirstVowel(const char *word)
{
    for (int i = 0 ; word[i] ; ++i)
    {
        char ch = tolower(word[i]);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u')
            return i;

        if (ch == 'y' && i != 0 && word[i + 1] != '\0')
            return i;
    }

    return -1;
}

void translateWord(const char *english, char *piglatin)
{
    while (!isalnum(*english))
        *piglatin++ = *english++;
    
    bool capital = isupper(*english);
    int pos = findFirstVowel(english);
    int i;
    
    // no capitals found, copy characters normally
    if (pos == -1)
    {
        for ( ; *english ; ++english)
            *piglatin++ = *english;
        
        return;
    }

    // copy first capital
    *piglatin++ = (capital) ? toupper(english[pos]) : english[pos];

    // copy rest of alnum characters
    for (i = pos + 1 ; isalnum(english[i]) ; ++i)
        *piglatin++ = tolower(english[i]);

    for (int j = 0 ; j < pos ; ++j)
        *piglatin++ = tolower(english[j]);

    // append "ay"
    *piglatin++ = 'a';
    *piglatin++ = 'y';

    // check whether english[i] is '\0'
    while (english[i])
        *piglatin++ = english[i++];

    *piglatin = '\0';
}

void translateStream(std::istream &in, std::ostream &out)
{
    char english_word[64], piglatin_word[66], ch;

    if (in.eof())
    {
        out << '\0';
        return;
    }

    in >> english_word;

    translateWord(english_word, piglatin_word);
    out << piglatin_word;

    in.get(ch);

    while (isspace(in.peek()))
        in.get(ch);

    out << ch;
    translateStream(in, out);
}