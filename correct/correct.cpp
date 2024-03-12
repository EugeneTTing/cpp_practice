#include <iostream>

using namespace std;

/* You are supplied with two helper functions */

/* converts a character into a binary string representation */
void ascii_to_binary(char ch, char *binary);

/* converts a binary string representation into a character */
char binary_to_ascii(char *binary);

void ascii_to_binary(char ch, char *binary) {
  for (int n = 128; n; n >>= 1) 
    *binary++ = (ch & n) ? '1' : '0';
  *binary = '\0';
}

char binary_to_ascii(char *binary) {
  int ch = 0;
  for (int n=0, slide=128; n<8; n++, slide >>= 1) {
    if (binary[n] == '1')
      ch = ch | slide;
  }
  return ch;
}

/* now add your own functions here */
void text_to_binary(const char *string, char *binary) {

  if (*string == '\0') {
    *binary = '\0';
    return;
  }

  ascii_to_binary(*string, binary);
  text_to_binary(string + 1, binary + 8);
}

void binary_to_text(char *binary, char *string) {

  if (*binary == '\0') {
    *string = '\0';
    return;
  }

  *string = binary_to_ascii(binary);
  binary_to_text(binary + 8, string + 1);
}

char parity(char one, char two, char three) {

  int sum = (one - '0') + (two - '0') + (three - '0');

  if (sum % 2 == 0) // even 
    return '0';
  else 
    return '1';
}

void add_error_correction(char *data, char *corrected) {

  if (*data == '\0') {
    *corrected = '\0';
    return;
  }

  corrected[2] = data[0];
  corrected[4] = data[1];
  corrected[5] = data[2];
  corrected[6] = data[3];
  corrected[0] = parity(data[0], data[1], data[3]);
  corrected[1] = parity(data[0], data[2], data[3]);
  corrected[3] = parity(data[1], data[2], data[3]);

  add_error_correction(data + 4, corrected + 7);
}