#include "Battle.h"

#include "DamageFormula.h"

bool Battle::CalculateTame()
{
	float rateModifier = 0.67f - (0.0033f * mEnemy->Front().lock()->Level());
	if (mEnemy->Front().lock()->Status() != Monster::HEALTHY)
		rateModifier *= 1.5f;

	float tameRate = (mEnemy->Front().lock()->TotalHealth() * 3.0f - mEnemy->Front().lock()->TotalHealth() * 2.0f) * rateModifier / (mEnemy->Front().lock()->TotalHealth() * 3.0f) * 100.0f;

	if (tameRate >= DamageFormula::Roll())
		return true;

	return false;
}

bool Battle::CalculateEscape()
{
	mEscapeAttempts++;

	int escape = mPlayer->Front().lock()->Level() * 25.0f / mEnemy->Front().lock()->Level() * mEscapeAttempts;
	if (escape >= DamageFormula::Roll())
		return true;

	return false;
}

void Battle::ProcessActions()
{
	if (mPlayerAction == SWAP_FRONT)
	{
		mInterface->AddDialogue(mPlayer->Name() + " swaps in " + mPlayer->Front().lock()->Name() + "!");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");
	}
	else if (mPlayerAction == USE_ITEM)
	{
		std::weak_ptr<Item> item = mPlayer->Inventory().lock()->GetItem(mPlayerInput - 1);
		
		mPlayer->Inventory().lock()->UseItem(mPlayerInput - 1);
		
		mInterface->AddDialogue(mPlayer->Name() + " uses " + item.lock()->Name() + "...");
		bool hasEffect = false;

		if (item.lock()->Check(Item::ItemEffect::HEAL))
		{
			mPlayer->Front().lock()->Heal(item.lock()->Potency());
			mInterface->AddDialogue(mPlayer->Front().lock()->Name() + " regains some health!");
			hasEffect = true;
		}
		if (item.lock()->Check(Item::ItemEffect::BOOST_ATTACK))
		{
			mPlayer->Front().lock()->IncreaseStage(Fighter::ATTACK);
			mInterface->AddDialogue(mPlayer->Front().lock()->Name() + "'s attack increases!");
			hasEffect = true;
		}
		if (item.lock()->Check(Item::ItemEffect::BOOST_DEFENSE))
		{
			mPlayer->Front().lock()->IncreaseStage(Fighter::DEFENSE);
			mInterface->AddDialogue(mPlayer->Front().lock()->Name() + "'s defense increases!");
			hasEffect = true;
		}
		if (item.lock()->Check(Item::ItemEffect::BOOST_SPEED))
		{
			mPlayer->Front().lock()->IncreaseStage(Fighter::SPEED);
			mInterface->AddDialogue(mPlayer->Front().lock()->Name() + "'s speed increases!");
			hasEffect = true;
		}
		if (item.lock()->Check(Item::ItemEffect::BOOST_ACCURACY))
		{
			mPlayer->Front().lock()->IncreaseStage(Fighter::ACCURACY);
			mInterface->AddDialogue(mPlayer->Front().lock()->Name() + "'s accuracy increases!");
			hasEffect = true;
		}
		if (item.lock()->Check(Item::ItemEffect::BOOST_EVASION))
		{
			mPlayer->Front().lock()->IncreaseStage(Fighter::EVASION);
			mInterface->AddDialogue(mPlayer->Front().lock()->Name() + "'s evasion increases!");
			hasEffect = true;
		}
		if (item.lock()->Check(Item::ItemEffect::BOOST_CRITICAL))
		{
			mPlayer->Front().lock()->IncreaseStage(Fighter::CRITICAL);
			mInterface->AddDialogue(mPlayer->Front().lock()->Name() + "'s critical increases!");
			hasEffect = true;
		}
		if (item.lock()->Check(Item::ItemEffect::REMOVE_SLEEP))
		{
			if (mPlayer->Front().lock()->Status() == Monster::SLEEP)
			{
				mPlayer->Front().lock()->Status(Monster::HEALTHY);
				mPlayer->Front().lock()->StatusCount(0);
				mInterface->AddDialogue(mPlayer->Front().lock()->Name() + " no longer sleeping.");
				hasEffect = true;
			}
		}
		if (item.lock()->Check(Item::ItemEffect::REMOVE_POISON))
		{
			if (mPlayer->Front().lock()->Status() == Monster::POISON)
			{
				mPlayer->Front().lock()->Status(Monster::HEALTHY);
				mInterface->AddDialogue(mPlayer->Front().lock()->Name() + " no longer poisoned.");
				hasEffect = true;
			}
		}
		if (item.lock()->Check(Item::ItemEffect::REMOVE_BURN))
		{
			if (mPlayer->Front().lock()->Status() == Monster::BURN)
			{
				mPlayer->Front().lock()->Status(Monster::HEALTHY);
				mInterface->AddDialogue(mPlayer->Front().lock()->Name() + " no longer burned.");
				hasEffect = true;
			}
		}
		if (item.lock()->Check(Item::ItemEffect::REMOVE_PARALYSIS))
		{
			if (mPlayer->Front().lock()->Status() == Monster::PARALYSIS)
			{
				mPlayer->Front().lock()->Status(Monster::HEALTHY);
				mInterface->AddDialogue(mPlayer->Front().lock()->Name() + " no longer paralyzed.");
				hasEffect = true;
			}
		}
		if (item.lock()->Check(Item::ItemEffect::REMOVE_FROZEN))
		{
			if (mPlayer->Front().lock()->Status() == Monster::FROZEN)
			{
				mPlayer->Front().lock()->Status(Monster::HEALTHY);
				mInterface->AddDialogue(mPlayer->Front().lock()->Name() + " no longer frozen.");
				hasEffect = true;
			}
		}
		if (item.lock()->Check(Item::ItemEffect::BAIT))
		{
			if (!mTrainerBattle)
			{
				mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
				mInterface->Show();
				system("PAUSE>nul");
				hasEffect = true;

				if (CalculateTame())
				{
					mInterface->AddDialogue("The wild " + mEnemy->Front().lock()->Name() + " is successfully tamed!");
					mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
					mInterface->Show();
					system("PAUSE>nul");

					if (!mPlayer->Full())
					{
						mPlayer->AddMonstersTamed();

						mInterface->AddDialogue("What would you like to call your new monster:");
						mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
						mInterface->Show();

						std::string nickname = "";
						do 
						{
							std::cout << ">";
							std::cin.clear();
							std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							std::getline(std::cin, nickname);
						} while (nickname.size() > 13);
						if (nickname.size() > 0)
							mEnemy->FrontMonster().lock()->Rename(nickname);
						mPlayer->AddMonster(mEnemy->FrontMonster());

						if (mEnemy->Party().size() > 1)
						{
							mEnemy->RemoveFront(); //Remove before display so new monster appears

							mInterface->AddDialogue(mEnemy->Front().lock()->Name() + " enters the battle!");
							mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
							mInterface->Show();
							system("PAUSE>nul");
						}
						else
							mFighting = false;
					}
					else
					{
						mInterface->AddDialogue("You have no room in your party for it though...");
						mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
						mInterface->Show();
						system("PAUSE>nul");
					}

					return;
				}
				else
				{
					mInterface->AddDialogue("The wild " + mEnemy->Front().lock()->Name() + " ignores the bait!");
				}
			}
		}

		if (!hasEffect) mInterface->AddDialogue("It has no effect...");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");
	}
	else if (mPlayerAction == ESCAPE_ATTEMPT)
	{
		mInterface->AddDialogue(mPlayer->Name() + " attempts to escape...");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");

		if (CalculateEscape())
		{
			mInterface->AddDialogue("The escape attempt is successful!");
			mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
			mInterface->Show();
			system("PAUSE>nul");

			mFighting = false;
		}
		else
		{
			mInterface->AddDialogue("The escape attempt fails!");
			mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
			mInterface->Show();
			system("PAUSE>nul");
		}
	}
}