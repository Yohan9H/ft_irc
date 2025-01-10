#pragma once 
#ifndef USER_HPP
# define USER_HPP

#include "Command.hpp"

class USER : public Command {
    public:
        USER();
        virtual ~USER();

        virtual void execCommand(Server &serv, Client &cli, const com &cmd);
};

#endif