#include "INVITE.hpp"

INVITE::INVITE() : Command("INVITE", 2, true) {};
INVITE::~INVITE() {};

void INVITE::execCommand(Server &serv, Client &client, const com &cmd)
{ 
	//verify if client is already invited =_> checkClientisInvited
	//verify if the channel exist => ERR_NOSUCHCHANNEL
	//verify if the user belong to the channel => ERR_NOTONCHANNEL
	//vin invite only mode, verify user is opreator => ERR_CHANOPRIVSNEEDED
	//verify if the nickname exist
	//verify if the user already belong to channel => ERR_USERONCHANNEL

	std::string msg;
	std::string nick = cmd.params[0];
	std::string chan_name = cmd.params[1];

	Channel* channel = serv.getChannelbyName(chan_name);
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
	else if (!channel->isOperator(client.getClientSocket()) && channel->hasMode('i')) // rajouter condition pour Channel mode invite only
	{
		std::cerr << "ERR_CHANOPRIVSNEEDED" << std::endl;
		return ;
	}
	else 
	{
		Client* invitedClient = serv.getClientbyName(nick);
		if (!invitedClient)
		{
			std::cerr << "ERR_NOSUCHNICK" << std::endl;
			return ;
		}
		else if (channel->checkClientIsMembre(invitedClient->getClientSocket()))
		{
			std::cerr << "ERR_USERONCHANNEL" << std::endl;
			return ;
		}
		else
		{
			channel->addInvited(invitedClient->getClientSocket());
			std::cout << nick << " has been invited to channel " << chan_name << std::endl;
		}
	}
}