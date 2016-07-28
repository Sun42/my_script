/*
** my_script.c for my_script
** 
** Made by christophe sundas
** Login   <sundas_c@epitech.net>
** 
** Started on  Thu Oct  9 15:09:57 2008 christophe sundas
** Last update Sun Feb 22 22:15:53 2009 christophe sundas
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

/*
** Lors du exit, execute 2 fois  
** kill fils & sousfils
*/
void	finish(int signo)
{
  int	die;
  int	pid;
  int	status;
  
  signo = signo;
  die = 0;
  while ((pid = wait(&status)) > 0)
    {
      if (pid == fils)
	die = 1;
    }
  if (die)
    done();
}

/*
** Process fils
** lit dans le fd master 
** ecrit sur la sortie standard et le fichier dans fscript
*/
void			mkoutput()
{
  int			nblu;
  time_t		tvec;
  char			obuf[BUFSIZ];

  close(0);
  tvec = time(NULL);
  fprintf(fscript, "Script started on %s", ctime(&tvec));
  signal(SIGALRM, scriptflush);
  while (1)
    {
      nblu = read(master, obuf, sizeof(obuf));
      if (nblu <= 0)
	break;
      write(1, obuf, nblu);
      fwrite(obuf, 1, nblu, fscript);
      totallu += nblu;
    }
  done();
}

/*
** scriptflush nest pas toujours execute
** force l'output dans le script 
** si le signal SIGALRM est envoye au fils
*/
void	scriptflush(int signo)
{
  signo = signo;
  if (totallu) 
    {
      fflush(fscript);
      totallu = 0;
    }
}

/*
** execute une seule fois par le fils
*/
void	mkshell()
{
  char		*shell;

  shell = getenv("SHELL");
  if (shell == NULL)
    shell = "/bin/sh";
  close(master);
  fclose(fscript);
#if defined TIOCSCTTY
  my_login_tty(slave);
# else
  solaris_login_tty(slave);
#endif
  execl(shell, shell, "-i", NULL);
  fail();
}

/*
** Ecrit le time sur la sortie standard, dans le fichier
**  et close le master
*/
void		done()
{
  time_t	tvec;

  if (sousfils)
    {
      tvec = time(NULL);
      fprintf(fscript,"\nScript done on %s", ctime(&tvec));
      fclose(fscript);
      close(master);
    } 
  else 
    {
      tcsetattr(0, TCSAFLUSH, &tt);
      my_printf("Script done, output file is %s\n", fname);
    }
  exit(0);
}
