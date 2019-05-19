#include "Map.h"

#include <random>
#include <fstream>

static std::random_device rand_dev;
static std::mt19937 generator(rand_dev());
static std::uniform_int_distribution<int> partyDist(1, 6);
static std::uniform_int_distribution<int> trainDist(2, 5);

Map::Map()
{
	Init();
}

int Map::HandleInput(int min, int max)
{
	int input;
	do
	{
		std::cout << ">";

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		std::cin >> input;
	} while (std::cin.fail() || (input < min || input > max));

	return input;
}

void Map::Init()
{
	auto file = "save.bin";
	std::ifstream is(file, std::ios::binary);

	if (is.good())
	{
		system("CLS");
		std::cout << "[1] Continue game" << std::endl;
		std::cout << "[2] New Game" << std::endl;
		int input = HandleInput(0, 2);

		if (input == 1)
		{
			cereal::PortableBinaryInputArchive archiveLoad(is);
			archiveLoad(mPlayer);
		}
		else if (input == 2)
		{
			PlayerFactory playerFactory;
			mPlayer = playerFactory.GetPlayer().lock();
		}
		else
		{
			mQuit = true;
			return;
		}
	}
	else
	{
		system("CLS");
		std::cout << "[1] New game" << std::endl;
		int input = HandleInput(0, 1);

		if (input == 1)
		{
			PlayerFactory playerFactory;
			mPlayer = playerFactory.GetPlayer().lock();
		}
		else
		{
			mQuit = true;
			return;
		}
	}

	mCurrentZone = mPlayer->Zone();

	mGuildCompleted = true;
	if (AssetManager::Instance()->GetZone(mCurrentZone).lock()->GuildLeader() > 0)
	{
		if (!mPlayer->GuildDefeated(AssetManager::Instance()->GetZone(mCurrentZone).lock()->GuildLeader()))
			mGuildCompleted = false;
	}

	if (mGuildCompleted)
	{
		for (const auto& zone : AssetManager::Instance()->GetZone(mCurrentZone).lock()->ConnectedZones())
			mPlayer->UnlockZone(zone);
	}
}

void Map::Hunt()
{
	std::vector<int> monsters = AssetManager::Instance()->GetZone(mCurrentZone).lock()->WildMonsters();

	int monsterIndex = rand() % monsters.size();
	monsterIndex = monsters[monsterIndex];

	int minLevel = AssetManager::Instance()->GetZone(mCurrentZone).lock()->LowestLevel();
	int maxLevel = AssetManager::Instance()->GetZone(mCurrentZone).lock()->HighestLevel();

	int monsterLevel = rand() % (maxLevel - minLevel + 1) + minLevel;

	std::shared_ptr<Monster> monster = std::make_shared<Monster>(monsterIndex, monsterLevel, true);
	std::shared_ptr<Tamer> wild = std::make_shared<Tamer>();
	wild->GetParty().lock()->Add(monster);

	Battle battle(mPlayer, wild, false);
	battle.Start();
}

void Map::Challenge()
{
	if (mGuildCompleted)
	{
		std::cout << "There are no other tamers left to challenge here!" << std::endl;
		system("PAUSE>nul");
		return;
	}

	std::vector<std::string> tamerNames = { "Jimmy", "Steve", "Janette", "Robin", "Danielle", "Ian", "Josh", "Kealana", "Shane", "Oscar", "Jake", "Ashley", "Chet", "Randy" };

	std::shared_ptr<Tamer> enemy = std::make_shared<Tamer>("Tamer " + tamerNames[rand() % tamerNames.size()]);

	std::vector<int> monsterIndexes;
	int partySize = partyDist(generator);

	std::vector<int> monsters = AssetManager::Instance()->GetZone(mCurrentZone).lock()->WildMonsters();
	int minLevel = AssetManager::Instance()->GetZone(mCurrentZone).lock()->LowestLevel();
	int maxLevel = AssetManager::Instance()->GetZone(mCurrentZone).lock()->HighestLevel();

	int prizeMoney = 0;

	for (int i = 0; i < partySize; i++)
	{
		int monsterIndex = rand() % monsters.size();
		monsterIndex = monsters[monsterIndex];

		int monsterLevel = maxLevel  + std::round(trainDist(generator)); //Tamers levelling their monsters
		prizeMoney += monsterLevel * 25;

		std::shared_ptr<Monster> monster = std::make_shared<Monster>(monsterIndex, monsterLevel);
		enemy->GetParty().lock()->Add(monster);
	}

	enemy->GetInvetory().lock()->AddCurrency(prizeMoney);

	Battle battle(mPlayer, enemy);
	battle.Start();
}

