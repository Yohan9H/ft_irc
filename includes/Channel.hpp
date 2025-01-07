/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:31:50 by yohurteb          #+#    #+#             */
/*   Updated: 2024/12/21 15:14:38 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include "proto.hpp"

class Client;

class Server;

typedef struct	s_perm t_perm;

class Channel
{
	private:
		std::string							_name;
		std::string							_key;
		std::string							_subject;
		size_t								_limit_user;
		t_perm								_perm;
		std::map<int, Client*>		_operators;
		std::map<int, Client*>		_membres;
		std::map<int, Client*>		_invited;
 
	public:
		Channel();
		Channel(const Channel &src);
		~Channel();

		Channel &operator=(const Channel &src);

		std::string							getKey() const;
		std::string							getName() const;
		std::string 						getSubject() const;
		t_perm								getPerm() const;
		std::map<int, Client*>		&getOperators();
		std::map<int, Client*>		&getMembres();

		void	setKey(std::string key);
		void	setName(std::string name);
		void	setSubject(std::string subject);
		void	setPerm(bool i, bool t, bool k, bool o, bool l); // idee: faire des fcts seules pour plus simple par monmment du style addModI()

		void		addOperators(Client &client);
		void		delOperatores(int socket_key);
		void		addMembres(Client &client);
		void		delMembres(int socket_key);
		std::string	formatJoinMessage(std::string name_new_client, Channel channel);
		void		sendJoinMsgAll(Channel &channel, std::string name_new_client, int socket_key);
		void		infoJoinChannel(std::string name_serv, Channel &channe, Client &client);
		std::string giveAllNameMembres();
		void		sendMsgMembre(std::string msg);

		bool	ifInvite();
		bool	ifTopic();
		bool	ifProtectedByPassWord();
		bool	ifLimitUser();

		bool	checkPassWord(std::string mdp);
		bool	checkClientIsInvited(Client client);
		bool	checkLimitUser();
};

