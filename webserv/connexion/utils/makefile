NAME = utils.a
SRCS = directory.cpp

OBJ = $(SRCS:.cpp=.o)

all: lib

compile:
	clang++ -Wall -Wextra -Werror -c $(SRCS)

lib: compile
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

clean:
		rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

