#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG() : Command("PRIVMSG", 1, false) {};
PRIVMSG::~PRIVMSG() {};

void PRIVMSG::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}