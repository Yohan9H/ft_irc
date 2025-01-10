/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apernot <apernot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:55:06 by yohurteb          #+#    #+#             */
/*   Updated: 2025/01/09 17:12:31 by apernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

class Server;
class Channel;
class Client;

#include "irc_head.hpp"

typedef struct s_perm
{
	bool	i; // INVITE
	bool	t; // TOPIC
	bool	k; // MDP
	bool	o; // OPERATOR
	bool	l; // LIMIT USER
} t_perm;

Channel 	*createChannel(Server &server, std::string name_channel, Client &first_membre);

void 		removeNewline(std::string& nick);

std::string	mdp_false(std::string name_serv, std::string name);

std::string invite_false(std::string serv, std::string request, std::string channel);

std::string limit_user_false(std::string name_serv, std::string name_client);

std::string msg_err(std::string name_serv, std::string code, std::string name_input, std::string motif);

void    parseCommand(Server &serv, Client &client, const std::string &input);
