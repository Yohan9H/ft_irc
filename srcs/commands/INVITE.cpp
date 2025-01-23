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
	int	numeric;
	std::string nick = cmd.params[0];
	std::string chan_name = cmd.params[1];

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
	else if (!channel->isOperator(client.getClientSocket()) && channel->hasMode('i')) // rajouter condition pour Channel mode invite only
	{
		msg = "You're not channel operator";
		numeric = ERR_CHANOPRIVSNEEDED;
	}
	else 
	{
		Client* invitedClient = serv.getClientbyName(nick);
		if (!invitedClient)
		{
			msg = "No such nick/channel";
			numeric = ERR_NOSUCHNICK;
		}
		else if (channel->checkClientIsMembre(invitedClient->getClientSocket()))
		{
			msg = "is already on channel";
			numeric = ERR_USERONCHANNEL;
		}
		//verifier si on garde cette confition
		else if (channel->checkClientIsInvited(invitedClient->getClientSocket()))
		{
			msg = "is already invited";
			numeric = ERR_USERONCHANNEL;
		}
		else
		{
			channel->addInvited(invitedClient->getClientSocket());
			numeric = RPL_INVITING;
			std::string invitedMsg = std::string(HOST) + " " + to_string(numeric) + " " + client.getNickname() + " " + invitedClient->getNickname() + " " + chan_name + ENDLINE_MSG;
			send(invitedClient->getClientSocket(), invitedMsg.c_str(), invitedMsg.size(), MSG_NOSIGNAL);
			return ;
		}
	}
	sendNumericCmd(client, numeric, cmd.command, msg + ENDLINE_MSG);
}