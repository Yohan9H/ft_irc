#include "Command.hpp"

Command::Command(std::string name, int nbParam, bool mustbeAuth) : _name(name), _nbParam(nbParam), _mustbeAuth(mustbeAuth) {}

Command::~Command() {}

// GETTERS
std::string const& Command::getName() {
	return (this->_name);
}

int const& Command::getNbParam() {
	return (this->_nbParam);
}

bool const& Command::getMustbeAuth() {
	return (this->_mustbeAuth);
}
