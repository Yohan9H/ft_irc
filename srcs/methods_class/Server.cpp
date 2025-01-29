#include "Server.hpp"

class Channel;
class Client;

bool Server::_signal = false;

Server::Server() : _serverSocket(-1), _port(8080)
{
  
}

Server::Server(int port, std::string password)
{
	this->_port = port;
	this->_password = password;
}

Server::Server(const Server &other)
{
  this->_serverSocket = other._serverSocket;
  this->_port = other._port;
  this->_password = other._password;
  this->_signal = other._signal;
  this->_listClients = other._listClients;
  this->_serverAddress = other._serverAddress; 
}

Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
	  this->_serverSocket = other._serverSocket;
	  this->_port = other._port;
	  this->_password = other._password;
	  this->_signal = other._signal;
	  this->_listClients = other._listClients;
	  this->_serverAddress = other._serverAddress; 
	}
	return (*this);
}

void Server::closeAndClear()
{
	std::cout << "Cleaning up all resources..." << std::endl;

	// suppression des commandes
	for (std::map<std::string, Command*>::iterator it = _commands.begin(); it != _commands.end(); it++) {
		delete it->second;
	}

	// Close all client sockets and clear arrays
	for (size_t i = 1; i < _fds.size(); i++) 
	{
		close(_fds[i].fd);
	}
	_fds.clear();

	for (std::map<int, Client*>::iterator it = _listClients.begin(); it != _listClients.end(); it++)
	{
		delete it->second;
	}
	_listClients.clear();

	for (std::map<std::string, Channel *>::iterator it = _listChannels.begin(); it != _listChannels.end(); it++)
	{
		delete it->second;
	}
	_listChannels.clear();

	// Close the server socket
	if (_serverSocket >= 0)
	{
		close(_serverSocket);
	}
	std::cout << GREEN << "All resources cleared." << COL_END <<  std::endl;
}

Server::~Server()
{
	//Clear all the opened clients in case of premature exit (signals ?)
	closeAndClear();
	close(this->_serverSocket);
}

void Server::signalHandler(int signum)
{
  (void)signum;
  std::cout << "Signal intercepted" << std::endl;
  Server::_signal = true;
}

bool Server::getSignal()
{
  return (Server::_signal);
}

bool Server::initServer()
{
  if (!createServerSocket())
	return (false);
  if (!manageEvents())
	return (false);
  return (true);
}

void	Server::setCommands() {
	_commands.insert(std::pair<std::string, Command*>("JOIN", new JOIN));
	_commands.insert(std::pair<std::string, Command*>("KICK", new KICK));
	_commands.insert(std::pair<std::string, Command*>("MODE", new MODE));
	_commands.insert(std::pair<std::string, Command*>("NICK", new NICK));
	_commands.insert(std::pair<std::string, Command*>("INVITE", new INVITE));
	_commands.insert(std::pair<std::string, Command*>("PART", new PART));
	_commands.insert(std::pair<std::string, Command*>("PASS", new PASS));
	_commands.insert(std::pair<std::string, Command*>("PRIVMSG", new PRIVMSG));
	_commands.insert(std::pair<std::string, Command*>("QUIT", new QUIT));
	_commands.insert(std::pair<std::string, Command*>("TOPIC", new TOPIC));
	_commands.insert(std::pair<std::string, Command*>("USER", new USER));
}

Command* Server::getCommandByName(std::string cmdName) {
  for (std::map<std::string, Command*>::iterator it = _commands.begin(); it != _commands.end(); it++) {
	if (it->first == cmdName) {
		return (it->second);
		break ;
	}
  }
  return NULL;
}

std::string	Server::getPassword() {
	return (this->_password);
}



bool Server::createServerSocket()
{
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(this->_port);
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	//verifier le bon emplacement de setcommands
	this->setCommands();
	//Checking if the socket has been successfully created
	if (this->_serverSocket == -1)
	{
	  std::cerr << RED << "Error creating the socket" << COL_END << std::endl;
	  return (false);
	}

	//Setting the options REUSE ADDRESS & REUSE PORT on the created socket
	int en = 1;
	  if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1 || setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEPORT, &en, sizeof(en)) == -1)
	{
	  std::cerr << RED "setsockopt: " << strerror(errno) << COL_END << std::endl;
	  close (this->_serverSocket);
	  return (false);
	}
	if (fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
	  close (this->_serverSocket);
	  return (false);
	}

	//3 - Binding the server socket to a port and an address, as defined in sockaddr_in + checking the return value of bind
	if (bind(this->_serverSocket, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) == -1)
	{
		std::cerr << RED << "bind: " << strerror(errno) << COL_END << std::endl;
		close(this->_serverSocket);
		return (false);
	}
	// std::cout << "Socket _serverSocket is bound to the port " << ntohs(_serverAddress.sin_port) << std::endl;
	if (listen(this->_serverSocket, MAX_CONNEXIONS) == -1)
	{
		std::cerr << RED  "listen: " << strerror(errno) << COL_END << std::endl;
		close(_serverSocket);
		return (false);
	}
	_newClient.fd = this->_serverSocket;
	_newClient.events = POLLIN;
	_newClient.revents = 0;
	_fds.push_back(_newClient);
	std::cout << PURPLE << "Server is ready and now listening to connexions..." << COL_END << std::endl;
  // close (this->_serverSocket);
  this->manageEvents();
  return (true);
}

