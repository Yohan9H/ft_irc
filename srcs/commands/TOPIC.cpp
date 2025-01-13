#include "TOPIC.hpp"

TOPIC::TOPIC() : Command("TOPIC", 1, false) {};
TOPIC::~TOPIC() {};

void TOPIC::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}