#pragma once

#include "Command.hpp"
#include "irc_head.hpp"

class PASS : public Command {
    public:
        PASS();
        virtual ~PASS();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};

