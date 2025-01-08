#include "PART.hpp"

PART::PART() : Command("PART", 1, false) {};
PART::~PART() {};

void PART::execCommand(Server &serv, Client &cli, const cmd &cmd)
{
    
}