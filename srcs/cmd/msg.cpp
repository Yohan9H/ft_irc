#include "Server.hpp"

std::string	mdp_false(std::string name_serv, std::string name)
{
	std::string msg = ":" + name_serv + " 464 " + name + " :Password incorrect" + "\r\n";
	return (msg);
}

std::string invite_false(std::string serv, std::string request, std::string channel)
{
	std::string msg = ":" + serv + " " + request + " " + channel + " :Cannot join channel (+i)" + "\r\n";
	return (msg);
}

std::string limit_user_false(std::string name_serv, std::string name_client)
{
	std::string msg = ":" + name_serv + " 471 " + name_client + " :Cannot join channel (+l)" + "\r\n";
	return msg;
}

// NICK
std::string msg_err(std::string name_serv, std::string code, std::string name_input, std::string motif)
{
	std::string msg = ":" + name_serv + " " + code + " " + name_input + " " + motif + "\n";
	return msg;
}