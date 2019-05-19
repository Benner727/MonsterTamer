#pragma once

#include <string>
#include <vector>
#include <iomanip>

class Zone
{
public:
	Zone();
	
private:
	int mIndex;
	std::string mName;

	int mLowestLevel;
	int mHighestLevel;

	int mGuildLeader;
	std::vector<int> mConnectedZones;

	std::vector<int> mWildMonsters;

public:
	int Index() const { return mIndex; }
	std::string Name() const { return mName; }

	int LowestLevel() const { return mLowestLevel; }
	int HighestLevel() const { return mHighestLevel; }

	int GuildLeader() const { return mGuildLeader; }
	std::vector<int> ConnectedZones() const { return mConnectedZones; }

	std::vector<int> WildMonsters() const { return mWildMonsters; }

	friend std::istream& operator>>(std::istream& is, Zone& zone);
	friend std::ostream& operator<<(std::ostream& os, const Zone& zone);
};