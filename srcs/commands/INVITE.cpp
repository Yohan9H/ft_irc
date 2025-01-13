#include "INVITE.hpp"

INVITE::INVITE() : Command("INVITE", 2, false) {};
INVITE::~INVITE() {};

void INVITE::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}