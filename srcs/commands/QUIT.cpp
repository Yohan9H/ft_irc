#include "QUIT.hpp"

QUIT::QUIT() : Command("QUIT", 1, false) {};
QUIT::~QUIT() {};

void QUIT::execCommand(Server &serv, Client &cli, const cmd &cmd)
{
    
}