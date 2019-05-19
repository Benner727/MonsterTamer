#include "Battle.h"

void Battle::HandleLevelUp(int slot)
{
	if (mParticipants[slot].lock()->GetMonster().lock()->LevelUp())
	{
		mInterface->AddDialogue(mParticipants[slot].lock()->Name() + " is now level " + std::to_string(mParticipants[slot].lock()->Level()) + "!");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");
	}

	if (mParticipants[slot].lock()->GetMonster().lock()->LearnableAbilities().size())
	{
		for (const auto& abilityIndex : mParticipants[slot].lock()->GetMonster().lock()->LearnableAbilities())
		{
			std::weak_ptr<Ability> ability = AssetManager::Instance()->GetAbility(abilityIndex);

			if (mParticipants[slot].lock()->GetMonster().lock()->Abilities().size() < 4)
			{
				mInterface->AddDialogue(mParticipants[slot].lock()->Name() + " learned the ability " + ability.lock()->Name() + "!");
				mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
				mInterface->Show();
				system("PAUSE>nul");

				mParticipants[slot].lock()->GetMonster().lock()->LearnAbility(abilityIndex);
				mParticipants[slot].lock()->UpdateAbilities();
			}
			else
			{
				int input, option;
				do
				{
					mInterface->AddDialogue(mParticipants[slot].lock()->Name() + " is trying to learn the ability " + ability.lock()->Name() + "...");
					mInterface->AddDialogue("Should an ability be forgotten to learn it?");
					mInterface->AddDialogue("[0] Don't learn " + ability.lock()->Name() + ".");
					mInterface->AddDialogue("[1] Forget " + mParticipants[slot].lock()->Abilities()[0].lock()->Name() + ".");
					mInterface->AddDialogue("[2] Forget " + mParticipants[slot].lock()->Abilities()[1].lock()->Name() + ".");
					mInterface->AddDialogue("[3] Forget " + mParticipants[slot].lock()->Abilities()[2].lock()->Name() + ".");
					mInterface->AddDialogue("[4] Forget " + mParticipants[slot].lock()->Abilities()[3].lock()->Name() + ".");
					mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
					mInterface->Show();

					input = Input(0, 4);

					if (input == 0)
						mInterface->AddDialogue(mParticipants[slot].lock()->Name() + " doesn't learn " + ability.lock()->Name() + ".");
					else
					{
						std::cout << "[0] Reconsider options." << std::endl;
						std::cout << "[5] You want " << mParticipants[slot].lock()->Name() << " to forget " + mParticipants[slot].lock()->Abilities()[input - 1].lock()->Name() << "." << std::endl;

						option;
						do
						{
							std::cout << ">";

							if (std::cin.fail())
							{
								std::cin.clear();
								std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							}

							std::cin >> option;
						} while (std::cin.fail() || (option != 0 && option != 5));

						if (option == 5)
						{
							mInterface->AddDialogue(mParticipants[slot].lock()->Name() + " forgets " + mParticipants[slot].lock()->Abilities()[input - 1].lock()->Name() + ".");
							mInterface->AddDialogue(mParticipants[slot].lock()->Name() + " learns " + ability.lock()->Name() + ".");
							mParticipants[slot].lock()->GetMonster().lock()->LearnAbility(input - 1, abilityIndex);
							mParticipants[slot].lock()->UpdateAbilities();
							break;
						}
					}
				} while (input != 0 && option != 5);

				mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
				mInterface->Show();
				system("PAUSE>nul");
			}
		}
	}
}

void Battle::CalculateExp()
{
	if (mParticipants.size() > 0)
	{
		int exp = mEnemy->Front().lock()->Level() * 8.0f;
		if (mTrainerBattle) exp *= 1.5f;

		exp /= mParticipants.size();
		if (exp < 1) exp = 1;

		for (int i = 0; i < mParticipants.size(); i++)
		{
			mInterface->AddDialogue(mParticipants[i].lock()->Name() + " gains " + std::to_string(exp) + " experience!");
			mInterface->Scene(BattleInterface::DIALOGUE_SCENE);

			mInterface->Show();
			system("PAUSE>nul");

			mParticipants[i].lock()->GetMonster().lock()->AddExperience(exp);
			HandleLevelUp(i);
		}

		mParticipants.clear();
	}
}

