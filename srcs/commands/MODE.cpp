#include "../includes/commands/MODE.hpp"

MODE::MODE() : Command("MODE", 1, false) {};
MODE::~MODE() {};

void MODE::execCommand(Server &serv, Client &cli, const com &cmd)
{
    
}