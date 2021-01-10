NAME = parsing.a
SRCS = config_file.cpp \
			http_request.cpp \
			utils.cpp

OBJS = $(SRCS:.cpp=.o)

all: lib

compile:
	clang++ -Wall -Wextra -Werror -c $(SRCS)

lib: compile
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

clean:
		rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
