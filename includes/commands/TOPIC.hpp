#pragma once 

#include "Command.hpp"

class TOPIC : public Command {
    public:
        TOPIC();
        virtual ~TOPIC();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};

