/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manip_chan.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apernot <apernot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:08:01 by yohurteb          #+#    #+#             */
/*   Updated: 2025/01/27 14:29:30 by apernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

class Server;
class Client;

Channel *createChannel(Server &server, std::string name_channel, Client &first_membre)
{
	std::map<std::string, Channel*>::iterator it = server.findChan(name_channel);
	if (it != server.getChannel().end())
		return it->second;

	Channel	*new_chan = new Channel();
	new_chan->setName(name_channel);
	new_chan->setPass("");
	new_chan->setTopic("");
	new_chan->setLimit(-1);
	new_chan->addOperators(first_membre.getClientSocket());

	server.addChannel(*new_chan);

	return (new_chan);
}

void removeNewline(std::string& nick)
{
	nick.erase(std::remove(nick.begin(), nick.end(), '\n'), nick.end());
}

