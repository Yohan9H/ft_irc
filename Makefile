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

CXX = g++ -g
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