#include <sys/cdefs.h>

#ifndef lint
#if 0
static char sccsid[] = "@(#)script.c    8.1 (Berkeley) 6/6/93";
#endif
/*__RCSID("$NetBSD: script.c,v 1.6 1998/04/02 11:08:34 kleink Exp $");*/
#endif /* not lint */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <libutil.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <paths.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
/*#include <tzfile.h>*/
#include <unistd.h>
/*#include <util.h>*/

#define SECSPERMIN 60

FILE    *fscript;
int     master, slave;
int     child, subchild;
int     outcc;
char    *fname;

struct  termios tt;

void    done __P((void));
void    dooutput __P((void));
void    doshell __P((void));
void    fail __P((void));
void    finish __P((int));
int     main __P((int, char **));
void    scriptflush __P((int));

int	main(int argc,char **argv)
{
  int cc;
  struct termios rtt;
  struct winsize win;
  int aflg, ch;
  char ibuf[BUFSIZ];

  aflg = 0;
  while ((ch = getopt(argc, argv, "a")) != -1)
    switch(ch) {
    case 'a':
      aflg = 1;
      break;
    case '?':
    default:
      (void)fprintf(stderr, "usage: script [-a] [file]\n");
      exit(1);
    }
  argc -= optind;
  argv += optind;

  if (argc > 0)
    fname = argv[0];
  else
    fname = "typescript";

  if ((fscript = fopen(fname, aflg ? "a" : "w")) == NULL)
    err(1, "fopen %s", fname);

  (void)tcgetattr(STDIN_FILENO, &tt);
  (void)ioctl(STDIN_FILENO, TIOCGWINSZ, &win);
  if (openpty(&master, &slave, NULL, &tt, &win) == -1)
    err(1, "openpty");

  (void)printf("Script started, output file is %s\n", fname);
  rtt = tt;
  cfmakeraw(&rtt);
  rtt.c_lflag &= ~ECHO;
  (void)tcsetattr(STDIN_FILENO, TCSAFLUSH, &rtt);

  (void)signal(SIGCHLD, finish);
  child = fork();
  if (child < 0) {
    warn("fork");
    fail();
  }
  if (child == 0) {
    subchild = child = fork();
    if (child < 0) {
      warn("fork");
      fail();
    }
    if (child)
      dooutput();
    else
      doshell();
  }

  (void)fclose(fscript);
  while ((cc = read(STDIN_FILENO, ibuf, BUFSIZ)) > 0)
    (void)write(master, ibuf, cc);
  done();
  /* NOTREACHED */
  return (0);
}

void finish(int signo)
{
  int die, pid;
  /* union wait status;*/
  int status;
  die = 0;
  while ((pid = wait3((int *)&status, WNOHANG, 0)) > 0)
    if (pid == child)
      die = 1;

  if (die)
    done();
}

void	dooutput()
{
  struct itimerval value;
  int cc;
  time_t tvec;
  char obuf[BUFSIZ];

  (void)close(STDIN_FILENO);
  tvec = time(NULL);
  (void)fprintf(fscript, "Script started on %s", ctime(&tvec));

  (void)signal(SIGALRM, scriptflush);
  value.it_interval.tv_sec = SECSPERMIN / 2;
  value.it_interval.tv_usec = 0;
  value.it_value = value.it_interval;
  (void)setitimer(ITIMER_REAL, &value, NULL);
  for (;;) {
    cc = read(master, obuf, sizeof (obuf));
    if (cc <= 0)
      break;
    (void)write(1, obuf, cc);
    (void)fwrite(obuf, 1, cc, fscript);
    outcc += cc;
  }
  done();
}

void	scriptflush(signo)
     int signo;
{
  if (outcc) {
    (void)fflush(fscript);
    outcc = 0;
  }
}

void	doshell()
{
  char *shell;

  shell = getenv("SHELL");
  if (shell == NULL)
    shell = _PATH_BSHELL;

  (void)close(master);
  (void)fclose(fscript);
  login_tty(slave);
  execl(shell, shell, "-i", NULL);
  warn("execl %s", shell);
  fail();
}

void	fail()
{

  (void)kill(0, SIGTERM);
  done();
}

void	done()
{
  time_t tvec;

  if (subchild) {
    tvec = time(NULL);
    (void)fprintf(fscript,"\nScript done on %s", ctime(&tvec));
    (void)fclose(fscript);
    (void)close(master);
  } else {
    (void)tcsetattr(STDIN_FILENO, TCSAFLUSH, &tt);
    (void)printf("Script done, output file is %s\n", fname);
  }
  exit(0);
}
