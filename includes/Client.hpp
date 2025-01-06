/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:13:34 by avandeve          #+#    #+#             */
/*   Updated: 2024/12/21 14:34:08 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
# define CLIENT_HPP
#include "Server.hpp"
#include "Channel.hpp"

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
	std::map<std::string, Channel>	 _listChan;

public:
    Client();
    Client(int clientSocket, sockaddr_in clientAddress);
    Client (const Client& other);
    Client &operator=(const Client &other);
    ~Client();
    int getClientSocket() const;
    void setClientSocket(int clientSocket);
    void setClientLen(int clientLen);
    void setClientAddress(struct sockaddr_in clientAddress);
    void setIPAdd(std::string ipAdd);
    std::string getIPAdd() const;

    struct sockaddr * getClientAddr();
    socklen_t getClientLen() const;
	
    std::string			getName() const; //Ajout Yohan
    std::string			getNick() const; //Ajout Yohan
	void				setName(std::string new_name);
	void				setNick(std::string new_nick);

	void				sendMsgAllChan(std::string msg); // add yoyo
	void				sendMsgAllChanNickInform(std::string msg); //add yoyo

    void				addChan(Channel chan); //Ajout Yohan
	void				delChan(Channel chan); //Ajout Yohan

	bool				if_identify(int code);

	void				print_for_test();
};

#endif



