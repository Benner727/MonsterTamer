#pragma once

#include <string>
#include <iostream>
#include <iomanip>

class Ability
{
public:
	Ability();

	enum AbilityEffect : uint32_t
	{
		NO_EFFECT = 0,
		SLEEP = 0x0000001,
		POISON = 0x0000002,
		PARALYZE = 0x0000004,
		BURN = 0x0000008,
		FREEZE = 0x0000010,
		FLINCH = 0x0000020,
		CONFUSE = 0x0000040,
		CURE = 0x0000080,
		BOOST_ATTACK = 0x0000100,
		BOOST_DEFENSE = 0x0000200,
		BOOST_SPEED = 0x0000400,
		BOOST_ACCURACY = 0x0000800,
		BOOST_EVASION = 0x0001000,
		BOOST_CRITICAL = 0x0002000,
		DECREASE_ATTACK = 0x0004000,
		DECREASE_DEFENSE = 0x0008000,
		DECREASE_SPEED = 0x0010000,
		DECREASE_ACCURACY = 0x0020000,
		DECREASE_EVASION = 0x0040000,
		DECREASE_CRITICAL = 0x0080000,
		HEAL = 0x0100000,
		MULTI_HIT = 0x0200000,
		CHARGED = 0x0400000,
		RECHARGED = 0x0800000,
		RECOIL = 0x1000000,
		LIFESTEAL = 0x2000000
	};

private:
	int mIndex;
	std::string mName;

	int mEffect;
	int mEffectChance;

	bool mPhysical;

	int mElementIndex;
	int mPower;
	int mAccuracy;
	int mTotalUses;

	int mTarget; // None = 0, Other = 1, Self = 2, Both = 3
	int mPriority; //Range from -6 to +6

public:
	int Index() const { return mIndex; }
	std::string Name() const { return mName; }

	int Effect() const { return mEffect; }
	int EffectChance() const { return mEffectChance; }

	bool Physical() const { return mPhysical; }

	int ElementIndex() const { return mElementIndex; }
	int Power() const { return mPower; }
	int Accuracy() const { return mAccuracy; }
	int TotalUses() const { return mTotalUses; }

	int Target() const { return mTarget; }
	int Priority() const { return mPriority; }

	inline bool Check(int flag) { return((mEffect & flag) == flag); }

	friend std::istream& operator>>(std::istream& is, Ability& ability);
	friend std::ostream& operator<<(std::ostream& os, const Ability& ability);
};

