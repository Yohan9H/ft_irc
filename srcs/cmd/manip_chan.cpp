/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manip_chan.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:08:01 by yohurteb          #+#    #+#             */
/*   Updated: 2024/12/21 15:32:37 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

class Server;
class Client;

Channel &createChannel(Server &server, std::string name_channel, Client &first_membre)
{
	std::map<std::string, Channel*>::iterator it = server.findChan(name_channel);
	if (it != server.getChannel().end())
		return *it->second;

	Channel	*new_chan = new Channel();
	new_chan->setName(name_channel);
	new_chan->setKey("");
	new_chan->setSubject("");
	new_chan->setPerm(0, 0, 0, 0, 0);
	new_chan->addOperators(first_membre);
	new_chan->addMembres(first_membre);

	first_membre.addChan(*new_chan);
	server.addChannel(*new_chan);

	return (server.getOneChan(new_chan->getName(), *new_chan));
}

void removeNewline(std::string& nick)
{
	nick.erase(std::remove(nick.begin(), nick.end(), '\n'), nick.end());
}