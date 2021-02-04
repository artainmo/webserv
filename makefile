NAME = webserv.a
SRCS = ./webserv/webserv.cpp \
				./webserv/connexion/answer.cpp \
				./webserv/connexion/cgi.cpp \
				./webserv/connexion/connexion.cpp \
				./webserv/connexion/parsing/config_file.cpp \
				./webserv/connexion/parsing/http_request.cpp \
				./webserv/connexion/parsing/url_to_local_path.cpp \
				./webserv/connexion/parsing/utils.cpp \
				./webserv/connexion/utils/utils.cpp

CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror

OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

run: re
	./webserv.a conf/webserv1.conf

test:
	yes | ./tests/ubuntu_tester http://127.0.0.1:1900

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY:all fclean re run
