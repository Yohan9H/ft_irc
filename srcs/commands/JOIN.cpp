#include "JOIN.hpp"

JOIN::JOIN() : Command("JOIN", 1, true) {};
JOIN::~JOIN() {};

void JOIN::execCommand(Server &serv, Client &client, const com &cmd)
{
	std::string msg;
	std::string name_chan = cmd.params[0];
	std::string mdp = (cmd.params.size() > 1) ? cmd.params[1] : "";

	// Verif if '#'
	if (name_chan[0] && name_chan[0] != '#')
		return ;

	// Verif client is identify
	if (client.getIsAuth() == false)
	{
		msg = ":" + std::string(NAME_SERV) + " 451 You have not registered" ENDLINE_MSG;
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}

	// Recup de la chaine
	Channel *channel = createChannel(serv, name_chan, client);

	// Permissions
	if (channel->ifProtectedByPassWord() && !channel->checkPassWord(mdp))
	{
		msg = mdp_false(NAME_SERV, name_chan);
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}

	if (channel->ifInvite() && !channel->checkClientIsInvited(client.getClientSocket()))
	{
		msg = invite_false(NAME_SERV, "JOIN", name_chan);
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}

	if (channel->ifLimitUser() && !channel->checkLimitUser())
	{
		msg = limit_user_false(NAME_SERV, client.getUsername());
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
":" + std::string(NAME_SERV) + " 451 You have not registered" ENDLINE_MSG;
	// Ajouter le membre
	channel->addMembres(client.getClientSocket());

	// Ajouter le chan au listchan du client
	std::vector<std::string> &listChan = client.getListChanJoined();
	listChan.push_back(name_chan);

	// Notifier les autres membres et le nouveau
	channel->sendJoinMsgAll(*channel, client.getUsername(), client.getClientSocket());

	// Envoyer les infos du channel au client
	channel->infoJoinChannel(serv, NAME_SERV, *channel, client);

}