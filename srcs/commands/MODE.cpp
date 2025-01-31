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
		OutDataNumericParam2(client, numeric, client.getNickname(), channel_name, msg + ENDLINE_MSG);
	}
	else
	{
		std::string mode = cmd.params[1];
		std::string param = (cmd.params.size() > 2) ? cmd.params[2] : "";

		Channel *channel = serv.getChannelbyName(channel_name);

		if (!channel)
		{
			msg = "No such channel";
			numeric = ERR_NOSUCHCHANNEL;
			OutDataNumericParam2(client, numeric, client.getNickname(), channel_name, msg + ENDLINE_MSG);
		}
		else if (!channel->isOperator(client.getClientSocket()))
		{
			msg = "You're not channel operator";
			numeric = ERR_CHANOPRIVSNEEDED;
			OutDataNumericParam2(client, numeric, client.getNickname(), channel_name, msg + ENDLINE_MSG);
		}
		else {
			char sign = mode[0];
			char type = mode[1];
			if (mode.length() != 2 || ((sign != '-') && (sign != '+')))
			{
				msg = "Unknown MODE flag";
				numeric = ERR_UMODEUNKNOWNFLAG;
				OutDataNumericParam1(client, numeric, client.getNickname(), msg + ENDLINE_MSG);
			}
			switch (type) {
				// Invite-only
				case 'i': {
					if (sign == '+')
					{
						channel->addMode('i');
						sendModeParamMsg(client, *channel, mode, param, serv);
					}
					else if (sign == '-') 
					{
						channel->errMode('i');
						sendModeParamMsg(client, *channel, mode, param, serv);
					}
					break;
				};
				// TOPIC
				case 't': {
					if (sign == '+')
					{
						channel->addMode('t');
						sendModeParamMsg(client, *channel, mode, param, serv);
					}
					else if (sign == '-')
					{
						channel->errMode('t');
						sendModeParamMsg(client, *channel, mode, param, serv);
					}
					break;
				};
				//Password
				case 'k': {
					if (sign == '+') {
						if (param.empty()) {
							msg = "Wrong password format";
							numeric = ERR_INVALIDMODEPARAM;
							OutDataNumericParam3(client, numeric, client.getNickname(), channel_name, "k", msg + ENDLINE_MSG);
						} else {
							channel->setPass(param);
							channel->addMode('k');
							sendModeParamMsg(client, *channel, mode, param, serv);
						}
					}
					else if (sign == '-') {
						channel->errMode('k');
						sendModeParamMsg(client, *channel, mode, param, serv);
					}
					break;
				};
				//Operator
				case 'o': {
					if (param.empty()) {
							msg = "Not enough parameters";
							numeric = ERR_NEEDMOREPARAMS;
							OutDataNumericParam2(client, numeric, client.getNickname(), "mode +o", msg + ENDLINE_MSG);
					} else {
						Client* modeOperator = serv.getClientbyName(param);
						if (!modeOperator)
						{
							msg = "No such nick";
							numeric = ERR_NOSUCHNICK;
							OutDataNumericParam2(client, numeric, client.getNickname(), cmd.params[2], msg + ENDLINE_MSG);
						}
						else if (!channel->checkClientIsMembre(modeOperator->getClientSocket()))
						{
							msg = "They aren't on that channel";
							numeric = ERR_USERNOTINCHANNEL;
							OutDataNumericParam3(client, numeric, client.getNickname(), modeOperator->getNickname(), channel_name, msg + ENDLINE_MSG);
						} else {
							if (sign == '+') {
								if (channel->checkClientIsOperator(modeOperator->getClientSocket()))
								{
									msg = "They are already an operator";
									numeric = ERR_USERNOTINCHANNEL;
									OutDataNumericParam3(client, numeric, client.getNickname(), modeOperator->getNickname(), channel_name, msg + ENDLINE_MSG);
								} else {
									channel->addOperators(modeOperator->getClientSocket());
									channel->addMode('o');
									sendModeParamMsg(client, *channel, mode, param, serv);
								}
							} else if (sign == '-') {
								if (channel->getOperatorsFd().size() > 1)
								{
									if (channel->checkClientIsOperator(modeOperator->getClientSocket()))
									{
										channel->delOperatores(modeOperator->getClientSocket());
										channel->errMode('o');
										sendModeParamMsg(client, *channel, mode, param, serv);
									}
									else
									{
										msg = "They are not an operator";
										numeric = ERR_USERNOTINCHANNEL;
										OutDataNumericParam3(client, numeric, client.getNickname(), modeOperator->getNickname(), channel_name, msg + ENDLINE_MSG);
									}

								} else if (channel->checkClientIsOperator(modeOperator->getClientSocket())) {
									msg = "Cannot have no operator";
									numeric = ERR_USERNOTINCHANNEL;
									OutDataNumericParam3(client, numeric, client.getNickname(), modeOperator->getNickname(), channel_name, msg + ENDLINE_MSG);
								}
								else
								{
									msg = "They are not an operator";
									numeric = ERR_USERNOTINCHANNEL;
									OutDataNumericParam3(client, numeric, client.getNickname(), modeOperator->getNickname(), channel_name, msg + ENDLINE_MSG);
								}
							}
						}
					}
					break ;
				};
				//Limits
				case 'l': {
					if (sign == '+') {
						if (param.empty())
						{
							msg = "Not enough parameters";
							numeric = ERR_NEEDMOREPARAMS;
							OutDataNumericParam2(client, numeric, client.getNickname(), "mode +l", msg + ENDLINE_MSG);
						}
						else 
						{
							int limit = atoi(param.c_str());
							if (limit < channel->getTotalMembers()) {
								msg = "Wrong limit number";
								numeric = ERR_INVALIDMODEPARAM;
								OutDataNumericParam3(client, numeric, client.getNickname(), channel_name, "l", msg + ENDLINE_MSG);
							} else {
								channel->addMode('l');
								channel->setLimit(limit);
								sendModeParamMsg(client, *channel, mode, param, serv); 
							}
						}
					} else {
							channel->errMode('l');
							channel->setLimit(-1);
							sendModeParamMsg(client, *channel, mode, param, serv);
					}
					break ;
				}
				default:{
					msg = "Unknown MODE flag";
					numeric = ERR_UMODEUNKNOWNFLAG;
					OutDataNumericParam1(client, numeric, client.getNickname(), msg + ENDLINE_MSG);
				}
			}
		}
	}
}