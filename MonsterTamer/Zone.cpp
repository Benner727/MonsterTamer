#include "Zone.h"

#include <sstream>

Zone::Zone()
{
}

std::istream& operator>>(std::istream& is, Zone& zone)
{
	std::string line;
	std::getline(is, line);
	std::stringstream ss(line);

	ss >> zone.mIndex >> std::quoted(zone.mName) >> zone.mLowestLevel >> zone.mHighestLevel >> zone.mGuildLeader;

	char delim;

	zone.mConnectedZones.clear();
	int zoneId;
	while (ss.peek() != ',' && ss >>zoneId)
		zone.mConnectedZones.push_back(zoneId);

	ss >> delim;

	zone.mWildMonsters.clear();
	int wildId;
	while (ss >> wildId)
		zone.mWildMonsters.push_back(wildId);
	return is;
}

std::ostream& operator<<(std::ostream& os, const Zone& zone)
{
	os << zone.mIndex << " " << std::quoted(zone.mName) << " " 
		<< zone.mLowestLevel << " " << zone.mHighestLevel << " " << zone.mGuildLeader;

	for (const auto& zoneId : zone.mConnectedZones)
		os << " " << zoneId;

	os << ',';

	for (const auto& wildId : zone.mWildMonsters)
		os << " " << wildId;

	return os;
}