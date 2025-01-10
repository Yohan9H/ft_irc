#include "../includes/commands/JOIN.hpp"

JOIN::JOIN() : Command("JOIN", 1, false) {};
JOIN::~JOIN() {};

void JOIN::execCommand(Server &serv, Client &cli, const com &cmd)
{
    
}