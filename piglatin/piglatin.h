#ifndef PIGLATIN_H
#define PIGLATIN_H

int findFirstVowel(const char *word);

void translateWord(const char *english, char *piglatin);

void translateStream(std::istream &in, std::ostream &out);

#endif