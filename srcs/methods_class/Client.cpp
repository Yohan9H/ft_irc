#include "../includes/Client.hpp"

Client::Client()
{
}

Client::Client(int clientSocket)  : _clientSocket(clientSocket)
{

}

Client::Client(const Client &other)  : _clientSocket(other._clientSocket)
{

}

Client &Client::operator=(const Client &other)
{
    (void)other;
    return (*this);
}

Client::~Client()
{
}

int &Client::getClientSocket()
{
    return (this->_clientSocket);
}

std::string	&Client::getUsername()
{
	return _username;
}

std::string	&Client::getNickname()
{
	return this->_nick;
}

std::vector<std::string>	&Client::getListChanJoined()
{
	return this->_listChan;
}

void		Client::setClientSocket(int clientFd)
{
	_clientSocket = clientFd;
}

void		Client::setName(std::string new_name)
{
	_username = new_name;
}

void		Client::setNick(std::string new_nick)
{
	_nick = new_nick;
}

void	Client::sendMsgAllChan(Server &serv, std::string msg)
{
	for (std::vector<std::string>::iterator it = _listChan.end(); it != _listChan.end(); it++)
	{
		Channel *channel = serv.getChannelbyName(*it);
		channel->sendMsgMembres(msg);
	}
}

bool	Client::if_identify(int code)
{
	print_for_test();
	if (code == 2)
	{
		if (_nick.empty() || _username.empty())
			return false;
		else
			return true;
	}
	else 
	{
		if (_nick.empty())
			return false;
		else
			return true;
	}
}

void	Client::print_for_test()
{
	std::cout << "--- INFO TEST ---\n" << std::endl;
	std::cout << "username = " << _username << std::endl;
	std::cout << "nick = " << _nick << std::endl;

	std::cout << "list channel :" << std::endl;
	for (std::vector<std::string>::iterator it = _listChan.begin(); it != _listChan.end(); it++)
	{
		std::cout << *it;
	}
	std::cout << "----- END TEST -----" << std::endl;
}