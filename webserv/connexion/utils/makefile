NAME = utils.a
SRCS = directory.cpp

OBJ = $(SRCS:.cpp=.o)

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




# all: lib

# compile:
# 	clang++ -Wall -Wextra -Werror -c $(SRCS)

# lib: compile
# 	ar rc $(NAME) $(OBJ)
# 	ranlib $(NAME)

# clean:
# 		rm -rf $(OBJ)

# fclean: clean
# 	rm -rf $(NAME)

# re: fclean all

