#include "Monster.h"

#include <random>

static std::random_device rand_dev;
static std::mt19937 generator(rand_dev());
static std::uniform_int_distribution<int> traitDist(0, 31);
static std::uniform_int_distribution<int> genderDist(1, 100);

Monster::Monster(int speciesIndex, int level, bool wild)
	: mSpeciesIndex(speciesIndex), mLevel(level), mWild(wild)
{
	mName = AssetManager::Instance()->GetSpecies(mSpeciesIndex).lock()->Name();

	mExperience = pow(level, 3.0f) * 0.8f;

	mHealth = 1;

	int genderRate = AssetManager::Instance()->GetSpecies(mSpeciesIndex).lock()->GenderRate();
	if (genderRate)
	{
		if (genderRate > genderDist(generator))
			mGender = 1; //Male
		else
			mGender = 2; //Female
	}
	else
		mGender = 0; //Genderless

	mAlpha = false;
	for (int i = 0; i < 4; ++i)
	{
		mStats[i] = 1;
		mTraits[i] = traitDist(generator);
		if (mTraits[i] == 31)
			mAlpha = true;
	}

	mStatus = 0;
	mStatusCount = 0;

	mKills = 0;

	mCaughtAt = mLevel;

	CalculateStats();
	UnlockAbilities();

	int index = 0;
	for (const auto& ability : mLearnableAbilities)
	{
		if (mAbilities.size() < 4)
			mAbilities.push_back(std::make_pair(ability, AssetManager::Instance()->GetAbility(ability).lock()->TotalUses()));
		else
		{
			mAbilities[index++] = std::make_pair(ability, AssetManager::Instance()->GetAbility(ability).lock()->TotalUses());
			if (index > 3) index = 0;
		}
	}
	mLearnableAbilities.clear();
}

void Monster::CalculateStats()
{
	std::weak_ptr<Species> species = AssetManager::Instance()->GetSpecies(mSpeciesIndex);

	int prevHealth = mStats[0];

	mStats[0] = ((species.lock()->BaseHP() + mTraits[0] + ((mLevel - mCaughtAt) * 0.25f)) * 2.0f * mLevel + mLevel + 10);
	mHealth += mStats[0] - prevHealth;
	mStats[1] = ((species.lock()->BaseAttack() + mTraits[1] + ((mLevel - mCaughtAt) * 0.25f)) * 2.0f * mLevel + 5);
	mStats[2] = ((species.lock()->BaseDefense() + mTraits[2] + ((mLevel - mCaughtAt) * 0.25f)) * 2.0f * mLevel + 5);
	mStats[3] = ((species.lock()->BaseSpeed() + mTraits[3] + ((mLevel - mCaughtAt) * 0.25f)) * 2.0f * mLevel + 5);
}

void Monster::UnlockAbilities(int prevLevel)
{
	std::vector<std::pair<int, int>> abilities = AssetManager::Instance()->GetSpecies(mSpeciesIndex).lock()->Abilities();

	for (const auto& ability : abilities)
	{
		if (ability.first > prevLevel && ability.first <= mLevel)
			mLearnableAbilities.push_back(ability.second);
	}
}

std::string Monster::Name() const
{
	std::string name = mName;

	if (mWild && mName == AssetManager::Instance()->GetSpecies(mSpeciesIndex).lock()->Name())
	{
		if (mAlpha) 
			name.insert(0, "Alpha ");
	}

	return name;
}

bool Monster::LevelUp()
{
	bool levelled = false;
	
	mLearnableAbilities.clear();

	if (mLevel < 100)
	{
		int requiredExp = pow(mLevel + 1, 3.0f) * 0.8f;
		
		while (mExperience >= requiredExp)
		{
			mLevel++;
			UnlockAbilities(mLevel - 1);
			levelled = true;

			requiredExp = pow(mLevel + 1, 3.0f);

			if (mLevel > 99)
				break;
		}
	}

	if (levelled) CalculateStats();
	return levelled;
}

void Monster::AddExperience(int exp)
{
	if (mLevel < 100) 
		mExperience += exp;
}

void Monster::LearnAbility(int slot, int index)
{
	mAbilities[slot].first = index;
	mAbilities[slot].second = AssetManager::Instance()->GetAbility(index).lock()->TotalUses();
}

void Monster::LearnAbility(int index)
{
	mAbilities.push_back(std::make_pair(index, AssetManager::Instance()->GetAbility(index).lock()->TotalUses()));
}

void Monster::Rest()
{
	mHealth = mStats[0];

	for (auto& ability : mAbilities)
		ability.second = AssetManager::Instance()->GetAbility(ability.first).lock()->TotalUses();

	mStatus = HEALTHY;
	mStatusCount = 0;
}

std::ostream& operator<<(std::ostream& os, const Monster& monster)
{
	os << "Name: " << monster.mName << "\n";
	os << "Caught at Lv: " << monster.mCaughtAt << "\n";
	os << "Current Lv: " << monster.mLevel << "\n\n";

	os << "Species: " << AssetManager::Instance()->GetSpecies(monster.mSpeciesIndex).lock()->Name();

	switch (monster.mGender)
	{
	case 1: //Male
		os << " (Male)\n";
		break;
	case 2: //Female
		os << " (Female)\n";
		break;
	case 0: //Genderless
		os << "\n";
		break;
	}

	std::string primaryElement = AssetManager::Instance()->GetElement(AssetManager::Instance()->GetSpecies(monster.mSpeciesIndex).lock()->PrimaryElement()).lock()->Name();
	std::string secondaryElement = AssetManager::Instance()->GetElement(AssetManager::Instance()->GetSpecies(monster.mSpeciesIndex).lock()->SecondaryElement()).lock()->Name();
	
	os << "Element: ";
	os << primaryElement; 
	if (secondaryElement != "None")
		os << "/" << secondaryElement;
	os << "\n\n";

	std::weak_ptr<Passive> passive = AssetManager::Instance()->GetPassive(AssetManager::Instance()->GetSpecies(monster.mSpeciesIndex).lock()->PassiveIndex());

	os << "Passive: ";
	os << passive.lock()->Name();
	if (passive.lock()->Name() != "None") os << " - " << passive.lock()->Description();
	os << "\n\n";

	os << "Stats:\n";
	os << std::left << std::setw(10) << " Health:" << monster.mStats[0] << "\n";
	os << std::left << std::setw(10) << " Attack:" << monster.mStats[1] << "\n";
	os << std::left << std::setw(10) << " Defense:" << monster.mStats[2] << "\n";
	os << std::left << std::setw(10) << " Speed:" << monster.mStats[3] << "\n\n";

	os << "Abilities:\n";
	int index = 1;
	for (const auto& ability : monster.mAbilities)
	{
		std::weak_ptr<Ability> temp = AssetManager::Instance()->GetAbility(ability.first);
		std::string uses = std::to_string(ability.second) + "/" + std::to_string(temp.lock()->TotalUses());

		os << " [" << index++ << "] " << std::left << std::setw(16) << temp.lock()->Name();
		os << std::left << std::setw(12) << AssetManager::Instance()->GetElement(temp.lock()->ElementIndex()).lock()->Name();
		os << std::left << std::setw(5) << uses << "\n";
		
		os << "     Power: " << std::left << std::setw(6) << (temp.lock()->Power() == 0 ? "---" : std::to_string(temp.lock()->Power()));
		os << "Accuracy: " << temp.lock()->Accuracy() << "\n\n";
	}

	os << "Lives Claimed: " << monster.mKills;

	return os;
}