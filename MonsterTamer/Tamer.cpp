#include "Tamer.h"

#include <algorithm>

Tamer::Tamer(std::string name)
	:mName(name)
{
	mIndex = 0;

	mParty = std::make_shared<Party>();
	mInventory = std::make_shared<Inventory>();

	mZone = 1;

	mMonstersTamed = 1;
	mMonstersKilled = 0;
	mTamersBeat = 0;
}

void Tamer::AddMonsterSeen(int index)
{
	if (!CheckMonsterSeen(index))
	{
		mMonstersSeen.push_back(index);
		std::sort(mMonstersSeen.begin(), mMonstersSeen.end());
	}
}

bool Tamer::CheckMonsterSeen(int index)
{
	for (const auto& monster : mMonstersSeen)
	{
		if (monster == index)
			return true;
	}

	return false;
}

void Tamer::UnlockZone(int index)
{
	if (!ZoneUnlocked(index))
		mZonesUnlocked.push_back(index);
}

bool Tamer::ZoneUnlocked(int index)
{
	for (const auto& zone : mZonesUnlocked)
	{
		if (zone == index)
			return true;
	}

	return false;
}

void Tamer::DefeatGuild(int index)
{
	if (!GuildDefeated(index))
		mGuildsDefeated.push_back(index);
}

bool Tamer::GuildDefeated(int index)
{
	for (const auto& guild : mGuildsDefeated)
	{
		if (guild == index)
			return true;
	}

	return false;
}

std::ostream& operator<<(std::ostream& os, const Tamer& tamer)
{
	os << "Name: " << tamer.mName << "\n\n";

	int guildsDefeated = 0;
	for (const auto& gd : tamer.mGuildsDefeated)
		if (gd) guildsDefeated++;

	os << "Guilds Defeated: " << guildsDefeated << "\n\n";

	os << "Stats:\n";
	os << " Monsters Tamed: " << tamer.mMonstersTamed << "\n";
	os << " Monsters Killed: " << tamer.mMonstersKilled << "\n";
	os << " Tamers Defeated: " << tamer.mTamersBeat << "\n";

	return os;
}