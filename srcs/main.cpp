/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:39:27 by marvin            #+#    #+#             */
/*   Updated: 2024/12/10 16:39:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/command.hpp"

int   main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << RED << "Usage = ./irc <port_number> <password>" << COL_END << std::endl;
        return (1);
    }
    //Perform mini parsing to validate the args
    Server serv(std::atoi(argv[1]), argv[2]);
    //Handling the signals
    signal(SIGQUIT, &Server::signalHandler);
    signal(SIGINT, &Server::signalHandler);
    if (!serv.initServer())
    {
        serv.closeAndClear();
        return (1);
    }
    return (0);
}