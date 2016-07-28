/*
** login_tty.c for my_script
**
** Made by christophe sundas
** Login   <sundas_c@epitech.net>
**
** Started on  Thu Oct  9 15:09:57 2008 christophe sundas
** Last update Sun Feb 22 21:44:53 2009 christophe sundas
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
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
#include "my_printf.h"

int	my_login_tty(int fd)
{
  setsid();
#if defined TIOCSCTTY
  if (ioctl(fd, TIOCSCTTY, (char *)NULL) == -1)
    return (-1);
#endif 
  dup2(fd, 0);
  dup2(fd, 1);
  dup2(fd, 2);
  if (fd > 2)
    close(fd);
  return (0);
}

int	solaris_login_tty(int fd)
{
 char	*fdname;
 int	newfd;
 
 setsid();
 fdname = ttyname(fd);
 if (fdname)
    {
      if (fd != 0)
	(void) close(0);
      if (fd != 1)
	(void ) close(1);
      if (fd != 2)
	(void) close(2);
      newfd = open (fdname, O_RDWR);
      (void) close (newfd);
    }
 (void) dup2(fd, 0);
 (void) dup2(fd, 1);
 (void) dup2(fd, 2);
 if (fd > 2)
   close(fd);
 return (0);
}
