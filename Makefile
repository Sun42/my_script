NAME = my_script
SRC = 	main.c \
	my_script.c \
	option.c \
	open_tty.c \
	login_tty.c \
	my_putstr.c \
	my_put_error.c \
	my_strdup.c \
	my_strcmp.c
CC_FreeBSD      =       gcc
CC_solaris      =       /usr/sfw/bin/gcc
CC              =       $(CC_${OSTYPE})

OBJ = $(SRC:.c=.o)
TMP = $(OBJ:.c=.c~)
CFLAGS += -ansi -pedantic -W -Wall -I. 
LIB = -L.  -ltermcap -lmy_printf_`uname -m`-`uname -s`
RM = rm -f

$(NAME) : $(OBJ) 
	$(CC) -o $(NAME) $(OBJ) $(LIB)

all : $(NAME)

clean : 
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(TMP)
	$(RM) $(NAME)

re: fclean all


