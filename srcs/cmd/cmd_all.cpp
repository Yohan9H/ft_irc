/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_all.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apernot <apernot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:26:36 by yohurteb          #+#    #+#             */
/*   Updated: 2025/01/10 14:35:11 by apernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Command.hpp"
#include "../includes/Command2.hpp"

bool	join(Server &serv, Client &client, std::string name_chan, std::string mdp)
{
	std::string msg;

	// Verif if '#'
	if (name_chan[0] && name_chan[0] != '#')
		return false;

	// Verif client is identify
	if (client.getIsAuth() == false)
	{
		msg = ":" + std::string(NAME_SERV) + " 451 You have not registered\n";
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}

	// Recup de la chaine
	Channel *channel = createChannel(serv, name_chan, client);

	// Permissions
	if (channel->ifProtectedByPassWord() && !channel->checkPassWord(mdp))
	{
		msg = mdp_false(NAME_SERV, name_chan);
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}

	if (channel->ifInvite() && !channel->checkClientIsInvited(client.getClientSocket()))
	{
		msg = invite_false(NAME_SERV, "JOIN", name_chan);
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}

	if (channel->ifLimitUser() && !channel->checkLimitUser())
	{
		msg = limit_user_false(NAME_SERV, client.getUsername());
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}

	// Ajouter le membre
	channel->addMembres(client.getClientSocket());

	// Ajouter le chan au listchan du client
	std::vector<std::string> &listChan = client.getListChanJoined();
	listChan.push_back(name_chan);

	// Notifier les autres membres et le nouveau
	channel->sendJoinMsgAll(*channel, client.getUsername(), client.getClientSocket());

	// Envoyer les infos du channel au client
	channel->infoJoinChannel(serv, NAME_SERV, *channel, client);

	return true;
}



bool	Nick(Server &serv, Client &client, std::string nick)
{
	std::string msg;

	// Verif valide name
	if (nick.length() > 9)
	{
		msg = msg_err(NAME_SERV, "432", nick, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
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
		msg = msg_err(NAME_SERV, "432", nick, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}
	if (std::isdigit(nick[0]))
	{
		msg = msg_err(NAME_SERV, "432", nick, ":Erroneous nickname (debug - isalpha)");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}
	
	removeNewline(nick);

	// Verif if already use
	for (std::map<int, Client*>::iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->second->getNickname() == nick)
		{
			std::cout << "in if" << std::endl;
			msg = msg_err(NAME_SERV, "433", nick, ":Nickname is already in use");
			send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
			return false;
		}
	}

	client.setNick(nick);

	// -- Voir si besoin d'envoye un msg a client qui met ou change son nom ?? --

	// Informe tous les channels dont il fait partie de son changement
	if (client.getUsername().empty())
	{
		msg = ":_NoName_!user@" + std::string(NAME_SERV) + "NICK :";
		client.sendMsgAllChan(serv, msg); 
	}
	else
	{
		msg = ":" + client.getUsername() + " !user@" + std::string(NAME_SERV) + "NICK :";
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
		msg = ":" + std::string(NAME_SERV) + " 431 No nickname given\n";
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}

	// Verif valide name
	removeNewline(username);
	if (username.length() > 9)
	{
		msg = msg_err(NAME_SERV, "432", username, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
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
		msg = msg_err(NAME_SERV, "432", username, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}
	if (std::isdigit(username[0]))
	{
		msg = msg_err(NAME_SERV, "432", username, ":Erroneous nickname");
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}

	// Verif if already use
	for (std::map<int, Client*>::const_iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->second->getUsername() == username)
		{
			msg = msg_err(NAME_SERV, "433", username, ":Username is already in use");
			send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
			return false;
		}
	}

	client.setName(username);
	client.AuthIsGood();

	msg = ":" + std::string(NAME_SERV) + " 001 " + client.getNickname() + " :Welcome to the ft_irc " + client.getNickname() + "! " + client.getUsername() + "@" + "localhost\n";
	msg += ":" + std::string(NAME_SERV) + " 002 " + client.getNickname() + " :Your host is ft_irc 1.0\n";
	msg += ":" + std::string(NAME_SERV) + " 003 " + client.getNickname() + " :Created at [" + serv.getTime() + "]\n";
	send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
	
	client.print_for_test();

	return true;
}

// FOR TEST

void	showMapClient(Server &serv)
{
	std::cout << "|Map Client|" << std::endl;
	for (std::map<int, Client*>::const_iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		std::cout << "name client : " << it->second->getUsername() << std::endl;	
	}
}

void	showMapChannel(Server &serv)
{
	for (std::map<std::string, Channel*>::const_iterator it = serv.getChannel().begin(); it != serv.getChannel().end(); it++)
	{
		std::cout << "name channel : " << it->first << std::endl;
	}
}