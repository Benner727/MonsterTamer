#pragma once

#include "Element.h"
#include "Ability.h"
#include "Passive.h"
#include "Species.h"
#include "Item.h"
#include "Zone.h"
#include "Guild.h"

class AssetManager
{
private:
	static AssetManager* sInstance;
	
	std::vector<std::shared_ptr<Element>> mElements;
	std::vector<std::shared_ptr<Ability>> mAbilities;
	std::vector<std::shared_ptr<Passive>> mPassives;
	std::vector<std::shared_ptr<Species>> mSpecies;

	std::vector<std::shared_ptr<Item>> mItems;

	std::vector<std::shared_ptr<Zone>> mZones;
	std::vector<std::shared_ptr<Guild>> mGuilds;

	bool LoadElements();
	bool LoadAbilities();
	bool LoadPassives();
	bool LoadSpecies();
	
	bool LoadItems();

	bool LoadZones();
	bool LoadGuilds();

	AssetManager();

public:
	static AssetManager* Instance();
	static void Release();

	std::weak_ptr<Element> GetElement(int index) const;
	std::weak_ptr<Ability> GetAbility(int index) const;
	std::weak_ptr<Passive> GetPassive(int index) const;
	std::weak_ptr<Species> GetSpecies(int index) const;

	std::weak_ptr<Item> GetItem(int index) const;
	int TotalItems() const { return mItems.size(); }

	std::weak_ptr<Zone> GetZone(int index) const;
	int TotalZones() const { return mZones.size(); }

	std::weak_ptr<Guild> GetGuild(int index) const;
};

