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
		msg = "You have not registered";
		numeric = ERR_NOTREGISTERED;
		sendNumericParam1(client, numeric, client.getUsername(), msg + ENDLINE_MSG);
	}
	else 
	{
		Channel* channel = serv.getChannelbyName(chan_name);
		if (!channel)
		{
			msg = "No such channel";
			numeric = ERR_NOSUCHCHANNEL;
			sendNumericParam2(client, numeric, client.getUsername(), chan_name, msg + ENDLINE_MSG);
		}
		else if (!channel->checkClientIsMembre(client.getClientSocket()))
		{
			msg = "You're not on that channel";
			numeric = ERR_NOTONCHANNEL;
			sendNumericParam2(client, numeric, client.getUsername(), chan_name, msg + ENDLINE_MSG);
		}
		else if (!channel->isOperator(client.getClientSocket()))
		{
			msg = "You're not channel operator";
			numeric = ERR_CHANOPRIVSNEEDED;
			sendNumericParam2(client, numeric, client.getUsername(), chan_name, msg + ENDLINE_MSG);
		}
		else
		{
			Client* kickClient = serv.getClientbyName(nick);
			std::string nick_name = kickClient->getNickname();
			if (!kickClient)
			{
				msg = "No such nick/channel";
				numeric = ERR_NOSUCHNICK;
				sendNumericParam2(client, numeric, client.getUsername(), nick_name, msg + ENDLINE_MSG);
			}
			else if (!channel->checkClientIsMembre(kickClient->getClientSocket()))
			{
				msg = "They aren't on that channel";
				numeric = ERR_USERNOTINCHANNEL;
				sendNumericParam3(client, numeric, client.getUsername(), nick_name, chan_name, msg + ENDLINE_MSG);
			}
			else
			{
				std::string reason = (cmd.hasText) ? cmd.trailing : "";
				std::string chanmsg = "KICK " + chan_name + " " + nick + " :" + reason;
				channel->sendMsgMembres(chanmsg + ENDLINE_MSG);
				channel->delOperatores(kickClient->getClientSocket());
				channel->delMembres(kickClient->getClientSocket());
				kickClient->removeChan(chan_name);
				if (channel->getMembresFd().empty())
				{
					delete channel;
					serv.getChannel().erase(chan_name);
				}
				else if (channel->getOperatorsFd().empty())
					channel->addOperators(channel->getMembresFd()[0]);
				std::string kickedmsg = "You have been kicked from " + chan_name + " by " + client.getNickname() + " (Reason: " + reason + ")" + ENDLINE_MSG;
				send(kickClient->getClientSocket(), kickedmsg.c_str(), kickedmsg.size(), MSG_NOSIGNAL);
			}
		}
	}
}