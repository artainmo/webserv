NAME = connexion.a
SRCS = connexion.cpp \
		answer.cpp \
		cgi.cpp \

OBJ = $(SRCS:.cpp=.o)

all: lib

compile: parsing_comp utils_comp
	clang++ -Wall -Wextra -Werror -c $(SRCS)

lib: compile
	ar rc $(NAME) $(OBJ) parsing/parsing.a utils/utils.a
	ranlib $(NAME)

clean: parsing_clean utils_clean
		rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

parsing_comp:
	make -C ./parsing all

parsing_clean:
	make -C ./parsing fclean

utils_comp:
	make -C ./utils all

utils_clean:
	make -C ./utils fclean
