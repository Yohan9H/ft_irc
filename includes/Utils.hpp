#pragma once

#include "irc_head.hpp"

bool	is_good(char c);
void	showMapClient(Server &serv);
void	showMapChannel(Server &serv);
void    normalizeCRLF(std::string& input);
