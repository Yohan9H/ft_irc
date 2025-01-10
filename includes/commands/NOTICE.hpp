#pragma once 
#include "Command.hpp"

class NOTICE : public Command {
    public:
        NOTICE();
        virtual ~NOTICE();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};
