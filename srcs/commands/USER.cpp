#include "USER.hpp"

USER::USER() : Command("USER", 3, false) {};
USER::~USER() {};

void USER::execCommand(Server &serv, Client &client, const com &cmd)
{
    
	std::string msg;
	int numeric;
    std::string username = cmd.params[0];

	// Verif valide name
	removeNewline(username);

	if (!client.getPasswordFilled() && !client.getIsAuth())
	{
		msg = "You may start by command PASS to log in with the password" ENDLINE_MSG;
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return ;
	}

	if (client.getIsAuth())
	{
		msg = "You may not reregister";
		numeric = ERR_ALREADYREGISTERED;
		sendNumeric(client, numeric, msg + ENDLINE_MSG);
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
	if (verif_char == false || username.length() > 9 || std::isdigit(username[0]))
	{
		msg = "Erroneous nickname";
		numeric = ERR_ERRONEUSNICKNAME;
		sendNumeric(client, numeric, msg + ENDLINE_MSG);
		return ;
	}

	// Verif if already use
	for (std::map<int, Client*>::const_iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->second->getUsername() == username)
		{
			msg = "Erroneous nickname";
			numeric = ERR_ERRONEUSNICKNAME;
			sendNumeric(client, numeric, msg + ENDLINE_MSG);
			return ;
		}
	}

	client.setName(username);
	client.setUserFilled(true);

	if (client.getNickFilled() && client.getPasswordFilled())
	{
		client.AuthIsGood();
		msg = std::string(HOST) + " 001 " + client.getNickname() + " :Welcome to the ft_irc " + client.getNickname() + "!" + client.getUsername() + "@" + "localhost\n";
		msg += std::string(HOST) + " 002 " + client.getNickname() + " :Your host is ft_irc 1.0\n";
		msg += std::string(HOST) + " 003 " + client.getNickname() + " :Created at [" + serv.getTime() + "]\n";
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	}
	return ;
}