#include "NOTICE.hpp"

NOTICE::NOTICE() : Command("NOTICE", 1, false) {};
NOTICE::~NOTICE() {};

void NOTICE::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}