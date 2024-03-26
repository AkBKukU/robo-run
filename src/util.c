

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

	/* Handle 0 explicitly, otherwise empty string is
	 * printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	// Process individual digits
	while (num != 0) {
		int rem =ERAPI_Mod( num , base);
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num =ERAPI_Div( num , base);
	}

	str[i] = '\0'; // Append string terminator

	// Reverse the string
	reverse(str, i);

	return str;
}
