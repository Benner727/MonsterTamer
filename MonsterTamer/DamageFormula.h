#pragma once

#include <random>

#include "Fighter.h"

class DamageFormula
{
private:
	static std::random_device generator;
	static std::uniform_int_distribution<int> roll;
	static std::uniform_real_distribution<float> modifier;
	static std::uniform_real_distribution<float> chance;

	static float Multiplier(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target);

public:
	static int Roll();

	static bool CheckAccuracy(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target);
	static int CalculateDamage(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target);
	static bool CheckCritical(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target);
};