void Battle::HandleSwap()
{
	//Allow user chance to swap out monsters
	if (mTrainerBattle)
	{
		std::string enemyName = mEnemy->GetSlot(1).lock()->Name();

		mInterface->AddDialogue(mEnemy->Name() + " is about to enter " + enemyName + "...");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");

		mInterface->AddDialogue("Would you like to swap out monsters?");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();

		std::cout << "[1] Keep " << mPlayer->FrontMonster().lock()->Name() << "." << std::endl;
		std::cout << "[2] Swap for new monster." << std::endl;

		int input = Input(1, 2);
		if (input == 2)
		{
			mInterface->Scene(BattleInterface::PARTY_SCENE);

			mInterface->Show();
			input = Input(0, mPlayer->Party().size());

			if (input > 1)
			{
				mPlayer->SwapFront(input - 1);
				
				mInterface->AddDialogue(mPlayer->Front().lock()->Name() + " enters the battle!");
				mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
				mInterface->Show();
				system("PAUSE>nul");
			}
		}
	}
}

void Battle::ProcessKill()
{
	if (mPlayer->Front().lock()->Dead())
	{
		mInterface->AddDialogue(mPlayer->Front().lock()->Name() + " has been killed!");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");

		for (int i = 0; i < mParticipants.size(); i++)
		{
			if (mParticipants[i].lock() == mPlayer->Front().lock())
			{
				mParticipants.erase(mParticipants.begin() + i);
				break;
			}
		}

		if (mPlayer->LastMonster())
		{
			mInterface->AddDialogue(mPlayer->Name() + " has no monsters left to defend him!");
			mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
			mInterface->Show();
			system("PAUSE>nul");

			mInterface->AddDialogue("It looks like this is the end of your journey...");
			mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
			mInterface->Show();
			system("PAUSE>nul");

			mPlayer->RemoveFront(); //Remove after display otherwise game will crash due to monster not existing!
			mFighting = false;
		}
		else
		{
			mPlayer->RemoveFront(); //Remove before display so new monster appears

			mInterface->AddDialogue(mPlayer->Front().lock()->Name() + " enters the battle!");
			mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
			mInterface->Show();
			system("PAUSE>nul");
		}

		mPlayerAction = NONE;
	}

	if (mEnemy->Front().lock()->Dead())
	{
		mInterface->AddDialogue(mEnemy->Front().lock()->Name() + " has been killed!");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");

		CalculateExp();

		mPlayer->AddMonstersKilled();
		mPlayer->FrontMonster().lock()->AddKill();

		if (mPlayer->Front().lock()->State() == Fighter::STATE::RECHARGING)
			mPlayer->Front().lock()->ChangeState(Fighter::STATE::IDLE);

		if (mEnemy->LastMonster())
		{
			if (mTrainerBattle)
			{
				mPlayer->AddTamersBeaten();

				int currency = mEnemy->Inventory().lock()->Currency();
				mPlayer->Inventory().lock()->AddCurrency(currency);

				mInterface->AddDialogue(mEnemy->Name() + " has no monsters left!");
				mInterface->AddDialogue("They throw " + std::to_string(currency) + " coins towards you.");
				mInterface->AddDialogue("Sadly this does not protect them...");
				mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
				mInterface->Show();
				system("PAUSE>nul");
			}

			mFighting = false;
		}
		else
		{
			if (mPlayer->Party().size() > 1)
				HandleSwap();

			mEnemy->RemoveFront();

			mInterface->AddDialogue(mEnemy->Front().lock()->Name() + " enters the battle!");
			mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
			mInterface->Show();
			system("PAUSE>nul");
		}
	}
}