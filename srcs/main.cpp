#include "irc_head.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << RED << "Usage = ./irc <port_number> <password>" << COL_END << std::endl;
		return (1);
	}
	//Perform mini parsing to validate the args
	Server serv(std::atoi(argv[1]), argv[2]);
	serv.setTime();
	//Handling the signals
	signal(SIGQUIT, &Server::signalHandler);
	signal(SIGINT, &Server::signalHandler);
	if (!serv.initServer())
	{
		serv.closeAndClear();
		return (1);
	}
	return (0);
}