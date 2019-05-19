#include "Combatant.h"

Combatant::Combatant(std::weak_ptr<Tamer> tamer)
	: mTamer(tamer)
{
	for (const auto& monster : mTamer.lock()->GetParty().lock()->GetParty())
		mParty.push_back(std::make_shared<Fighter>(monster));
}

std::string Combatant::Name() const
{
	return mTamer.lock()->Name();
}

std::weak_ptr<Inventory> Combatant::Inventory() const
{
	return mTamer.lock()->GetInvetory();
}

const std::vector<std::shared_ptr<Fighter>> Combatant::Party()
{
	return mParty;
}

void Combatant::RemoveFront()
{
	mParty.erase(mParty.begin());
	mTamer.lock()->GetParty().lock()->Release(0);
}

void Combatant::SwapFront(int slot)
{
	mParty[slot]->ChangeState(Fighter::STATE::IDLE);
	mParty[slot]->NewEntry(true);

	std::iter_swap(mParty.begin(), mParty.begin() + slot);
	mTamer.lock()->GetParty().lock()->Swap(0, slot);
}

void Combatant::AddMonster(std::weak_ptr<Monster> monster)
{
	mTamer.lock()->GetParty().lock()->Add(monster);
}
