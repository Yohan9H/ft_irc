#ifndef COMMAND_HPP

#define COMMAND_HPP

# include <string>
# include <vector>


class Server; 
class Client;

struct cmd;

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
    
        virtual void execCommand(Server &serv, Client &cli, const cmd &cmd) = 0;
};


#endif