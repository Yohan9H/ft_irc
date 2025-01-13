#pragma once

#include "Command.hpp"
#include "irc_head.hpp"

class JOIN : public Command {
    public:
        JOIN();
        virtual ~JOIN();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};
