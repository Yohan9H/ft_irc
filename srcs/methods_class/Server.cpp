#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/irc_head.hpp"

class Channel;

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

    // Close all client sockets and clear arrays
    for (size_t i = 1; i < _fds.size(); i++) 
    {
        close(_fds[i].fd);
    }
    _fds.clear();
    _listClients.clear();

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

bool Server::createServerSocket()
{
    this->_serverAddress.sin_family = AF_INET;
    this->_serverAddress.sin_port = htons(this->_port);
    this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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
      if (_fds[i].revents & POLLIN)
      {
        if (_fds[i].fd < 0) 
        {
            std::cerr << RED << "Invalid file descriptor: " << COL_END << _fds[i].fd << std::endl;
            return (false);
        }
        if (_fds[i].fd == this->_serverSocket)
        {
          if (!acceptClients())
            std::cerr << RED << "Failed to accept new client." << COL_END << std::endl;
          continue;
        }
        else
        {
          if (!receiveData(this->_fds[i].fd))
          {
            close(_fds[i].fd);
            _fds.erase(_fds.begin() + i);
            --i;
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
        Command command = parseCommand(*this, *(this->getClientbyFd(fd)),  bufferToParse);
        std::cout << "Prefix: "<< (command.prefix.empty() ? "empty" : command.prefix) << std::endl;
        std::cout << "Command: " << command.command << std::endl;
        if (command.params.size() == 0)
          std::cout << "Params: empty" << std::endl;
        for (size_t i = 0; i < command.params.size(); i++)
          std::cout << "Params " << i << ": " << command.params[i] << std::endl;
        std::cout << "Trailing: "<< (command.trailing.empty() ? "empty" : command.trailing) << std::endl;

    //Just some tests 
      std::string ar = (std::string)"You sent";
      ar += YELLOW;
      ar += ": ";
      ar += (std::string)buffer;
      ar += COL_END;
      // Just a test to reply to the client
      const char *reply = ar.c_str();
      send(fd, reply, strlen(reply), 0);
      //End of sending messages test 
      return (true);
    }
    else
    {
        std::cerr << RED << "recv: " << strerror(errno) << COL_END << std::endl;
        return false;
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
			this->delClient(*it->second);
			// arret : del le client dans la liste des channels du serv et faire une verif si le channel est empty le supprimer aussi
			break;
		}
	}
}