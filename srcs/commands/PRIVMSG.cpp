#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG() : Command("PRIVMSG", 1, false) {};
PRIVMSG::~PRIVMSG() {};

void PRIVMSG::execCommand(Server &serv, Client &client, const com &cmd)
{
	// pas de destinataire => ERR_NORECIPIENT
	// pas de text a envoyer ==> ERR_NOTEXTTOSEND
    // destinataire n'existe pas => ERR_NOSUCHNICK
	// if # => channel
	//	channel n'existe pas => ERR_NOSUCHCHANNEL
	// pas membre sur le channel => ERR_CANNOTSENDTOCHAN
	//

	std::string target = cmd.params[0];
	std::string message = cmd.trailing;

	if (target.empty()) {
		std::cerr << "ERR_NORECIPIENT" << std::endl;
	} else if (!cmd.hasText){
		std::cerr << "ERR_NOTEXTTOSEND" << std::endl;
	} else if (target[0] == '#') {
		Channel* channel = serv.getChannelbyName(target);
		if (!channel)
			std::cerr << "ERR_NOSUCHCHANNEL" << std::endl;
		else if (!channel->checkClientIsMembre(client.getClientSocket()))
			std::cerr << "ERR_CANNOTSENDTOCHAN" << std::endl;
		else
			channel->sendMsgMembresExceptFd(message, client.getClientSocket());
	} else {
		Client* targetclient = serv.getClientbyName(target);
		if (!targetclient)
			std::cerr << "ERR_NOSUCHNICK" << std::endl;
		else {
			send(targetclient->getClientSocket(), message.c_str(), message.size(), 0);
			send(client.getClientSocket(), message.c_str(), message.size(), 0);
		}
	}

}