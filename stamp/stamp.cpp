#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */
int leading_zeros(const char *digest) {

  int count = 0;

  for ( ; *digest ; ++digest) {

    if (!isxdigit(*digest))
      return -1;

    if (*digest == '0') {
      ++count;
      continue;
    }

    return count;
  }

  return count;
}

bool file_to_SHA1_digest(const char *filename, char *header) {

  ifstream in;
  char message[100000];
  in.open(filename);

  if (in.fail()) {
    strcpy(header, "error");
    in.close();
    return false;
  }

  in.getline(message, 100000, '\0');
  text_to_SHA1_digest(message, header);
  in.close();
  
  return true;
}

bool make_header(const char *recipient, const char *filename, char *header) {

  char message_digest[41], counter_char[10], header_digest[41];

  // create message digest
  if (!file_to_SHA1_digest(filename, message_digest))
    return false;

  for (int counter = 0 ; counter < 10000000 ; ++counter) {

    // clear header cstring for each loop
    strcpy(header, "");

    // convert counter to cstring
    sprintf(counter_char, "%d", counter);

    // append recipient, message body digest, and counter to header
    strncat(header, recipient, strlen(recipient));
    strcat(header, ":");
    strncat(header, message_digest, strlen(message_digest));
    strcat(header, ":");
    strncat(header, counter_char, strlen(counter_char));

    // create header digest
    text_to_SHA1_digest(header, header_digest);

    // check number of leading zeroes in header digest
    if (leading_zeros(header_digest) > 4)
      return true;

  }
    
  return false;
}

MessageStatus check_header(const char *email, const char *header, const char *filename) {

  char header_digest[41], email_from_header[64], digest_from_header[41], message_digest[41];
  int num_colon = 0, i = 0;

  // confirm header digest leading zeros
  text_to_SHA1_digest(header, header_digest);

  if (leading_zeros(header_digest) < 5)
    return INVALID_HEADER_DIGEST;

  // check header format
  for (const char *temp = header; *temp ; ++temp) 
    if (*temp == ':')
      ++num_colon;

  if (num_colon != 2)
    return INVALID_HEADER;

  // extract email header
  for ( ; *header != ':' ; ++header) {
    email_from_header[i] = *header;
    ++i;
  } // *header is the first ':'
  email_from_header[i] = '\0';
  
  // compare recipient email addresses
  if (strcmp(email_from_header, email))
    return WRONG_RECIPIENT;

  ++header; // *header is the first digit of message digest

  // extract message digest from header
  for (int j = 0 ; j < 40 ; ++j) {
    digest_from_header[j] = *header;
    ++header;
  } // *header should be second colon
  digest_from_header[40] = '\0';

  if (*header != ':')
    return INVALID_HEADER;

  // check message digest
  file_to_SHA1_digest(filename, message_digest);
  if (strcmp(digest_from_header, message_digest))
    return INVALID_MESSAGE_DIGEST;

  // all checks passed
  return VALID_EMAIL;
}