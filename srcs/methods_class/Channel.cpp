/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:59:13 by yohurteb          #+#    #+#             */
/*   Updated: 2024/12/21 15:46:10 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(const Channel &src)
	:	_name(src._name),
		_key(src._key),
		_subject(src._subject),
		_perm(src._perm),
		_operators(src._operators),
		_membres(src._membres)
{
	_perm.i = 0;
	_perm.t = 0;
	_perm.k = 0;
	_perm.o = 0;
	_perm.l = 0;
}

Channel::~Channel()
{

}


Channel		&Channel::operator=(const Channel &src)
{
	if (this != &src)
	{
		_name = src._name;
		_key = src._key;
		_subject = src._subject;
		_perm = src._perm;
		_operators = src._operators;
		_membres = src._membres;
	}
	return *this;
}



std::string		Channel::getKey() const
{
	return _key;
}

std::string		Channel::getName() const
{
	return _name;
}

std::string		Channel::getSubject() const
{
	return _subject;
}

t_perm		Channel::getPerm() const
{
	return _perm;
}

std::map<int, Client*>	&Channel::getOperators()
{
	return _operators;
}

std::map<int, Client*>	&Channel::getMembres()
{
	return _membres;
}



void	Channel::setKey(std::string key)
{
	_key = key;
}

void	Channel::setName(std::string name)
{
	_name = name;
}

void	Channel::setSubject(std::string subject)
{
	_subject = subject;
}

void	Channel::setPerm(bool i, bool t, bool k, bool o, bool l)
{
	_perm.i = i;
	_perm.t = t;
	_perm.k = k;
	_perm.o = o;
	_perm.l = l;
}


void	Channel::addOperators(Client &client)
{
	_operators.insert(std::make_pair(client.getClientSocket(), &client));
}

void	Channel::delOperatores(int socket_key)
{
	_operators.erase(socket_key);
}

void	Channel::addMembres(Client &client)
{
	_membres.insert(std::make_pair(client.getClientSocket(), &client)); 
}

void	Channel::delMembres(int socket_key)
{
	_membres.erase(socket_key);
}

std::string	Channel::formatJoinMessage(std::string name_new_client, Channel channel)
{
	std::string msg = ":" + name_new_client + " JOIN " + channel.getName();
	return msg;
}

void	Channel::sendJoinMsgAll(Channel &channel, std::string name_new_client, int socket_key)
{
	for (std::map<int, Client*>::iterator it = _membres.begin(); it != _membres.end(); it++)
	{
		if (it->first != socket_key)
		{
			std::string msg = formatJoinMessage(name_new_client, channel);
			send(it->second->getClientSocket(), msg.c_str(), msg.size(), 0);
		}
	}
}

void	Channel::infoJoinChannel(std::string name_serv, Channel &channel, Client &client)
{
	std::string msg = ":" + name_serv + " 332 " + client.getName() + " " + channel.getSubject() + " :Bienvenue dans le channel !\n";
	send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
	msg.clear();

	msg = ":" + name_serv + " 353 " + client.getName() + " = " + channel.getSubject() + " :" + channel.giveAllNameMembres() + "\n"; 
	send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
	msg.clear();

	msg = ":" + name_serv + " 366 " + client.getName() + " " + channel.getSubject() + " :End of /NAMES list\n";
	send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
}

std::string Channel::giveAllNameMembres()
{
	std::string msg;

	for (std::map<int, Client*>::iterator it = this->getMembres().begin(); it != this->getMembres().end(); it++)
	{
		std::map<int, Client*>::iterator it_ope = this->getOperators().find(it->first);
		if (it_ope != this->getOperators().end())
			msg += "@" + it->second->getName() + " ";
		else
			msg += it->second->getName() + " ";
	}
	return msg;
}

void	Channel::sendMsgMembre(std::string msg)
{
	for (std::map<int, Client*>::iterator it = _membres.begin(); it != _membres.end() ;it++)
	{
		send(it->second->getClientSocket(), msg.c_str(), msg.size(), 0);
	}
}


bool	Channel::ifInvite()
{
	if (_perm.i == 1)
		return 1;
	else
		return 0;
}

bool	Channel::ifTopic()
{
	if (_perm.t == 1)
		return 1;
	else
		return 0;
}

bool	Channel::ifProtectedByPassWord()
{
	if (_perm.k == 1)
		return 1;
	else
		return 0;
}

bool	Channel::ifLimitUser()
{
	if (_perm.l == 1)
		return 1;
	else
		return 0;
}

bool	Channel::checkPassWord(std::string mdp)
{
	if (_key == mdp)
		return 1;
	else
		return 0;
}

bool	Channel::checkClientIsInvited(Client client)
{
	std::map<int, Client*>::iterator it = _invited.find(client.getClientSocket());
	if (it != _invited.end())
		return 1;
	else
		return 0;
}

bool	Channel::checkLimitUser()
{
	if (_membres.size() + 1 != _limit_user)
		return 1;
	else
		return 0;
}