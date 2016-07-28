/*
** main.c for my_script
**
** Made by christophe sundas
** Login   <sundas_c@epitech.net>
**
** Started on  Thu Oct  9 15:09:57 2008 christophe sundas
** Last update Sun Feb 22 21:09:45 2009 christophe sundas
*/
#include <sys/ioctl.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include "my_script.h"
#include "my.h"

#define DEVPTY	"/dev/ptyXY"
#define PTY_X	"pqrstuvwxyz"
#define PTY_Y	"0123456789abcdef"

int	select_master(char **name)
{
  int	i;
  int	j;
  char	*pty_name;
  int	fd;

  if ((pty_name = my_strdup(DEVPTY)) == 0)
    return (-1);
  i = 0;
  while (PTY_X[i])
    {
      j = 0;
      while (PTY_Y[j])
        {
          pty_name[8] = PTY_X[i];
          pty_name[9] = PTY_Y[j];
          if ((fd = open(pty_name, O_RDWR)) != -1)
            {
              *name = pty_name;
              return (fd);
            }
          j++;
        }
      i++;
    }
  return (-1);
}

/*
** int openpty(int *amaster, int *aslave, char *name, struct termios *termp,struct winsize *winp);
*/
int	my_openpty(int *amaster, int *aslave, struct termios *termp, struct winsize *winp)
{
  int	fd_master;
  char	*intern_name;
  int	fd_slave;

  if ((fd_master = select_master(&intern_name)) < 0)
    return (-1);
  intern_name[5] = 't';
  if ((fd_slave = open(intern_name, O_RDWR | O_CREAT | O_NOCTTY)) < 0)
    return (-1);
  if(termp)
    if (tcsetattr (fd_slave, TCSAFLUSH, termp) < 0)
      return (-1);
  if (winp)
    if (ioctl (fd_slave, TIOCSWINSZ, winp) < 0)
      return (-1);
  if (amaster)
    *amaster = fd_master;
  if (aslave)
    *aslave = fd_slave;
  return (0);
}
