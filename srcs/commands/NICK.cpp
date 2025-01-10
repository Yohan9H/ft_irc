#include "../includes/commands/NICK.hpp"

NICK::NICK() : Command("NICK", 1, false) {};
NICK::~NICK() {};

void NICK::execCommand(Server &serv, Client &cli, const com &cmd)
{
    	std::string msg;
        std::string nick = cmd.params[0];

	// Verif valide name

	if (nick.length() > 9)
	{
		msg = msg_err(NAME_SERV, "432", nick, ":Erroneous nickname");
		send(cli.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
	bool verif_char = true;
	for (std::string::const_iterator it = nick.begin(); it != nick.end(); ++it)
	{
		if (is_good(*it) == false)
		{
			verif_char = false;
			break;
		}
	}
	if (verif_char == false)
	{
		msg = msg_err(NAME_SERV, "432", nick, ":Erroneous nickname");
		send(cli.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
	if (std::isdigit(nick[0]))
	{
		msg = msg_err(NAME_SERV, "432", nick, ":Erroneous nickname (debug - isalpha)");
		send(cli.getClientSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
	
	removeNewline(nick);

	// Verif if already use
	for (std::map<int, Client*>::iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		if (it->second->getNickname() == nick)
		{
			std::cout << "in if" << std::endl;
			msg = msg_err(NAME_SERV, "433", nick, ":Nickname is already in use");
			send(cli.getClientSocket(), msg.c_str(), msg.size(), 0);
			return ;
		}
	}

	cli.setNick(nick);

	// -- Voir si besoin d'envoye un msg a client qui met ou change son nom ?? --

	// Informe tous les channels dont il fait partie de son changement
	if (cli.getUsername().empty())
	{
		msg = ":_NoName_!user@" + std::string(NAME_SERV) + "NICK :";
		cli.sendMsgAllChan(serv, msg); 
	}
	else
	{
		msg = ":" + cli.getUsername() + " !user@" + std::string(NAME_SERV) + "NICK :";
		cli.sendMsgAllChan(serv, msg);
	}

	cli.print_for_test();
	return ;
}