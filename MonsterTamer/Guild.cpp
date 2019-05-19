#include "Guild.h"

#include <sstream>

Guild::Guild()
{
	mIndex = 0;
	mName = "NULL GUILD";
}

std::istream& operator>>(std::istream& is, Guild& guild)
{
	std::string line;
	std::getline(is, line);
	std::stringstream ss(line);

	ss >> guild.mIndex >> std::quoted(guild.mName);

	int speciesIndex, level;
	char delim;
	guild.mParty.clear();
	while (ss >> speciesIndex >> delim >> level)
		guild.mParty.push_back(std::make_pair(speciesIndex, level));;

	return is;
}

std::ostream& operator<<(std::ostream& os, const Guild& guild)
{
	os << guild.mIndex << std::quoted(guild.mName);

	for (const auto slot : guild.mParty)
		os << slot.first << "," << slot.second;

	return os;
}