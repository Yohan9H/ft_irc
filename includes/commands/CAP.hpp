#pragma once

#include "Command.hpp"
#include "irc_head.hpp"

class CAP : public Command {
    public:
        CAP();
        virtual ~CAP();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};