bool Server::manageEvents()
{
  while (Server::_signal == false)
  {
	if (poll(&this->_fds[0], _fds.size(), -1) == -1)
	{
		std::cerr << "poll: " << strerror(errno) << std::endl;
		return (false);
	}
	for (size_t i = 0; i < _fds.size(); i++)
	{
	  if (_fds[i].revents & POLLIN | POLLOUT)
	  {
		if (_fds[i].fd < 0) 
		{
			std::cerr << RED << "Invalid file descriptor: " << COL_END << _fds[i].fd << std::endl;
			return (false);
		}
		if (_fds[i].fd == this->_serverSocket) // nouveau client
		{
		  if (!acceptClients())
			std::cerr << RED << "Failed to accept new client." << COL_END << std::endl;
		  continue;
		}
		else if (!receiveData(this->_fds[i].fd))
		{
			close(_fds[i].fd);
			_fds.erase(_fds.begin() + i);
			--i;
		}
		else {
			Client* client = getClientbyFd(_fds[i].fd);
			if (client) 
			{
				std::string outData = client->getOutData();
				send()
			}
		}
	  }
	}
  }
  close(this->_serverSocket);
  return (true);
}

bool Server::acceptClients() 
{
	Client *newClient = new Client();

	memset(&_cliAddress, 0, sizeof(_cliAddress));
	socklen_t clientLength = sizeof(_cliAddress);
	int clientSocket = accept(this->_serverSocket, (struct sockaddr*)&_cliAddress, &clientLength);

	if (clientSocket < 0) {
		std::cerr << RED << "accept: " << strerror(errno) << COL_END <<std::endl;
		return false;
	}
	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1) 
	{
		std::cerr << "fcntl: " << strerror(errno) << std::endl;
		close(clientSocket);
		return false;
	}
	if (_fds.size() == MAX_CONNEXIONS + 1)
	{
	  std::cout << RED << "Max number of connexions reached" << std::endl;
	  close(clientSocket);
	  return (false);
	}
	_newClient.fd = clientSocket;
	_newClient.events = POLLIN;
	_newClient.revents = 0;
	newClient->setClientSocket(clientSocket);
	newClient->setNickFilled(false);
	newClient->setUserFilled(false);
	newClient->setPasswordFilled(false);

	// newClient->setClientAddress(_cliAddress);
	// newClient->setIPAdd(inet_ntoa(_cliAddress.sin_addr));
	_listClients.insert(std::make_pair(newClient->getClientSocket(), newClient));
	_fds.push_back(_newClient); 

	std::cout << GREEN << "New client connected! Socket FD: " << clientSocket << COL_END << std::endl;
	return true;
}

bool Server::receiveData(int fd)
{
	char buffer[BUFFER_MAX];
	memset(buffer, 0, BUFFER_MAX);
	ssize_t bytesRead = recv(fd, buffer, BUFFER_MAX - 1, 0);


	if (bytesRead == 0)
	{
		std::cout << RED << "Client " << fd - 3 << " disconnected." << COL_END << std::endl;
		this->delClientWithFd(fd);
		return (false);
	}
	else if (errno == EAGAIN || errno == EWOULDBLOCK)
		return (true); //Checker si OK eval
	else if (bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
		std::cout << BLUE << "Message from client " << (fd - 3) << ": " << COL_END << buffer;
		if (strcmp(buffer, "exit\n") == 0)
		{
		  std::cout << RED << "The client wants to cut the connexion" << COL_END << std::endl;
		  return (false);
		}
		// //Merge partie Arthur 
		std::string bufferToParse;
		bufferToParse = std::string(buffer);
		parseCommand(*(this->getClientbyFd(fd)), bufferToParse);

	// //Just some tests 
	//   std::string ar = (std::string)"You sent";
	//   ar += YELLOW;
	//   ar += ": ";
	//   ar += (std::string)buffer;
	//   ar += COL_END;
	//   // Just a test to reply to the client
	//   const char *reply = ar.c_str();
	//   send(fd, reply, strlen(reply), MSG_NOSIGNAL);
	//   //End of sending messages test 
	  return (true);
	}
	else
	{
		std::cerr << RED << "recv: " << strerror(errno) << COL_END << std::endl;
		return false;
	}
}

