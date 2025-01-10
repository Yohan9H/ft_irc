#pragma once

#include <string>
#include <vector>

class Client;
class Server;

struct com;

class Command
{
    private:
        std::string _name;
        int _nbParam;
        bool _mustbeAuth;
    

    public:
        Command(std::string name, int nbParam, bool mustbeAuth);
        virtual ~Command();
        // GETTERS
        std::string const& getName();
        int const& getNbParam();
        bool const& getMustbeAuth();
    
        virtual void execCommand(Server &serv, Client &cli, const com &cmd) = 0;
};
