#pragma once

#include "Command.hpp"
#include "irc_head.hpp"

class INVITE : public Command {
    public:
        INVITE();
        virtual ~INVITE();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};