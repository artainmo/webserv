NAME = webserv.a
SRCS = webserv.cpp

OBJ = $(SRCS:.cpp=.o)

all: compile

compile: connexion_comp $(OBJ)
		clang++ -Wall -Wextra -Werror $(OBJ) connexion/connexion.a connexion/parsing/parsing.a -o $(NAME)

run: re
	./webserv conf/webserv1.conf

clean: connexion_clean
		rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

connexion_comp:
	make -C ./connexion all

connexion_clean:
	make -C ./connexion fclean
