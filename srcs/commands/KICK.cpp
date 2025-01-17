#include "KICK.hpp"

KICK::KICK() : Command("KICK", 2, true) {};
KICK::~KICK() {};

void KICK::execCommand(Server &serv, Client &client, const com &cmd)
{

	//verif if name is not on a channel -> ERR_USERNOTINCHANNEL
	// verif if User is a channel operator => ERR_CHANOPRIVSNEEDED
	//verif if channel exist and valid => ERR_BADCHANMASK or ERR_NOSUCHCHANNEL
	//verif if there is a reason
	//Send message to all channel that user has been KICK

	std::string msg;
	std::string chan_name = cmd.params[0];
	std::string nick = cmd.params[1];

	// verify the the user is authentified
	if (client.getIsAuth() == false)
	{
		msg = ":" + std::string(NAME_SERV) + " 451 You have not registered" ENDLINE_MSG;
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}

	Channel* channel = serv.getChannelbyName(chan_name);
	if (!channel)
	{
		std::cerr << "ERR_NOSUCHCHANNEL" << std::endl;
		return ;
	}
	else if (!channel->checkClientIsMembre(client.getClientSocket()))
	{
		std::cerr << "ERR_USERNOTINCHANNEL" << std::endl;
		return ;
	}
	else if (!channel->isOperator(client.getClientSocket()))
	{
		std::cerr << "ERR_CHANOPRIVSNEEDED" << std::endl;
		return ;
	}
	else
	{
		Client* kickClient = serv.getClientbyName(nick);
		if (!kickClient)
		{
			std::cerr << "ERR_NOSUCHNICK" << std::endl;
			return ;
		}
		else
		{
			channel->delMembres(kickClient->getClientSocket());
			//retirer de la list des channels du client
			msg = nick + " has been kicked by " + client.getNickname();
			channel->sendMsgMembres(msg);
		}
		return ;
	}

	if (!cmd.trailing.empty())
		std::string reason = cmd.trailing;

   


	//verif if name exist -> ERR_NOSUCHNICK


}