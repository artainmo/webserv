NAME = webserv.a
SRCS = webserv.cpp

CXX = clang++
CXXFLAGS = #-Wall -Wextra -Werror

CONNEXION_P = connexion
CONNEXION = $(CONNEXION_P)/connexion.a

CONNEXION_LIB = $(CONNEXION_P)/connexion.a
PARSING_LIB = $(CONNEXION_P)/parsing/parsing.a
UTILS_LIB = $(CONNEXION_P)/utils/utils.a

OBJ = $(SRCS:.cpp=.o)

all: $(CONNEXION) $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(CONNEXION_LIB) $(PARSING_LIB) $(UTILS_LIB) -o $(NAME)

$(CONNEXION):
	(cd $(CONNEXION_P) && $(MAKE))

run: fclean all
	./webserv.a conf/webserv1.conf

clean:
	(cd $(CONNEXION_P) && $(MAKE) $@)
	rm -rf $(OBJ)

fclean: clean
	(cd $(CONNEXION_P) && $(MAKE) $@)
	rm -rf $(NAME)

re: fclean all

.PHONY:all fclean re run
