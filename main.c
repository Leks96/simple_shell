#include "shell.h"

/**
 * main - main entry point of the shell programme
 * @ac: argument count
 * @av: argument vector
 *
 * Return: (0) on success, (1) on error
 */
int main(int ac, char **av)
{
	info_t inf[] = { INFO_INIT };
	int file_disc = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (file_disc)
		: "r" (file_disc));

	if (ac == 2)
	{
		file_disc = open(av[1], O_RDONLY);
		if (file_disc == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs(": 0: Can't open ");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		inf->readfile_disc = file_disc;
	}
	populate_env_list(inf);
	read_history(inf);
	hsh(inf, av);
	return (EXIT_SUCCESS);
}