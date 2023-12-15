#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfile_d: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfile_disc;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* to sh_loop.c */
int sh_loop(info_t *sh_info, char **av);
int status_builtin(info_t *sh_info);
void find_cmd(info_t *sh_info);
void fork_cmd(info_t *sh_info);

/* toem_parser.c */
int is_cmd(info_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* to sh_realloc.c */
char *sh_membyte(char *ptr, char membyte, unsigned int nbyte);
void free_string(char **str);
void *mem_realloc(void *ptr, unsigned int old_byte, unsigned int new_byte);

/* toem_memory.c */
int bfree(void **);

/* toem_atoi.c */
int interactive(info_t *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* to sh_err.c */
void sh_input(char *str);
int sh_putchar(char c);
int put_fd(char c, int fd);
int put_st(char *str, int fd);

/* to sh_moreerr.c */
int error_atoi(char *str);
void print_error(info_t *sh_info, char *estr);
int print_dec(int input, int fd);
char *convert_num(long int num, int base, int flags);
void cancel_comments(char *buf);

/* to sh_builtin.c */
int sh_exit(info_t *sh_info);
int sh_cd(info_t *sh_info);
int sh_help(info_t *sh_info);

/* to sh_buitltin.c */
int sh_history(info_t *sh_info);
int unset_alias(info_t *sh_info, char *str);
int set_alias(info_t *sh_info, char *str);
int print_alias(list_t *node);
int sh_alias(info_t *sh_info);

/*to sh_getline.c */
ssize_t sh_input_buf(info_t *sh_info, char **buf, size_t *len);
ssize_t sh_getinput(info_t *sh_info);
ssize_t sh_readbuf(info_t *sh_info, char *buf, size_t *i);
int sh_getline(info_t *sh_info, char **ptr, size_t *length);
void Handler(__attribute__((unused))int sig_num);

/* to sh_getinfo.c */
void init_info(info_t *sh_info);
void set_info(info_t *sh_info, char **av);
void free_info(info_t *sh_info, int freeall);

/* to sh_env.c */
int sh_env(info_t *sh_info);
char *get_sh_env(info_t *sh_info, const char *envName);
int set_sh_env(info_t *sh_info);
int unset_sh_env(info_t *sh_info);
int sh_env_list(info_t *sh_info);

/* to sh_getenv.c */
char **sh_getenv(info_t *sh_info);
int sh_unsetenv(info_t *sh_info, char *envar);
int sh_setenv(info_t *sh_info, char *envar, char *enval);

/* toem_history.c */
char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* toem_lists.c */
list_t *add_envNode(list_t **, const char *, int);
list_t *add_envNode_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_envNode_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* toem_lists1.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);

#endif
