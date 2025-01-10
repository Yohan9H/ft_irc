#include "../includes/commands/USER.hpp"

USER::USER() : Command("USER", 3, false) {};
USER::~USER() {};

void USER::execCommand(Server &serv, Client &cli, const cmd &cmd)
{
    
}