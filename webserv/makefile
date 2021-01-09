NAME = webserv
SRCS = webserv.cpp

OBJ = $(SRCS:.cpp=.o)

all: compile

compile: $(OBJ) connexion_comp
		clang++ -Wall -Wextra -Werror $(OBJ) connexion.a -o $(NAME)

run: compile
	./webserv conf/webserv1.conf

clean: connexion_clean
		rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

connexion_comp:
	make -C ./connexion all
	mv ./connexion/connexion.a .

connexion_clean:
	make -C ./connexion fclean
	rm -rf connexion.a
