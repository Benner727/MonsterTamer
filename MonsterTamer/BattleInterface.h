#pragma once

#include <queue>

#include "Combatant.h"

class BattleInterface
{
public:
	BattleInterface(std::weak_ptr<Combatant> player, std::weak_ptr<Combatant> enemy, bool trainer);

	enum SCENE
	{
		INTRO_SCENE, MENU_SCENE, ATTACK_SCENE, INVENTORY_SCENE, PARTY_SCENE, DIALOGUE_SCENE
	};

private:
	std::shared_ptr<Combatant> mPlayer;
	std::shared_ptr<Combatant> mEnemy;

	bool mTrainer;

	SCENE mScene;

	std::queue<std::string> mDialogue;

	void MonsterDisplay();

	void Intro();

	void Attack();
	void Inventory();
	void Party();
	void Menu();

	void Dialogue();

public:
	SCENE Scene() const { return mScene; }
	void Scene(SCENE scene) { mScene = scene; }

	void AddDialogue(std::string dialogue);
	bool HasDialogue() const { return !mDialogue.empty(); }
	void ClearDialogue();

	void Show();
};

