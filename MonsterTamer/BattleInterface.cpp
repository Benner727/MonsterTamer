#include "BattleInterface.h"

BattleInterface::BattleInterface(std::weak_ptr<Combatant> player, std::weak_ptr<Combatant> enemy, bool trainer)
	: mPlayer(player), mEnemy(enemy), mTrainer(trainer)
{
	mScene = INTRO_SCENE;
}

void BattleInterface::MonsterDisplay()
{
	if (mTrainer)
	{
		std::cout << mEnemy->Name() <<  " | ";
		for (int i = 0; i < mEnemy->Party().size(); ++i)
			std::cout << "*";
		std::cout << std::endl;
	}
	std::cout << mEnemy->Front().lock()->Name() << "   Lv: " << mEnemy->Front().lock()->Level() << std::endl;
	std::cout << "Hp: " << mEnemy->Front().lock()->Health() << " / " << mEnemy->Front().lock()->TotalHealth();
	switch (mEnemy->Front().lock()->Status())
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
	std::cout << std::endl;
	std::cout << mPlayer->Front().lock()->Name() << "   Lv: " << mPlayer->Front().lock()->Level() << std::endl;
	std::cout << "Hp: " << mPlayer->Front().lock()->Health() << " / " << mPlayer->Front().lock()->TotalHealth();
	switch (mPlayer->Front().lock()->Status())
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
}

void BattleInterface::Intro()
{
	if (mTrainer)
		std::cout << mPlayer->Name() << " challenges " << mEnemy->Name() << " to a battle!" << std::endl;
	else
	{
		if (mEnemy->Party().size() > 1)
			std::cout << "A pack of " << "wild " << mEnemy->Front().lock()->Name() << "s appear!" << std::endl;
		else
			std::cout << "A wild " << mEnemy->Front().lock()->Name() << " appears!" << std::endl;
	}
}

void BattleInterface::Attack()
{
	MonsterDisplay();

	std::cout << std::endl;

	for (int i = 0; i < mPlayer->Front().lock()->Abilities().size(); ++i)
	{
		std::cout << "[" << i + 1 << "] " << mPlayer->Front().lock()->Abilities()[i].lock()->Name() << std::endl;
		std::cout << "   " << AssetManager::Instance()->GetElement(mPlayer->Front().lock()->Abilities()[i].lock()->ElementIndex()).lock()->Name() << " ";
		std::cout << mPlayer->Front().lock()->AbilityUses(i) << "/" << mPlayer->Front().lock()->Abilities()[i].lock()->TotalUses() << std::endl;
	}
}

void BattleInterface::Inventory()
{
	std::cout << "Inventory" << std::endl;

	for (int i = 0; i < mPlayer->Inventory().lock()->GetInventory().size(); ++i)
	{
		std::cout << "[" << i + 1 << "] " << mPlayer->Inventory().lock()->GetItem(i).lock()->Name() << "   x";
		std::cout << mPlayer->Inventory().lock()->GetInventory()[i].second << std::endl;
	}
}

void BattleInterface::Party()
{
	std::cout << "Party" << std::endl;

	for (int i = 0; i < mPlayer->Party().size(); ++i)
	{
		std::cout << "[" << i + 1 << "] " << mPlayer->Party()[i]->Name() << "   Lv: " << mPlayer->Party()[i]->Level() << std::endl;
		std::cout << "   Health: " << mPlayer->Party()[i]->Health() << " / " << mPlayer->Party()[i]->TotalHealth();
		switch (mPlayer->Party()[i]->Status())
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
	}
}

void BattleInterface::Menu()
{
	MonsterDisplay();
	
	std::cout << std::endl;

	std::cout << "[1] Attack" << std::endl;
	std::cout << "[2] Inventory" << std::endl;
	std::cout << "[3] Party" << std::endl;
	std::cout << "[4] Escape" << std::endl;
}

void BattleInterface::Dialogue()
{
	MonsterDisplay();

	std::cout << std::endl;

	while (!mDialogue.empty())
	{
		std::cout << mDialogue.front() << std::endl;
		mDialogue.pop();
	}
}

void BattleInterface::AddDialogue(std::string dialogue)
{
	mDialogue.push(dialogue);
}

void BattleInterface::ClearDialogue()
{
	while (!mDialogue.empty())
		mDialogue.pop();
}

void BattleInterface::Show()
{
	system("CLS");

	switch (mScene)
	{
	case INTRO_SCENE:
		Intro();
		break;
	case MENU_SCENE:
		Menu();
		break;
	case ATTACK_SCENE:
		Attack();
		break;
	case INVENTORY_SCENE:
		Inventory();
		break;
	case PARTY_SCENE:
		Party();
		break;
	case DIALOGUE_SCENE:
		Dialogue();
		break;
	}
}
