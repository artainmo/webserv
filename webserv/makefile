SRC = main.cpp \
	  parsing/config_file.cpp \


all: run

re: fclean run

run:
	clang++ -Wall -Wextra -Werror $(SRC) -o webserv

fclean:
	rm -rf webserv
