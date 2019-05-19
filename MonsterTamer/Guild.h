#pragma once

#include <string>
#include <vector>
#include <iomanip>

class Guild
{
public:
	Guild();

private:
	int mIndex;
	std::string mName;

	std::vector<std::pair<int, int>> mParty;

public:
	int Index() const { return mIndex; }
	std::string Name() const { return mName; }

	std::vector<std::pair<int, int>> Party() const { return mParty; }

	friend std::istream& operator>>(std::istream& is, Guild& guild);
	friend std::ostream& operator<<(std::ostream& os, const Guild& guild);
};