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
bool	Client::getNickFilled()
{
	return this->_nickfilled;
}

bool	Client::getUserFilled()
{
	return this->_userfilled;
}

void	Client::setClientSocket(int clientFd)
{
	_clientSocket = clientFd;
}

void	Client::setName(std::string new_name)
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

void	Client::setNickFilled(bool isNickFilled)
{
	_nickfilled = isNickFilled;
}

void	Client::setUserFilled(bool isUserFilled)
{
	_userfilled = isUserFilled;
}

void	Client::sendMsgAllChan(Server &serv, std::string msg)
{
	for (std::vector<std::string>::iterator it = _listChan.begin(); it != _listChan.end(); it++)
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
	std::cout << "--- INFO CLIENT TEST ---\n" << std::endl;
	std::cout << "username = " << _username << std::endl;
	std::cout << "nick = " << _nick << std::endl;

	std::cout << "list channel : ";
	for (std::vector<std::string>::iterator it = _listChan.begin(); it != _listChan.end(); it++)
	{
		std::cout << *it << " - ";
	}
	std::cout << std::endl;
	std::cout << "----- END TEST -----" << std::endl;
}

void Client::parseline(const std::string &line)
{
   (void)line;
    //split when you find /r/n
}

void Client::executeCommand(Server &serv, const com &cmd)
{
	std::string msg;
	int numeric;
    const std::string validCommands[] = {
        "NICK", "USER", "JOIN", "PRIVMSG", "PART", "QUIT", "TOPIC", "KICK", "MODE", "INVITE", "CAP", "PASS", "PING", "NOTICE"
    };

    if (cmd.command.empty())
    {
		msg = "Not enough parameters";
		numeric = ERR_NEEDMOREPARAMS;
	}
	else
	{
		bool found = false;
		for (int i = 0; i < (sizeof(validCommands) / sizeof(validCommands[0])); i++) {
			if (cmd.command == validCommands[i]) {
				found = true;
				break;
			}
		}
		if (!found)
		{
			msg = "Unknown command";
			numeric = ERR_UNKNOWNCOMMAND;
		}
		else {
			Command *myCommand = serv.getCommandByName(cmd.command);
			if (!myCommand)
			{
				msg = "Unknown command";
				numeric = ERR_UNKNOWNCOMMAND;
			}
			else if (cmd.params.size() < myCommand->getNbParam())
			{
				msg = "Not enough parameters";
				numeric = ERR_NEEDMOREPARAMS;
			}
			else if (!this->getIsAuth() && myCommand->getMustbeAuth())
			{
				msg = "Need to be logged in";
				numeric = ERR_UNKNOWNCOMMAND;
			}
			else 
				myCommand->execCommand(serv, *this, cmd);
		}
	}
	if (!msg.empty())
		sendNumericCmd(*this, numeric, cmd.command, msg + ENDLINE_MSG);
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