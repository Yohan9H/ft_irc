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