#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @sh_info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t sh_input_buf(info_t *sh_info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)sh_info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(sh_info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			sh_info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(sh_info, *buf, sh_info->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				sh_info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * get_input - gets a line minus the newline
 * @sh_info: parameter struct
 *
 * Return: bytes read
 */
ssize_t sh_getinput(info_t *sh_info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t cindex, nindex, len;
	ssize_t r = 0;
	char **buf_p = &(sh_info->arg), *p;

	_putchar(BUF_FLUSH);
	r = input_buf(sh_info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		nindex = cindex; /* init new iterator to current buf position */
		p = buf + cindex; /* get pointer for return */

		check_chain(sh_info, buf, &nindex, cindex, len);
		while (nindex < len) /* iterate to semicolon or end */
		{
			if (is_chain(sh_info, buf, &nindex))
				break;
			j++;
		}

		cindex = nindex + 1; /* increment past nulled ';'' */
		if (cindex >= len) /* reached end of buffer? */
		{
			cindex = len = 0; /* reset position and length */
			sh_info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_strlen(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _getline() */
	return (r); /* return length of buffer from _getline() */
}

/**
 * read_buf - reads a buffer
 * @sh_info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t sh_readbuf(info_t *sh_info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(sh_info->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - gets the next line of input from STDIN
 * @sh_info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int sh_getline(info_t *sh_info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t cindex, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (cindex == len)
		cindex = len = 0;

	r = read_buf(sh_info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + cindex, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + cindex, k - cindex);
	else
		_strncpy(new_p, buf + cindex, k - cindex + 1);

	s += k - cindex;
	cindex = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * Handler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void Handler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}