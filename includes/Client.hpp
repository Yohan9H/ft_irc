#pragma once
#include "Server.hpp"
#include "Channel.hpp"
#include "irc_head.hpp"


#define IS_OP 9
#define NOT_OP 8

class Channel;
class Server;

struct com;

class Client
{
private:
	int								_clientSocket;
	bool							_isAuth;
	std::string						_nick;
	std::string						_username;
	std::vector<std::string>		_listChan;
	bool							_passwordfilled;
	bool							_nickfilled;
	bool							_userfilled;
	std::string						_outdata;
	std::string						_indata;
	

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
	bool						getPasswordFilled();
	bool						getNickFilled();
	bool						getUserFilled();
	std::string					&getOutData();
	std::string					&getInData();

	// setters
	void			setClientSocket(int clientFd);
	void			setName(std::string new_name);
	void			setNick(std::string new_nick);
	void			setPasswordFilled(bool isPasswordFilled);
	void			setNickFilled(bool isNickFilled);
	void			setUserFilled(bool isNickFilled);
	void			appendOutData(std::string message);
	void			appendInData(std::string command);

	// methods
	void			sendMsgAllChan(Server &serv, std::string msg);
	bool			if_identify(int code);
	bool			if_NickIsCreate();
	void			AuthIsGood();
	void			removeChan(std::string channel);

	void			print_for_test();

	void 	executeCommand(Server &serv, const com &cmd);
};