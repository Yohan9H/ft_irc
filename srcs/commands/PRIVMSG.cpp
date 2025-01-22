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
	} 
	else if (!cmd.hasText)
	{
		msg = "No text to send";
		numeric = ERR_NOTEXTTOSEND;
	} 
	else if (target[0] == '#') 
	{
		Channel* channel = serv.getChannelbyName(target);
		if (!channel)
		{
			msg = "No such channel";
			numeric = ERR_NOSUCHCHANNEL;
		}
		else if (!channel->checkClientIsMembre(client.getClientSocket()))
		{
			msg = "Cannot send to channel";
			numeric = ERR_CANNOTSENDTOCHAN;
		}
		else
		{
			std::string privmsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PRIVMSG " + channel->getName() + " :" + message + ENDLINE_MSG;
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
		}
		else {
			std :: string privmsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PRIVMSG " + targetclient->getNickname() + " :" + message + ENDLINE_MSG;
			send(targetclient->getClientSocket(), privmsg.c_str(), privmsg.size(), MSG_NOSIGNAL);
		}
	}
	if (!msg.empty())
		sendNumericCmd(client, numeric, cmd.command, msg + ENDLINE_MSG);
}