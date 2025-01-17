#include "../includes/Client.hpp"

Client::Client() : _clientSocket(-1), _isAuth(false)
{
}

Client::Client(int clientSocket)  : _clientSocket(clientSocket), _isAuth(false)
{

}

Client::Client(const Client &other)  : _clientSocket(other._clientSocket), _isAuth(other._isAuth)
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

bool	Client::getIsAuth()
{
	return _isAuth;
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

bool	Client::getPasswordFilled() 
{
	return this->_passwordfilled;
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

void		Client::setPasswordFilled(bool isPasswordFilled)
{
	_passwordfilled = isPasswordFilled;
}

void	Client::sendMsgAllChan(Server &serv, std::string msg)
{
	for (std::vector<std::string>::iterator it = _listChan.end(); it != _listChan.end(); it++)
	{
		Channel *channel = serv.getChannelbyName(*it);
		channel->sendMsgMembres(msg);
	}
}

bool	Client::if_NickIsCreate()
{
	if (_nick.empty())
		return false;
	else
		return true;
}

bool	Client::if_identify(int code)
{
	print_for_test();
	if (code == 2)
	{
		if (_isAuth == false)
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

void	Client::AuthIsGood()
{
	_isAuth = true;
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

void Client::parseline(const std::string &line)
{
   (void)line;
    //split when you find /r/n
}

void Client::executeCommand(Server &serv, const com &cmd)
{
    const std::string validCommands[] = {
        "NICK", "USER", "JOIN", "PRIVMSG", "PART", "QUIT", "TOPIC", "KICK", "MODE", "INVITE", "CAP", "PASS", "PING", "NOTICE"
    };

    if (cmd.command.empty())
        throw std::logic_error("Empty command");
    bool found = false;
    for (size_t i = 0; i < (sizeof(validCommands) / sizeof(validCommands[0])); i++) {
        if (cmd.command == validCommands[i]) {
            found = true;
            break;
        }
    }
    if (!found)
        throw std::logic_error("Invalid command");
    if (cmd.trailing.length() > 512)
        throw std::logic_error("traling too long");

    Command *myCommand = serv.getCommandByName(cmd.command);
	if (!myCommand)
		throw std::logic_error("Command not found");

    if (cmd.params.size() < myCommand->getNbParam())
        throw std::logic_error("Wrong params");
    
    if (!this->getIsAuth() && myCommand->getMustbeAuth())
        throw std::logic_error("User not authentified");

    myCommand->execCommand(serv, *this, cmd);

}

void Client::handlePrivmsg(const com &cmd) {
    if (cmd.params.size() < 2) {
        std::string errorCode = "461";
        std::string errorMessage = "PRIVMSG :Not enough parameters";
        throw std::logic_error(errorCode + " " + errorMessage);
    }

    const std::string &target = cmd.params[0];
    const std::string &message = cmd.trailing;

    if (target.empty() || message.empty()) {
        std::string errorCode = "412";
        std::string errorMessage = "No text to send";
        throw std::logic_error(errorCode + " " + errorMessage);
    }

    // Logique métier (envoyer le message au destinataire)
}


void	Client::removeChan(std::string channel)
{
	std::vector<std::string>::iterator it = std::find(_listChan.begin(), _listChan.end(), channel);
	if (it != _listChan.end())
		_listChan.erase(it);
}