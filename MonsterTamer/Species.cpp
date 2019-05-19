#include "Species.h"

#include <sstream>

Species::Species()
{
	mIndex = 0;
	mName = "NULL SPECIES";

	for (int i = 0; i < 4; ++i)
		mBaseStats[i] = 0;

	mGenderRate = 0;

	mPassiveIndex = 0;
}

std::istream& operator>>(std::istream& is, Species& species)
{	
	std::string line;
	std::getline(is, line);
	std::stringstream ss(line);

	ss >> species.mIndex >> std::quoted(species.mName) >> species.mPrimaryElement >> species.mSecondaryElement;
		
	for (int i = 0; i < 4; ++i)
		ss >> species.mBaseStats[i];

	ss >> species.mGenderRate >> species.mPassiveIndex;

	int level, moveIndex;
	char delim;
	species.mAbilities.clear();
	while (ss >> level >> delim >> moveIndex)
		species.mAbilities.push_back(std::make_pair(level, moveIndex));

	return is;
}

std::ostream& operator<<(std::ostream& os, const Species& species)
{
	os << species.mIndex << " " << std::quoted(species.mName) << " " << species.mPrimaryElement << " " << species.mSecondaryElement << " ";

	for (int i = 0; i < 4; ++i)
		os << species.mBaseStats[i] << " ";

	os << species.mGenderRate << " " << species.mPassiveIndex;

	for (const auto& move : species.mAbilities)
		os << " " << move.first << "," << move.second;

	return os;
}