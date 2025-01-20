#include "KICK.hpp"

KICK::KICK() : Command("KICK", 2, true) {};
KICK::~KICK() {};

void KICK::execCommand(Server &serv, Client &client, const com &cmd)
{
	std::string msg;
	std::string chan_name = cmd.params[0];
	std::string nick = cmd.params[1];
	int numeric;

	if (client.getIsAuth() == false)
	{
		msg = ":" + std::string(NAME_SERV) + " 451 You have not registered" ENDLINE_MSG;
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel* channel = serv.getChannelbyName(chan_name);
	if (!channel)
	{
		msg = "No such channel";
		numeric = ERR_NOSUCHCHANNEL;
	}
	else if (!channel->checkClientIsMembre(client.getClientSocket()))
	{
		msg = "You're not on that channel";
		numeric = ERR_NOTONCHANNEL;
	}
	else if (!channel->isOperator(client.getClientSocket()))
	{
		msg = "You're not channel operator";
		numeric = ERR_CHANOPRIVSNEEDED;
	}
	else
	{
		Client* kickClient = serv.getClientbyName(nick);
		if (!kickClient)
		{
			msg = "No such nick/channel";
			numeric = ERR_NOSUCHNICK;
		}
		else if (!channel->checkClientIsMembre(kickClient->getClientSocket()))
		{
			msg = "They aren't on that channel";
			numeric = ERR_USERNOTINCHANNEL;
		}
		else
		{
			std::string reason = (cmd.hasText) ? cmd.trailing : "";
			std::string chanmsg = " KICK " + chan_name + " " + nick + " :" + reason;
			channel->sendMsgMembres(msg + ENDLINE_MSG);
			channel->delMembres(kickClient->getClientSocket());
			kickClient->removeChan(chan_name);
			std::string kickedmsg = "You have been kicked from " + chan_name + " by " + client.getNickname() + " (Reason: " + reason + ")" + ENDLINE_MSG;
			send(kickClient->getClientSocket(), kickedmsg.c_str(), kickedmsg.size(), 0);
		}
	}
	if (!msg.empty())
		sendNumericCmd(client, numeric, cmd.command, msg + ENDLINE_MSG);
}