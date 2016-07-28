/*
** my_script.h for my_script
**
** Made by christophe sundas
** Login   <sundas_c@epitech.net>
**
** Started on  Thu Oct  9 15:09:57 2008 christophe sundas
** Last update Sun Feb 22 21:11:39 2009 christophe sundas
*/

#ifndef __MYSCRIPT_H__
# define __MYSCRIPT_H__

#define SECSPERMIN 60
#define BUFF_SIZE 1024
#define USAGE "usage: script [-a] [file]\n" 
#include <stdio.h>
#include <termios.h>

FILE		*fscript;
int		master;
int		slave;
int		fils;
int		sousfils;
int		totallu;
char		*fname;
struct  termios	tt;

/*
** option.c
*/
void	get_option(int argc, char **argv, int *aflag);
int	is_option(char *str);
void	error_exit(char *error);

/*
** main.c
*/
void	open_thispty(int aflag, struct winsize  *win);
void	mkfork();

/*
** open_tty.c
*/
int     select_master(char **name);
int	my_openpty(int *amaster, int *aslave, struct termios *termp, struct winsize *winp);

/*
** login_tty.c
*/
int	my_login_tty(int fd);
int	solaris_login_tty(int fd);

void	done();
void	mkoutput();
void	mkshell();
void	fail();
void	scriptflush(int signo);
void	finish(int signo);

#endif
