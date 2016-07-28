/*
** my_put_error.c for errors
** 
** Made by christophe sundas
** Login   <sundas_c@epitech.net>
** 
** Started on  Wed Oct  8 21:13:07 2008 christophe sundas
** Last update Mon Nov 10 06:08:20 2008 christophe sundas
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "my.h"

void    my_put_nbr_error(int numerror)
{
  char  *error;

  error = strerror(numerror);
  write(2, error, my_strlen(error));
}

void    my_putstr_error(char *error)
{
  write(2, error, my_strlen(error));
}

