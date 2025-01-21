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
	}
	else if (!channel->checkClientIsMembre(client.getClientSocket()))
	{
		msg = "You're not on that channel";
		numeric = ERR_NOTONCHANNEL;
	}
	else
	{
		if (!cmd.hasText)
		{
			if (channel->getTopic().empty())
			{
				msg = "No topic is set";
				numeric = RPL_NOTOPIC;
			}
			else
			{
				msg = channel->getName() + " :" + channel->getTopic();
				numeric = RPL_TOPIC;
			}
		}
		else if (channel->hasMode('t') && !channel->isOperator(client.getClientSocket()))
		{
			msg = "You're not channel operator";
			numeric = ERR_CHANOPRIVSNEEDED;
		}
		else if (topic == "")
		{
			// gerer le texte de la commande dans le cas du clearing
			channel->setTopic("");
			cmdmsg = "TOPIC " + channel_name + " :";
		}
		else
		{
			channel->setTopic(topic);
			cmdmsg = ":localhost " + to_string(RPL_TOPIC) + " " + client.getNickname() + " " + channel->getName() + topic;
		}
	}
	if (!msg.empty())
		sendNumericCmd(client, numeric, cmd.command, msg + ENDLINE_MSG);
	else
		channel->sendMsgMembres(cmdmsg + ENDLINE_MSG);
}