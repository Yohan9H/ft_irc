#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include "proto.hpp"
#include "irc_head.hpp"

class Client;

class Server;

class Channel
{
	private:
		std::string			_name;
		std::string			_pass;
		std::string			_topic;
		int					_limit_user;
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

		void				setPass(std::string pass);
		void				setName(std::string name);
		void				setTopic(std::string topic);
		void				setLimit(int limit);
		
		void		addOperators(int clientFd);
		void		addMembres(int clientFd);
		void		addInvited(int clientFd);
		void		delOperatores(int clientFd);
		void		delMembres(int clientFd);
		void		delInvited(int clientFd);
		std::string	formatJoinMessage(std::string name_new_client, Channel channel);
		//void		sendJoinMsgAll(Channel &channel, std::string name_serv, std::string username_client, int clientFd);
		void		infoJoinChannel(Server &serv, std::string name_serv, Channel &channe, Client &client);
		std::string giveAllNameMembres(Server &serv);
		void		sendMsgMembres(std::string msg, Server& serv);
		void		sendMsgMembresExceptFd(std::string msg, int clientFd, Server& serv);
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
		bool	checkClientIsOperator(int clientFd);
		bool	checkOverLimitUser();
		
		bool	hasMode(char mode);
		int		getTotalMembers();

		//test
		void	printChannelForTest(Server &serv);
		
		
};

