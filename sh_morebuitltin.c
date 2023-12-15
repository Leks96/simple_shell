#include "shell.h"

/**
 * sh_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @sh_info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int sh_history(info_t *sh_info)
{
	print_list(sh_info->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @sh_info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(info_t *sh_info, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(sh_info->alias),
		get_node_index(sh_info->alias, node_starts_with(sh_info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @sh_info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *sh_info, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(sh_info, str));

	unset_alias(sh_info, str);
	return (add_node_end(&(sh_info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *p = NULL, *strt = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (strt = node->str; strt <= p; a++)
			_putchar(*strt);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * sh_alias - mimics the alias builtin (man alias)
 * @sh_info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int sh_alias(info_t *sh_info)
{
	int index = 0;
	char *p = NULL;
	list_t *aliasNode = NULL;

	if (sh_info->argc == 1)
	{
		aliasNode = sh_info->alias;
		while (aliasNode)
		{
			print_alias(aliasNode);
			aliasNode = aliasNode->next;
		}
		return (0);
	}
	for (index = 1; sh_info->argv[index]; index++)
	{
		p = _strchr(sh_info->argv[index], '=');
		if (p)
			set_alias(sh_info, sh_info->argv[index]);
		else
			print_alias(aliasNode(sh_info->alias, sh_info->argv[index], '='));
	}

	return (0);
}