#include "Server.hpp"
#include "Command.hpp"

bool	join(Server &serv, Client &client, std::string name_chan, std::string mdp)
{
	std::string msg;

	// Verif if '#'
	if (name_chan[0] && name_chan[0] != '#')
		return false;

	// Verif client is identify
	if (client.getIsAuth() == false)
	{
		msg = ":" + std::string(HOST) + " 451 You have not registered\n";
		//send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		client.appendOutData(msg);
		return false;
	}

	// Recup de la chaine
	Channel *channel = createChannel(serv, name_chan, client);

	// Permissions
	if (channel->ifProtectedByPassWord() && !channel->checkPassWord(mdp))
	{
		msg = mdp_false(HOST, name_chan);
		//send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		client.appendOutData(msg);
		return false;
	}

	if (channel->ifInvite() && !channel->checkClientIsInvited(client.getClientSocket()))
	{
		msg = invite_false(HOST, "JOIN", name_chan);
		//send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		client.appendOutData(msg);
		return false;
	}

	if (channel->ifLimitUser() && !channel->checkOverLimitUser())
	{
		msg = limit_user_false(HOST, client.getUsername());
		//send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		client.appendOutData(msg);
		return false;
	}

	// Ajouter le membre
	channel->addMembres(client.getClientSocket());

	// Ajouter le chan au listchan du client
	std::vector<std::string> &listChan = client.getListChanJoined();
	listChan.push_back(name_chan);

	// Notifier les autres membres et le nouveau
	//channel->sendJoinMsgAll(*channel, client.getUsername(), client.getClientSocket());

	// Envoyer les infos du channel au client
	channel->infoJoinChannel(serv, HOST, *channel, client);

	return true;
}



bool	Nick(Server &serv, Client &client, std::string nick)
{
	std::string msg;

	// Verif valide name
	if (nick.length() > 9)
	{
		msg = msg_err(HOST, "432", nick, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return false;
	}
	bool verif_char = true;
	for (std::string::iterator it = nick.begin(); it != nick.end(); ++it)
	{
		if (is_good(*it) == false)
		{
			verif_char = false;
			break;
		}
	}
	if (verif_char == false)
	{
		msg = msg_err(HOST, "432", nick, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return false;
	}
	if (std::isdigit(nick[0]))
	{
		msg = msg_err(HOST, "432", nick, ":Erroneous nickname (debug - isalpha)");
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return false;
	}
	
	removeNewline(nick);

	// Verif if already use
	for (std::map<int, Client*>::iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->second->getNickname() == nick)
		{
			std::cout << "in if" << std::endl;
			msg = msg_err(HOST, "433", nick, ":Nickname is already in use");
			send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
			return false;
		}
	}

	client.setNick(nick);

	// -- Voir si besoin d'envoye un msg a client qui met ou change son nom ?? --

	// Informe tous les channels dont il fait partie de son changement
	if (client.getUsername().empty())
	{
		msg = ":_NoName_!user@" + std::string(HOST) + "NICK :";
		client.sendMsgAllChan(serv, msg); 
	}
	else
	{
		msg = ":" + client.getUsername() + " !user@" + std::string(HOST) + "NICK :";
		client.sendMsgAllChan(serv, msg);
	}

	client.print_for_test();
	return true;
}

bool	User(Server &serv, Client &client, std::string username)
{
	std::string msg;

	// Verif client if identify
	if (client.if_identify(1) == false)
	{
		msg = ":" + std::string(HOST) + " 431 No nickname given\n";
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return false;
	}

	// Verif valide name
	removeNewline(username);
	if (username.length() > 9)
	{
		msg = msg_err(HOST, "432", username, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return false;
	}
	bool verif_char = true;
	for (std::string::iterator it = username.begin(); it != username.end(); ++it)
	{
		if (is_good(*it) == false)
		{
			verif_char = false;
			break;
		}
	}
	if (verif_char == false)
	{
		msg = msg_err(HOST, "432", username, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return false;
	}
	if (std::isdigit(username[0]))
	{
		msg = msg_err(HOST, "432", username, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return false;
	}

	// Verif if already use
	for (std::map<int, Client*>::const_iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->second->getUsername() == username)
		{
			msg = msg_err(HOST, "433", username, ":Username is already in use");
			send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
			return false;
		}
	}

	client.setName(username);
	client.AuthIsGood();

	msg = ":" + std::string(HOST) + " 001 " + client.getNickname() + " :Welcome to the ft_irc " + client.getNickname() + "! " + client.getUsername() + "@" + "localhost\n";
	msg += ":" + std::string(HOST) + " 002 " + client.getNickname() + " :Your host is ft_irc 1.0\n";
	msg += ":" + std::string(HOST) + " 003 " + client.getNickname() + " :Created at [" + serv.getTime() + "]\n";
	send(client.getClientSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	
	client.print_for_test();

	return true;
}

// FOR TEST

