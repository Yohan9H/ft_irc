#pragma once 

#include "Command.hpp"
#include "irc_head.hpp"

class PRIVMSG : public Command {
    public:
        PRIVMSG();
        virtual ~PRIVMSG();

        virtual void execCommand(Server &serv, Client &client, const com &cmd);
};

