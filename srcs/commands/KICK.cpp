#include "KICK.hpp"

KICK::KICK() : Command("KICK", 1, false) {};
KICK::~KICK() {};

void KICK::execCommand(Server &serv, Client &cli, const cmd &cmd)
{
    
}