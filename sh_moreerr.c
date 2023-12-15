#include "shell.h"

/**
 * error_atoi - converts a string to an integer
 * @str: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int error_atoi(char *str)
{
	int index = 0;
	unsigned long int result = 0;

	if (*str == '+')
		str++;
	for (index = 0;  str[index] != '\0'; index++)
	{
		if (str[index] >= '0' && str[index] <= '9')
		{
			result *= 10;
			result += (str[index] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * print_error - prints an error message
 * @sh_info: the parameter & return info struct
 * @estr: string containing specified error type
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on error
 */
void print_error(info_t *sh_info, char *estr)
{
	sh_input(sh_info->fname);
	sh_input(": ");
	print_dec(sh_info->line_count, STDERR_FILENO);
	sh_input(": ");
	sh_input(sh_info->argv[0]);
	sh_input(": ");
	sh_input(estr);
}

/**
 * print_dec - prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the file descriptor to write to
 *
 * Return: number of characters printed
 */
int print_dec(int input, int fd)
{
	int (*out_putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int abs_value, curr_digit;

	if (fd == STDERR_FILENO)
		out_putchar = sh_putchar;
	if (input < 0)
	{
		abs_value = -input;
		out_putchar('-');
		count++;
	}
	else
		abs_value = input;
	curr_digit = abs_value;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (abs_value / i)
		{
			out_putchar('0' + curr_digit / i);
			count++;
		}
		curr_digit %= i;
	}
	out_putchar('0' + curr_digit);
	count++;

	return (count);
}

/**
 * convert_num - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *convert_num(long int num, int base, int flags)
{
	static char *array;
	static char buffer[120];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * cancel_comments - function replaces first instance of '#' with '\0'
 * @buf: address of the string to modify
 *
 * Return: Always 0;
 */
void cancel_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}