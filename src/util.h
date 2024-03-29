#ifndef UTIL_H
#define UTIL_H
#include "def.h"
#include "erapi.h"

#define RGB(a) ((a>>16)/8 | (((a&0xFF00)>>8)/8)<<5 | ((a&0xFF)/8) << 10)

void reverse(char str[], int length);
char* citoa(int num, char* str, int base);
#endif
