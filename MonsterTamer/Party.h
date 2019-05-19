#pragma once

#include <cereal/types/memory.hpp>

#include "Monster.h"

class Party
{
public:
	Party();
	
private:
	std::vector<std::shared_ptr<Monster>> mParty;

public:
	void Swap(int slotOne, int slotTwo);
	
	bool Full() const { return mParty.size() > 5; }
	bool Empty() const { return mParty.empty(); }

	void Add(std::weak_ptr<Monster> monster);
	void Release(int slot);

	std::weak_ptr<Monster> Front() const { return mParty.front(); }

	std::vector<std::shared_ptr<Monster>>& GetParty() { return mParty; }

	template<class Archive>
	void save(Archive & archive) const
	{
		archive(mParty);
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(mParty);
	}
};

