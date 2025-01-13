#pragma once

#include "Command.hpp"
#include "irc_head.hpp"

class MODE : public Command {
    public:
        MODE();
        virtual ~MODE();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};
