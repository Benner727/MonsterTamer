#pragma once

#include "Party.h"
#include "Inventory.h"

class Tamer
{
public:
	Tamer(std::string name = "");
	
private:
	int mIndex;
	std::string mName;

	std::shared_ptr<Inventory> mInventory;
	std::shared_ptr<Party> mParty;

	std::vector<int> mZonesUnlocked;
	std::vector<int> mMonstersSeen;
	std::vector<int> mGuildsDefeated;

	int mZone;

	int mMonstersTamed, mMonstersKilled, mTamersBeat;

public:
	int Index() const { return mIndex; }
	std::string Name() { return mName; }
	void Rename(std::string name) { mName = name; }

	std::weak_ptr<Party> GetParty() { return mParty; }
	std::weak_ptr<Inventory> GetInvetory() { return mInventory; }

	void AddMonsterSeen(int index);
	bool CheckMonsterSeen(int index);
	std::vector<int> MonstersSeen() const { return mMonstersSeen; }

	void AddMonstersTamed() { mMonstersTamed++; }
	void AddMonstersKilled() { mMonstersKilled++; }
	void AddTamersBeaten() { mTamersBeat++; }

	void UnlockZone(int index);
	bool ZoneUnlocked(int index);

	int Zone() const { return mZone; }
	void Zone(int zone) { mZone = zone; }

	void DefeatGuild(int index);
	bool GuildDefeated(int index);

	friend std::ostream& operator<<(std::ostream& os, const Tamer& tamer);

	template<class Archive>
	void save(Archive & archive) const
	{
		archive(mIndex, mName, mZonesUnlocked, mMonstersSeen, mGuildsDefeated, mZone, mInventory, mParty, mMonstersTamed, mMonstersKilled, mTamersBeat);
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(mIndex, mName, mZonesUnlocked, mMonstersSeen, mGuildsDefeated, mZone, mInventory, mParty, mMonstersTamed, mMonstersKilled, mTamersBeat);
	}
};