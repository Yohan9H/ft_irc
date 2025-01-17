#include "QUIT.hpp"

QUIT::QUIT() : Command("QUIT", 0, true) {};
QUIT::~QUIT() {};

void QUIT::execCommand(Server &serv, Client &client, const com &cmd)
{
    // message after QUIT ? 
	// Parcourir les canaux:
		//send message to channel
		// remove from channel list
	// send message to the server
	// Clean
		// close socket
		// clean structures
		// if client is last in canal --> delete channel

	std::string reason = (cmd.trailing.empty()) ? "" : cmd.trailing;
	std::string message = (cmd.hasText) ? reason : "Client Quit";
	
	std::vector<std::string> listChan = client.getListChanJoined();
	for (std::vector<std::string>::iterator it = listChan.begin(); it != listChan.end() ; it++) {
		Channel* channel = serv.getChannelbyName(*it);
		if (channel && channel->getMembresFd().size() > 1) {
			channel->sendMsgMembresExceptFd(message, client.getClientSocket());
			channel->delMembres(client.getClientSocket());
		} else if (channel && channel->getMembresFd().size() == 1){
			delete channel;
		}
	}

}