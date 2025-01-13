#pragma once 

#include "Command.hpp"
#include "irc_head.hpp"

class USER : public Command {
    public:
        USER();
        virtual ~USER();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};
