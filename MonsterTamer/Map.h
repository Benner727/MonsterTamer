#pragma once

#include "PlayerFactory.h"
#include "Battle.h"

class Map
{
public:
	Map();
	
private:
	std::shared_ptr<Tamer> mPlayer;

	std::vector<int> mZones;
	int mCurrentZone;
	bool mGuildCompleted;

	bool mQuit;

	int HandleInput(int min, int max);

	void Init();

	void Hunt();
	void Challenge();
	void Shop();
	void Rest();
	void Bestiary();
	void Party();
	void Guild();
	void Travel();
	void Save();
	void Quit();

	void Interface();

public:
	void Show();
};