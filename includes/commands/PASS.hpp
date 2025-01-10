#pragma once 
#include "Command.hpp"

class PASS : public Command {
    public:
        PASS();
        virtual ~PASS();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};

