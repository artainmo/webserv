NAME = parsing.a
SRCS = config_file.cpp \
			http_request.cpp \
			utils.cpp \
			url_to_local_path.cpp \

CXX = clang++
CXXFLAGS = #-Wall -Wextra -Werror

OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY:all fclean re

# compile:
# 	clang++ -Wall -Wextra -Werror -c $(SRCS)

# lib: compile
# 	ar rc $(NAME) $(OBJS)
# 	ranlib $(NAME)

# clean:
# 		rm -f $(OBJS)

# fclean: clean
# 	rm -rf $(NAME)

# re: fclean all
