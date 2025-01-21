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
	}
	else if (pwd != serv.getPassword())
	{
		msg = "Password incorrect";
		numeric = ERR_PASSWDMISMATCH;
	}
	else {
		client.setPasswordFilled(true);
	}
	if (!msg.empty())
		sendNumericCmd(client, numeric, cmd.command, msg + ENDLINE_MSG);
	
}