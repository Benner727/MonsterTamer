#include "AssetManager.h"

#include <fstream>

AssetManager* AssetManager::sInstance = nullptr;

AssetManager* AssetManager::Instance()
{
	if (sInstance == nullptr)
		sInstance = new AssetManager();

	return sInstance;
}

void AssetManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

AssetManager::AssetManager()
{
	if (!LoadElements()) std::cerr << "Unable to load elements!" << std::endl;
	if (!LoadAbilities()) std::cerr << "Unable to load abilities!" << std::endl;
	if (!LoadPassives()) std::cerr << "Unable to load passives!" << std::endl;
	if (!LoadSpecies()) std::cerr << "Unable to load species!" << std::endl;

	if (!LoadItems()) std::cerr << "Unable to load items!" << std::endl;

	if (!LoadZones()) std::cerr << "Unable to load zones!" << std::endl;
	if (!LoadGuilds()) std::cerr << "Unable to load guilds!" << std::endl;
}

bool AssetManager::LoadElements()
{
	std::ifstream infile("Elements.txt");
	if (!infile) std::cerr << "Elements.txt could not be opened!" << std::endl;

	Element element;
	while (infile >> element)
		mElements.push_back(std::make_shared<Element>(element));

	return mElements.size();
}

bool AssetManager::LoadAbilities()
{
	std::ifstream infile("Abilities.txt");
	if (!infile) std::cerr << "Abilities.txt could not be opened!" << std::endl;

	Ability ability;
	while (infile >> ability)
		mAbilities.push_back(std::make_shared<Ability>(ability));

	return mAbilities.size();
}

bool AssetManager::LoadPassives()
{
	std::ifstream infile("Passives.txt");
	if (!infile) std::cerr << "Passives.txt could not be opened!" << std::endl;

	Passive passive;
	while (infile >> passive)
		mPassives.push_back(std::make_shared<Passive>(passive));

	return mPassives.size();
}

bool AssetManager::LoadSpecies()
{
	std::ifstream infile("Species.txt");
	if (!infile) std::cerr << "Species.txt could not be opened!" << std::endl;

	Species species;
	while (infile >> species)
		mSpecies.push_back(std::make_shared<Species>(species));

	return mSpecies.size();
}

bool AssetManager::LoadItems()
{
	std::ifstream infile("Items.txt");
	if (!infile) std::cerr << "Items.txt could not be opened!" << std::endl;

	Item item;
	while (infile >> item)
		mItems.push_back(std::make_shared<Item>(item));

	return mItems.size();
}

bool AssetManager::LoadZones()
{
	std::ifstream infile("Zones.txt");
	if (!infile) std::cerr << "Zones.txt could not be opened!" << std::endl;

	Zone zone;
	while (infile >> zone)
		mZones.push_back(std::make_shared<Zone>(zone));

	return mZones.size();
}

bool AssetManager::LoadGuilds()
{
	std::ifstream infile("Guilds.txt");
	if (!infile) std::cerr << "Guilds.txt could not be opened!" << std::endl;

	Guild guild;
	while (infile >> guild)
		mGuilds.push_back(std::make_shared<Guild>(guild));

	return mGuilds.size();
}

std::weak_ptr<Element> AssetManager::GetElement(int index) const
{
	for (auto& element : mElements)
	{
		if (element->Index() == index)
			return element;
	}

	return std::weak_ptr<Element>();
}

std::weak_ptr<Ability> AssetManager::GetAbility(int index) const
{
	for (auto& ability : mAbilities)
	{
		if (ability->Index() == index)
			return ability;
	}

	return std::weak_ptr<Ability>();
}

std::weak_ptr<Passive> AssetManager::GetPassive(int index) const
{
	for (auto& passive : mPassives)
	{
		if (passive->Index() == index)
			return passive;
	}

	return std::weak_ptr<Passive>();
}

std::weak_ptr<Species> AssetManager::GetSpecies(int index) const
{
	for (auto& species : mSpecies)
	{
		if (species->Index() == index)
			return species;
	}

	return std::weak_ptr<Species>();
}

std::weak_ptr<Item> AssetManager::GetItem(int index) const
{
	for (auto& item : mItems)
	{
		if (item->Index() == index)
			return item;
	}

	return std::weak_ptr<Item>();
}

std::weak_ptr<Zone> AssetManager::GetZone(int index) const
{
	for (auto& zone : mZones)
	{
		if (zone->Index() == index)
			return zone;
	}

	return std::weak_ptr<Zone>();
}

std::weak_ptr<Guild> AssetManager::GetGuild(int index) const
{
	for (auto& guild : mGuilds)
	{
		if (guild->Index() == index)
			return guild;
	}

	return std::weak_ptr<Guild>();
}
