#pragma once

#include <string>
#include <iostream>
#include <iomanip>

class Passive
{
public:
	Passive();
	
	enum PassiveEffect : uint16_t
	{
		NO_EFFECT = 0,
		BERSERKER = 0x0001, //User deals bonus damage based on missing health
		VAMPIRIC = 0x0002, //User has 1/8 of damage dealt to opponent returned as health
		VENGEANCE = 0x0004, //Opponents take 1/16 of total health as damage after attack on user
		CONCENTRATION = 0x0008, //Both fighters are guaranteed to land their attacks
		ARMORED = 0x0010, //User cannot be critically striked
		ATTACK_BOOST = 0x0020, //User gains 1 attack stage per turn
		DEFENSE_BOOST = 0x0040, //User gains 1 defense stage per turn
		SPEED_BOOST = 0x0080, //User gains 1 speed stage per turn
		LUCKY = 0x0100, //User gains 1 critical stage at start
		FILTER = 0x0200, //User takes 1/4 reduced damage
		FOCUSED = 0x0400, //User gains 1 accuracy stage at start
		INTIMIDATE = 0x0800, //User reduces opponent's attack stage by 1 on entry
		GUARDED = 0x1000, //User's stats cannot be reduced by opponent
		SURGE = 0x2000, //User has a 25% chance to deal 50% increased damage
	};

private:
	int mIndex;
	std::string mName;

	int mEffect;

	std::string mDescription;

public:
	int Index() const { return mIndex; }
	std::string Name() const { return mName; }
	
	int Effect() const { return mEffect; }

	std::string Description() const { return mDescription; }

	inline bool Check(int flag) { return((mEffect & flag) == flag); }

	friend std::istream& operator>>(std::istream& is, Passive& passive);
	friend std::ostream& operator<<(std::ostream& os, const Passive passive);
};

