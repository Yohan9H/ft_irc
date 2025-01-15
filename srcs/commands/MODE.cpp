#include "MODE.hpp"

MODE::MODE() : Command("MODE", 1, false) {};
MODE::~MODE() {};

void MODE::execCommand(Server &serv, Client &client, const com &cmd)
{
    // target NICK or CHANN ?

	


	//Channel MODE
	// channel doesn't exit ->  ERR_NOSUCHCHANNEL
	// no modestring =>  RPL_CHANNELMODEIS
	// modestring
		// user no priviledge on channel ? =e> ERR_CHANOPRIVSNEEDED
		//
	std::string channel_name = cmd.params[0];
	if (channel_name[0] != '#' || cmd.params.size() < 2)
		return ;
	std::string mode = cmd.params[1];
	std::string mdp = (cmd.params.size() > 2) ? cmd.params[1] : "";

	Channel *channel = serv.getChannelbyName(channel_name);

	if (!channel)
	{
		std::cerr << "ERR_NOSUCHCHANNEL" << std::endl;
		return ;
	}
	else if (!channel->isOperator(client.getClientSocket()))
	{
		std::cerr << "ERR_CHANOPRIVSNEEDED" << std::endl;
		return ;
	}
	else {
		switch (mode[1])

	}
}