void Server::parseCommand(Client &client, const std::string &input) {

	size_t pos = 0;
	std::string data = input;
	normalizeCRLF(data);
	while ((pos = data.find("\r\n")) != std::string::npos) {
		std::string line = input.substr(0, pos);
		std::string token;
		std::istringstream tokenStream(line);
		com command;

		command.hasText = false;
		// Check if the command has a prefix
		if (!line.empty() && data[0] == ':') {
			std::getline(tokenStream, token, ' ');
			command.prefix = token.substr(1);
		}
		// Get the command
		if (std::getline(tokenStream, token, ' '))
			command.command = token;

		// Get the parameters and trailing
		while (std::getline(tokenStream, token, ' ')) {
			if (!token.empty() && token[0] == ':') {
				command.trailing = token.substr(1);
				command.hasText = true;
				std::string rest;
				std::getline(tokenStream, rest);
				if (!rest.empty())
					command.trailing += " " + rest; 
				break;
			} else {
				command.params.push_back(token);
			}
		}

		// FOR TEST
	
		client.executeCommand(*this, command);
		std::cout << std::endl << "%%%%%%%%%%%%%%%%%%%%%% BEGIN TEST %%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
		showMapClient(*this);
		showMapChannel(*this);
		std::cout << "%%%%%%%%%%%%%%%%%%%%%% END TEST %%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl  << std::endl ;

	// return (command);
		data.erase(0, pos + 2);
	}
}

void Server::clearClient(int fd)
{
	for (size_t i = 0; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			close(_fds[i].fd);
			_fds.erase(_fds.begin() + i);
			break;
		}
	}

	std::map<int, Client*>::iterator it = _listClients.find(fd);
	if (it != _listClients.end())
		_listClients.erase(it);
}

int Server::getServSocket() const
{
  return (this->_serverSocket);
}

std::map<int, Client*>		&Server::getClients()
{
	return _listClients;
}

std::map<std::string, Channel*>	&Server::getChannel()
{
	return _listChannels;
}

Client	*Server::getClientbyFd(int clientFd)
{
	if (clientFd < 0)
		return NULL;
	std::map<int, Client*>::iterator it = _listClients.find(clientFd);
	if (it != _listClients.end())
		return it->second;
	else
		return NULL;
}

Channel	*Server::getChannelbyName(std::string name)
{
	std::map<std::string, Channel*>::iterator it = _listChannels.find(name);
	if (it != _listChannels.end())
		return it->second;
	else
		return NULL;
}

Client  *Server::getClientbyName(std::string name) {
	
	for (std::map<int, Client*>::iterator it = _listClients.begin(); it != _listClients.end(); it++ ) {
		if (it->second->getNickname() == name)
			return (it->second);
	}
	return (NULL);
}

void	Server::addClient(Client &client)
{
	_listClients.insert(std::make_pair(client.getClientSocket(), &client));
}

void	Server::delClient(Client &client)
{
	_listClients.erase(client.getClientSocket());
}

void	Server::addChannel(Channel &channel)
{
	_listChannels.insert(std::make_pair(channel.getName(), &channel));
}

void	Server::delChannel(Channel &channel)
{
	_listChannels.erase(channel.getName());
}

std::map<std::string, Channel*>::iterator	Server::findChan(std::string name)
{
	std::map<std::string, Channel*>::iterator it = _listChannels.find(name);
	return it;
}

void	Server::setTime()
{
	std::time_t current_time = std::time(NULL);
	std::tm* local_time = std::localtime(&current_time);
	local_time->tm_mon += 1;
	local_time->tm_year += 1900;

	std::ostringstream oss;
	oss << local_time->tm_mday << '.' << local_time->tm_mon << '.' << local_time->tm_year;

	_timeBeginServ = oss.str();
}

std::string		Server::getTime()
{
	return _timeBeginServ;
}

void	Server::delClientWithFd(int fd)
{
	for (std::map<int, Client*>::iterator it = _listClients.begin(); it != _listClients.end(); it++)
	{
		if (it->first == fd)
		{
			this->delClientChannelAssociate(*it->second);
			delete it->second;
			_listClients.erase(it->first);
			break;
		}
	}
}

void	Server::delClientChannelAssociate(Client &client)
{
	std::vector<std::string> listChanJoined = client.getListChanJoined();

	for (std::vector<std::string>::iterator it = listChanJoined.begin(); it != listChanJoined.end(); it++)
	{
		std::map<std::string, Channel*>::iterator it_lst = _listChannels.find(*it);
		if (it_lst != _listChannels.end())
		{
			it_lst->second->delOperatores(client.getClientSocket());
			it_lst->second->delMembres(client.getClientSocket());
			it_lst->second->delInvited(client.getClientSocket());
			client.removeChan(it_lst->second->getName());

			// Supprimer le channel si vide
			if (it_lst->second->getMembresFd().size() == 0)
			{
				delete it_lst->second;
				_listChannels.erase(it_lst);
			}
			else
			{ //verifier si c'est le dernier operator
				if (it_lst->second->getOperatorsFd().empty())
				{
					it_lst->second->addOperators(it_lst->second->getMembresFd()[0]);
				}
				std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost QUIT :";
				it_lst->second->sendMsgMembres(msg + ENDLINE_MSG);
			}
		}
	}
}