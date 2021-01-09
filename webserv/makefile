# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymanzi <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/09 14:05:13 by ymanzi            #+#    #+#              #
#    Updated: 2020/11/22 13:49:15 by ymanzi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = clang++
GCC = clang++
NAME = webserv
CXXFLAGS = #-Wall -Wextra -Werror 
SRCS = jacob.cpp \
	  parsing/config_file.cpp \
	  parsing/http_request.cpp \
	  parsing/utils.cpp
OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CXX) $(CFLAGS) $(OBJ) -o $(NAME)
run: all
	@./webserv conf/webserv1.conf
clean:
		rm -f $(OBJ)
fclean: clean
	rm -rf $(NAME)
re: fclean all
