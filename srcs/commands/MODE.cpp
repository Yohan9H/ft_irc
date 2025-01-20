#include "MODE.hpp"

MODE::MODE() : Command("MODE", 1, true) {};
MODE::~MODE() {};

void MODE::execCommand(Server &serv, Client &client, const com &cmd)
{
	//rajouter message qui annonce le changement de mode
	std::string msg;
	int numeric;
	std::string channel_name = cmd.params[0];
	if (channel_name[0] != '#' || cmd.params.size() < 2)
	{
		msg = "Wrong params, not a channel";
		numeric = ERR_INVALIDMODEPARAM;
		sendNumericCmd(client, numeric, cmd.command, msg);
		return ;
	}
	
	std::string mode = cmd.params[1];
	std::string mdp = (cmd.params.size() > 2) ? cmd.params[2] : "";

	Channel *channel = serv.getChannelbyName(channel_name);

	if (!channel)
	{
		msg = "No such channel";
		numeric = ERR_NOSUCHCHANNEL;
	}
	else if (!channel->isOperator(client.getClientSocket()))
	{
		msg = "You're not channel operator";
		numeric = ERR_CHANOPRIVSNEEDED;
	}
	else {
		char sign = mode[0];
		char type = mode[1];
		if (mode.length() != 2 || ((sign != '-') && (sign != '+')))
		{
			msg = "Unknown MODE flag";
			numeric = ERR_UMODEUNKNOWNFLAG;
		}
		switch (type) {
			// Invite-only
			case 'i': {
				if (sign == '+')
				{
					channel->addMode('i');
					sendModeParamMsg(client, *channel, mode, mdp);
				}
				else
				{
					channel->errMode('i');
					sendModeParamMsg(client, *channel, mode, mdp);
				}
				break;
			};
			// TOPIC
			case 't': {
				if (sign == '+')
				{
					channel->addMode('t');
					sendModeParamMsg(client, *channel, mode, mdp);
				}
				else
				{
					channel->errMode('t');
					sendModeParamMsg(client, *channel, mode, mdp);
				}
				break;
			};
			//Password
			case 'k': {
				if (sign == '+') {
					if (mdp.empty()) {
						msg = "Wrong password format";
						numeric = ERR_INVALIDMODEPARAM;
					} else {
						channel->setKey(mdp);
						channel->addMode('k');
						sendModeParamMsg(client, *channel, mode, mdp);
					}
				}
				else {
					channel->errMode('k');
					sendModeParamMsg(client, *channel, mode, mdp);
				}
				break;
			};
			//Operator
			case 'o': {
				if (mdp.empty()) {
						msg = "Not enough parameters";
						numeric = ERR_NEEDMOREPARAMS;
				} else {
					Client* modeOperator = serv.getClientbyName(mdp);
					if (!modeOperator)
					{
						msg = "No such nick";
						numeric = ERR_NOSUCHNICK;
					}
					else if (channel->checkClientIsMembre(modeOperator->getClientSocket()))
					{
						msg = "They aren't on that channel";
						numeric = ERR_USERNOTINCHANNEL;
					}
					else {
						if (sign == '+') {
							if (!channel->isOperator(modeOperator->getClientSocket()))
							{
								channel->addOperators(modeOperator->getClientSocket());
								channel->addMode('o');
								sendModeParamMsg(client, *channel, mode, mdp);
							}
						} else {
							if (channel->isOperator(modeOperator->getClientSocket()))
							{
								channel->delOperatores(modeOperator->getClientSocket());
								channel->errMode('o');
								sendModeParamMsg(client, *channel, mode, mdp);
							}	
						}
					}
				}
				break ;
			};
			//Limits
			case 'l': {
				if (sign == '+') {
					if (mdp.empty())
					{
						msg = "Not enough parameters" ENDLINE_MSG;
						numeric = ERR_NEEDMOREPARAMS;
					}
					else 
					{
						int limit = atoi(mdp.c_str());
						if (limit < channel->getTotalMembers()) {
							msg = "Wrong limit number" ENDLINE_MSG;
							numeric = ERR_INVALIDMODEPARAM;
						} else {
							channel->addMode('l');
							channel->setLimit(limit);
							sendModeParamMsg(client, *channel, mode, mdp); 
						}
					}
				} else {
					if (!mdp.empty())
					{
						msg = "Not enough parameters" ENDLINE_MSG;
						numeric = ERR_NEEDMOREPARAMS;
					}
					else {
						channel->errMode('l');
						sendModeParamMsg(client, *channel, mode, mdp);
					}
				}
				break ;
			}
			default:{
				msg = "Unknown MODE flag" ENDLINE_MSG;
				numeric = ERR_UMODEUNKNOWNFLAG;
			}
		}
	}
	if (!msg.empty())
		sendNumericCmd(client, numeric, cmd.command, msg);
}