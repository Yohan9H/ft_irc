#pragma once 
#ifndef PING_HPP
# define PING_HPP

#include "Command.hpp"

class PING : public Command {
    public:
        PING();
        virtual ~PING();

        virtual void execCommand(Server &serv, Client &cli, const cmd &cmd);
};

#endif