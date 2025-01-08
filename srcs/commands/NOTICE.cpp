#include "NOTICE.hpp"

NOTICE::NOTICE() : Command("NOTICE", 1, false) {};
NOTICE::~NOTICE() {};

void NOTICE::execCommand(Server &serv, Client &cli, const cmd &cmd)
{
    
}