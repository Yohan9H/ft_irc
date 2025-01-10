#include "../includes/commands/PING.hpp"

PING::PING() : Command("PING", 1, false) {};
PING::~PING() {};

void PING::execCommand(Server &serv, Client &cli, const com &cmd)
{
    
}