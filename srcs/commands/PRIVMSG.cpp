#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG() : Command("PRIVMSG", 1, true) {};
PRIVMSG::~PRIVMSG() {};

void PRIVMSG::execCommand(Server &serv, Client &client, const com &cmd)
{


	std::string target = cmd.params[0];
	std::string message = cmd.trailing;
	std::string msg;
	int numeric;

	if (target.empty()) 
	{
		msg = "No recipient given " + cmd.command;
		numeric = ERR_NORECIPIENT;
		sendNumericParam1(client, numeric, client.getNickname(), msg + ENDLINE_MSG);
	} 
	else if (!cmd.hasText)
	{
		msg = "No text to send";
		numeric = ERR_NOTEXTTOSEND;
		sendNumericParam1(client, numeric, client.getNickname(), msg + ENDLINE_MSG);
	} 
	else if (target[0] == '#') 
	{
		Channel* channel = serv.getChannelbyName(target);
		if (!channel)
		{
			msg = "No such channel";
			numeric = ERR_NOSUCHCHANNEL;
			sendNumericParam2(client, numeric, client.getNickname(), target, msg + ENDLINE_MSG);
		}
		else if (!channel->checkClientIsMembre(client.getClientSocket()))
		{
			msg = "Cannot send to channel";
			numeric = ERR_CANNOTSENDTOCHAN;
			sendNumericParam2(client, numeric, client.getNickname(), target, msg + ENDLINE_MSG);		}
		else
		{
			std::string privmsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PRIVMSG " + target + " :" + message + ENDLINE_MSG;
			channel->sendMsgMembresExceptFd(privmsg, client.getClientSocket());
		}	
	} 
	else 
	{
		Client* targetclient = serv.getClientbyName(target);
		if (!targetclient)
		{
			msg = "No such nick";
			numeric = ERR_NOSUCHNICK;
			sendNumericParam2(client, numeric, client.getNickname(), targetclient->getNickname(), msg + ENDLINE_MSG);

		}
		else {
			std :: string privmsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PRIVMSG " + targetclient->getNickname() + " :" + message + ENDLINE_MSG;
			send(targetclient->getClientSocket(), privmsg.c_str(), privmsg.size(), MSG_NOSIGNAL);
		}
	}
}