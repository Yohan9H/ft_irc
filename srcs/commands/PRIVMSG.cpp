#include "../includes/commands/PRIVMSG.hpp"

PRIVMSG::PRIVMSG() : Command("PRIVMSG", 1, false) {};
PRIVMSG::~PRIVMSG() {};

void PRIVMSG::execCommand(Server &serv, Client &cli, const com &cmd)
{
    
}