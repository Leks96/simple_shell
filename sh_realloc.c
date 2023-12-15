#include "shell.h"

/**
** _memset - fills memory with a constant byte
* @s: the pointer to the memory area
* @b: the byte to fill *s with
* @n: the amount of bytes to be filled
* Return: (s) a pointer to the memory area s
*/
void *_memset(void *s, int b, size_t n)
{
	unsigned char *p = s;
	size_t i;

	for (i = 0; i < n; i++)
		p[i] = (unsigned char)b;

	return (s);
}

/**
* ffree - frees a string of strings
* @pp: array of strings
*/
void ffree(char **pp)
{
	char **a = pp;

	if (!pp)
		return;

	while (*pp)
		free(*pp++);

	free(a);
}

/**
* _realloc - reallocates a block of memory
* @ptr: pointer to previous malloc'ated block
* @old_size: byte size of previous block
* @new_size: byte size of new block
*
* Return: pointer to da ol'block nameen.
*/
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	/* If ptr is NULL, equivalent to malloc(new_size)*/
	if (!ptr)
		return (malloc(new_size));

	/* If new_size is 0, equivalent to free(ptr) and return NULL */
	if (!new_size)
		return (free(ptr), NULL);

	/* If new_size is equal to old_size, return ptr as it is */
	if (new_size == old_size)
		return (ptr);

	/* Allocate a new memory block with the desired size */
	p = malloc(new_size);

	/* Check if the allocation was successful */
	if (!p)
		return (NULL);

	/* Copy the contents from the old block to the new block */
	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];

	/* Free the old memory block */
	free(ptr);

	/* Return the pointer to the newly allocated memory block */
	return (p);
}
