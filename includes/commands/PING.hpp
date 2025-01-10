#pragma once 
#include "Command.hpp"

class PING : public Command {
    public:
        PING();
        virtual ~PING();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};
