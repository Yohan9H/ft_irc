/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_all.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:26:36 by yohurteb          #+#    #+#             */
/*   Updated: 2024/12/21 15:42:44 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/command.hpp"

bool	join(Server &serv, Client &client, std::string name_chan, std::string mdp)
{
	// Recup de la chaine
	Channel channel = createChannel(serv, name_chan, client);
	std::string msg;

	// Verif client is identify
	if (client.if_identify(2) == false)
	{
		msg = ":" + std::string(NAME_SERV) + " 451 You have not registered\n";
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}
	
	// Permissions
	if (channel.ifProtectedByPassWord() && !channel.checkPassWord(mdp))
	{
		msg = mdp_false(NAME_SERV, name_chan);
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}

	if (channel.ifInvite() && !channel.checkClientIsInvited(client))
	{
		msg = invite_false(NAME_SERV, "JOIN", name_chan);
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}

	if (channel.ifLimitUser() && !channel.checkLimitUser())
	{
		msg = limit_user_false(NAME_SERV, client.getName());
		send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
		return false;
	}

	// Ajouter le membre
	channel.addMembres(client);

	// Notifier les autres membres et le nouveau
	channel.sendJoinMsgAll(channel);

	// Envoyer les infos du channel au client
	channel.infoJoinChannel(NAME_SERV, channel, client);

	return true;
}

bool	is_good(char c)
{
	std::string valide_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-#&+!.^,`|~$%*:<>\n";
	std::size_t pos = valide_char.find(c);

	if (pos != std::string::npos)
		return true;
	else
		return false;
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
	
	// Verif if already use
	for (std::map<std::string, Client>::const_iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->first == nick)
		{
			msg = msg_err(NAME_SERV, "433", nick, ":Nickname is already in use");
			send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
			return false;
		}
	}

	removeNewline(nick);
	client.setNick(nick);

	// Voir si besoin d'envoye un msg a client qui met ou change son nom

	//Informe tous les channels dont il fait partie de son changement
	if (client.getName().empty())
	{
		msg = ":_NoName_!user@" + std::string(NAME_SERV) + "NICK :";
		client.sendMsgAllChanNickInform(msg); 
	}
	else
	{
		msg = ":" + client.getName() + "!user@" + std::string(NAME_SERV) + "NICK :";
		client.sendMsgAllChanNickInform(msg); 
	}

	client.print_for_test();
	return true;
}

bool	User(Server &serv, Client &client, std::string username)
{
	std::string msg;

	// Verif client is identify
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
	for (std::map<std::string, Client>::const_iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->first == username)
		{
			msg = msg_err(NAME_SERV, "433", username, ":Nickname is already in use");
			send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
			return false;
		}
	}

	client.setName(username);

	msg = ":" + std::string(NAME_SERV) + " 001 " + client.getNick() + " :Welcome to the Internet Relay Network" + client.getNick() + "!" + client.getName() + "@" + "\n"; // + hostname (hostname pas encore implemente)  
	msg += ":" + std::string(NAME_SERV) + " 002 " + client.getNick() + " :Your host is " + std::string(NAME_SERV) + ", running version ... (a  voir)\n";
	msg += ":" + std::string(NAME_SERV) + " 003 " + client.getNick() + " :This server was created ... (a voir)\n";
	send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
	
	client.print_for_test();
	return true;
}
