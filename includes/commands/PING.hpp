#pragma once 

#include "Command.hpp"
#include "irc_head.hpp"

class PING : public Command {
    public:
        PING();
        virtual ~PING();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};
