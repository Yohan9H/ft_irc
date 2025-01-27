#include "PASS.hpp"

PASS::PASS() : Command("PASS", 1, false) {};
PASS::~PASS() {};

void PASS::execCommand(Server &serv, Client &client, const com &cmd)
{
    std::string pwd = cmd.params[0];
	std::string msg;
	int numeric;

	if (client.getIsAuth()) {
		msg = "You may not reregister";
		numeric = ERR_ALREADYREGISTERED;
		sendNumericParam1(client, numeric, client.getNickname(), msg + ENDLINE_MSG);
	}
	else if (pwd != serv.getPassword())
	{
		msg = "Password incorrect";
		numeric = ERR_PASSWDMISMATCH;
		sendNumericParam1(client, numeric, client.getNickname(), msg + ENDLINE_MSG);
	}
	else {
		client.setPasswordFilled(true);
	}
	
}