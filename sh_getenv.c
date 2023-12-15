#include "shell.h"

/**
 * sh_getenv - returns the string array copy of our environ
 * @sh_info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **sh_getenv(info_t *sh_info)
{
	if (!sh_info->environ || sh_info->env_changed)
	{
		sh_info->environ = list_to_strings(sh_info->env);
		sh_info->env_changed = 0;
	}

	return (sh_info->environ);
}

/**
 * sh_unsetenv - Remove an environment ariable
 * @sh_info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: 1 on delete, 0 otherwise
 * @envar: the string env var property
 */
int sh_unsetenv(info_t *sh_info, char *envar)
{
	list_t *envNode = sh_info->env;
	size_t i = 0;
	char *p;

	if (!envNode || !envar)
		return (0);

	while (envNode)
	{
		p = starts_with(envNode->str, envar);
		if (p && *p == '=')
		{
			sh_info->env_changed = delete_envNode_at_index(&(sh_info->env), i);
			i = 0;
			envNode = sh_info->env;
			continue;
		}
		envNode = envNode->next;
		i++;
	}
	return (sh_info->env_changed);
}

/**
 * sh_setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @sh_info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @envar: the string environment variable property
 * @enval: the string environment variable value
 *  Return: Always 0
 */
int sh_setenv(info_t *sh_info, char *envar, char *enval)
{
	char *buf = NULL;
	list_t *envNode;
	char *p;

	if (!envar || !enval)
		return (0);

	buf = malloc(_strlen(envar) + _strlen(enval) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, envar);
	_strcat(buf, "=");
	_strcat(buf, enval);
	envNode = sh_info->env;
	while (envNode)
	{
		p = starts_with(envNode->str, envar);
		if (p && *p == '=')
		{
			free(envNode->str);
			envNode->str = buf;
			sh_info->env_changed = 1;
			return (0);
		}
		envNode = envNode->next;
	}
	add_envNode_end(&(sh_info->env), buf, 0);
	free(buf);
	sh_info->env_changed = 1;
	return (0);
}
