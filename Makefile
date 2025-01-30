NAME = ircserv

SRC_DIR = srcs
CMD_DIR = srcs/cmd
CMD_DIR2 = srcs/commands
MTH_DIR = srcs/methods_class

OBJ_DIR = .obj

INCLUDE = ./includes -I./includes/commands

SRCS = $(SRC_DIR)/main.cpp \
	   $(SRC_DIR)/command.cpp \
	   $(SRC_DIR)/utils.cpp \
	   $(MTH_DIR)/Server.cpp \
	   $(MTH_DIR)/Client.cpp \
	   $(MTH_DIR)/Channel.cpp \
	   $(CMD_DIR)/manip_chan.cpp \
	   $(CMD_DIR)/msg.cpp \
	   $(CDM_DIR2)/JOIN.cpp \
	   $(CDM_DIR2)/KICK.cpp \
	   $(CDM_DIR2)/MODE.cpp \
	   $(CDM_DIR2)/NICK.cpp \
	   $(CDM_DIR2)/PART.cpp \
	   $(CDM_DIR2)/PASS.cpp \
	   $(CDM_DIR2)/INVITE.cpp \
	   $(CDM_DIR2)/PRIVMSG.cpp \
	   $(CDM_DIR2)/QUIT.cpp \
	   $(CDM_DIR2)/TOPIC.cpp \
	   $(CDM_DIR2)/USER.cpp

OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

CXX = g++ -g
CXXFLAGS = -std=c++98 -I$(INCLUDE)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(CMD_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(CMD_DIR2)/%.cpp | $(OBJ_DIR)
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