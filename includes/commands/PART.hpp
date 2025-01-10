#pragma once 
#include "Command.hpp"

class PART : public Command {
    public:
        PART();
        virtual ~PART();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};
