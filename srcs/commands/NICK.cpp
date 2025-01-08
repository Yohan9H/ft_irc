#include "NICK.hpp"

NICK::NICK() : Command("NICK", 1, false) {};
NICK::~NICK() {};

void NICK::execCommand(Server &serv, Client &cli, const cmd &cmd)
{
    
}