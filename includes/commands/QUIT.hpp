#pragma once 

#include "Command.hpp"
#include "irc_head.hpp"

class QUIT : public Command {
    public:
        QUIT();
        virtual ~QUIT();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};
