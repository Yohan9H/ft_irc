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

NAME = irc

SRC_DIR = srcs
OBJ_DIR = .obj

INCLUDE = ./includes

SRCS = $(SRC_DIR)/main.cpp \
	   $(SRC_DIR)/Server.cpp \
	   $(SRC_DIR)/Client.cpp \
	   $(SRC_DIR)/command.cpp \
	   $(SRC_DIR)/Channel.cpp \
	   $(SRC_DIR)/manip_chan.cpp \
	   $(SRC_DIR)/msg.cpp \
	   $(SRC_DIR)/cmd_all.cpp \
	   $(SRC_DIR)/cmd_ope.cpp

OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

CXX = c++ -g
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I$(INCLUDE)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re