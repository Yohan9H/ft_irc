#include "QUIT.hpp"

QUIT::QUIT() : Command("QUIT", 0, true) {};
QUIT::~QUIT() {};

void QUIT::execCommand(Server &serv, Client &client, const com &cmd)
{	
	int	fd_tmp = client.getClientSocket();
	close(fd_tmp);
	serv.delClientWithFd(fd_tmp);
	serv.delClientInFds(fd_tmp);
}