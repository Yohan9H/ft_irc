#include "TOPIC.hpp"

TOPIC::TOPIC() : Command("TOPIC", 1, true) {};
TOPIC::~TOPIC() {};

void TOPIC::execCommand(Server &serv, Client &client, const com &cmd)
{

// TOPIC #test :New topic  ; Setting the topic on "#test" to "New topic".
// TOPIC #test :           ; Clearing the topic on "#test"
// TOPIC #test             ; Checking the topic for "#test"
// channel exist ? => NOSUCHCHANNEL
// user appartient au channel => NOTONCHANNEL
// si private, user n'a pas les permissions => CHANOPRIVSNEEDED
// 
	std::string msg;
	std::string cmdmsg;
	int numeric;
	std::string channel_name = cmd.params[0];
	std::string topic;
	if (cmd.hasText)
		topic = cmd.trailing;
	Channel* channel = serv.getChannelbyName(channel_name);
	if (!channel)
	{
		msg = "No such channel";
		numeric = ERR_NOSUCHCHANNEL;
		OutDataNumericParam2(client, numeric, client.getNickname(), channel_name, msg + ENDLINE_MSG);
	}
	else if (!channel->checkClientIsMembre(client.getClientSocket()))
	{
		msg = "You're not on that channel";
		numeric = ERR_NOTONCHANNEL;
		OutDataNumericParam2(client, numeric, client.getNickname(), channel_name, msg + ENDLINE_MSG);	}
	else
	{
		if (!cmd.hasText)
		{
			if (channel->getTopic().empty())
			{
				msg = "No topic is set";
				numeric = RPL_NOTOPIC;
				OutDataNumericParam2(client, numeric, client.getNickname(), channel_name, msg + ENDLINE_MSG);
			}
			else
			{
				msg = channel->getName() + " :" + channel->getTopic();
				numeric = RPL_TOPIC;
				OutDataNumericParam2(client, numeric, client.getNickname(), channel_name, msg + ENDLINE_MSG);
			}
		}
		else if (channel->hasMode('t') && !channel->isOperator(client.getClientSocket()))
		{
			msg = "You're not channel operator";
			numeric = ERR_CHANOPRIVSNEEDED;
			OutDataNumericParam2(client, numeric, client.getNickname(), channel_name, msg + ENDLINE_MSG);
		}
		else if (topic == "")
		{
			channel->setTopic("");
			cmdmsg = "TOPIC " + channel_name + " :";
			channel->sendMsgMembres(cmdmsg + ENDLINE_MSG, serv);
		}
		else
		{
			channel->setTopic(topic);
			cmdmsg = HOST + to_string(RPL_TOPIC) + " " + client.getNickname() + " " + channel_name + " " + topic;
			channel->sendMsgMembres(cmdmsg + ENDLINE_MSG, serv);
		}
	}
}