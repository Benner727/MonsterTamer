#include "Battle.h"

#include "DamageFormula.h"

void Battle::ProcessPreEffects(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target)
{
	if (target.lock()->Status() == Monster::FROZEN)
	{
		if (ability.lock()->ElementIndex() == 2)
		{
			target.lock()->Status(Monster::HEALTHY);
			target.lock()->ChangeState(Fighter::IDLE);
			mInterface->AddDialogue(target.lock()->Name() + " is thawed out!");
		}
	}
	if (user.lock()->Status() == Monster::SLEEP)
	{
		if (user.lock()->StatusCount() < 1)
		{
			user.lock()->Status(Monster::HEALTHY);
			user.lock()->ChangeState(Fighter::IDLE);
			mInterface->AddDialogue(user.lock()->Name() + " wakes up!");
		}
		else
		{
			user.lock()->StatusCount(user.lock()->StatusCount() - 1);
			mInterface->AddDialogue(user.lock()->Name() + " is asleep...");
		}
	}
	if (user.lock()->Status() == Monster::PARALYSIS)
	{
		if (!(rand() % 5))
		{
			user.lock()->ChangeState(Fighter::DISABLED);
			mInterface->AddDialogue(user.lock()->Name() + " is paralyzed!");
		}
		else if (user.lock()->State() == Fighter::DISABLED)
		{
			user.lock()->ChangeState(Fighter::IDLE);
		}
	}
	if (user.lock()->Status() == Monster::FROZEN)
	{
		if (!(rand() % 4))
		{
			user.lock()->ChangeState(Fighter::DISABLED);
			mInterface->AddDialogue(user.lock()->Name() + " is frozen!");
		}
		else if (user.lock()->State() == Fighter::DISABLED)
		{
			user.lock()->Status(Monster::HEALTHY);
			user.lock()->ChangeState(Fighter::IDLE);
			mInterface->AddDialogue(user.lock()->Name() + " unfreezes!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::CHARGED))
	{
		if (user.lock()->State() == Fighter::CHARGING)
			user.lock()->ChangeState(Fighter::IDLE);
		else if (user.lock()->State() == Fighter::IDLE)
		{
			user.lock()->ChangeState(Fighter::CHARGING);
			mInterface->AddDialogue(user.lock()->Name() + " charges the attack...");
		}
	}
}

void Battle::ApplyEffects(std::weak_ptr<Fighter> user)
{
	if (user.lock()->Status() == Monster::HEALTHY)
		return;

	if (user.lock()->Status() == Monster::POISON)
	{
		int damage = user.lock()->TotalHealth() * 6.25f / 100.0f;
		user.lock()->Damage(damage);
		mInterface->AddDialogue(user.lock()->Name() + " suffers poison damage!");
	}
	if (user.lock()->Status() == Monster::BURN)
	{
		int damage = user.lock()->TotalHealth() * 6.25f / 100.0f;
		user.lock()->Damage(damage);
		mInterface->AddDialogue(user.lock()->Name() + " suffers burn damage!");
	}

	if (mInterface->HasDialogue())
	{
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");
	}
}

void Battle::ProcessEffects(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target)
{
	if (user.lock()->State() != Fighter::IDLE)
		return;

	if (ability.lock()->Effect() == Ability::AbilityEffect::NO_EFFECT)
		return;

	if (ability.lock()->EffectChance() < DamageFormula::Roll())
		return;

	if (ability.lock()->Check(Ability::AbilityEffect::SLEEP))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (target.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				target.lock()->Status(Monster::SLEEP);
				target.lock()->ChangeState(Fighter::DISABLED);
				target.lock()->StatusCount(rand() % 5 + 2);
				mInterface->AddDialogue(target.lock()->Name() + " falls asleep!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (user.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				user.lock()->Status(Monster::SLEEP);
				user.lock()->ChangeState(Fighter::DISABLED);
				user.lock()->StatusCount(rand() % 5 + 2);
				mInterface->AddDialogue(user.lock()->Name() + " falls asleep!");
			}
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::POISON))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (target.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				target.lock()->Status(Monster::POISON);
				mInterface->AddDialogue(target.lock()->Name() + " is poisoned!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (user.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				user.lock()->Status(Monster::POISON);
				mInterface->AddDialogue(user.lock()->Name() + " is poisoned!");
			}
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::PARALYZE))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (target.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				target.lock()->Status(Monster::PARALYSIS);
				mInterface->AddDialogue(target.lock()->Name() + " becomes paralyzed!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (user.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				user.lock()->Status(Monster::PARALYSIS);
				mInterface->AddDialogue(user.lock()->Name() + " becomes paralyzed!");
			}
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::BURN))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (target.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				target.lock()->Status(Monster::BURN);
				mInterface->AddDialogue(target.lock()->Name() + " gets burned!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (user.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				user.lock()->Status(Monster::BURN);
				mInterface->AddDialogue(user.lock()->Name() + " gets burned!");
			}
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::FREEZE))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (target.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				target.lock()->Status(Monster::FROZEN);
				mInterface->AddDialogue(target.lock()->Name() + " freezes!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (user.lock()->Status() == Monster::STATUS::HEALTHY)
			{
				user.lock()->Status(Monster::FROZEN);
				mInterface->AddDialogue(user.lock()->Name() + " freezes!");
			}
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::FLINCH))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{

			target.lock()->Flinch(true);
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->Flinch(true);
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::CONFUSE))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (!target.lock()->Confused())
			{
				target.lock()->Confused(true);
				mInterface->AddDialogue(target.lock()->Name() + " becomes confused!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (!user.lock()->Confused())
			{
				user.lock()->Confused(true);
				mInterface->AddDialogue(user.lock()->Name() + " becomes confused!");
			}
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::CURE))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->Status(Monster::HEALTHY);
			mInterface->AddDialogue(target.lock()->Name() + " is cured!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->Status(Monster::HEALTHY);
			mInterface->AddDialogue(user.lock()->Name() + " is cured!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::BOOST_ATTACK))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->IncreaseStage(Fighter::ATTACK);
			mInterface->AddDialogue(target.lock()->Name() + "'s attack increases!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->IncreaseStage(Fighter::ATTACK);
			mInterface->AddDialogue(user.lock()->Name() + "'s attack increases!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::BOOST_DEFENSE))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->IncreaseStage(Fighter::DEFENSE);
			mInterface->AddDialogue(target.lock()->Name() + "'s defense increases!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->IncreaseStage(Fighter::DEFENSE);
			mInterface->AddDialogue(user.lock()->Name() + "'s defense increases!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::BOOST_SPEED))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->IncreaseStage(Fighter::SPEED);
			mInterface->AddDialogue(target.lock()->Name() + "'s speed increases!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->IncreaseStage(Fighter::SPEED);
			mInterface->AddDialogue(user.lock()->Name() + "'s speed increases!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::BOOST_ACCURACY))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->IncreaseStage(Fighter::ACCURACY);
			mInterface->AddDialogue(target.lock()->Name() + "'s accuracy increases!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->IncreaseStage(Fighter::ACCURACY);
			mInterface->AddDialogue(user.lock()->Name() + "'s accuracy increases!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::BOOST_EVASION))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->IncreaseStage(Fighter::EVASION);
			mInterface->AddDialogue(target.lock()->Name() + "'s evasion increases!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->IncreaseStage(Fighter::EVASION);
			mInterface->AddDialogue(user.lock()->Name() + "'s evasion increases!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::BOOST_CRITICAL))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->IncreaseStage(Fighter::CRITICAL);
			mInterface->AddDialogue(target.lock()->Name() + "'s critical increases!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->IncreaseStage(Fighter::CRITICAL);
			mInterface->AddDialogue(user.lock()->Name() + "'s critical increases!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::DECREASE_ATTACK))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (!target.lock()->Passive().lock()->Check(Passive::PassiveEffect::GUARDED))
			{
				target.lock()->LowerStage(Fighter::ATTACK);
				mInterface->AddDialogue(target.lock()->Name() + "'s attack lowers!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (!user.lock()->Passive().lock()->Check(Passive::PassiveEffect::GUARDED))
			{
				user.lock()->LowerStage(Fighter::ATTACK);
				mInterface->AddDialogue(user.lock()->Name() + "'s attack lowers!");
			}
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::DECREASE_DEFENSE))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (!target.lock()->Passive().lock()->Check(Passive::PassiveEffect::GUARDED))
			{
				target.lock()->LowerStage(Fighter::DEFENSE);
				mInterface->AddDialogue(target.lock()->Name() + "'s defense lowers!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (!user.lock()->Passive().lock()->Check(Passive::PassiveEffect::GUARDED))
			{
				user.lock()->LowerStage(Fighter::DEFENSE);
				mInterface->AddDialogue(user.lock()->Name() + "'s defense lowers!");
			}
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::DECREASE_SPEED))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (!target.lock()->Passive().lock()->Check(Passive::PassiveEffect::GUARDED))
			{
				target.lock()->LowerStage(Fighter::SPEED);
				mInterface->AddDialogue(target.lock()->Name() + "'s speed lowers!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (!user.lock()->Passive().lock()->Check(Passive::PassiveEffect::GUARDED))
			{
				user.lock()->LowerStage(Fighter::SPEED);
				mInterface->AddDialogue(user.lock()->Name() + "'s speed lowers!");
			}
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::DECREASE_ACCURACY))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->LowerStage(Fighter::ACCURACY);
			mInterface->AddDialogue(target.lock()->Name() + "'s accuracy lowers!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->LowerStage(Fighter::ACCURACY);
			mInterface->AddDialogue(user.lock()->Name() + "'s accuracy lowers!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::DECREASE_EVASION))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->LowerStage(Fighter::EVASION);
			mInterface->AddDialogue(target.lock()->Name() + "'s evasion lowers!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->LowerStage(Fighter::EVASION);
			mInterface->AddDialogue(user.lock()->Name() + "'s evasion lowers!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::DECREASE_CRITICAL))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			target.lock()->LowerStage(Fighter::CRITICAL);
			mInterface->AddDialogue(target.lock()->Name() + "'s critical lowers!");
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			user.lock()->LowerStage(Fighter::CRITICAL);
			mInterface->AddDialogue(user.lock()->Name() + "'s critical lowers!");
		}
	}
	if (ability.lock()->Check(Ability::AbilityEffect::HEAL))
	{
		if (ability.lock()->Target() == 1 || ability.lock()->Target() == 3) //Target
		{
			if (!target.lock()->Dead())
			{
				target.lock()->Heal(target.lock()->TotalHealth() * ability.lock()->Power() / 100.0f);
				mInterface->AddDialogue(target.lock()->Name() + " regains some health!");
			}
		}

		if (ability.lock()->Target() == 2 || ability.lock()->Target() == 3) //User
		{
			if (!user.lock()->Dead())
			{
				user.lock()->Heal(user.lock()->TotalHealth() * ability.lock()->Power() / 100.0f);
				mInterface->AddDialogue(user.lock()->Name() + " regains some health!");
			}
		}
	}

	if (mInterface->HasDialogue())
	{
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");
	}
}

void Battle::ProcessPostEffects(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target)
{
	if (ability.lock()->Check(Ability::AbilityEffect::RECHARGED))
	{
		if (user.lock()->State() == Fighter::IDLE)
			user.lock()->ChangeState(Fighter::RECHARGING);
		else if(user.lock()->State() == Fighter::RECHARGING)
		{
			user.lock()->ChangeState(Fighter::IDLE);
			mInterface->AddDialogue(user.lock()->Name() + " needs to recharge...");
		}
	}
	if (user.lock()->Flinch())
	{
		user.lock()->Flinch(false);
	}
	if (user.lock()->Confused())
	{
		if (!(rand() % 4))
			user.lock()->Confused(false);
	}
}