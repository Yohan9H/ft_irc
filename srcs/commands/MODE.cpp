#include "MODE.hpp"

MODE::MODE() : Command("MODE", 1, false) {};
MODE::~MODE() {};

void MODE::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}