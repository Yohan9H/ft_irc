#include "NICK.hpp"

NICK::NICK() : Command("NICK", 1, false) {};
NICK::~NICK() {};

void NICK::execCommand(Server &serv, Client &client, const com &cmd)
{
    std::string msg;
	int numeric;
    std::string nick = cmd.params[0];

	if (!client.getPasswordFilled() && !client.getIsAuth())
	{
		msg = "You may start by command PASS to log in with the password" ENDLINE_MSG;
		client.appendOutData(msg);
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
	if (verif_char == false || nick.length() > 9 || std::isdigit(nick[0]))
	{
		msg = "Erroneus nickname";
		numeric = ERR_ERRONEUSNICKNAME;
		OutDataNumericParam2(client, numeric, client.getNickname(), nick, msg + ENDLINE_MSG);
		return ;
	}
	
	removeNewline(nick);
	// Verif if already use
	for (std::map<int, Client*>::iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->second->getNickname() == nick)
		{
			msg = "Nickname is already in use";
			numeric = ERR_NICKNAMEINUSE;
			OutDataNumericParam2(client, numeric, client.getNickname(), nick, msg + ENDLINE_MSG);
			return ;
		}
	}
	if (client.getIsAuth() == false)
	{
		if (client.getUserFilled())
			msg = ":!" + client.getUsername() + "@localhost NICK " + nick + ENDLINE_MSG;
		else
			msg = ":!@localhost NICK " + nick + ENDLINE_MSG;
		if (client.getUserFilled() && client.getPasswordFilled())
		{
			client.AuthIsGood();
			msg += std::string(HOST) + " 001 " + nick + " :Welcome to the ft_irc : " + nick + "!" + client.getUsername() + "@" + "localhost" + ENDLINE_MSG;
			msg += std::string(HOST) + " 002 " + nick + " :Your host is ft_irc 1.0" + ENDLINE_MSG;
			msg += std::string(HOST) + " 003 " + nick + " :Created at [" + serv.getTime() + "]" + ENDLINE_MSG;

		}
		client.appendOutData(msg);
	}
	// Informe tous les channels dont il fait partie de son changement
	else
	{
		msg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost NICK " + nick + ENDLINE_MSG;
		client.sendMsgAllChan(serv, msg);
	}

	client.setNick(nick);
	client.setNickFilled(true);

	return ;

}