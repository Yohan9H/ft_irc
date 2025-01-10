#pragma once 
#ifndef KICK_HPP
# define KICK_HPP

#include "Command.hpp"

class KICK : public Command {
    public:
        KICK();
        virtual ~KICK();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};

#endif