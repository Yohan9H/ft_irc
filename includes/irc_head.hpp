#pragma once

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> //socket functions 
#include <netinet/in.h> //data type sockaddr
#include <iostream>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h> // to close fds
#include <arpa/inet.h> //function inet_ntoa
#include <vector>
#include <csignal>
#include <fcntl.h>
#include <poll.h>
#include <map>
#include <algorithm>
#include <iterator>
#include <exception>
#include <cctype>
#include <ctime>
#include <sstream>

#include "proto.hpp"
#include "Utils.hpp"
#include "Server.hpp"

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
# define COL_END "\033[0m"

# include "CAP.hpp"
# include "INVITE.hpp"
# include "JOIN.hpp"
# include "KICK.hpp"
# include "MODE.hpp"
# include "NICK.hpp"
# include "NOTICE.hpp"
# include "PART.hpp"
# include "PASS.hpp"
# include "PING.hpp"
# include "PRIVMSG.hpp"
# include "QUIT.hpp"
# include "TOPIC.hpp"
# include "USER.hpp"

struct com {
    std::string prefix;
    std::string command;
    std::vector<std::string> params;
    std::string trailing;
	bool hasText;
};

//utils
bool	is_good(char c);
void	showMapClient(Server &serv);
void	showMapChannel(Server &serv);
void    normalizeCRLF(std::string& input);

#define MAX_CONNEXIONS 5
#define BUFFER_MAX 1024
#define NAME_SERV "irc.42.com"
#define ENDLINE_MSG + "\n"
