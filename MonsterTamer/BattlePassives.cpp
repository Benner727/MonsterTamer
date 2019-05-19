#include "Battle.h"

void Battle::ProcessEntryPassives(std::weak_ptr<Fighter> user, std::weak_ptr<Fighter> target)
{
	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::ATTACK_BOOST))
	{
		user.lock()->IncreaseStage(Fighter::ATTACK);
		mInterface->AddDialogue(user.lock()->Name() + "'s attack increases!");
	}
	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::DEFENSE_BOOST))
	{
		user.lock()->IncreaseStage(Fighter::DEFENSE);
		mInterface->AddDialogue(user.lock()->Name() + "'s defense increases!");
	}
	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::SPEED_BOOST))
	{
		user.lock()->IncreaseStage(Fighter::SPEED);
		mInterface->AddDialogue(user.lock()->Name() + "'s speed increases!");
	}
	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::LUCKY))
	{
		if (user.lock()->NewEntry())
		{
			user.lock()->IncreaseStage(Fighter::CRITICAL);
			mInterface->AddDialogue(user.lock()->Name() + " is feeling lucky...");
			mInterface->AddDialogue(user.lock()->Name() + "'s critical increases!");
		}
	}
	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::FOCUSED))
	{
		if (user.lock()->NewEntry())
		{
			user.lock()->IncreaseStage(Fighter::ACCURACY);
			mInterface->AddDialogue(user.lock()->Name() + " is focused...");
			mInterface->AddDialogue(user.lock()->Name() + "'s accuracy increases!");
		}
	}
	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::INTIMIDATE))
	{
		if (target.lock()->NewEntry())
		{
			target.lock()->LowerStage(Fighter::ATTACK);
			mInterface->AddDialogue(target.lock()->Name() + " is intimidated...");
			mInterface->AddDialogue(target.lock()->Name() + "'s attack decreases!");
		}
	}

	if (mInterface->HasDialogue())
	{
		mInterface->Scene(BattleInterface::DIALOGUE_SCENE);
		mInterface->Show();
		system("PAUSE>nul");
	}
}

void Battle::ProcessAttackPassives(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target, int damage)
{
	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::VAMPIRIC))
	{
		damage *= 0.125f;
		user.lock()->Heal(damage > 0 ? damage : 1);
		mInterface->AddDialogue(std::to_string(damage) + " health is leached!");
	}
	if (target.lock()->Passive().lock()->Check(Passive::PassiveEffect::VENGEANCE))
	{
		int veng = user.lock()->TotalHealth() * 6.25f / 100.0f;
		user.lock()->Damage(veng);
		mInterface->AddDialogue(user.lock()->Name() + " tastes vengeance!");
	}
}