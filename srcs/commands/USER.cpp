#include "USER.hpp"

USER::USER() : Command("USER", 3, false) {};
USER::~USER() {};

void USER::execCommand(Server &serv, Client &client, const com &cmd)
{
    
	std::string msg;
	int numeric;
    std::string username = cmd.params[0];

	// Verif client if identify
	if (client.if_NickIsCreate() == false)
	{
		msg = ":" + std::string(NAME_SERV) + " 431 No nickname given\n";
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}

	// Verif valide name
	removeNewline(username);

	if (username.length() > 9)
	{
		msg = msg_err(NAME_SERV, "432", username, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
	bool verif_char = true;
	for (std::string::iterator it = username.begin(); it != username.end(); ++it)
	{
		if (is_good(*it) == false)
		{
			verif_char = false;
			break;
		}
	}
	if (verif_char == false)
	{
		msg = msg_err(NAME_SERV, "432", username, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
	if (std::isdigit(username[0]))
	{
		msg = msg_err(NAME_SERV, "432", username, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}

	// Verif if already use
	for (std::map<int, Client*>::const_iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->second->getUsername() == username)
		{
			msg = msg_err(NAME_SERV, "433", username, ":Username is already in use");
			send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
			return ;
		}
	}

	client.setName(username);
	client.AuthIsGood();

	msg = ":" + std::string(NAME_SERV) + " 001 " + client.getNickname() + " :Welcome to the ft_irc " + client.getNickname() + "! " + client.getUsername() + "@" + "localhost\n";
	msg += ":" + std::string(NAME_SERV) + " 002 " + client.getNickname() + " :Your host is ft_irc 1.0\n";
	msg += ":" + std::string(NAME_SERV) + " 003 " + client.getNickname() + " :Created at [" + serv.getTime() + "]\n";
	send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
	
	return ;
}