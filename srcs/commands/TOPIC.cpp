#include "../includes/commands/TOPIC.hpp"

TOPIC::TOPIC() : Command("TOPIC", 1, false) {};
TOPIC::~TOPIC() {};

void TOPIC::execCommand(Server &serv, Client &cli, const com &cmd)
{
    
}