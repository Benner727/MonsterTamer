#include "PlayerFactory.h"

#include "AssetManager.h"

PlayerFactory::PlayerFactory()
{
}

void PlayerFactory::ShowDialogue()
{
	while (!mDialogue.empty())
	{
		system("CLS");
		std::cout << mDialogue.front() << std::endl;
		mDialogue.pop();
		std::cout << ">";
		system("PAUSE>nul");
	}
}

std::string PlayerFactory::GetName()
{
	system("CLS");
	std::cout << "Please tell me what your name is." << std::endl;
	std::cout << ">";

	std::string name = "";

	std::getline(std::cin, name);

	if (name.size() < 1 || name.size() > 12)
		return GetName();

	return name;
}

std::string PlayerFactory::GetNickname(int species)
{
	system("CLS");
	std::cout << "The " + AssetManager::Instance()->GetSpecies(species).lock()->Name() + ", great choice!\nWould you like to give it a nickname?" << std::endl;
	std::cout << ">";

	std::string nickname = "";

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(std::cin, nickname);

	if (nickname.size() > 12)
		return GetNickname(species);

	return nickname;
}

int PlayerFactory::ChooseMonster()
{
	int input;

	do 
	{
		system("CLS");
		std::cout << "I have three choices for you,\nI will let you decide which one to take:" << std::endl << std::endl;

		for (int i = 1; i < 4; i++)
		{
			std::cout << "[" << i << "] " << AssetManager::Instance()->GetSpecies(i).lock()->Name();
			std::cout << " (" << AssetManager::Instance()->GetElement(AssetManager::Instance()->GetSpecies(i).lock()->PrimaryElement()).lock()->Name() << ")" << std::endl;
		}

		std::cout << ">";
		std::cin >> input;
	} while (input < 1 || input > 3);

	return input;
}

void PlayerFactory::CreatePlayer(std::string name, int species, std::string nickname)
{
	std::shared_ptr<Monster> playerMonster = std::make_shared<Monster>(species, 3);
	if (nickname.size()) playerMonster->Rename(nickname);

	mPlayer = std::make_shared<Tamer>();
	mPlayer->Rename(name);
	mPlayer->GetParty().lock()->Add(playerMonster);
	mPlayer->AddMonsterSeen(species);
	mPlayer->GetInvetory().lock()->AddCurrency(2500);
	mPlayer->GetInvetory().lock()->Purchase(1, 5);
	mPlayer->UnlockZone(1);
}

void PlayerFactory::PlayIntroduction()
{
	mDialogue.push("Welcome to the world of Monster Tamer!\nMy name is Reginald.");
	mDialogue.push("Before we go any further,\nI would like to tell you a few things\nyou should know about this world...");
	mDialogue.push("Dangerous monsters roam around this world. But\npeople have learned how to tame these monsters.");
	mDialogue.push("With this knowledge, tamers have begun collecting\nwild monsters and battling each other.");
	mDialogue.push("It can be a lot of fun, but it is not a sport \nfor the meek.");
	mDialogue.push("The monsters are killed during these battles, and the \nlosing tamer is often sacrificed too!");
	mDialogue.push("I know...");
	mDialogue.push("It sounds pretty fun, doesn't it?");
	mDialogue.push("I'll stop boring you with these tales now \nand let you start on your own adventure!");
	ShowDialogue();

	std::string name = GetName();

	mDialogue.push(name + "! I hope you are ready\nfor what is about to happen.");
	mDialogue.push("Monsters will die, other tamers are going die...\nHeck, even you might die!");
	mDialogue.push("Just remember to have fun while all this is going on.");
	mDialogue.push("Let's enter the world of Monster Tamer!");
	mDialogue.push("I'll talk to you again soon.");
	mDialogue.push("...");
	mDialogue.push("...\n...");
	mDialogue.push("...\n...\n...");
	mDialogue.push("Oh wait! I better give you a monster of your own.");
	mDialogue.push("Without one you would have probably died immediately!");
	ShowDialogue();

	int species = ChooseMonster();
	std::string nickname = GetNickname(species);

	CreatePlayer(name, species, nickname);
}

std::weak_ptr<Tamer> PlayerFactory::GetPlayer()
{
	//Start a new game
	if (true)
	{
		PlayIntroduction();

		system("CLS");
		std::cout << "Hopefully I didn't forget anything else.\nJust get out there and kill something!" << std::endl;
		system("PAUSE>nul");
	}

	return mPlayer;
}
