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
			return ;
		}
		else
		{
			channel->addInvited(invitedClient->getClientSocket());
			msg = chan_name + " " + nick;
			numeric = RPL_INVITING;
			std::string invitedMsg = "You have been invited to " + chan_name + " by " + client.getNickname() + ENDLINE_MSG; 
			sendNotice(*invitedClient, invitedMsg);
		}
	}
	sendNumericCmd(client, numeric, cmd.command, msg + ENDLINE_MSG);
}