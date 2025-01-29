#include "irc_head.hpp"

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
		it->second->print_for_test();
	}
}

void	showMapChannel(Server &serv)
{
	std::cout << "|Map Channel|" << std::endl;
	for (std::map<std::string, Channel*>::const_iterator it = serv.getChannel().begin(); it != serv.getChannel().end(); it++)
	{
		it->second->printChannelForTest(serv);
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

std::string to_string(int value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

void	OutDataNumericParam1(Client &client, int numeric, std::string param, const std::string& message)
{
	std::string fullMessage = std::string(HOST) + to_string(numeric) + " " + param + " :" + message;
	//send(client.getClientSocket(), fullMessage.c_str(), fullMessage.size(), MSG_NOSIGNAL);
	client.setOutData(fullMessage);
}

void	OutDataNumericParam2(Client &client, int numeric, const std::string param, const std::string param2, const std::string& message)
{
	std::string fullMessage = std::string(HOST) + to_string(numeric) + " " + param + " " + param2 + " :" + message;
	//send(client.getClientSocket(), fullMessage.c_str(), fullMessage.size(), MSG_NOSIGNAL);
	client.setOutData(fullMessage);
}

void	OutDataNumericParam3(Client &client, int numeric, const std::string param, const std::string param2, const std::string param3, const std::string& message)
{
	std::string fullMessage = std::string(HOST) + to_string(numeric) + " " + param + " " + param2 + " " + param3 + " :" + message;
	//send(client.getClientSocket(), fullMessage.c_str(), fullMessage.size(), MSG_NOSIGNAL);
	client.setOutData(fullMessage);
}



void sendNotice(Client &client, const std::string& message) {
	std::string notice = std::string(HOST) + "NOTICE " + client.getNickname() + " :" + message;
	send(client.getClientSocket(), notice.c_str(), notice.size(), 0);
}


void sendModeParamMsg (Client &client, Channel &channel, std::string mode, std::string param, Server &serv)
{
	std::string msg = std::string(HOST) + "MODE " + channel.getName() + " " + mode + " " + param;
	channel.sendMsgMembres(msg + ENDLINE_MSG, serv);
}

std::vector<std::string> split(std::string const& str, char sep) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream stream(str);
	while (std::getline(stream, token, sep))
		tokens.push_back(token);
	return tokens;
};

std::map<std::string,std::string> createMap(std::vector<std::string> channel, std::vector<std::string> mdp)
{
	std::map<std::string,std::string> res;
	if (channel.empty())
		return (res);
	else if (mdp.empty())
		mdp.push_back("");

	for (size_t i = 0; i < channel.size(); i++)
	{
		std::string key = channel[i];
		std::string value = (i < mdp.size()) ? mdp[i] : mdp.back();
		res.insert(std::pair<std::string, std::string>(key, value));
	}
	return (res);
}