#pragma once 

#include "Command.hpp"

class QUIT : public Command {
    public:
        QUIT();
        virtual ~QUIT();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};
