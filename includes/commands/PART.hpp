#pragma once

#include "Command.hpp"
#include "irc_head.hpp"

class PART : public Command {
    public:
        PART();
        virtual ~PART();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};
