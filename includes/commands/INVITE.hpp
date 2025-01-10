#pragma once 
#ifndef INVITE_HPP
# define INVITE_HPP

#include "Command.hpp"

class INVITE : public Command {
    public:
        INVITE();
        virtual ~INVITE();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};

#endif