#include "CAP.hpp"

CAP::CAP() : Command("CAP", 1, true) {};
CAP::~CAP() {};

void CAP::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}