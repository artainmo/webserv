NAME = parsing.a
SRCS = config_file.cpp \
			http_request.cpp \
			parsing/utils.cpp

OBJ = $(SRCS:.cpp=.o)

all: compile

compile: $(OBJ)
		clang++ -Wall -Wextra -Werror $(OBJ) -o $(NAME)

clean:
		rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
