#include "Battle.h"

#include "DamageFormula.h"

void Battle::ProcessAttack(std::weak_ptr<Fighter> user, std::weak_ptr<Fighter> target, int input)
{
	if (user.lock()->AbilityUses(input - 1) < 1)
	{
		//Unable to attack
		mInterface->AddDialogue(user.lock()->Name() + " is unable to use that ability...");
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");
		return;
	}

	std::weak_ptr<Ability> ability = user.lock()->Abilities()[input - 1];

	//Handle pre-attack effects
	ProcessPreEffects(user, ability, target);
	
	bool hit = true;
	if (user.lock()->State() == Fighter::IDLE)
	{
		user.lock()->UseAbility(input - 1);
		mInterface->AddDialogue(user.lock()->Name() + " uses " + ability.lock()->Name() + "!");

		if (user.lock()->Flinch())
		{
			//User flinches
			mInterface->AddDialogue("It flinches...");
			hit = false;
		}
		else
		{
			if (ability.lock()->Target() == 1)
				hit = DamageFormula::CheckAccuracy(user, ability, target);

			if (!hit)
			{
				//Attack misses
				mInterface->AddDialogue("The attack misses...");
			}
		}

		if (ability.lock()->Physical() && hit)
		{
			int damage = DamageFormula::CalculateDamage(user, ability, target);
			bool critical = DamageFormula::CheckCritical(user, ability, target);

			if (critical) damage *= 2.0f;

			float modifier = 1.0f;
			if (target.lock()->PrimaryElement().lock()->ImmuneTo(ability.lock()->ElementIndex()))
				modifier *= 0.0f;
			else
			{
				if (target.lock()->SecondaryElement().lock()->ImmuneTo(ability.lock()->ElementIndex()))
					modifier *= 0.0f;
				else
				{
					if (target.lock()->PrimaryElement().lock()->Resists(ability.lock()->ElementIndex()))
						modifier *= 0.5f;
					if (target.lock()->PrimaryElement().lock()->WeakTo(ability.lock()->ElementIndex()))
						modifier *= 2.0f;
					if (target.lock()->SecondaryElement().lock()->Resists(ability.lock()->ElementIndex()))
						modifier *= 0.5f;
					if (target.lock()->SecondaryElement().lock()->WeakTo(ability.lock()->ElementIndex()))
						modifier *= 2.0f;
				}
			}

			if (user.lock()->Confused() && rand() % 2)
			{
				if (damage > target.lock()->Health())
					damage = target.lock()->Health();
				if (damage > user.lock()->Health())
					damage = user.lock()->Health();

				user.lock()->Damage(damage);
				mInterface->AddDialogue("It hurts itself in its confusion!");
			}
			else
			{
				int n = 1;
				if (ability.lock()->Check(Ability::AbilityEffect::MULTI_HIT))
					n += rand() % 3 + 1;

				damage *= modifier;

				if (damage > target.lock()->Health())
					damage = target.lock()->Health();

				for (int i = 0; i < n; ++i)
				{
					if (damage > 0)
					{
						target.lock()->Damage(damage);
						mInterface->AddDialogue("It does " + std::to_string(damage) + (critical ? " critical damage!" : " damage!"));

						if (target.lock()->Dead())
							break;
					}

					if (modifier == 0.0f)
						mInterface->AddDialogue("It has no effect.");
					else if (modifier < 1.0f)
						mInterface->AddDialogue("It is not very effective.");
					else if (modifier > 1.0f)
						mInterface->AddDialogue("It is super effective.");

					if (i != n - 1)
					{
						mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
						mInterface->Show();
						system("PAUSE>nul");
					}
				}

				if (ability.lock()->Check(Ability::AbilityEffect::RECOIL))
				{
					damage *= 0.25f;
					user.lock()->Damage(damage);
					mInterface->AddDialogue(std::to_string(damage) + " damage is recoiled!");
				}
				else if (ability.lock()->Check(Ability::AbilityEffect::LIFESTEAL))
				{
					damage *= 0.5f;
					user.lock()->Heal(damage);
					mInterface->AddDialogue(std::to_string(damage) + " health is recovered!");
				}

				ProcessAttackPassives(user, ability, target, damage);
			}
		}
	}

	//Handle post-attack effects
	ProcessPostEffects(user, ability, target);

	if (mInterface->HasDialogue())
	{
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");
	}

	if (hit) ProcessEffects(user, ability, target);
}