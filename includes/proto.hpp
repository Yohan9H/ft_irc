/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:55:06 by yohurteb          #+#    #+#             */
/*   Updated: 2024/12/21 15:15:50 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

class Server;
class Channel;
class Client;

typedef struct s_perm
{
	bool	i; // INVITE
	bool	t; // TOPIC
	bool	k; // MDP
	bool	o; // OPERATOR
	bool	l; // LIMIT USER
} t_perm;

Channel 	&createChannel(Server &server, std::string name_channel, Client &first_membre);

void 		removeNewline(std::string& nick);

std::string	mdp_false(std::string name_serv, std::string name);

std::string invite_false(std::string serv, std::string request, std::string channel);

std::string limit_user_false(std::string name_serv, std::string name_client);

std::string msg_err(std::string name_serv, std::string code, std::string name_input, std::string motif);
