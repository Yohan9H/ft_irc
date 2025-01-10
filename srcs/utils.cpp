#include "Utils.hpp"

bool	is_good(char c)
{
	std::string valide_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-#&+!.^,`|~$%*:<>\n";
	std::size_t pos = valide_char.find(c);

	if (pos != std::string::npos)
		return true;
	else
		return false;
}

void	showMapClient(Server &serv)
{
	std::cout << "|Map Client|" << std::endl;
	for (std::map<int, Client*>::const_iterator it = serv.getClients().begin(); it != serv.getClients().end(); it++)
	{
		std::cout << "name client : " << it->second->getUsername() << std::endl;	
	}
}

void	showMapChannel(Server &serv)
{
	for (std::map<std::string, Channel*>::const_iterator it = serv.getChannel().begin(); it != serv.getChannel().end(); it++)
	{
		std::cout << "name channel : " << it->first << std::endl;
	}
}

void normalizeCRLF(std::string& input) {
    size_t pos = 0;

    while (pos < input.size()) {
        pos = input.find("\n", pos);
        if (pos == std::string::npos) {
            break;
        }
        if (pos == 0 || input[pos - 1] != '\r') {
            input.insert(pos, "\r");
            pos += 2;
        } else {
            pos += 1;
        }
    }
}