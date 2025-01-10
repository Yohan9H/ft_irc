#pragma once 
#ifndef JOIN_HPP
# define JOIN_HPP

#include "Command.hpp"

class JOIN : public Command {
    public:
        JOIN();
        virtual ~JOIN();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};

#endif