#pragma once

#include "Monster.h"

class Fighter
{
public:
	Fighter(std::weak_ptr<Monster> monster);

	enum STAGE
	{
		ATTACK, DEFENSE, SPEED, ACCURACY, EVASION, CRITICAL
	};

	enum STATE
	{
		IDLE, CHARGING, RECHARGING, DISABLED
	};

private:
	std::weak_ptr<Monster> mMonster;

	int mStage[6];
	STATE mState;
	bool mNewEntry;

	bool mFlinched;
	bool mConfused;

	std::weak_ptr<Element> mPrimaryElement;
	std::weak_ptr<Element> mSecondaryElement;
	std::weak_ptr<Passive> mPassive;
	std::vector<std::weak_ptr<Ability>> mAbilities;

public:
	bool LowerStage(STAGE stage);
	bool IncreaseStage(STAGE stage);
	int Stage(STAGE stage) const { return mStage[stage]; }

	STATE State() const { return mState; }
	void ChangeState(STATE state) { mState = state; }

	bool NewEntry() const { return mNewEntry; }
	void NewEntry(bool entry) { mNewEntry = entry; }
	
	void Flinch(bool flinch) { mFlinched = flinch; }
	bool Flinch() const { return mFlinched; }

	void Confused(bool confused) { mConfused = confused; }
	bool Confused() const { return mConfused; }
 
	std::string Name() const;
	int Level() const;

	int Health() const;
	int TotalHealth() const;
	bool Dead() const;

	int Attack() const;
	int Defense() const;
	int Speed() const;

	float Accuracy() const;
	float Evasion() const;
	float Critical() const;

	int Status() const { return mMonster.lock()->Status(); }
	void Status(Monster::STATUS status) { mMonster.lock()->Status(status); }

	int StatusCount() const { return mMonster.lock()->StatusCount(); }
	void StatusCount(int count) { mMonster.lock()->StatusCount(count); }

	std::weak_ptr<Element> PrimaryElement() const { return mPrimaryElement; }
	std::weak_ptr<Element> SecondaryElement() const { return mSecondaryElement; }
	std::weak_ptr<Passive> Passive() const { return mPassive; }
	std::vector<std::weak_ptr<Ability>> Abilities() const { return mAbilities; }

	int AbilityUses(int slot) { return mMonster.lock()->Abilities()[slot].second; }
	void UseAbility(int slot) { mMonster.lock()->Abilities()[slot].second--; }
	bool HasAbilityUses() const;
	void UpdateAbilities();

	void Damage(int amount) { mMonster.lock()->Damage(amount); }
	void Heal(int amount) { mMonster.lock()->Heal(amount); }

	std::weak_ptr<Monster> GetMonster() const { return mMonster; }
};