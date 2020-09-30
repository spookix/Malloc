##
## ETNA PROJECT, 10/01/2019 by soubri_j/martin_m
## Makeflile
## File description: 
##      Makefile for Malloc library.
##

NAME				= libmy_malloc
CCFLAGS 			= -W -Wall -Werror -shared
LIB_NAME			= $(NAME)_$(shell uname).so
RM					= rm -f
LN					= ln -s
CC					= gcc
SRC					= $(shell ls *.c)

all: $(LIB_NAME) symlink

$(LIB_NAME):
	$(CC) $(CCFLAGS) -o $(LIB_NAME) -fPIC $(SRC)

symlink:
	$(LN) $(LIB_NAME) $(NAME).so

cleanlink:
	$(RM) $(NAME).so

clean: 
	$(RM) 

fclean: clean cleanlink
	$(RM) $(LIB_NAME)

re: fclean all

.PHONY: all symlink $(LIB_NAME) cleanlink clean fclean re
