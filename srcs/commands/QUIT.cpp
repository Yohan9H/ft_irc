#include "QUIT.hpp"

QUIT::QUIT() : Command("QUIT", 0, false) {};
QUIT::~QUIT() {};

void QUIT::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}