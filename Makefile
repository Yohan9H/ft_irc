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
NAME			= ircserv
CC				= c++ -g
CFLAGS 			= -Wall -Werror -Wextra --std=c++98


#### RULES ####

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJ) $(INCLUDES)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

NAME = irc

SRC_DIR = srcs
CMD_DIR = srcs/cmd
MTH_DIR = srcs/methods_class

OBJ_DIR = .obj

INCLUDE = ./includes

SRCS = $(SRC_DIR)/main.cpp \
	   $(SRC_DIR)/command.cpp \
	   $(MTH_DIR)/Server.cpp \
	   $(MTH_DIR)/Client.cpp \
	   $(MTH_DIR)/Channel.cpp \
	   $(CMD_DIR)/manip_chan.cpp \
	   $(CMD_DIR)/msg.cpp \
	   $(CMD_DIR)/cmd_all.cpp \
	   $(CMD_DIR)/cmd_ope.cpp

OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

CXX = c++ -g
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I$(INCLUDE)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(CMD_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(MTH_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re