#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG() : Command("PRIVMSG", 1, true) {};
PRIVMSG::~PRIVMSG() {};

void PRIVMSG::execCommand(Server &serv, Client &client, const com &cmd)
{
	// pas de destinataire => ERR_NORECIPIENT
	// pas de text a envoyer ==> ERR_NOTEXTTOSEND
    // destinataire n'existe pas => ERR_NOSUCHNICK
	// if # => channel
	//	channel n'existe pas => ERR_NOSUCHCHANNEL
	// pas membre sur le channel => ERR_CANNOTSENDTOCHAN
	//

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
			channel->sendMsgMembresExceptFd(message, client.getClientSocket());
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
			send(targetclient->getClientSocket(), message.c_str(), message.size(), 0);
			send(client.getClientSocket(), message.c_str(), message.size(), 0);
		}
	}
	if (msg.empty())
		sendNumeric(client, numeric, msg);
}