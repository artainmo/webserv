NAME = connexion.a
SRCS = connexion.cpp \

OBJ = $(SRCS:.cpp=.o)

all: lib

compile: parsing_comp
	clang++ -Wall -Wextra -Werror -c $(SRCS)

lib: compile
	ar rc $(NAME) $(OBJ) parsing/parsing.a
	ranlib $(NAME)

clean: parsing_clean
		rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

parsing_comp:
	make -C ./parsing all

parsing_clean:
	make -C ./parsing fclean
