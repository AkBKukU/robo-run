#ifndef UTIL_H
#define UTIL_H
#include "def.h"
#include "erapi.h"

#define RGB(a) ((a>>16)/8 | (((a&0xFF00)>>8)/8)<<5 | ((a&0xFF)/8) << 10)

void reverse(char str[], int length);
char* citoa(int num, char* str, int base);

#define REG_DEBUG_ENABLE (vu16*) 0x4FFF780
#define REG_DEBUG_FLAGS (vu16*) 0x4FFF700
#define REG_DEBUG_STRING (char*) 0x4FFF600

//void mgba_print_init(void);

//void mgba_print_string(const char *str);

#endif
