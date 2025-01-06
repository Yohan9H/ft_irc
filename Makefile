# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: avandeve <avandeve@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/04 14:27:02 by avandeve          #+#    #+#              #
#    Updated: 2024/10/26 14:37:34 by avandeve         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#### SOURCES ####


INCLUDES	= ./includes/Server.hpp \
			./includes/Client.hpp \
			./includes/irc_head.hpp \
			./includes/command.hpp \
			./includes/Channel.hpp \
			./includes/proto.hpp \

FILES     = ./srcs/main.cpp \
			./srcs/Server.cpp \
			./srcs/Client.cpp \
			./srcs/command.cpp \
			./srcs/Channel.cpp \
			./srcs/manip_chan.cpp \
			./srcs/msg.cpp \
			./srcs/cmd_all.cpp \
			./srcs/cmd_ope.cpp \




#### MACROS ####

DIRPATH 		=	$(sh pwd)
SRCPATH			=	$(DIRPATH)
SRC				=	$(addprefix $(SRCPATH), $(FILES))
OBJ				= $(SRC:.cpp=.o)
NAME			= irc
CC				= c++
CFLAGS 			= -Wall -Werror -Wextra --std=c++98


#### RULES ####

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJ) $(INCLUDES)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)


clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re bonus