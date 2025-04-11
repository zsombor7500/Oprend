#ifndef REGISTRAR_UTILS_H
#define REGISTRAR_UTILS_H

#include <stdio.h>
#include "entry.h"

/* conversions */
entry* convert_to_entry(char* str, int id);

/* input handling */
char*  read_line(FILE* stream);
char*  receive_str(FILE* stream, const char* message);
int    receive_confirmation(FILE* stream, const char* message);
long   receive_long(FILE* stream, const char* message);
void   receive_modify_entry(FILE* stream, entry* e, int required_params);

#endif
