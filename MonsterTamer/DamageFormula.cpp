#include "DamageFormula.h"

#include <chrono> 

std::random_device DamageFormula::generator;
std::uniform_int_distribution<int> DamageFormula::roll = std::uniform_int_distribution<int>(1, 100);
std::uniform_real_distribution<float> DamageFormula::modifier = std::uniform_real_distribution<float>(0.85f, 1.0f);
std::uniform_real_distribution<float> DamageFormula::chance = std::uniform_real_distribution<float>(0.0f, 1.0f);

float DamageFormula::Multiplier(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target)
{
	float multiplier = 1.0f;

	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::BERSERKER))
		multiplier *= (1.0f + (1.0f - ((float)user.lock()->Health() / user.lock()->TotalHealth())));
	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::SURGE))
	{
		if (roll(generator) <= 25)
			multiplier *= 1.5f;
	}
	if (target.lock()->Passive().lock()->Check(Passive::PassiveEffect::FILTER))
		multiplier *= 0.75f;

	return multiplier;
}

int DamageFormula::Roll()
{
	return roll(generator);
}

bool DamageFormula::CheckAccuracy(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target)
{
	if (user.lock()->Passive().lock()->Check(Passive::PassiveEffect::CONCENTRATION)
		|| target.lock()->Passive().lock()->Check(Passive::PassiveEffect::CONCENTRATION))
		return true;

	int accuracy = user.lock()->Accuracy() * target.lock()->Evasion() * ability.lock()->Accuracy();

	if (accuracy >= roll(generator))
		return true;

	return false;
}

int DamageFormula::CalculateDamage(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target)
{
	int damage = (2.0f * user.lock()->Level() + 2.0f) * ability.lock()->Power() * ((float)user.lock()->Attack() / target.lock()->Defense()) / 10.0f + 2.0f;
	
	if (user.lock()->PrimaryElement().lock()->Index() == ability.lock()->ElementIndex()
		|| user.lock()->SecondaryElement().lock()->Index() == ability.lock()->ElementIndex())
		damage *= 1.5f;

	damage *= modifier(generator);
	damage *= DamageFormula::Multiplier(user, ability, target);

	return damage > 0 ? damage : 1;
}

bool DamageFormula::CheckCritical(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target)
{
	if (target.lock()->Passive().lock()->Check(Passive::PassiveEffect::ARMORED))
		return false;

	int critical = 100.0f * user.lock()->Critical() / 66.0f;
	if (critical >= roll(generator))
		return true;

	return false;
}
