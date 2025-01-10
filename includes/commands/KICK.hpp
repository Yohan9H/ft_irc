#pragma once 
#include "Command.hpp"

class KICK : public Command {
    public:
        KICK();
        virtual ~KICK();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};
