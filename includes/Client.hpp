/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apernot <apernot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:13:34 by avandeve          #+#    #+#             */
/*   Updated: 2025/01/10 11:05:14 by apernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
# define CLIENT_HPP
#include "Server.hpp"
#include "Channel.hpp"
#include "irc_head.hpp"


#define IS_OP 9
#define NOT_OP 8

class Channel;
class Server;

class Client
{
private:
    int								_clientSocket;
	bool							_isAuth;
	std::string						_nick;
    std::string						_username;
	std::vector<std::string>		_listChan;

public:
    Client();
    Client(int clientSocket);
    Client (const Client& other);
    Client &operator=(const Client &other);
    ~Client();

	// getters
    int 						&getClientSocket();
	bool						getIsAuth();
    std::string					&getNickname();
    std::string					&getUsername();
	std::vector<std::string>	&getListChanJoined();

	// setters
	void			setClientSocket(int clientFd);
	void			setName(std::string new_name);
	void			setNick(std::string new_nick);

	// methods
	void			sendMsgAllChan(Server &serv, std::string msg);
	bool			if_identify(int code);
	void			AuthIsGood();

	void			print_for_test();
};

#endif



