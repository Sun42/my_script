/*
** my_strncat.c for strncat in /u/epitech_2011s/sundas_c/cu/rendu/piscine/Jour_07
** 
** Made by christophe sundas
** Login   <sundas_c@epitech.net>
** 
** Started on  Wed Oct 15 16:00:28 2008 christophe sundas
** Last update Wed Oct 15 19:53:28 2008 christophe sundas
*/

char	*my_strncat(char *str1, char *str2, int n)
{
  int	str1_len;
  int	i;

  i  = 0;
  str1_len = my_strlen(str1);
  while (str2[i] != '\0' && (i < n))
    {
      str1[(str1_len + i)] = str2[i];
      i = i + 1;
    }
  str1[str1_len + i] = '\0';
  return (str1);
}
