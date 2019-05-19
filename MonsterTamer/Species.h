#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

class Species
{
public:
	Species();
	
private:
	int mIndex;
	std::string mName;

	int mPrimaryElement;
	int mSecondaryElement;

	int mBaseStats[4];

	int mGenderRate;

	int mPassiveIndex;

	//Level unlock, Ability index
	std::vector<std::pair<int, int>> mAbilities;

public:
	int Index() const { return mIndex; }
	std::string Name() const { return mName; }

	int PrimaryElement() const { return mPrimaryElement; }
	int SecondaryElement() const { return mSecondaryElement; }

	int BaseHP() const { return mBaseStats[0]; }
	int BaseAttack() const { return mBaseStats[1]; }
	int BaseDefense() const { return mBaseStats[2]; }
	int BaseSpeed() const { return mBaseStats[3]; }

	int GenderRate() const { return mGenderRate; }

	int PassiveIndex() const { return mPassiveIndex; }

	std::vector<std::pair<int, int>> Abilities() const { return mAbilities; }

	friend std::istream& operator>>(std::istream& is, Species& species);
	friend std::ostream& operator<<(std::ostream& os, const Species& species);
};

