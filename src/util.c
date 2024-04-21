

#include "util.h"

// A utility function to reverse a string
void reverse(char str[], int length)
{
	int start = 0;
	int end = length - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		end--;
		start++;
	}
}

// Implementation of citoa()
char* citoa(int num, char* str, int base)
{

	int i = 0;

	u8 isNegative = 0;
	/* Handle 0 explicitly, otherwise empty string is
	 * printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	// In standard itoa(), negative numbers are handled
	// only with base 10. Otherwise numbers are
	// considered unsigned.
	if (num < 0 && base == 10) {
		isNegative = 1;
		num = -num;
	}

	// Process individual digits
	while (num != 0) {
		int rem =ERAPI_Mod( num , base);
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num =ERAPI_Div( num , base);
	}

	// If number is negative, append '-'
	if (isNegative)
		str[i++] = '-';

	str[i] = '\0'; // Append string terminator

	// Reverse the string
	reverse(str, i);

	return str;
}

#ifdef DEBUG_MGBA
// Emulator debug functions
void mgba_print_init(void)
{
	*REG_DEBUG_ENABLE = 0xC0DE;
}

void mgba_print_string(const char *str)
{
	strcpy(REG_DEBUG_STRING, str);
	*REG_DEBUG_FLAGS = 0x100 | 4; // MGBA_LOG_DEBUG
}

void mgba_print_num(int num)
{
	char str[12];
	citoa(num, &str, 10);
	mgba_print_string(&str);
}

#endif
