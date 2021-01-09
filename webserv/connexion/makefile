NAME = connexion.a
SRCS = connexion.cpp \


OBJ = $(SRCS:.cpp=.o)

all: compile

compile: $(OBJ) parsing_comp
		clang++ -Wall -Wextra -Werror $(OBJ) parsing.a -o $(NAME)

clean: parsing_clean
		rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

parsing_comp:
	make -C ./parsing all
	mv ./parsing/parsing.a .

parsing_clean:
	make -C ./parsing fclean
	rm -rf parsing.a
