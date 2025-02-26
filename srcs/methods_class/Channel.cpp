#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(const Channel &src)
	:	_name(src._name),
		_pass(src._pass),
		_topic(src._topic),
		_limit_user(src._limit_user),
		_modes(src._modes),
		_operatorsFd(src._operatorsFd),
		_membresFd(src._membresFd),
		_invitedFd(src._invitedFd)
{

}

Channel::~Channel()
{

}


Channel		&Channel::operator=(const Channel &src)
{
	if (this != &src)
	{
		_name = src._name;
		_pass = src._pass;
		_topic = src._topic;
		_limit_user = src._limit_user;
		_modes = src._modes;
		_operatorsFd = src._operatorsFd;
		_membresFd = src._membresFd;
		_invitedFd = src._invitedFd;
	}
	return *this;
}



std::string		Channel::getKey() const
{
	return _pass;
}

std::string		Channel::getName() const
{
	return _name;
}

std::string		Channel::getTopic() const
{
	return _topic;
}

std::vector<char>		&Channel::getModes()
{
	return _modes;
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


void	Channel::setPass(std::string key)
{
	_pass = key;
}

void	Channel::setName(std::string name)
{
	_name = name;
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void	Channel::setLimit(int limit)
{
	_limit_user = limit;
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
		_membresFd.erase(it);
}

void		Channel::delInvited(int clientFd)
{
	std::vector<int>::iterator it = std::find(_invitedFd.begin(), _invitedFd.end(), clientFd);
	if (it != _invitedFd.end())
		_invitedFd.erase(it);
}

std::string	Channel::formatJoinMessage(std::string name_new_client, Channel channel)
{
	std::string msg = ":" + name_new_client + " JOIN " + channel.getName() + "\r\n";
	return msg;
}

void	Channel::infoJoinChannel(Server &serv, std::string name_serv, Channel &channel, Client &client)
{
	std::string msg = ":" + name_serv + " 332 " + client.getNickname() + " " + channel.getTopic() + " :Bienvenue dans le channel !\n";
	msg += ":" + name_serv + " 353 " + client.getNickname() + " = " + channel.getTopic() + " :" + channel.giveAllNameMembres(serv) + "\n"; 
	msg += ":" + name_serv + " 366 " + client.getNickname() + " " + channel.getTopic() + " :End of /NAMES list\n";
	client.appendOutData(msg);
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
			msg += "@" + client_it->getNickname() + " ";
		}
		else
		{
			msg += client_it->getNickname() + " ";
		}
	}
	return msg;
}

void	Channel::sendMsgMembres(std::string msg, Server& serv)
{
	for (std::vector<int>::iterator it = _membresFd.begin(); it != _membresFd.end() ;it++)
	{
		Client* client = serv.getClientbyFd(*it);
		client->appendOutData(msg);
		std::string am = "shit";
	}
}

void	Channel::sendMsgMembresExceptFd(std::string msg, int clientFd, Server& serv) {
	for (std::vector<int>::iterator it = _membresFd.begin(); it != _membresFd.end() ;it++)
	{
		if (*it != clientFd)
		{
			Client* client = serv.getClientbyFd(*it);
			client->appendOutData(msg);
		}
	}
}


bool	Channel::ifInvite()
{
	if (this->hasMode('i') == 1)
		return 1;
	else
		return 0;
}

bool	Channel::ifTopic()
{
	if (this->hasMode('t') == 1)
		return 1;
	else
		return 0;
}

bool	Channel::ifProtectedByPassWord()
{
	if (this->hasMode('k') == 1)
		return 1;
	else
		return 0;
}

bool	Channel::ifLimitUser()
{
	if (this->hasMode('l') == 1)
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
	if (_pass == mdp)
		return 1;
	else
		return 0;
}

bool	Channel::checkClientIsInvited(int clientFd)
{
	std::vector<int>::iterator it = std::find(_invitedFd.begin(), _invitedFd.end(), clientFd);
	if (it != _invitedFd.end())
		return true;
	else
		return false;
}

bool	Channel::checkClientIsMembre(int clientFd) {
	std::vector<int>::iterator it = std::find(_membresFd.begin(), _membresFd.end(), clientFd);
	if (it != _membresFd.end())
		return true;
	else
		return false;
}

bool	Channel::checkClientIsOperator(int clientFd) {
	std::vector<int>::iterator it = std::find(_operatorsFd.begin(), _operatorsFd.end(), clientFd);
	if (it != _operatorsFd.end())
		return true;
	else
		return false;
}

bool	Channel::checkOverLimitUser()
{
	if (_membresFd.size() >= _limit_user)
		return 1;
	else
		return 0;
}

bool	Channel::hasMode(char mode) {
	std::vector<char>::iterator it = std::find(_modes.begin(), _modes.end(), mode);
	if (it != _modes.end())
		return (true);
	else
		return (false);
}

void	Channel::addMode(char mode) {
	if (std::find(_modes.begin(), _modes.end(), mode) == _modes.end()) {
		_modes.push_back(mode);
	}
}

void	Channel::errMode(char mode) {
	std::vector<char>::iterator it = std::find(_modes.begin(), _modes.end(), mode);
	if (it != _modes.end())
		_modes.erase(it);
}

int		Channel::getTotalMembers() {
	return (_membresFd.size());
}

void	Channel::printChannelForTest(Server &serv) 
{
	std::cout << "--- INFO CHANNEL TEST ---\n" << std::endl;
	std::cout << "Channel Name - " << this->_name << std::endl;
	std::cout << "Topic - " << this->_topic << std::endl;
	std::cout << "limit - " << this->_limit_user << std::endl;
	for (std::vector<char>::iterator it = this->_modes.begin(); it != this->_modes.end(); it++)
	{
		std::cout << "mode - " << *it << std::endl;
	}
	std::cout << "Operators - ";
	for (std::vector<int>::iterator it = this->_operatorsFd.begin(); it != this->_operatorsFd.end(); it++)
	{
		Client* client = serv.getClientbyFd(*it);
		std::cout << client->getNickname() << " - ";
	}
	std::cout << std::endl;
	std::cout << "membres - ";
	for (std::vector<int>::iterator it = this->_membresFd.begin(); it != this->_membresFd.end(); it++)
	{
		Client* client = serv.getClientbyFd(*it);
		std::cout << client->getNickname() << " - ";
	}
	std::cout << std::endl;
	std::cout << "Invited - ";
	for (std::vector<int>::iterator it = this->_invitedFd.begin(); it != this->_invitedFd.end(); it++)
	{
		Client* client = serv.getClientbyFd(*it);
		std::cout << client->getNickname() << " - ";
	}
	std::cout << std::endl;
	std::cout << "----- END CHANNEL TEST -----" << std::endl;
}