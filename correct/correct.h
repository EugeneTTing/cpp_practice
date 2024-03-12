#ifndef CORRECT_H
#define CORRECT_H

void ascii_to_binary(char letter, char *output);
char binary_to_ascii(char *binary);
void text_to_binary(const char *string, char *binary);
void binary_to_text(char *binary, char *string);
void add_error_correction(char *data, char *corrected);

#endif
