#pragma once
#include <iostream>

class Server;
class Channel;
class Client;

#include "irc_head.hpp"

Channel		*createChannel(Server &server, std::string name_channel, Client &first_membre);

void		removeNewline(std::string& nick);

std::string	mdp_false(std::string name_serv, std::string name);

std::string invite_false(std::string serv, std::string request, std::string channel);

std::string limit_user_false(std::string name_serv, std::string name_client);

std::string msg_err(std::string name_serv, std::string code, std::string name_input, std::string motif);
