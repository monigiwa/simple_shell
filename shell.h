<<<<<<< HEAD
#ifndef SHELL_H
#define SHELL_H
#define bool int
#define true 1
#define false 0
#define NPTRS(n) (n * sizeof(void *))
#define NCHARS(n) (n * sizeof(char))

#include "headers/include.h"
#include "headers/list.h"
#include "headers/string.h"
#include "headers/builtin.h"
#include "headers/path.h"

extern char **environ;

/* io */
void printerr(const char *message);
void printout(const char *message);
void fprinterr(char *message);
void fprintout(char *message);
void prompt(state *self);


/* quote */
int findquote(char *str, char quote);
void comment(char *string);
bool should_quote(const char *string);
char *remove_quotes(char *string);

/* line */
char *getlines(int fd);
char **split(char *string, char *delimiter,
		unsigned int max, bool group_quote);

/* find */
char *findcmd(const char *command, const char *PATH);
struct dirent *findfile(DIR *dir, const char *filename);
char *joinpath(const char *base, const char *child);

/* run */
int interactive(state *self);
int non_interactive(state *self, int fd);
int execute(const char *program, char **args, char **env);
int runline(state *self, char *line);
bool runbuiltin(state *self, char **arguments);
bool runprogram(state *self, char **arguments);
bool runalias(state *self, char **command);

/*tokenizefile*/
char ***tokenizefiles(char *line);
char **tokenizefile(char *line);
void replaceline(char *av[]);

/* append */
bool appendStr(char ***arr, size_t *size, char *str, int index);
bool appendChar(char **string, size_t *size, char chr, int index);
bool appendInt(char **string, size_t *size, int num, int index);

/*atoi*/
int checkatoi(char *s);
int _atoi(char *s);


/* format */
char *format(const char *fmt, ...);


/* main */
state *init(char *prog, char **env);
void deinit(state *self);
void cleanup(state *self);

/* replacement */
char *replace(state *self, char *var);

/* realloc */
void *_realloc(void *ptr, unsigned int osize, unsigned int nsize);

#endif
=======
#include "shell.h"
#include <limits.h>

/**
 * init - initialize the shell's state
 *
 * @prog: the program name
 * @env: the process' environment variables
 *
 * Return: the shell's state
 */
state *init(char *prog, char **env)
{
	state *self = malloc(sizeof(state));

	self->lineno = 1;
	self->aliases = NULL;
	self->env = from_strarr(env);
	self->prog = prog;
	self->_errno = 0;
	self->content = NULL;
	self->lines = NULL;
	self->tokens = NULL;
	self->errno_buf = malloc(NCHARS(12));
	self->pid_buf = format("%d", getpid());
	self->parts = NULL;
	self->command = NULL;
	self->fd = 0;
	self->buf = NULL;

	return (self);
}

/**
 * deinit - destroy the shell's state
 *
 * @self: the shell's state
 *
 * Return: nothing
 */
void deinit(state *self)
{
	if (!self)
		return;

	free_list(self->aliases);
	free_list(self->env);
	free(self->content);
	free(self->lines);
	free(self->tokens);
	free(self->parts);
	if (self->fd)
		close(self->fd);
	free(self->pid_buf);
	free(self->errno_buf);
	free(self->command);
	free(self->buf);
	free(self);
}

/**
 * cleanup - routine clean up that frees up memory in the state
 * @self: the shell's state
 * Return: nothing
 */
void cleanup(state *self)
{

	if (!self)
		return;

	free(self->content);
	self->content = NULL;
	free(self->lines);
	self->lines = NULL;
	free(self->parts);
	self->parts = NULL;
	free(self->tokens);
	self->tokens = NULL;
	free(self->command);
	self->command = NULL;
}

/**
 * open_file - used by the main function to open a file
 *
 * @self: the shell's state
 * @path: the path of the file
 *
 * Return: the open file descriptor
 */
int open_file(state *self, char *path)
{
	int fd;

	fd = open(path, O_RDONLY);

	if (fd == -1)
	{
		fprinterr(format("%s: 0: Can't open %s\n",
			self->prog, path));
		deinit(self);
		exit(127);
	}
	self->fd = fd;
	return (fd);
}


/**
 * main - entry point
 *
 * @ac: number of argument variables
 * @av: array of argument variables
 * @env: array of environment variables
 *
 * Return: alway (0)
 */
int main(int ac, char **av, char **env)
{
	int status;
	state *self;
	int fd;

	(void)ac;
	fd = STDIN_FILENO;
	signal(SIGINT, SIG_IGN);
	self = init(av[0], env);

	if (av[1])
		fd = open_file(self, av[1]);
	if (isatty(fd))
		interactive(self);
	else
		non_interactive(self, fd);
	status = self->_errno;
	deinit(self);
	return (status);
}
>>>>>>> e4dd82adf3293277ea0c24e81f9fea04a908cc74
