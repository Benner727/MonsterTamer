#include "Ability.h"

Ability::Ability()
{
	mIndex = -1;
	mName = "NULL ABILITY";

	mEffect = 0;
	mEffectChance = 0;

	mPhysical = false;

	mPower = 0;
	mAccuracy = 0;
	mTotalUses = 0;

	mTarget = 0;
	mPriority = 0;
}

std::istream& operator>>(std::istream& is, Ability& ability)
{
	is >> ability.mIndex >> std::quoted(ability.mName) >> std::hex >> ability.mEffect >> std::dec >> ability.mEffectChance
		>> ability.mPhysical >> ability.mElementIndex >> ability.mPower >> ability.mAccuracy 
		>> ability.mTotalUses >> ability.mTarget >> ability.mPriority;

	return is;
}

std::ostream& operator<<(std::ostream& os, const Ability& ability)
{
	os << ability.mIndex << " " << std::quoted(ability.mName) << " " 
		<< ability.mEffect << " " << ability.mEffectChance << " " << ability.mPhysical << " "
		<< ability.mElementIndex << " " << ability.mPower << " " << ability.mAccuracy << " " 
		<< ability.mTotalUses << " " << ability.mTarget << " " << ability.mPriority;

	return os;
}