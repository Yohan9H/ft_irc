#include "../includes/Client.hpp"

Client::Client()
{
}

Client::Client(int clientSocket, sockaddr_in clientAddress)  : _clientSocket(clientSocket), _clientAddress(clientAddress)
{

}

Client::Client(const Client &other)  : _clientSocket(other._clientSocket), _clientAddress(other._clientAddress)
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

int Client::getClientSocket() const
{
    return (this->_clientSocket);
}

void Client::setClientSocket(int clientSocket)
{
    this->_clientSocket = clientSocket;
}

void Client::setClientAddress(struct sockaddr_in clientAddress)
{
    this->_clientAddress = clientAddress;
}

void Client::setClientLen(int clientLen)
{
    this->_clientLen = clientLen;
}

void Client::setIPAdd(std::string ipAdd)
{
    this->_clientIPAdd = ipAdd;
}

// sockaddr_in *Client::getClientAddr() const
// {
//     return (this->_clientAddress);
// }

struct sockaddr* Client::getClientAddr()
{
    return reinterpret_cast<struct sockaddr*>(&this->_clientAddress);
}

std::string Client::getIPAdd() const
{
    return (this->_clientIPAdd);
}

socklen_t Client::getClientLen() const
{
    return (sizeof(this->_clientAddress));
}

std::string	Client::getName() const
{
	return _username;
}

std::string	Client::getNick() const
{
	return _nick;
}

void		Client::setName(std::string new_name)
{
	_username = new_name;
}

void		Client::setNick(std::string new_nick)
{
	_nick = new_nick;
}

void	Client::addChan(Channel chan)
{
	_listChan.insert(std::make_pair(chan.getName(), chan));
}

void	Client::delChan(Channel chan)
{
	_listChan.erase(chan.getName());
}

void	Client::sendMsgAllChanNickInform(std::string msg) 
{
	for (std::map<std::string, Channel>::iterator it = _listChan.end(); it != _listChan.end(); it++)
	{
		std::string msg_temp = msg;
		msg_temp += it->second.getName();
		it->second.sendMsgMembre(msg);
	}
}

void	Client::sendMsgAllChan(std::string msg)
{
	for (std::map<std::string, Channel>::iterator it = _listChan.end(); it != _listChan.end(); it++)
	{
		it->second.sendMsgMembre(msg);
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
	for (std::map<std::string, Channel>::iterator it = _listChan.begin(); it != _listChan.end(); it++)
	{
		std::cout << it->first;
	}
	std::cout << "----- END TEST -----" << std::endl;
}