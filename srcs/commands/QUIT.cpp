#include "../includes/commands/QUIT.hpp"

QUIT::QUIT() : Command("QUIT", 0, false) {};
QUIT::~QUIT() {};

void QUIT::execCommand(Server &serv, Client &cli, const com &cmd)
{
    
}