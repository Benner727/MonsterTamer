#pragma once

#include "Tamer.h"
#include "Fighter.h"

class Combatant
{
public:
	Combatant(std::weak_ptr<Tamer> tamer);

private:
	std::weak_ptr<Tamer> mTamer;

	std::vector<std::shared_ptr<Fighter>> mParty;

public:
	std::string Name() const;
	std::weak_ptr<Inventory> Inventory() const;
	
	const std::vector<std::shared_ptr<Fighter>> Party();

	std::weak_ptr<Fighter> Front() const { return mParty.front(); }
	void RemoveFront();
	void SwapFront(int slot);

	std::weak_ptr<Fighter> GetSlot(int slot) const { return mParty[slot]; }
	
	bool Full() const { return mTamer.lock()->GetParty().lock()->Full(); }
	bool Empty() const { return mTamer.lock()->GetParty().lock()->Empty(); }
	bool LastMonster() const { return (mParty.size() < 2); }

	void AddMonster(std::weak_ptr<Monster> monster);
	void DiscoverMonster(int index) { mTamer.lock()->AddMonsterSeen(index); }

	void AddMonstersTamed() { mTamer.lock()->AddMonstersTamed(); }
	void AddMonstersKilled() { mTamer.lock()->AddMonstersKilled(); }
	void AddTamersBeaten() { mTamer.lock()->AddTamersBeaten(); }

	std::weak_ptr<Monster> FrontMonster() const { return mTamer.lock()->GetParty().lock()->Front(); }
};