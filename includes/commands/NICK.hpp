#pragma once 
#ifndef NICK_HPP
# define NICK_HPP

#include "Command.hpp"

class NICK : public Command {
    public:
        NICK();
        virtual ~NICK();

        virtual void execCommand(Server &serv, Client &cli, const cmd &cmd);
};

#endif
