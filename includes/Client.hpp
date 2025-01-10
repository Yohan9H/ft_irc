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

class Client
{
private:
    int 							_clientSocket;
    struct sockaddr_in 				_clientAddress;
    std::string 					_clientIPAdd;
    socklen_t 						_clientLen;

    std::string        				 _username;
	std::string						 _nick;
	std::map<std::string, Channel*>	 _listChan;
    bool                             _isAuth;

public:
    Client();
    Client(int clientSocket, sockaddr_in clientAddress);
    Client (const Client& other);
    Client &operator=(const Client &other);
    ~Client();

    int 		getClientSocket() const;
    void 		setClientSocket(int clientSocket);
    void 		setClientLen(int clientLen);
    void 		setClientAddress(struct sockaddr_in clientAddress);
    void 		setIPAdd(std::string ipAdd);
    std::string getIPAdd() const;

    struct sockaddr * 	getClientAddr();
    socklen_t 			getClientLen() const;
	
    std::string			getName() const;
    std::string			getNick() const;
	void				setName(std::string new_name);
	void				setNick(std::string new_nick);

	void				sendMsgAllChan(std::string msg);
	void				sendMsgAllChanNickInform(std::string msg);

    void				addChan(Channel &chan);
	void				delChan(Channel &chan);

	bool				if_identify(int code);

	void				print_for_test();
    
};

#endif



