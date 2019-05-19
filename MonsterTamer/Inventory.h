#pragma once

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>

#include "AssetManager.h"

class Inventory
{
public:
	Inventory(int currency = 0);
	
private:
	int mCurrency;
	std::vector<std::pair<int, int>> mInventory;

public:
	int Currency() const { return mCurrency; }
	void AddCurrency(int currency) { mCurrency += currency; }
	void LoseCurrency(int currency) { mCurrency -= currency; }

	bool CanAfford(int index, int amount = 1) const;
	bool HasSpace(int index, int amount = 1) const;
	void Purchase(int index, int amount = 1);
	void Sell(int slot, int amount = 1);

	std::vector<std::pair<int, int>> GetInventory() const { return mInventory; }

	std::weak_ptr<Item> GetItem(int slot) const;
	void UseItem(int slot);
	void TossItem(int slot);

	template<class Archive>
	void save(Archive & archive) const
	{
		archive(mCurrency, mInventory);
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(mCurrency, mInventory);
	}
};