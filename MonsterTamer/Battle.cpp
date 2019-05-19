#include "Battle.h"

Battle::Battle(std::weak_ptr<Tamer> player, std::weak_ptr<Tamer> enemy, bool trainerBattle)
	: mTrainerBattle(trainerBattle)
{
	mPlayer = std::make_shared<Combatant>(player);
	mEnemy = std::make_shared<Combatant>(enemy);

	mInterface = std::make_unique<BattleInterface>(mPlayer, mEnemy, mTrainerBattle);

	mState = FIRST_TURN;
	mPlayerAction = NONE;
	mPlayerInput = 0;

	mFighting = true;
	mEscapeAttempts = 0;
	mEscaped = false;
}

int Battle::Input(int min, int max)
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

void Battle::HandleIntro()
{
	mInterface->Show();
	std::cout << ">";
	system("PAUSE>nul");
}

void Battle::HandleMenu()
{
	do 
	{
		if (mPlayerAction == ATTACK)
		{
			mPlayerAction = NONE;
			HandleAttack();
		}
		else
		{
			mPlayerAction = NONE;
			mInterface->Scene(BattleInterface::MENU_SCENE);
			mInterface->Show();

			switch (Input(1, 4))
			{
			case 1:
				HandleAttack();
				break;
			case 2:
				HandleInventory();
				break;
			case 3:
				HandleParty();
				break;
			case 4:
				HandleEscape();
				break;
			}
		}
	} while (mPlayerAction == NONE);
}

void Battle::HandleAttack()
{
	mInterface->Scene(BattleInterface::ATTACK_SCENE);
	mInterface->Show();

	mPlayerInput = Input(0, mPlayer->Front().lock()->Abilities().size());
	if (mPlayerInput == 0)
		mInterface->Scene(BattleInterface::MENU_SCENE);
	else
	{
		//Handle attack
		mPlayerAction = ATTACK;
		mEscapeAttempts = 0;
	}
}

void Battle::HandleInventory()
{
	mInterface->Scene(BattleInterface::INVENTORY_SCENE);
	mInterface->Show();

	mPlayerInput = Input(0, mPlayer->Inventory().lock()->GetInventory().size());
	if (mPlayerInput == 0 || mPlayer->Inventory().lock()->GetInventory().size() < 1)
		mInterface->Scene(BattleInterface::MENU_SCENE);
	else
	{
		//Handle item use
		mPlayerAction = USE_ITEM;
	}
}

void Battle::HandleParty()
{
	mInterface->Scene(BattleInterface::PARTY_SCENE);

	do
	{
		mInterface->Show();
		mPlayerInput = Input(0, mPlayer->Party().size());

		if (mPlayerInput > 1)
		{
			mPlayer->SwapFront(mPlayerInput - 1);
			CheckFighters();
			mPlayerAction = SWAP_FRONT;
			return;
		}
	} while (mPlayerInput != 0);
}

void Battle::HandleEscape()
{
	//Handle escape attempt
	if (mTrainerBattle)
	{
		mInterface->AddDialogue("You can't run from this challenge...");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");
	}
	else
		mPlayerAction = ESCAPE_ATTEMPT;
}

bool Battle::PlayerFirst()
{
	if (mPlayer->Front().lock()->State() == Fighter::IDLE)
	{
		if (mPlayer->Front().lock()->Abilities()[mPlayerInput - 1].lock()->Priority() > mEnemy->Front().lock()->Abilities()[mEnemyInput - 1].lock()->Priority())
			return true;
	}
	if (mEnemy->Front().lock()->State() == Fighter::IDLE)
	{
		if (mEnemy->Front().lock()->Abilities()[mEnemyInput - 1].lock()->Priority() > mPlayer->Front().lock()->Abilities()[mPlayerInput - 1].lock()->Priority())
			return false;
	}

	if ((float)mPlayer->Front().lock()->Speed() / mPlayer->Front().lock()->Level() > (float)mEnemy->Front().lock()->Speed() / mEnemy->Front().lock()->Level())
		return true;
	if ((float)mPlayer->Front().lock()->Speed() / mPlayer->Front().lock()->Level() == (float)mEnemy->Front().lock()->Speed() / mEnemy->Front().lock()->Level() && (rand() % 2))
		return true;

	return false;
}

