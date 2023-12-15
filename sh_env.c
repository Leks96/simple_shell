#include "shell.h"

/**
 * sh_env - prints current environment
 * @sh_info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int sh_env(info_t *sh_info)
{
	print_list_str(sh_info->env);
	return (0);
}

/**
 * get_sh_env - gets the value of an environment variable
 * @sh_info: Structure containing potential arguments. Used to maintain
 * @envName: environment Variable name
 *
 * Return: the value
 */
char *get_sh_env(info_t *sh_info, const char *envName)
{
	list_t *envNode = sh_info->env;
	char *p;

	while (envNode)
	{
		p = starts_with(envNode->str, envName);
		if (p && *p)
			return (p);
		envNode = envNode->next;
	}
	return (NULL);
}

/**
 * set_sh_env - Initialize a new environment variable,
 *             or modify an existing one
 * @sh_info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int set_sh_env(info_t *sh_info)
{
	if (sh_info->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (sh_setenv(sh_info, sh_info->argv[1], sh_info->argv[2]))
		return (0);
	return (1);
}

/**
 * unset_sh_env - Remove an environment variable
 * @sh_info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int unset_sh_env(info_t *sh_info)
{
	int i;

	if (sh_info->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= sh_info->argc; i++)
		sh_unsetenv(sh_info, sh_info->argv[i]);

	return (0);
}

/**
 * sh_env_list - populates environments linked list
 * @sh_info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int sh_env_list(info_t *sh_info)
{
	list_t *envNode = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_envNode_end(&envNode, environ[i], 0);
	sh_info->env = envNode;
	return (0);
}