void Map::Shop()
{
	int input;

	do
	{
		system("CLS");
		std::cout << "Shop:" << std::endl;

		for (int i = 1; i <= AssetManager::Instance()->TotalItems(); ++i)
		{
			std::cout << "[" << i << "] " << AssetManager::Instance()->GetItem(i).lock()->Name() << "   ";
			std::cout << AssetManager::Instance()->GetItem(i).lock()->Value() << std::endl;
		}

		std::cout << "Currency: " << mPlayer->GetInvetory().lock()->Currency() << std::endl;

		input = HandleInput(0, AssetManager::Instance()->TotalItems());

		if (input > 0)
		{
			std::cout << "How many would you like to buy? (1-99)" << std::endl;
			int amount = HandleInput(0, 99);

			if (amount > 0)
			{
				if (mPlayer->GetInvetory().lock()->CanAfford(input, amount))
				{
					if (mPlayer->GetInvetory().lock()->HasSpace(input, amount))
					{
						std::cout << "You purchase " << AssetManager::Instance()->GetItem(input).lock()->Name() << " x" << amount << "." << std::endl;
						mPlayer->GetInvetory().lock()->Purchase(input, amount);
					}
					else
						std::cout << "You have no space for that!" << std::endl;
				}
				else
					std::cout << "You can't afford that!" << std::endl;

				system("PAUSE>nul");
			}
		}
	} while (input != 0);
}

void Map::Rest()
{
	int cost = 0;
	int highestLevel = 0;

	for (int i = 0; i < mPlayer->GetParty().lock()->GetParty().size(); ++i)
	{
		if (mPlayer->GetParty().lock()->GetParty()[i]->Level() > highestLevel)
			highestLevel = mPlayer->GetParty().lock()->GetParty()[i]->Level();

		if (mPlayer->GetParty().lock()->GetParty()[i]->Health() < mPlayer->GetParty().lock()->GetParty()[i]->TotalHealth()
			|| mPlayer->GetParty().lock()->GetParty()[i]->Status() != Monster::STATUS::HEALTHY)
			cost += 5 * mPlayer->GetParty().lock()->GetParty()[i]->Level();
	}

	cost += 15 * highestLevel;

	system("CLS");
	std::cout << "Would you like us to get your monsters battle ready?" << std::endl;
	std::cout << "It will only cost you: " << cost << "." << std::endl;
	std::cout << "Currency: " << mPlayer->GetInvetory().lock()->Currency() << std::endl;
	std::cout << "[1] Yes" << std::endl;
	std::cout << "[2] No" << std::endl;

	int input = HandleInput(0, 2);

	if (input == 1)
	{
		if (mPlayer->GetInvetory().lock()->Currency() >= cost)
		{
			mPlayer->GetInvetory().lock()->LoseCurrency(cost);

			for (auto& monster : mPlayer->GetParty().lock()->GetParty())
				monster->Rest();

			std::cout << "Your monsters are now rested!" << std::endl;
			system("PAUSE>nul");
		}
		else
		{
			std::cout << "You don't have the currency!" << std::endl;
			system("PAUSE>nul");
		}
	}
}

void Map::Bestiary()
{
	system("CLS");
	std::cout << "Bestiary: You have discovered " << mPlayer->MonstersSeen().size() << " monsters!" << std::endl;
	for (const auto& index : mPlayer->MonstersSeen())
	{
		std::weak_ptr<Species> species = AssetManager::Instance()->GetSpecies(index);
		std::cout << " [" << index << "] " << species.lock()->Name() << std::endl;
		
		std::string primaryElement = AssetManager::Instance()->GetElement(species.lock()->PrimaryElement()).lock()->Name();
		std::string secondaryElement = AssetManager::Instance()->GetElement(species.lock()->SecondaryElement()).lock()->Name();
		
		std::cout << "    " << primaryElement;
		if (secondaryElement != "None")
			std::cout << "/" << secondaryElement;
		std::cout << std::endl;
	}
	system("PAUSE>nul");
}

