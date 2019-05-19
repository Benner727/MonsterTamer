#pragma once

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>

#include "AssetManager.h"

class Monster
{
public:
	Monster() {} //Needed for saving/loading
	Monster(int speciesIndex, int level = 1, bool wild = false);
	
	enum STATUS { HEALTHY = 0, SLEEP, POISON, BURN, PARALYSIS, FROZEN };

private:
	int mSpeciesIndex;
	std::string mName;

	int mLevel;
	int mExperience;

	int mGender;

	int mTraits[4];

	bool mAlpha;
	bool mWild;

	int mHealth;
	int mStats[4];

	int mStatus;
	int mStatusCount;

	int mKills;

	int mCaughtAt;

	//Ability index, Uses
	std::vector<std::pair<int, int>> mAbilities;
	std::vector<int> mLearnableAbilities;

	void CalculateStats();
	void UnlockAbilities(int prevLevel = 0);

public:
	int SpeciesIndex() { return mSpeciesIndex; }

	std::string Name() const;
	void Rename(std::string name) { mName = name; }

	int Level() const { return mLevel; }
	int Experience() const { return mExperience; }

	bool LevelUp();
	void AddExperience(int exp);
	std::vector<int> LearnableAbilities() const { return mLearnableAbilities; }
	void LearnAbility(int slot, int index);
	void LearnAbility(int index);

	int Gender() const { return mGender; }

	int Health() const { return mHealth; }
	int TotalHealth() const { return mStats[0]; }
	int Attack() const { return mStats[1]; }
	int Defense() const { return mStats[2]; }
	int Speed() const { return mStats[3]; }

	int Status() const { return mStatus; }
	void Status(STATUS status) { mStatus = status; }
	
	int StatusCount() const { return mStatusCount; }
	void StatusCount(int count) { mStatusCount = count; }

	int CaughtAt() const { return mCaughtAt; }

	void AddKill() { mKills++; }
	int Kills() const { return mKills; }

	std::vector<std::pair<int, int>>& Abilities() { return mAbilities; }

	void Rest();

	inline void Damage(int amount) { mHealth -= amount; if (mHealth < 0) mHealth = 0; }
	inline void Heal(int amount) { mHealth += amount; if (mHealth > mStats[0]) mHealth = mStats[0]; }

	friend std::ostream& operator<<(std::ostream& os, const Monster& monster);

	template<class Archive>
	void save(Archive & archive) const
	{
		archive(mSpeciesIndex, mName, mExperience, mLevel, mGender, mTraits[0], mTraits[1], mTraits[2], mTraits[3], mAlpha, mWild, mHealth, mStats[0], mStatus, mStatusCount, mKills, mCaughtAt, mAbilities);
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(mSpeciesIndex, mName, mExperience, mLevel, mGender, mTraits[0], mTraits[1], mTraits[2], mTraits[3], mAlpha, mWild, mHealth, mStats[0], mStatus, mStatusCount, mKills, mCaughtAt, mAbilities);
		CalculateStats();
	}
};