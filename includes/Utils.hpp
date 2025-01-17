#pragma once

#include "irc_head.hpp"

bool		is_good(char c);
void		showMapClient(Server &serv);
void		showMapChannel(Server &serv);
void		normalizeCRLF(std::string& input);
std::string	to_string(int value);
void		sendNumeric(Client &client, int numeric, const std::string& message);
void		sendNotice(Client &client, const std::string& message);
void		sendModeParamMsg (Client &client, Channel &channel, std::string mode, std::string param);
