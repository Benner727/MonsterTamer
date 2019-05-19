#pragma once

#include <string>
#include <iostream>
#include <iomanip>

class Item
{
public:
	Item();

	enum ItemEffect : uint16_t
	{
		NO_EFFECT = 0,
		HEAL = 0x0001,
		BOOST_ATTACK = 0x0002,
		BOOST_DEFENSE = 0x0004,
		BOOST_SPEED = 0x0008,
		BOOST_ACCURACY = 0x0010,
		BOOST_EVASION = 0x0020,
		BOOST_CRITICAL = 0x0040,
		REMOVE_SLEEP = 0x0080, 
		REMOVE_POISON = 0x0100,
		REMOVE_BURN = 0x0200,
		REMOVE_PARALYSIS = 0x0400,
		REMOVE_FROZEN = 0x0800,
		BAIT = 0x1000
	};

private:
	int mIndex;
	std::string mName;

	int mValue;

	int mEffect;
	int mPotency;

public:
	int Index() const { return mIndex; }
	std::string Name() const { return mName; }

	int Value() const { return mValue; }

	int Effect() const { return mEffect; }
	int Potency() const { return mPotency; }

	inline bool Check(int flag) { return((mEffect & flag) == flag); }

	friend std::istream& operator>>(std::istream& is, Item& item);
	friend std::ostream& operator<<(std::ostream& os, const Item& item);
};

