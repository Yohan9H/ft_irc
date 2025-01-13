#pragma once



#include <string>
#include <vector>

class Client; // Forward declaration
class Server; // Forward declaration
struct com;   // Forward declaration


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
