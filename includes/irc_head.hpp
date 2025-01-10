#ifndef IRC_HEAD_HPP

# define IRC_HEAD_HPP

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
# define COL_END "\033[0m"


# include "../includes/commands/CAP.hpp"
# include "../includes/commands/INVITE.hpp"
# include "../includes/commands/JOIN.hpp"
# include "../includes/commands/KICK.hpp"
# include "../includes/commands/MODE.hpp"
# include "../includes/commands/NICK.hpp"
# include "../includes/commands/NOTICE.hpp"
# include "../includes/commands/PART.hpp"
# include "../includes/commands/PASS.hpp"
# include "../includes/commands/PING.hpp"
# include "../includes/commands/PRIVMSG.hpp"
# include "../includes/commands/QUIT.hpp"
# include "../includes/commands/TOPIC.hpp"
# include "../includes/commands/USER.hpp"

struct com {
    std::string prefix;
    std::string command;
    std::vector<std::string> params;
    std::string trailing; 
};

#endif