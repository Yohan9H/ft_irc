#include "PART.hpp"

PART::PART() : Command("PART", 1, true) {};
PART::~PART() {};

void PART::execCommand(Server &serv, Client &client, const com &cmd)
{
    std::string msg;
	int	numeric;
	std::vector<std::string> channel_list = split(cmd.params[0], ',');
	std::string reason = (cmd.hasText) ? cmd.trailing : "No reason specified";

	for (std::vector<std::string>::iterator it = channel_list.begin(); it != channel_list.end(); it++)
	{
		Channel* channel = serv.getChannelbyName(*it);
		if (!channel)
		{
			msg = "No such channel";
			numeric = ERR_NOSUCHCHANNEL;
			sendNumericParam2(client, numeric, client.getNickname(), channel->getName(), msg + ENDLINE_MSG);
		}
		else if (!channel->checkClientIsMembre(client.getClientSocket()))
		{
			msg = "You're not on that channel";
			numeric = ERR_NOTONCHANNEL;
			sendNumericParam2(client, numeric, client.getNickname(), channel->getName(), msg + ENDLINE_MSG);
		}
		else 
		{
			std::string partmsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channel->getName() + " :" + reason + ENDLINE_MSG;
			channel->sendMsgMembres(partmsg);
			channel->delMembres(client.getClientSocket());
			channel->delOperatores(client.getClientSocket());
			client.removeChan(channel->getName());
			if (channel->getMembresFd().empty())
			{
				delete channel;
	 			serv.getChannel().erase(*it);
			}
		}
	}
}