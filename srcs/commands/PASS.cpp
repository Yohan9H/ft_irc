#include "PASS.hpp"

PASS::PASS() : Command("PASS", 1, false) {};
PASS::~PASS() {};

void PASS::execCommand(Server &serv, Client &client, const com &cmd)
{
    
}