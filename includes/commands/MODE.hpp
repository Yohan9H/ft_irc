#pragma once 
#include "Command.hpp"

class MODE : public Command {
    public:
        MODE();
        virtual ~MODE();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};
