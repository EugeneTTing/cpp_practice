#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
   static Dictionary dictionary("words.txt");
   return dictionary.search(word);
}

/* add your function definitions here */
bool valid_step(const char *curr_word, const char *next_word) {

   int count = 0;

   // check words are same length
   if (strlen(curr_word) != strlen(next_word))
      return false;
   
   // check one char differs between words
   for (int i = 0 ; i < strlen(curr_word) ; ++i) 
      if (curr_word[i] != next_word[i])
         ++count;

   if (count != 1)
      return false;

   // check both words are in dictionary
   return (dictionary_search(curr_word) && dictionary_search(next_word));
}

bool display_chain(const char **chain, std::ostream &out) {

   // count words in chain
   int count, i = 0;
   for (count = 0 ; chain[count] != NULL ; ++count);
   // chain[count] = NULL

   // *chain[0] and *chain[count - 1] should be printed in capitals
   out << chain[i] << '\n';

   for (i = 1 ; i < count - 1; ++i) {
      for (int j = 0 ; chain[i][j] ; ++j)
         out << (char) tolower(chain[i][j]);
      out << '\n';
   } // i = count - 1

   out << chain[i] << '\n';

   return i == count - 1;
}

bool duplicates_found(const char **chain, const int count) {

   for (int i = 0 ; i < count ; ++i)
      for (int j = i + 1 ; j < count ; ++j)
         if (!strcmp(chain[i], chain[j])) // words are equal
            return true;

   return false;
}

bool valid_chain(const char **chain) {

   // count words in chain
   int count, i = 0;
   for (count = 0 ; chain[count] != NULL ; ++count);

   // chain must contain two or more words
   if (count < 2)
      return false;

   // check for duplicates
   if (duplicates_found(chain, count))
      return false;

   // check validity between steps
   for ( ; i < count - 2 ; ++i) 
      if (!valid_step(chain[i], chain[i + 1]))
         return false;
   
   return true;
}

bool recursive_solver(const char **chain, const char *target, const int max_steps, int curr_step) {

   // reached max depth 
   if (curr_step == max_steps) {
      if (valid_step(chain[curr_step - 1], chain[curr_step]) && valid_chain(chain)) {
         return true;
      }
      else {
         return false;
       }
   }

   // try to find valid_step(chain[curr_step - 1], chain[curr_step])
   char *word = new char[strlen(target) + 1];
   strcpy(word, chain[curr_step - 1]);

   for (int i = 0 ; i < strlen(target) ; ++i) {
      for (char ch = 'A' ; ch <= 'Z' ; ++ch) {

         word[i] = ch;

         // target rached before max_steps
         if (!strcmp(target, word)) {
            chain[curr_step] = (const char *) word;
            chain[curr_step + 1] = NULL;
            return true;
         }

         if (valid_step(chain[curr_step - 1], word)) {

            // chain[curr_step] = word
            chain[curr_step] = (const char *) word;

            if (!duplicates_found(chain, curr_step + 1))
               if (recursive_solver(chain, target, max_steps, curr_step + 1))
                  return true;
         }
      }
      strcpy(word, chain[curr_step - 1]);
   }

   delete [] word;
   return false;
}

bool find_chain(const char *start, const char *target, const char **chain, const int max_steps) {


   if (strlen(target) != strlen(start))
      return false;

   // initiliase chain
   chain[0] = start;
   chain[max_steps] = target;
   chain[max_steps + 1] = NULL;

   return recursive_solver(chain, target, max_steps, 1);
}