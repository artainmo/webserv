SRC = main.cpp \
	  parsing/config_file.cpp \
	  parsing/utils.cpp \


all: compile

re: fclean compile

compile:
	@clang++ -Wall -Wextra -Werror $(SRC) -o webserv

run: compile
	@./webserv conf/webserv2.conf

fclean:
	rm -rf webserv
