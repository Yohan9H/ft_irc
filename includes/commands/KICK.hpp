#pragma once

#include "Command.hpp"
#include "irc_head.hpp"

class KICK : public Command {
    public:
        KICK();
        virtual ~KICK();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};
