/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:36:21 by marvin            #+#    #+#             */
/*   Updated: 2024/12/10 16:36:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> //socket functions 
#include <netinet/in.h> //data type sockaddr
#include <iostream>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h> // to close fds
#include <arpa/inet.h> //function inet_ntoa
#include <vector>
#include <csignal>
#include <fcntl.h>
#include <poll.h>
#include <map>
#include <algorithm>
#include <iterator>
#include <exception>
#include <cctype>
#include <ctime>
#include "Channel.hpp"
#include "proto.hpp"
#include "Client.hpp"
#include "irc_head.hpp"
#include "command.hpp"

#define MAX_CONNEXIONS 5
#define BUFFER_MAX 1024
#define NAME_SERV "irc.42.com"

class Client;

struct Command;

class Server
{
private:
	int									_serverSocket;
	int									_port;
	std::string							_password;
	static bool							_signal;
	std::vector<Client>					_clientsVec;
	std::map<std::string, Channel>		_channelArr;
	std::map<std::string, Client>		_clientsArr;

	std::vector<struct pollfd>			_fds;
	struct sockaddr_in					_serverAddress;
	struct sockaddr_in					_cliAddress;
	struct pollfd						_newClient;
	std::string							_timeBeginServ;

	Server(const Server& other);

public:
	Server();
	Server(int port, std::string password);
	Server &operator=(const Server& other);
	~Server();

	static void		signalHandler(int signum);
	static bool		getSignal();
	bool			initServer();
	bool			createServerSocket();
	bool 			manageEvents();
	int 			getServSocket() const;
	// int 			getPort() const;
	bool 			acceptClients();
	Client* 		getClientFromFd(int fd);
	bool 			receiveData(int fd);
	void 			clearClient(int fd);
	void 			closeAndClear();

	const std::map<std::string, Channel>				&getChannel() const;
	const std::map<std::string, Client>					&getClients() const;
	std::map<std::string, Channel>::iterator			findChan(std::string name);

	Channel					&getOneChan(std::string name, Channel &new_chan);
	void					addClient(Client client);
	void					delClient(Client client);
	void					addChannel(Channel &chan);
	void					delChannel(Channel channel);
	void					setTime();
	std::string				getTime();
};

#endif