void Map::Party()
{
	int input;
	do
	{
		system("CLS");
		std::cout << "Party:" << std::endl;

		for (int i = 0; i < mPlayer->GetParty().lock()->GetParty().size(); ++i)
		{
			std::cout << "[" << i + 1 << "] " << mPlayer->GetParty().lock()->GetParty()[i]->Name() << "   Lv: " << mPlayer->GetParty().lock()->GetParty()[i]->Level() << std::endl;
			std::cout << "   Health: " << mPlayer->GetParty().lock()->GetParty()[i]->Health() << " / " << mPlayer->GetParty().lock()->GetParty()[i]->TotalHealth();
			switch (mPlayer->GetParty().lock()->GetParty()[i]->Status())
			{
			case 0:
				std::cout << "" << std::endl;
				break;
			case 1:
				std::cout << "  <SLP>" << std::endl;
				break;
			case 2:
				std::cout << "  <PSN>" << std::endl;
				break;
			case 3:
				std::cout << "  <BRN>" << std::endl;
				break;
			case 4:
				std::cout << "  <PAR>" << std::endl;
				break;
			case 5:
				std::cout << "  <FRZ>" << std::endl;
				break;
			}
			std::cout << "   Exp: " << mPlayer->GetParty().lock()->GetParty()[i]->Experience() << " / " <<  std::to_string((int)(pow(mPlayer->GetParty().lock()->GetParty()[i]->Level() + 1.0f, 3.0f) * 0.8f)) << std::endl;
		}

		input = HandleInput(0, mPlayer->GetParty().lock()->GetParty().size());

		if (input > 0)
		{
			//Allow for swap, release, inspect
			std::cout << "[1] Inspect" << std::endl;
			std::cout << "[2] Swap" << std::endl;
			std::cout << "[3] Release" << std::endl;

			int action = HandleInput(0, 3);
			if (action > 0)
			{
				if (action == 1)
				{
					system("CLS");
					std::cout << *mPlayer->GetParty().lock()->GetParty()[input - 1] << std::endl;
					system("PAUSE>nul");
				}
				else if (action == 2)
				{
					int slot = HandleInput(0, mPlayer->GetParty().lock()->GetParty().size());
					if (slot > 0)
						mPlayer->GetParty().lock()->Swap(input - 1, slot - 1);
				}
				else if (action == 3)
				{
					std::cout << "Are you sure you want to release your level " << mPlayer->GetParty().lock()->GetParty()[input - 1]->Level() << " " << mPlayer->GetParty().lock()->GetParty()[input - 1]->Name() << "?" << std::endl;
					std::cout << "[1] Yes" << std::endl;
					std::cout << "[0] No" << std::endl;
					int response = HandleInput(0, 1);
					if (response == 1)
						mPlayer->GetParty().lock()->Release(input - 1);
				}
			}
		}
	} while (input != 0);
}

void Map::Guild()
{
	int guild = AssetManager::Instance()->GetZone(mCurrentZone).lock()->GuildLeader();

	std::shared_ptr<Tamer> enemy = std::make_shared<Tamer>(AssetManager::Instance()->GetGuild(guild).lock()->Name());
	enemy->GetInvetory().lock()->AddCurrency(5000 * guild);

	for (const auto& slot : AssetManager::Instance()->GetGuild(guild).lock()->Party())
	{
		Monster monster(slot.first, slot.second);
		enemy->GetParty().lock()->Add(std::make_shared<Monster>(monster));
	}

	Battle battle(mPlayer, enemy);
	battle.Start();

	if (!mPlayer->GetParty().lock()->Empty())
	{
		mGuildCompleted = true;
		mPlayer->DefeatGuild(AssetManager::Instance()->GetZone(mCurrentZone).lock()->GuildLeader());

		for (const auto& zone : AssetManager::Instance()->GetZone(mCurrentZone).lock()->ConnectedZones())
			mPlayer->UnlockZone(zone);

		system("CLS");
		std::cout << "You are now a member of the local guild!" << std::endl;
		system("PAUSE>nul");
	}
}

