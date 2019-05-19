#pragma once

#include <queue>

#include "Tamer.h"

class PlayerFactory
{
public:
	PlayerFactory();
	
private:
	std::shared_ptr<Tamer> mPlayer;

	std::queue<std::string> mDialogue;

	void ShowDialogue();
	
	std::string GetName();
	std::string GetNickname(int species);

	int ChooseMonster();

	void CreatePlayer(std::string name, int species, std::string nickname);
	void PlayIntroduction();

public:
	std::weak_ptr<Tamer> GetPlayer();
};

