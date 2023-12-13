#include "shell.h"

/**
 * clear_info - initializes info_t struct
 * @sh_info: struct address
 */
void init_info(info_t *sh_info)
{
	sh_info->arg = NULL;
	sh_info->argv = NULL;
	sh_info->path = NULL;
	sh_info->argc = 0;
}

/**
 * set_info - initializes info_t struct
 * @sh_info: struct address
 * @av: argument vector
 */
void set_info(info_t *sh_info, char **av)
{
	int i = 0;

	sh_info->fname = av[0];
	if (sh_info->arg)
	{
		sh_info->argv = strtow(sh_info->arg, " \t");
		if (!sh_info->argv)
		{

			sh_info->argv = malloc(sizeof(char *) * 2);
			if (sh_info->argv)
			{
				sh_info->argv[0] = _strdup(sh_info->arg);
				sh_info->argv[1] = NULL;
			}
		}
		for (i = 0; sh_info->argv && sh_info->argv[i]; i++)
			;
		sh_info->argc = i;

		replace_alias(sh_info);
		replace_vars(sh_info);
	}
}

/**
 * free_info - frees info_t struct fields
 * @sh_info: struct address
 * @freeall: true if freeing all fields
 */
void free_info(info_t *sh_info, int freeall)
{
	ffree(sh_info->argv);
	sh_info->argv = NULL;
	sh_info->path = NULL;
	if (freeall)
	{
		if (!sh_info->cmd_buf)
			free(sh_info->arg);
		if (sh_info->env)
			free_list(&(sh_info->env));
		if (sh_info->history)
			free_list(&(sh_info->history));
		if (sh_info->alias)
			free_list(&(sh_info->alias));
		ffree(sh_info->environ);
			sh_info->environ = NULL;
		bfree((void **)sh_info->cmd_buf);
		if (sh_info->readfd > 2)
			close(sh_info->readfd);
		_putchar(BUF_FLUSH);
	}
}