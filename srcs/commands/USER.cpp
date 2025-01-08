#include "USER.hpp"

USER::USER() : Command("USER", 1, false) {};
USER::~USER() {};

void USER::execCommand(Server &serv, Client &cli, const cmd &cmd)
{
    
}