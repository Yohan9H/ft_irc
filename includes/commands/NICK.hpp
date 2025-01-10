#pragma once 
#include "Command.hpp"
#include "irc_head.hpp"

class NICK : public Command {
    public:
        NICK();
        virtual ~NICK();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};

