#pragma once

#include "Command.hpp"
#include "irc_head.hpp"

class NOTICE : public Command {
    public:
        NOTICE();
        virtual ~NOTICE();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};
