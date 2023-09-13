#ifndef _PRINT_H
#define _PRINT_H

#include "./gcclib/stddef.h"
#include "./gcclib/stdint.h"
#include "./gcclib/stdarg.h"
#include "uart.h"
#include "string.h"

void printf(char *string, ...);
void format_specifier(int zero_flag, int width_flag, int width, int precision_flag, int precision, char *buffer, int *index);

#endif