void Battle::CheckFighters()
{
	if (mPlayer->Party().size() > 0)
	{
		if (mParticipants.size() < 1)
			mParticipants.push_back(mPlayer->Front());
		else
		{
			for (int i = 0; i < mParticipants.size(); i++)
			{
				if (mParticipants[i].lock() == mPlayer->Front().lock())
					break;

				if (i == mParticipants.size() - 1)
					mParticipants.push_back(mPlayer->Front());
			}
		}
	}
}

void Battle::Start()
{
	HandleIntro();

	//Add active fighter at start of battle
	CheckFighters();

	while (mFighting)
	{
		mPlayer->DiscoverMonster(mEnemy->FrontMonster().lock()->SpeciesIndex());

		if (mState == FIRST_TURN)
		{
			//Handle passives before turns begin
			ProcessEntryPassives(mPlayer->Front(), mEnemy->Front());
			ProcessEntryPassives(mEnemy->Front(), mPlayer->Front());
			HandleMenu();
		}
		else
		{
			if (mPlayer->Front().lock()->State() == Fighter::IDLE || mPlayer->Front().lock()->State() == Fighter::DISABLED)
				mState = PLAYER_TURN;
			else
				mState = SKIP_TURN;
		}
		
		if (mState == PLAYER_TURN)
		{
			HandleMenu();
		}

		if (mState != FIRST_TURN)
		{
			//Handle passives after player input begin
			ProcessEntryPassives(mPlayer->Front(), mEnemy->Front());
			ProcessEntryPassives(mEnemy->Front(), mPlayer->Front());
		}

		if (mEnemy->Front().lock()->State() == Fighter::IDLE || mPlayer->Front().lock()->State() == Fighter::DISABLED)
			mState = ENEMY_TURN;
		else
			mState = SKIP_TURN;
		
		if (mState == ENEMY_TURN)
		{
			mEnemyInput = rand() % mEnemy->Front().lock()->Abilities().size() + 1;

			if (mEnemy->Front().lock()->HasAbilityUses())
			{
				while (mEnemy->Front().lock()->AbilityUses(mEnemyInput - 1) < 1)
					mEnemyInput = rand() % mEnemy->Front().lock()->Abilities().size() + 1;
			}
		}

		if (mPlayerAction == ATTACK)
		{
			if (PlayerFirst())
			{
				ProcessAttack(mPlayer->Front(), mEnemy->Front(), mPlayerInput);

				if(!mEnemy->Front().lock()->Dead()) 
					ProcessAttack(mEnemy->Front(), mPlayer->Front(), mEnemyInput);
			}
			else //Enemy goes first
			{
				ProcessAttack(mEnemy->Front(), mPlayer->Front(), mEnemyInput);

				if (!mPlayer->Front().lock()->Dead())
					ProcessAttack(mPlayer->Front(), mEnemy->Front(), mPlayerInput);
			}
		}
		else
		{
			//Handle other actions
			ProcessActions();
			if (!mFighting) break; //Successful escape or bait attempt

			ProcessAttack(mEnemy->Front(), mPlayer->Front(), mEnemyInput);
		}

		//Monsters are no longer new entries
		mPlayer->Front().lock()->NewEntry(false);
		mEnemy->Front().lock()->NewEntry(false);

		if (!mPlayer->Front().lock()->Dead())
			ApplyEffects(mPlayer->Front());
		if (!mEnemy->Front().lock()->Dead())
			ApplyEffects(mEnemy->Front());

		if (mPlayer->Front().lock()->Dead() || mEnemy->Front().lock()->Dead())
		{
			ProcessKill();

			//Reset active fighters and add in new one
			CheckFighters();
		}
	}
}