#include "PASS.hpp"

PASS::PASS() : Command("PASS", 1, false) {};
PASS::~PASS() {};

void PASS::execCommand(Server &serv, Client &client, const com &cmd)
{
    std::string pwd = cmd.params[0];

	if (client.getIsAuth()) {
		std::cerr << "ERR_ALREADYREGISTERED" << std::endl;
	}
	else if (pwd != serv.getPassword())
	{
		std::cerr << "ERR_PASSWDMISMATCH" << std::endl;
	}
	else {
		client.setPasswordFilled(true);
	}
	
}