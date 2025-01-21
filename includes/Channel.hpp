/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apernot <apernot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:31:50 by yohurteb          #+#    #+#             */
/*   Updated: 2025/01/21 13:16:00 by apernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include "proto.hpp"
#include "irc_head.hpp"

class Client;

class Server;

typedef struct	s_perm t_perm;

class Channel
{
	private:
		std::string			_name;
		std::string			_key;
		std::string			_topic;
		size_t				_limit_user;
		std::vector<char>	_modes;
		std::vector<int>	_operatorsFd;
		std::vector<int>	_membresFd;
		std::vector<int>	_invitedFd;
 
	public:
		Channel();
		Channel(const Channel &src);
		~Channel();

		Channel &operator=(const Channel &src);

		std::string			getKey() const;
		std::string			getName() const;
		std::string 		getTopic() const;
		std::vector<char>	&getModes();
		std::vector<int>	&getOperatorsFd();
		std::vector<int>	&getMembresFd();
		std::vector<int>	&getInvitedFd();

		void				setKey(std::string key);
		void				setName(std::string name);
		void				setTopic(std::string topic);
		void				setLimit(size_t limit);
		
		void		addOperators(int clientFd);
		void		addMembres(int clientFd);
		void		addInvited(int clientFd);
		void		delOperatores(int clientFd);
		void		delMembres(int clientFd);
		void		delInvited(int clientFd);
		std::string	formatJoinMessage(std::string name_new_client, Channel channel);
		void		sendJoinMsgAll(Channel &channel, std::string name_serv, std::string username_client, int clientFd);
		void		infoJoinChannel(Server &serv, std::string name_serv, Channel &channe, Client &client);
		std::string giveAllNameMembres(Server &serv);
		void		sendMsgMembres(std::string msg);
		void		sendMsgMembresExceptFd(std::string msg, int clientFd);
		void		addMode(char mode);
		void		errMode(char mode);

		bool	ifInvite();
		bool	ifTopic();
		bool	ifProtectedByPassWord();
		bool	ifLimitUser();
		bool	isOperator(int clientFd);

		bool	checkPassWord(std::string mdp);
		bool	checkClientIsInvited(int clientFd);
		bool	checkClientIsMembre(int clientFd);
		bool	checkLimitUser();
		
		bool	hasMode(char mode);
		int		getTotalMembers();
		
};

