#include "PING.hpp"

PING::PING() : Command("PING", 1, false) {};
PING::~PING() {};

void PING::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}