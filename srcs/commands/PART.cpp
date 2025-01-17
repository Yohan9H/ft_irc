#include "PART.hpp"

PART::PART() : Command("PART", 1, true) {};
PART::~PART() {};

void PART::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}