NAME = connexion.a
SRCS = connexion.cpp \
		answer.cpp \
		cgi.cpp \

CXX = clang++
CXXFLAGS = #-Wall -Wextra -Werror

PARSING_P = parsing
PARSING = $(PARSING_P)/parsing.a

UTILS_P = utils
UTILS = $(UTILS_P)/utils.a

OBJ = $(SRCS:.cpp=.o)

all: $(PARSING) $(UTILS)  $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)

$(PARSING):
	(cd $(PARSING_P) && $(MAKE))

$(UTILS):
	(cd $(UTILS_P) && $(MAKE))

clean:
	(cd $(PARSING_P) && $(MAKE) $@)
	(cd $(UTILS_P) && $(MAKE) $@)
	rm -rf $(OBJ)

fclean: clean
	(cd $(PARSING_P) && $(MAKE) $@)
	(cd $(UTILS_P) && $(MAKE) $@)
	rm -rf $(NAME)

re: fclean all

.PHONY:all fclean re