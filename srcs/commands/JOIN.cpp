#include "JOIN.hpp"

JOIN::JOIN() : Command("JOIN", 1, true) {};
JOIN::~JOIN() {};

void JOIN::execCommand(Server &serv, Client &client, const com &cmd)
{

	std::vector<std::string> channel_list = split(cmd.params[0], ',');
	std::string pass = (cmd.params.size() > 1) ? cmd.params[1] : ""; 
	std::vector<std::string> mdp_list = split(pass, ',');
	
	std::map<std::string,std::string> chan_mdp = createMap(channel_list, mdp_list);

	//refaire join en verifiant que quand quelqu'un a ete invite puisqu'il join, il faut retirer
	// de la liste des invited
	for (std::map<std::string,std::string>::iterator it = chan_mdp.begin(); it != chan_mdp.end(); it++)
	{
		std::string name_chan = it->first;
		std::string mdp = it->second;
		std::string msg;
		int numeric;
		// Verif if '#'
		if (name_chan[0] != '#')
		{
			msg = "Bad Channel Mask";
			numeric = ERR_BADCHANMASK;
			OutDataNumericParam1(client, numeric, name_chan, msg + ENDLINE_MSG);
		}
		else 
		{
			// Recup de la chaine
			Channel *channel = createChannel(serv, name_chan, client);

			// Permissions
			if (channel->ifInvite() && !channel->checkClientIsInvited(client.getClientSocket()))
			{
				msg = "Cannot join channel (+i)";
				numeric = ERR_INVITEONLYCHAN;
				OutDataNumericParam2(client, numeric, client.getNickname(), name_chan, msg + ENDLINE_MSG);
			}
			else if (channel->checkClientIsMembre(client.getClientSocket()))
			{
				msg = "You're already on that channel";
				numeric = ERR_USERONCHANNEL;
				OutDataNumericParam2(client, numeric, client.getNickname(), name_chan, msg + ENDLINE_MSG);
			}
			else if (channel->ifProtectedByPassWord() && !channel->checkPassWord(mdp))
			{
				msg = "Cannot join channel (+k)";
				numeric = ERR_BADCHANNELKEY;
				OutDataNumericParam2(client, numeric, client.getNickname(), name_chan, msg + ENDLINE_MSG);
			}
			else if (channel->ifLimitUser() && channel->checkOverLimitUser())
			{
				msg = "Cannot join channel (+l)";
				numeric = ERR_CHANNELISFULL;
				OutDataNumericParam2(client, numeric, client.getNickname(), name_chan, msg + ENDLINE_MSG);
			}
			else 
			{
				// Ajouter le membre et supprimer des invited
				channel->addMembres(client.getClientSocket());
				if (channel->checkClientIsInvited(client.getClientSocket()))
					channel->delInvited(client.getClientSocket());

				// Ajouter le chan au listchan du client
				std::vector<std::string> &listChan = client.getListChanJoined();
				listChan.push_back(name_chan);
				
				//Envoyer les infos:
				std::string msg1 = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost JOIN " + channel->getName();
				channel->sendMsgMembres(msg1 + ENDLINE_MSG);
				if (!channel->getTopic().empty())
				{
					std::string msg2 = HOST + to_string(RPL_TOPIC) + " " + client.getNickname() + " " + channel->getTopic();
					channel->sendMsgMembres(msg2 + ENDLINE_MSG);
				}
				std::string msg3 = HOST + to_string(RPL_NAMREPLY) + " "  + client.getNickname() + " = " + channel->getName() + " :" + channel->giveAllNameMembres(serv);
				channel->sendMsgMembres(msg3 + ENDLINE_MSG);
				std::string msg4 = HOST + to_string(RPL_ENDOFNAMES) + " " + client.getNickname() + " " + channel->getName() + " :End of /NAMES list";
				channel->sendMsgMembres(msg4 + ENDLINE_MSG);
			}
		}
	}
}