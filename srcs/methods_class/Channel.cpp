/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apernot <apernot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:59:13 by yohurteb          #+#    #+#             */
/*   Updated: 2025/01/13 18:05:38 by apernot          ###   ########.fr       */
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
		_operatorsFd(src._operatorsFd),
		_membresFd(src._membresFd)
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
		_operatorsFd = src._operatorsFd;
		_membresFd = src._membresFd;
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

std::vector<int>	&Channel::getOperatorsFd()
{
	return _operatorsFd;
}

std::vector<int>	&Channel::getMembresFd()
{
	return _membresFd;
}

std::vector<int>	&Channel::getInvitedFd()
{
	return _invitedFd;
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


void	Channel::addOperators(int clientFd)
{
	_operatorsFd.push_back(clientFd);
}

void	Channel::addMembres(int clientFd)
{
	_membresFd.push_back(clientFd); 
}

void	Channel::addInvited(int clientFd)
{
	_invitedFd.push_back(clientFd);
}

void	Channel::delOperatores(int clientFd)
{
	std::vector<int>::iterator it = std::find(_operatorsFd.begin(), _operatorsFd.end(), clientFd);
	if (it != _operatorsFd.end())
		_operatorsFd.erase(it);
}

void	Channel::delMembres(int clientFd)
{
	std::vector<int>::iterator it = std::find(_membresFd.begin(), _membresFd.end(), clientFd);
	if (it != _membresFd.end())
	{
		_membresFd.erase(it);
	}
}

std::string	Channel::formatJoinMessage(std::string name_new_client, Channel channel)
{
	std::string msg = ":" + name_new_client + " JOIN " + channel.getName();
	return msg;
}

void	Channel::sendJoinMsgAll(Channel &channel, std::string username_client, int clientFd)
{
	for (std::vector<int>::iterator it = _membresFd.begin(); it != _membresFd.end(); it++)
	{	
		if (*it != clientFd)
		{
			std::string msg = formatJoinMessage(username_client, channel);
			send(*it, msg.c_str(), msg.size(), 0);
		}
	}
}

void	Channel::infoJoinChannel(Server &serv, std::string name_serv, Channel &channel, Client &client)
{
	std::string msg = ":" + name_serv + " 332 " + client.getUsername() + " " + channel.getSubject() + " :Bienvenue dans le channel !\n";
	send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
	msg.clear();

	msg = ":" + name_serv + " 353 " + client.getUsername() + " = " + channel.getSubject() + " :" + channel.giveAllNameMembres(serv) + "\n"; 
	send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
	msg.clear();

	msg = ":" + name_serv + " 366 " + client.getUsername() + " " + channel.getSubject() + " :End of /NAMES list\n";
	send(client.getClientSocket(), msg.c_str(), msg.size(), 0);
}

std::string Channel::giveAllNameMembres(Server &serv)
{
	std::string msg;

	for (std::vector<int>::iterator it = _membresFd.begin(); it != _membresFd.end(); it++)
	{
		Client *client_it = serv.getClientbyFd(*it);

		std::vector<int>::iterator it_ope = std::find(_operatorsFd.begin(), _operatorsFd.end(), *it);
		if (it_ope != _operatorsFd.end())
		{
			msg += "@" + client_it->getUsername() + " ";
		}
		else
		{
			msg += client_it->getUsername() + " ";
		}
	}
	return msg;
}

void	Channel::sendMsgMembres(std::string msg)
{
	for (std::vector<int>::iterator it = _membresFd.begin(); it != _membresFd.end() ;it++)
	{
		send(*it, msg.c_str(), msg.size(), 0);
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

bool	Channel::isOperator(int clientFd) {
	std::vector<int> listOperators = this->getOperatorsFd();
	std::vector<int>::iterator it = std::find(listOperators.begin(), listOperators.end(), clientFd);
	if (it != listOperators.end())
		return (true);
	else
		return (false);
}

bool	Channel::checkPassWord(std::string mdp)
{
	if (_key == mdp)
		return 1;
	else
		return 0;
}

bool	Channel::checkClientIsInvited(int clientFd)
{
	std::vector<int> listInvited = this->getInvitedFd();
	std::vector<int>::iterator it = std::find(listInvited.begin(), listInvited.end(), clientFd);
	if (it != _invitedFd.end())
		return true;
	else
		return false;
}

bool	Channel::checkClientIsMembre(int clientFd) {
	std::vector<int> listMembres = this->getMembresFd();
	std::vector<int>::iterator it = std::find(listMembres.begin(), listMembres.end(), clientFd);
	if (it != _membresFd.end())
		return true;
	else
		return false;
}

bool	Channel::checkLimitUser()
{
	if (_membresFd.size() + 1 != _limit_user)
		return 1;
	else
		return 0;
}