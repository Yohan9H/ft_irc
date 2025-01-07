/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:17:28 by yohurteb          #+#    #+#             */
/*   Updated: 2024/12/20 13:24:52 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string	mdp_false(std::string name_serv, std::string name)
{
	std::string msg = ":" + name_serv + " 464 " + name + " :Password incorrect";
	return (msg);
}

std::string invite_false(std::string serv, std::string request, std::string channel)
{
	std::string msg = ":" + serv + " " + request + " " + channel + " :Cannot join channel (+i)";
	return (msg);
}

std::string limit_user_false(std::string name_serv, std::string name_client)
{
	std::string msg = ":" + name_serv + " 471 " + name_client + " :Cannot join channel (+l)";
	return msg;
}

// NICK
std::string msg_err(std::string name_serv, std::string code, std::string name_input, std::string motif)
{
	std::string msg = ":" + name_serv + " " + code + " " + name_input + " " + motif + "\n";
	return msg;
}