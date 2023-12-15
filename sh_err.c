#include "shell.h"

/**
 *sh_input - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void sh_input(char *str)
{
	int index = 0;

	if (!str)
		return;
	while (str[index] != '\0')
	{
		sh_putchar(str[index]);
		index++;
	}
}

/**
 * sh_putchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int sh_putchar(char c)
{
	static int index;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(2, buf, index);
		index = 0;
	}
	if (c != BUF_FLUSH)
		buf[index++] = c;
	return (1);
}

/**
 * put_fd - writes the character c to given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int put_fd(char c, int fd)
{
	static int index;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(fd, buf, index);
		index = 0;
	}
	if (c != BUF_FLUSH)
		buf[index++] = c;
	return (1);
}

/**
 *put_st - prints an input string
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of chars put
 */
int put_st(char *str, int fd)
{
	int index = 0;

	if (!str)
		return (0);
	while (*str)
	{
		index += _putfd(*str++, fd);
	}
	return (index);
}