void Map::Travel()
{
	int input;
	do
	{
		system("CLS");

		std::weak_ptr<Zone> zone = AssetManager::Instance()->GetZone(mCurrentZone);

		for (int i = 0; i < zone.lock()->ConnectedZones().size(); ++i)
		{
			std::cout << "[" << i + 1 << "] " << AssetManager::Instance()->GetZone(zone.lock()->ConnectedZones()[i]).lock()->Name();
			if (!mPlayer->ZoneUnlocked(zone.lock()->ConnectedZones()[i])) std::cout << " [LOCKED]";
			std::cout << std::endl;
		}

		input = HandleInput(0, zone.lock()->ConnectedZones().size());

		if (input > 0)
		{
			if (mPlayer->ZoneUnlocked(zone.lock()->ConnectedZones()[input - 1]))
			{
				mCurrentZone = zone.lock()->ConnectedZones()[input - 1];
				mPlayer->Zone(mCurrentZone);
				std::cout << "You travel to " << AssetManager::Instance()->GetZone(zone.lock()->ConnectedZones()[input - 1]).lock()->Name() << "..." << std::endl;

				mGuildCompleted = true;
				if (AssetManager::Instance()->GetZone(mCurrentZone).lock()->GuildLeader() > 0)
				{
					if (!mPlayer->GuildDefeated(AssetManager::Instance()->GetZone(mCurrentZone).lock()->GuildLeader()))
						mGuildCompleted = false;
				}

				if (mGuildCompleted)
				{
					for (const auto& zone : AssetManager::Instance()->GetZone(mCurrentZone).lock()->ConnectedZones())
						mPlayer->UnlockZone(zone);
				}
			}
			else
				std::cout << "You must become a member of the local guild first." << std::endl;

			system("PAUSE>nul");
		}
	} while (input != 0);
}

void Map::Save()
{
	auto file = "save.bin";
	std::ofstream os(file, std::ios::binary);
	cereal::PortableBinaryOutputArchive archiveSave(os);
	archiveSave(mPlayer);
}

void Map::Quit()
{
	Save();
	mQuit = true;
}

void Map::Interface()
{
	system("CLS");

	std::cout << "Welcome to " << AssetManager::Instance()->GetZone(mCurrentZone).lock()->Name() << "!" << std::endl;
	std::cout << "[1] Hunt for wild monsters" << std::endl; //Return a monster based off of an index in current zone and within level range
	std::cout << "[2] Challenge other tamers" << std::endl; //Return a tamer based off of an index in the current zone
	std::cout << "[3] Visit Shop" << std::endl; //Display standard shop
	std::cout << "[4] Rest up" << std::endl; //Heal all monsters and remove status effects
	std::cout << "[5] View Bestiary" << std::endl; //Show the name and elements (possibly base stats?) of seen monsters
	std::cout << "[6] Open Party" << std::endl; //Allow play to swap monsters around or release them (Considering adding monster storage?)
	std::cout << "[7] Travel" << std::endl;
	if (!mGuildCompleted) std::cout << "[8] Enter Guild" << std::endl; //If defeated, unlocks nearby routes
	std::cout << "[0] Save and Quit" << std::endl; //Save game to file and exit program
}

void Map::Show()
{
	while (!mQuit)
	{
		Interface();
		int input = HandleInput(0, 8 - mGuildCompleted);

		switch (input)
		{
		case 0:
			Quit();
			break;
		case 1:
			Hunt();
			Save();
			break;
		case 2:
			Challenge();
			Save();
			break;
		case 3:
			Shop();
			break;
		case 4:
			Rest();
			break;
		case 5:
			Bestiary();
			break;
		case 6:
			Party();
			break;
		case 7:
			Travel();
			break;
		case 8:
			Guild();
			Save();
			break;
		}

		if (mPlayer->GetParty().lock()->Empty())
		{
			auto file = "save.bin";
			std::remove(file);

			system("CLS");
			std::cout << *mPlayer << std::endl;
			system("PAUSE>nul");

			Init();
		}
	}
}
