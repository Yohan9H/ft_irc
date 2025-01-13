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

#pragma once

#include "irc_head.hpp"

class Client;
class Channel;
class Command;

struct com;

class Server
{
private:
	int									_serverSocket;
	int									_port;
	std::string							_password;
	static bool							_signal;
	std::map<std::string, Channel*>		_listChannels;
	std::map<int, Client*>				_listClients;

	std::vector<struct pollfd>			_fds;
	struct sockaddr_in					_serverAddress;
	struct sockaddr_in					_cliAddress;
	struct pollfd						_newClient;
	std::string							_timeBeginServ;
	
	std::map<std::string, Command*>		_commands; 

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
	bool 			receiveData(int fd);
	void 			clearClient(int fd);
	void 			closeAndClear();

	std::map<std::string, Channel*>				&getChannel();
	std::map<int, Client*>						&getClients();
	Client										*getClientbyFd(int clientFd);
	Channel										*getChannelbyName(std::string name);
	Client										*getClientbyName(std::string name);
	std::map<std::string, Channel*>::iterator	findChan(std::string name);

	Command* getCommandByName(std::string cmdName);

	//SETTER
	void		setCommands();

	void					addClient(Client &client);
	void					delClient(Client &client);
	void					addChannel(Channel &chan);
	void					delChannel(Channel &channel);
	void					delClientChannelAssociate(Client &client);
	void					setTime();
	std::string				getTime();
	void					delClientWithFd(int fd);	

	void					parseCommand(Client &client, const std::string &input);
};

#include "Channel.hpp"
#include "Client.hpp"