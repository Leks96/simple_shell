#include "shell.h"

/**
 * sh_interactive - check if shell is interactive mode
 * @sh_info: struct address
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int sh_interactive(info_t *sh_info)
{
	return (isatty(STDIN_FILENO) && sh_info->readfd <= 2);
}

/**
 * is_delim - checks if character is a delimeter
 * @chr: the char to check
 * @delim: the delimeter string
 * Return: 1 if true, 0 if false
 */
int is_delim(char chr, char *delim)
{
	while (*delim)
		if (*delim++ == chr)
			return (1);
	return (0);
}

/**
 *sh_alpha - checks for alphabetic character
 *@c: The character to input
 *Return: 1 if c is alphabetic, 0 otherwise
 */

int sh_alpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 *sh_atoi - converts a string to an integer
 *@string: the string to be converted
 *Return: 0 if no numbers in string, converted number otherwise
 */

int sh_atoi(char *string)
{
	int i, neg_sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0;  string[i] != '\0' && flag != 2; i++)
	{
		if (string[i] == '-')
			neg_sign *= -1;

		if (string[i] >= '0' && string[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (string[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (neg_sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}