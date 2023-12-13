#include "shell.h"

/**
 **sh_membyte - fills memory with a constant byte
 *@ptr: represents a pointer to the memory area
 *@membyte: the byte to fill memory
 *@nbyte: holds the number of bytes to be filled
 *Return: (s) a pointer to the memory area s
 */
char *sh_membyte(char *ptr, char membyte, unsigned int nbyte)
{
	unsigned int i;

	for (i = 0; i < nbyte; i++)
		ptr[i] = membyte;
	return (ptr);
}

/**
 * free_string - frees a string of strings
 * @str: string of strings
 */
void free_string(char **str)
{
	char **a = str;

	if (!str)
		return;
	while (*str)
		free(*str++);
	free(a);
}

/**
 * mem_realloc - reallocates a block of memory
 * @ptr: pointer to previous malloc'ated block
 * @old_byte: byte size of previous block
 * @new_byte: byte size of new block
 *
 * Return: pointer to da ol'block nameen.
 */
void *mem_realloc(void *ptr, unsigned int old_byte, unsigned int new_byte)
{
	char *p;

	if (!ptr)
		return (malloc(new_byte));
	if (!new_byte)
		return (free(ptr), NULL);
	if (new_byte == old_byte)
		return (ptr);

	p = malloc(new_byte);
	if (!p)
		return (NULL);

	old_byte = old_byte < new_byte ? old_byte : new_byte;
	while (old_byte--)
		p[old_byte] = ((char *)ptr)[old_byte];
	free(ptr);
	return (p);
}