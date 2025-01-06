#ifndef COMMAND_HPP

#define COMMAND_HPP

class Server; 
class Client;

struct Command {
    std::string prefix;
    std::string command;
    std::vector<std::string> params;
    std::string trailing; 

};

void 	handlePrivmsg(const Command &cmd);
Command parseCommand(Server &serv, Client &cli, const std::string &input);
bool 	isValidCommand(Server &serv, Client &cli, const Command &cmd);
void 	parseline(const std::string &line);

// CMD
bool	join(Server &serv, Client &client, std::string name_chan, std::string mdp);

bool	Nick(Server &serv, Client &client, std::string nick);

bool	User(Server &serv, Client &client, std::string username);

#endif
