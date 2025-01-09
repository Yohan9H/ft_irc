#include "../includes/commands/KICK.hpp"

KICK::KICK() : Command("KICK", 2, false) {};
KICK::~KICK() {};

void KICK::execCommand(Server &serv, Client &cli, const cmd &cmd)
{
    
}