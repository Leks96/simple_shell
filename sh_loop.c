#include "shell.h"

/**
 * sh_loop - shell loop
 * @sh_info: parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: (0) on success, (1) on error, or error code
 */
int sh_loop(info_t *sh_info, char **av)
{
	ssize_t rtn = 0;
	int return_builtin = 0;

	while (rtn != -1 && return_builtin != -2)
	{
		clear_info(sh_info);
		if (interactive(sh_info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		rtn = get_input(sh_info);
		if (rtn != -1)
		{
			set_info(sh_info, av);
			return_builtin = status_builtin(sh_info);
			if (return_builtin == -1)
				find_cmd(sh_info);
		}
		else if (interactive(sh_info))
			_putchar('\n');
		free_info(sh_info, 0);
	}
	write_history(sh_info);
	free_info(sh_info, 1);
	if (!interactive(sh_info) && sh_info->status)
		exit(sh_info->status);
	if (return_builtin == -2)
	{
		if (sh_info->err_num == -1)
			exit(sh_info->status);
		exit(sh_info->err_num);
	}
	return (return_builtin);
}

/**
 * status_builtin - finds a builtin command
 * @sh_info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int status_builtin(info_t *sh_info)
{
	int i, built_in_ret = -1;
	builtin_table table_builtin[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; table_builtin[i].type; i++)
		if (_strcmp(sh_info->argv[0], table_builtin[i].type) == 0)
		{
			sh_info->line_count++;
			built_in_ret = table_builtin[i].func(sh_info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds command in PATH
 * @sh_info: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(info_t *sh_info)
{
	char *com_path = NULL;
	int i, k;

	sh_info->com_path = sh_info->argv[0];
	if (sh_info->linecount_flag == 1)
	{
		sh_info->line_count++;
		sh_info->linecount_flag = 0;
	}
	for (i = 0, k = 0; sh_info->arg[i]; i++)
		if (!is_delim(sh_info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	com_path = find_com_path(sh_info, _getenv(sh_info, "PATH="), sh_info->argv[0]);
	if (com_path)
	{
		sh_info->com_path = com_path;
		fork_cmd(sh_info);
	}
	else
	{
		if ((interactive(sh_info) || _getenv(sh_info, "PATH=")
			|| sh_info->argv[0][0] == '/') && is_cmd(sh_info, sh_info->argv[0]))
			fork_cmd(sh_info);
		else if (*(sh_info->arg) != '\n')
		{
			sh_info->status = 127;
			print_error(sh_info, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @sh_info: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(info_t *sh_info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(sh_info->com_path, sh_info->argv, get_environ(sh_info)) == -1)
		{
			free_info(sh_info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(sh_info->status));
		if (WIFEXITED(sh_info->status))
		{
			sh_info->status = WEXITSTATUS(sh_info->status);
			if (sh_info->status == 126)
				print_error(sh_info, "Permission denied\n");
		}
	}
}