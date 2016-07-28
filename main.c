/*
** main.c for my_script
**
** Made by christophe sundas
** Login   <sundas_c@epitech.net>
**
** Started on  Thu Oct  9 15:09:57 2008 christophe sundas
** Last update Sun Feb 22 23:11:46 2009 christophe sundas
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
#include "my_printf.h"

int	main(int argc, char **argv)
{
  int			cc;
  struct termios	rtt;
  struct winsize	win;
  int			aflag;
  char			ibuf[BUFSIZ];

  aflag = 0;
  get_option(argc, argv, &aflag);
  open_thispty(aflag, &win);
  my_printf("Script started, output file is %s\n", fname);
  rtt = tt;
  rtt.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
  rtt.c_oflag &= ~OPOST;
  rtt.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
  rtt.c_cflag &= ~(CSIZE|PARENB);
  rtt.c_cflag |= CS8;
  tcsetattr(0, TCSAFLUSH, &rtt);
  signal(SIGCHLD, finish);
  mkfork();
  fclose(fscript);
  while ((cc = read(0, ibuf, BUFSIZ)) > 0)
    write(master, ibuf, cc);
  done();
  return (0);
}

/*
** sous fils =>mkouput
** fils => mkshell
*/
void	mkfork()
{
  fils = fork();
  if (fils < 0)
    fail();
  if (fils == 0)
    {
      sousfils = fils = fork();
      if (fils < 0)
	fail();
      if (fils)
	mkoutput();
      else
	mkshell();
    }
}

void	open_thispty(int aflag, struct winsize *win)
{
  if ((fscript = fopen(fname, aflag ? "a" : "w")) == NULL)
    {
      my_putstr_error("fopen error\n");
      exit(-1);
    }
  tcgetattr(0, &tt);
  ioctl(0, TIOCGWINSZ, win);
  if (my_openpty(&master, &slave, &tt, win) == -1)
    {
      my_putstr_error("openpty error\n");
      exit(-1);
    }
}

/*
** envoi le signal SIGTERM
** a tous les process dont l'id du groupe est egal au  groupid du sender
*/
void	fail()
{
  kill(0, SIGTERM);
  done();
}
