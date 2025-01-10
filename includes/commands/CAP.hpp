#pragma once 
#include "Command.hpp"

class CAP : public Command {
    public:
        CAP();
        virtual ~CAP();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};

