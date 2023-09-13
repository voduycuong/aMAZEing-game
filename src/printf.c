#include "printf.h"

#define MAX_PRINT_SIZE 256

/*
 * va_arg returns the current argument. va_copy, va_start and va_end don't return values.
 */

void printf(char *string, ...)
{
	va_list ap;			  // Type to hold information about variable arguments (type)
	va_start(ap, string); // Initialize a variable argument list (macro)

	char buffer[MAX_PRINT_SIZE];
	int buffer_index = 0;

	char temp_buffer[MAX_PRINT_SIZE];

	int16_t width = 0;
	int16_t precision = 0;
	int16_t zero_flag = 0;
	int16_t width_flag = 0;
	int16_t precision_flag = 0;

	while (1)
	{
		if (*string == 0)
		{
			buffer[buffer_index] = '\0'; // Enclose the buffer
			break;
		}

		if (*string == '%')
		{
			string++;

			if (*string == '*') // Width is not specified in the format string
			{
				width = va_arg(ap, int); // Retrieve next argument (preceding the argument that has to be formatted)
				string++;
			}

			if (*string == '0') // Check for 0 flag
			{
				zero_flag = 1;
				string++;
			}

			// Calculate width
			while (*string >= '0' && *string <= '9')
			{
				width = width * 10 + (*string - '0');
				string++;
			}
			if (width > 0)
				width_flag = 1;

			if (*string == '.') // Check for precision flag
			{
				precision_flag = 1;
				string++;
				if (*string == '*') // Precision is not specified in the format string
				{
					precision = va_arg(ap, int); // Retrieve next argument (preceding the argument that has to be formatted)
					string++;
				}
			}

			// Calculate precision
			while (*string >= '0' && *string <= '9')
			{
				precision = precision * 10 + (*string - '0');
				string++;
			}

			// %d format specifier
			if (*string == 'd')
			{
				string++;
				int x = va_arg(ap, int); // Retrieve next argument
				int temp_index = MAX_PRINT_SIZE - 1;

				// Check for negative number
				if (x < 0)
				{
					buffer[buffer_index++] = '-'; // Put negative sign
					x *= -1;					  // Invert number
				}

				// Convert into char
				do
				{
					temp_buffer[temp_index--] = (x % 10) + '0';
					x /= 10;
				} while (x != 0);

				// Check for format specifier
				format_specifier(zero_flag, width_flag, width, precision_flag, precision, temp_buffer, &temp_index);

				// Load into buffer
				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
					buffer[buffer_index++] = temp_buffer[i];
			}

			// %c format specifier
			else if (*string == 'c')
			{
				string++;
				int x = va_arg(ap, int); // Retrieve next argument
				int temp_index = MAX_PRINT_SIZE - 1;

				temp_buffer[temp_index--] = x;

				// Check for format specifier
				format_specifier(zero_flag, width_flag, width, precision_flag, precision, temp_buffer, &temp_index);

				// Load into buffer
				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
					buffer[buffer_index++] = temp_buffer[i];
			}

			// %s format specifier
			else if (*string == 's')
			{
				string++;
				char *x = va_arg(ap, char *); // Retrieve next argument
				int temp_index = MAX_PRINT_SIZE - 1;

				for (int i = strlen(x) - 1; i >= 0; i--)
					temp_buffer[temp_index--] = x[i];

				// Check for format specifier
				format_specifier(zero_flag, width_flag, width, precision_flag, precision, temp_buffer, &temp_index);

				// Load into buffer
				if (precision_flag == 1 && precision < strlen(x))
					for (int i = temp_index + 1; i < temp_index + 1 + precision; i++)
						buffer[buffer_index++] = temp_buffer[i];
				else
					for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
						buffer[buffer_index++] = temp_buffer[i];
			}

			// %f format specifier
			else if (*string == 'f')
			{
				string++;
				double x = va_arg(ap, double); // Retrieve next argument
				int temp_index = MAX_PRINT_SIZE - 1;

				if (!precision_flag)
					precision = 6;

				// Check for negative number
				if (x < 0)
				{
					buffer[buffer_index++] = '-'; // Put negative sign
					x *= -1;					  // Invert number
				}

				int integer_part = x;					  // Get the integer part
				double decimal_part = (x - integer_part); // Get the decimal part

				// while (decimal_part > 0) // Loop until x is zero
				for (int i = 0; i < precision; i++)
				{
					decimal_part *= 10.0;							// Multiply x by 10
					temp_buffer[temp_index--] = '0' + decimal_part; // Store the first digit
					decimal_part -= (int)decimal_part;				// Remove the first digit
				}

				temp_buffer[temp_index--] = '.'; // Add decimal point

				while (integer_part > 0) // Loop until integer_part is zero
				{
					temp_buffer[temp_index--] = '0' + integer_part % 10; // Store the last digit
					integer_part /= 10;									 // Remove the last digit
				}

				// Check for format specifier
				format_specifier(zero_flag, width_flag, width, precision_flag, precision, temp_buffer, &temp_index);

				// Feed temp_buffer to buffer from left to right
				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
				{
					buffer[buffer_index++] = temp_buffer[i];
					if (temp_buffer[i] == '.')
						break;
				}

				// Feed temp_buffer to buffer from right to left
				for (int i = MAX_PRINT_SIZE - 1; i > temp_index; i--)
				{
					if (temp_buffer[i] == '.')
						break;
					buffer[buffer_index++] = temp_buffer[i];
				}
			}

			// %x format specifier
			else if (*string == 'x')
			{
				string++;
				long int x = va_arg(ap, long int); // Retrieve next argument
												   // long int data type for two's complement output
				int temp_index = MAX_PRINT_SIZE - 1;
				static char hex_char[] = "0123456789ABCDEF";

				if (x < 0)
					x = 4294967295 + x + 1; // Convert to two's complement

				// Convert to hex char
				do
				{
					temp_buffer[temp_index--] = hex_char[x % 16];
					x /= 16;
				} while (x != 0);

				// Check for format specifier
				format_specifier(zero_flag, width_flag, width, precision_flag, precision, temp_buffer, &temp_index);

				// Load into buffer
				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
					buffer[buffer_index++] = temp_buffer[i];
			}

			else if (*string == '%')
				uart_sendc('%');
		}

		// No format specifier
		else
		{
			buffer[buffer_index++] = *string;
			string++;
		}

		// Reach end of the format string
		if (buffer_index == MAX_PRINT_SIZE - 1)
			break;

		// Reset width and precision counting
		width = 0;
		precision = 0;
	}

	va_end(ap); // End using variable argument list

	// Print out the buffer
	uart_puts(buffer);
}

// Function for setup 0-flag, width and precision
void format_specifier(int zero_flag, int width_flag, int width, int precision_flag, int precision, char *buffer, int *index)
{
	int temp = *index;

	// Check for 0 flag or width
	while (MAX_PRINT_SIZE - temp <= width)
	{
		if (zero_flag)
			buffer[temp--] = '0';
		else if (width_flag)
			buffer[temp--] = ' ';
	}

	// Check for precision
	while (MAX_PRINT_SIZE - temp <= precision)
		buffer[temp--] = '0';

	*index = temp;
}