#include "QUIT.hpp"

QUIT::QUIT() : Command("QUIT", 0, true) {};
QUIT::~QUIT() {};

void QUIT::execCommand(Server &serv, Client &client, const com &cmd)
{
	// std::string reason = (cmd.trailing.empty()) ? "" : cmd.trailing;
	// std::string message = (cmd.hasText) ? reason : "Client Quit";
	
	// std::vector<std::string> listChanjoined = client.getListChanJoined();

	// for (std::vector<std::string>::iterator it = listChanjoined.begin(); it != listChanjoined.end() ; it++) {
	// 	std::map<std::string, Channel*>::iterator it_lst = serv.getChannel().find(*it);
	// 	if (it_lst != serv.getChannel().end()) {
	// 		it_lst->second->delMembres(client.getClientSocket());
	// 		it_lst->second->delOperatores(client.getClientSocket());
	// 		it_lst->second->delInvited(client.getClientSocket());
	// 		client.removeChan(it_lst->second->getName());
		
	// 		if (it_lst->second->getMembresFd().size() == 0)
	// 		{
	// 			delete it_lst->second;
	// 			serv.getChannel().erase(it_lst);
	// 		} else {
	// 			std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost QUIT :" + reason;
	// 			it_lst->second->sendMsgMembres(msg + ENDLINE_MSG);
	// 		}
	// 	}
	// }
	
	close(client.getClientSocket());
	serv.delClientWithFd(client.getClientSocket());
	
}