#include "Fighter.h"

Fighter::Fighter(std::weak_ptr<Monster> monster)
	: mMonster(monster)
{
	for (int i = 0; i < 6; ++i)
		mStage[i] = 0;

	mState = STATE::IDLE;

	mNewEntry = true;

	mFlinched = false;
	mConfused = false;

	std::weak_ptr<Species> species = AssetManager::Instance()->GetSpecies(mMonster.lock()->SpeciesIndex());

	mPrimaryElement = AssetManager::Instance()->GetElement(species.lock()->PrimaryElement());
	mSecondaryElement = AssetManager::Instance()->GetElement(species.lock()->SecondaryElement());

	mPassive = AssetManager::Instance()->GetPassive(species.lock()->PassiveIndex());

	for (const auto& ability : mMonster.lock()->Abilities())
		mAbilities.push_back(AssetManager::Instance()->GetAbility(ability.first));
}


bool Fighter::LowerStage(STAGE stage)
{
	if (mStage[stage] > -6)
	{
		mStage[stage]--;
		return true;
	}

	return false;
}

bool Fighter::IncreaseStage(STAGE stage)
{
	if (mStage[stage] < 6)
	{
		mStage[stage]++;
		return true;
	}

	return false;
}

std::string Fighter::Name() const
{
	return mMonster.lock()->Name();
}

int Fighter::Level() const
{
	return mMonster.lock()->Level();
}

int Fighter::Health() const
{
	return mMonster.lock()->Health();
}

int Fighter::TotalHealth() const
{
	return mMonster.lock()->TotalHealth();
}

bool Fighter::Dead() const
{
	return mMonster.lock()->Health() < 1;
}

int Fighter::Attack() const
{
	int attack = mMonster.lock()->Attack();

	if (mStage[ATTACK] > 0)
		attack *= (3.0f + mStage[ATTACK]) / 3.0f;
	else
		attack *= 3.0f / (3.0f - mStage[ATTACK]);

	return attack;
}

int Fighter::Defense() const
{
	int defense = mMonster.lock()->Defense();

	if (mStage[DEFENSE] > 0)
		defense *= (3.0f + mStage[DEFENSE]) / 3.0f;
	else
		defense *= 3.0f / (3.0f - mStage[DEFENSE]);

	return defense;
}

int Fighter::Speed() const
{
	int speed = mMonster.lock()->Speed();

	if (mStage[SPEED] > 0)
		speed *= (3.0f + mStage[SPEED]) / 3.0f;
	else
		speed *= 3.0f / (3.0f - mStage[SPEED]);

	return speed;
}

float Fighter::Accuracy() const
{
	float accuracy = 1.0f;

	if (mStage[ACCURACY] > 0.0f)
		accuracy *= (4.0f + mStage[ACCURACY]) / 4.0f;
	else
		accuracy *= 4.0f / (4.0f - mStage[ACCURACY]);

	return accuracy;
}

float Fighter::Evasion() const
{
	float evasion = 1.0f;

	if (mStage[EVASION] < 0.0f)
		evasion *= (4.0f - mStage[EVASION]) / 4.0f;
	else
		evasion *= 4.0f / (4.0f + mStage[EVASION]);

	return evasion;
}

float Fighter::Critical() const
{
	float critical = 1.0f;

	if (mStage[CRITICAL] > 0.0f)
		critical *= (1.0f + mStage[CRITICAL]) / 1.0f;
	else
		critical *= 1.0f / (1.0f - mStage[CRITICAL]);

	return critical;
}

bool Fighter::HasAbilityUses() const
{
	for (const auto& ability : mMonster.lock()->Abilities())
	{
		if (ability.second > 0)
			return true;
	}

	return false;
}

void Fighter::UpdateAbilities()
{
	mAbilities.clear();
	for (const auto& ability : mMonster.lock()->Abilities())
		mAbilities.push_back(AssetManager::Instance()->GetAbility(ability.first));
}
