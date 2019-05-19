#pragma once

#include "BattleInterface.h"

class Battle
{
public:
	Battle(std::weak_ptr<Tamer> player, std::weak_ptr<Tamer> enemy, bool trainerBattle = true);
	
private:
	std::shared_ptr<Combatant> mPlayer;
	std::shared_ptr<Combatant> mEnemy;
	bool mTrainerBattle;

	std::vector<std::weak_ptr<Fighter>> mParticipants;

	std::unique_ptr<BattleInterface> mInterface;

	enum STATE { FIRST_TURN, SKIP_TURN, PLAYER_TURN, ENEMY_TURN };
	STATE mState;

	enum ACTION { NONE, ATTACK, USE_ITEM, SWAP_FRONT, ESCAPE_ATTEMPT };
	
	ACTION mPlayerAction;
	
	int mPlayerInput;	
	int mEnemyInput;

	bool mFighting;
	int mEscapeAttempts;
	bool mEscaped;

	int Input(int min, int max);

	void HandleIntro();

	void HandleMenu();
	void HandleAttack();
	void HandleInventory();
	void HandleParty();
	void HandleEscape();

	bool PlayerFirst();

	void CheckFighters();

	bool CalculateTame();
	bool CalculateEscape();
	void ProcessActions();


	void ProcessPreEffects(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target);
	void ProcessPostEffects(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target);
	void ApplyEffects(std::weak_ptr<Fighter> user);
	void ProcessEffects(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target);

	void ProcessEntryPassives(std::weak_ptr<Fighter> user, std::weak_ptr<Fighter> target);
	void ProcessAttackPassives(std::weak_ptr<Fighter> user, std::weak_ptr<Ability> ability, std::weak_ptr<Fighter> target, int damage);

	void ProcessAttack(std::weak_ptr<Fighter> user, std::weak_ptr<Fighter> target, int input);

	void HandleLevelUp(int slot);
	void CalculateExp();
	void HandleSwap();
	void ProcessKill();

public:
	void Start();
};