#pragma once 
#ifndef CAP_HPP
# define CAP_HPP

#include "Command.hpp"

class CAP : public Command {
    public:
        CAP();
        virtual ~CAP();

        virtual void execCommand(Server &serv, Client &cli, const cmd &cmd);
};

#endif