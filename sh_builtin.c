#include "shell.h"

/**
 * sh_exit - function to exit the shell
 * @sh_info: Structure containing arguments for maintaining function prototype.
 *  Return: exits with a given exit status
 *         (0) if sh_info.argv[0] != "exit"
 */
int sh_exit(info_t *sh_info)
{
	int exitstatus;

	if (sh_info->argv[1])  /* If there is an exit arguement */
	{
		exitstatus = _erratoi(sh_info->argv[1]);
		if (exitstatus == -1)
		{
			sh_info->status = 2;
			print_error(sh_info, "Illegal number: ");
			_eputs(sh_info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		sh_info->err_num = _erratoi(sh_info->argv[1]);
		return (-2);
	}
	sh_info->err_num = -1;
	return (-2);
}

/**
 * sh_cd - changes the current directory of the process
 * @sh_info: Structure containing arguments for maintaining
 *          function prototype.
 *  Return: Always 0
 */
int sh_cd(info_t *sh_info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	currdir = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!sh_info->argv[1])
	{
		dir = _getenv(sh_info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getenv(sh_info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(sh_info->argv[1], "-") == 0)
	{
		if (!_getenv(sh_info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(sh_info, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getenv(sh_info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(sh_info->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(sh_info, "can't cd to ");
		_eputs(sh_info->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(sh_info, "OLDPWD", _getenv(sh_info, "PWD="));
		_setenv(sh_info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * sh_help - changes the current directory of the process
 * @sh_info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int sh_help(info_t *sh_info)
{
	char **argList;

	argList = sh_info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*argList); /* temp att_unused workaround */
	return (0);
}