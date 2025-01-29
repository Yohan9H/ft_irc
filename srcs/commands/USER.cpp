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

	if (cmd.params[1] != "*" || cmd.params[2] != "0")
	{
		msg =  client.getUsername() + " " + cmd.command + " :Not enough parameters" ENDLINE_MSG;
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return ;
	}

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
		OutDataNumericParam1(client, numeric, client.getNickname(), msg + ENDLINE_MSG);
		return ;
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