#include "TOPIC.hpp"

TOPIC::TOPIC() : Command("TOPIC", 1, false) {};
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

	std::string channel_name = cmd.params[0];
	std::string topic;
	if (cmd.hasText)
		topic = cmd.trailing;
	Channel* channel = serv.getChannelbyName(channel_name);
	if (!channel)
	{
		std::cerr << "ERR_NOSUCHCHANNEL" << std::endl;
		return ;
	}
	else if (!channel->checkClientIsMembre(client.getClientSocket()))
	{
		std::cerr << "ERR_NOTONCHANNEL" << std::endl;
		return ;
	}
	else if (!channel->isOperator(client.getClientSocket()) && channel->hasMode('t')) // rajouter permissions
	{
		std::cerr << "ERR_CHANOPRIVSNEEDED" << std::endl;
		return ;
	}
	else
	{
		if (!cmd.hasText)
		{
			if (channel->getTopic().empty())
				std::cerr << "No topic is set" << std::endl;
			else
				std::cout << channel->getTopic() << std::endl;
		}
		else if (topic == "")
		{
			channel->setTopic("");
			std::cout << "TOPIC " << channel_name << " :" << std::endl;
		}
		else
		{
			channel->setTopic(topic);
			std::cout << "TOPIC " << channel_name << " :" << topic << std::endl;
		}

	}
}