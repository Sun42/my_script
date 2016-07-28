/*
** option.c for my_script
**
** Made by christophe sundas
** Login   <sundas_c@epitech.net>
** 
** Started on  Thu Oct  9 15:09:57 2008 christophe sundas
** Last update Sun Feb 22 20:25:12 2009 christophe sundas
*/
#include <stdlib.h>
#include <sys/types.h>
#include "my.h"
#include "my_script.h"

void	get_option(int argc, char **argv, int *aflag)
{
  int	indice;

  *aflag = 0;
  indice = 1;
  fname = "typescript";
  if (indice == argc)
    return ;
  if (is_option(argv[indice]))
    {
      if (my_strcmp(argv[indice], "-a") == 0)
        *aflag = 1;
      else
	error_exit(USAGE);
      indice++;
    }
  if (indice != argc)
    {
      if (is_option(argv[indice]))
        error_exit(USAGE);
      else
        fname = argv[indice];
    }
}

int	is_option(char *str)
{
  if (*str == '-')
    return 1;
  return 0;
}

void	error_exit(char *error)
{
  my_putstr_error(error);
  exit (-1);
}
