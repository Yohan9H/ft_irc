#pragma once 
#ifndef PART_HPP
# define PART_HPP

#include "Command.hpp"

class PART : public Command {
    public:
        PART();
        virtual ~PART();

        virtual void execCommand(Server &serv, Client &cli, const cmd &cmd);
};

#endif