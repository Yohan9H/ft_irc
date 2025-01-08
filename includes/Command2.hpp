#ifndef COMMAND2_HPP

#define COMMAND2_HPP

# include <string>
# include <vector>

class Server; 
class Client;

struct cmd {
    std::string prefix;
    std::string command;
    std::vector<std::string> params;
    std::string trailing; 
};

typedef struct s_timeServ
{
	int	day;
	int	month;
	int	year;
} t_timeServ;


void 	handlePrivmsg(const cmd &cmd);
void    parseCommand(Server &serv, Client &cli, const std::string &input);
void 	execCommand(Server &serv, Client &cli, const cmd &cmd);
void 	parseline(const std::string &line);

// CMD
bool	join(Server &serv, Client &client, std::string name_chan, std::string mdp);

bool	Nick(Server &serv, Client &client, std::string nick);

bool	User(Server &serv, Client &client, std::string username);

// FOR TEST
void	showMapClient(Server &serv);

void	showMapChannel(Server &serv);

#endif
