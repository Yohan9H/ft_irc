#pragma once 
#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

#include "Command.hpp"

class PRIVMSG : public Command {
    public:
        PRIVMSG();
        virtual ~PRIVMSG();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};

#endif