#pragma once 
#ifndef TOPIC_HPP
# define TOPIC_HPP

#include "Command.hpp"

class TOPIC : public Command {
    public:
        TOPIC();
        virtual ~TOPIC();

        virtual void execCommand(Server &serv, Client &cli, const cmd &cmd);
};

#endif