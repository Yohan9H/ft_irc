#include "MODE.hpp"

MODE::MODE() : Command("MODE", 1, true) {};
MODE::~MODE() {};

void MODE::execCommand(Server &serv, Client &client, const com &cmd)
{
	//rajouter message qui annonce le changement de mode
	 
	std::string channel_name = cmd.params[0];
	if (channel_name[0] != '#' || cmd.params.size() < 2)
		return ;
	std::string mode = cmd.params[1];
	std::string mdp = (cmd.params.size() > 2) ? cmd.params[2] : "";

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
		char sign = mode[0];
		char type = mode[1];
		if (mode.length() != 2 || ((sign != '-') && (sign != '+')))
		{
			std::cerr << "ERR_UMODEUNKNOWNFLAG" << std::endl;
			return ;
		}
		switch (type) {
			// Invite-only
			case 'i': {
				if (sign == '+')
					channel->addMode('i');
				else
					channel->errMode('i');
				break;
			};
			// TOPIC
			case 't': {
				if (sign == '+')
					channel->addMode('t');
				else
					channel->errMode('t');
				break;
			};
			//Password
			case 'k': {
				if (sign == '+') {
					if (mdp.empty()) {
						std::cerr << "ERR_NOPASSWORD" << std::endl;
					} else {
						channel->setKey(mdp);
						channel->addMode('k');
						std::cout << "channel password added" << std::endl; 
					}
				}
				else {
					channel->errMode('k');
					std::cout << "channel password removed" << std::endl;
				}
				break;
			};
			//Operator
			case 'o': {
				if (mdp.empty()) {
					std::cerr << "ERR_NOOP" << std::endl;
				} else {
					Client* modeOperator = serv.getClientbyName(mdp);
					if (!modeOperator)
					{
						std::cerr << "ERR_WRONOPERATORNAME" << std::endl;
						return;
					}
					if (sign == '+') {
						if (!channel->isOperator(modeOperator->getClientSocket()))
						{
							channel->addOperators(modeOperator->getClientSocket());
							channel->addMode('o');
							std::cout << modeOperator->getNickname() << " added as operator" << std::endl;
						} else {
							std::cerr << "ERR_ALREADYOPERATOR" << std::endl;
						}
					} else {
						if (channel->isOperator(modeOperator->getClientSocket()))
						{
							channel->delOperatores(modeOperator->getClientSocket());
							channel->errMode('o');
							std::cout << modeOperator->getNickname() << " removed as operator" << std::endl;
						} else {
							std::cerr << "ERR_WASNOTOPERATOR" << std::endl;
						}
					}
				}
				break ;
			};
			//Limits
			case 'l': {
				if (sign == '+') {
					if (mdp.empty())
						std::cerr << "ERR_NOLIMIT" << std::endl;
					else {
						int limit = atoi(mdp.c_str());
						if (limit < channel->getTotalMembers()) {
							std::cerr << "ERR_WRONGLIMIT" << std::endl;
						} else {
							channel->addMode('l');
							channel->setLimit(limit);
							std::cout << "Channel set limit to " << limit << std::endl; 
						}
					}
				} else {
					if (!mdp.empty())
						std::cerr << "ERR_WRONGPARAM" << std::endl;
					else {
						channel->errMode('l');
						std::cout << "Channel limit removed " << std::endl;
					}
				}
				break ;
			}
			default:{
				std::cerr << "ERR_UMODEUNKNOWNFLAG" << std::endl;
				return ;
			}
		}
		

	}
}