#include "CAP.hpp"

CAP::CAP() : Command("CAP", 1, false) {};
CAP::~CAP() {};

void CAP::execCommand(Server &serv, Client &cli, const cmd &cmd)
{
    
}