#pragma once 

#include "Command.hpp"
#include "irc_head.hpp"

class TOPIC : public Command {
    public:
        TOPIC();
        virtual ~TOPIC();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};

