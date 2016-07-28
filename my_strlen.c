/*
** my_strlen.c for my_str_len in /u/all/sundas_c/cu/rendu/piscine/Jour_04
** 
** Made by christophe sundas
** Login   <sundas_c@epitech.net>
** 
** Started on  Thu Oct  9 15:29:26 2008 christophe sundas
** Last update Wed Oct 15 19:50:50 2008 christophe sundas
*/

int	my_strlen(char *c)
{
  int	i;

  i = 0;
  while (c[i] != '\0')
    {
      i = i + 1;
    }
  return (i);
}
