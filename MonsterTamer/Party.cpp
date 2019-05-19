#include "Party.h"

Party::Party()
{
}

void Party::Swap(int slotOne, int slotTwo)
{
	std::iter_swap(mParty.begin() + slotOne, mParty.begin() + slotTwo);
}

void Party::Add(std::weak_ptr<Monster> monster)
{
	mParty.push_back(monster.lock());
}

void Party::Release(int slot)
{
	mParty.erase(mParty.begin() + slot);
}