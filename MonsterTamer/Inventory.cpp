#include "Inventory.h"

Inventory::Inventory(int currency)
	: mCurrency(currency)
{
}

bool Inventory::CanAfford(int index, int amount) const
{
	int value = AssetManager::Instance()->GetItem(index).lock()->Value();
	value *= amount;

	return (value <= mCurrency);
}

bool Inventory::HasSpace(int index, int amount) const
{
	for (auto& slot : mInventory)
	{
		if (slot.first == index)
		{
			if (slot.second > 98)
				return false;
			break;
		}
	}

	return true;
}

void Inventory::Purchase(int index, int amount)
{
	int value = AssetManager::Instance()->GetItem(index).lock()->Value();
	mCurrency -= value * amount;

	for (auto& slot : mInventory)
	{
		if (slot.first == index)
		{
			slot.second += amount;
			return;
		}
	}

	mInventory.push_back(std::make_pair(index, amount));
}

void Inventory::Sell(int slot, int amount)
{
	if (amount > mInventory[slot].second)
		amount = mInventory[slot].second;

	int value = AssetManager::Instance()->GetItem(mInventory[slot].first).lock()->Value();
	value *= amount;

	mCurrency += value;

	mInventory[slot].second -= amount;
	if (mInventory[slot].second == 0)
		mInventory.erase(mInventory.begin() + slot);
}

std::weak_ptr<Item> Inventory::GetItem(int slot) const
{
	return AssetManager::Instance()->GetItem(mInventory[slot].first);
}

void Inventory::UseItem(int slot)
{
	if (--mInventory[slot].second < 1)
		mInventory.erase(mInventory.begin() + slot);
}

void Inventory::TossItem(int slot)
{
	mInventory.erase(mInventory.begin() + slot);
}
