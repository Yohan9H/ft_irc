#pragma once 
#ifndef QUIT_HPP
# define QUIT_HPP

#include "Command.hpp"

class QUIT : public Command {
    public:
        QUIT();
        virtual ~QUIT();

        virtual void execCommand(Server &serv, Client &cli, const cmd &cmd);
};

#endif