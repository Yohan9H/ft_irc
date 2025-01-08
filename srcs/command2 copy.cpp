
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Command2.hpp"

void parseline(const std::string &line)
{
   (void)line;
    //split when you find /r/n
}

void execCommand(Server &serv, Client &cli, const cmd &cmd) // passer ref a mon avis
{
    const std::string validCommands[] = {
        "NICK", "USER", "JOIN", "PRIVMSG", "PART", "QUIT", "TOPIC", "KICK", "MODE", "INVITE", "CAP", "PASS", "PING", "NOTICE"
    };

    if (cmd.command.empty())
        throw std::logic_error("Empty command");
    bool found = false;
    for (size_t i = 0; i < (sizeof(validCommands) / sizeof(validCommands[0])); i++) {
        if (cmd.command == validCommands[i]) {
            found = true;
            break;
        }
    }
    if (!found)
        throw std::logic_error("Invalid command");
    if (cmd.command == "NICK" && cmd.params.size() == 1)
    {
		Nick(serv, cli, cmd.params[0]);
    }
    else if (cmd.command == "USER" && cmd.params.size() < 3)
	{
		User(serv, cli, cmd.params[0]);
	}
    else if (cmd.command == "JOIN" && cmd.params.size() >= 1)
    {
        if (cmd.params.size() == 1)
            join(serv, cli, cmd.params[0], "");
        else if (cmd.params.size() == 2)
            join(serv, cli, cmd.params[0], cmd.params[1]);
         // Si jamais pas de param 1 (pas de mdp, param 1 a set a NULL)
    }
    if (cmd.command == "PRIVMSG" && cmd.params.size() != 1) return false; 
    if (cmd.command == "PART" && cmd.params.size() != 1) return false;
    if (cmd.command == "QUIT" && cmd.params.size() != 0) return false;
    if (cmd.command == "TOPIC" && cmd.params.size() != 1) return false;
    if (cmd.command == "KICK" && cmd.params.size() != 2) return false;
    if (cmd.command == "MODE" && cmd.params.size() < 1) return false;
    if (cmd.command == "INVITE" && cmd.params.size() != 2) return false;
    if (cmd.command == "CAP" && cmd.params.size() != 1) return false;
    if (cmd.command == "PASS" && cmd.params.size() != 1) return false;
    if (cmd.command == "PING" && cmd.params.size() != 1) return false;
    if (cmd.command == "NOTICE" && cmd.params.size() != 2) return false;

    if (cmd.trailing.length() > 512)
        return (false);
    return (true);
}

void handlePrivmsg(const cmd &cmd) {
    if (cmd.params.size() < 2) {
        std::string errorCode = "461";
        std::string errorMessage = "PRIVMSG :Not enough parameters";
        throw std::logic_error(errorCode + " " + errorMessage);
    }

    const std::string &target = cmd.params[0];
    const std::string &message = cmd.trailing;

    if (target.empty() || message.empty()) {
        std::string errorCode = "412";
        std::string errorMessage = "No text to send";
        throw std::logic_error(errorCode + " " + errorMessage);
    }

    // Logique métier (envoyer le message au destinataire)
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

void parseCommand(Server &serv, Client &cli, const std::string &input) {

    size_t pos = 0;
    std::string data = input;
    normalizeCRLF(data);
    while ((pos = data.find("\r\n")) != std::string::npos) {

        std::string line = input.substr(0, pos);
        std::string token;
        std::istringstream tokenStream(line);
        cmd command;

        // Check if the command has a prefix
        if (!line.empty() && data[0] == ':') {
            std::getline(tokenStream, token, ' ');
            command.prefix = token.substr(1);
        }
        // Get the command
        if (std::getline(tokenStream, token, ' '))
            command.command = token;

        // Get the parameters and trailing
        while (std::getline(tokenStream, token, ' ')) {
            if (!token.empty() && token[0] == ':') {
                command.trailing = token.substr(1);
                break;
            } else {
                command.params.push_back(token);
            }
        }

        // FOR TEST
	    if (command.command == "CLIENT")
		    showMapClient(serv);
	    if (command.command == "CHANNEL")
		    showMapChannel(serv);
        
        try
        {
           execCommand(serv, cli, command);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

    // return (command);
        data.erase(0, pos + 2);
    }
}

