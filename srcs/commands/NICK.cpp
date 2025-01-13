#include "NICK.hpp"

NICK::NICK() : Command("NICK", 1, false) {};
NICK::~NICK() {};

void NICK::execCommand(Server &serv, Client &client, const com &cmd)
{
    std::string msg;
    std::string nick = cmd.params[0];

	// Verif valide name
	if (nick.length() > 9)
	{
		msg = msg_err(NAME_SERV, "432", nick, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
	bool verif_char = true;
	for (std::string::const_iterator it = nick.begin(); it != nick.end(); ++it)
	{
		if (is_good(*it) == false)
		{
			verif_char = false;
			break;
		}
	}
	if (verif_char == false)
	{
		msg = msg_err(NAME_SERV, "432", nick, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
	if (std::isdigit(nick[0]))
	{
		msg = msg_err(NAME_SERV, "432", nick, ":Erroneous nickname (debug - isalpha)");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
	
	removeNewline(nick);

	// Verif if already use
	for (std::map<int, Client*>::iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->second->getNickname() == nick)
		{
			msg = msg_err(NAME_SERV, "433", nick, ":Nickname is already in use");
			send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
			return ;
		}
	}

	// Informe tous les channels dont il fait partie de son changement
	if (client.getIsAuth() == false)
	{
		msg = ":" + nick + " NICK :" + nick ENDLINE_MSG;
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
	}
	else
	{
		msg = ":" + client.getNickname() + " NICK :" + nick ENDLINE_MSG;
		client.sendMsgAllChan(serv, msg);
	}

	client.setNick(nick);

	return ;

}