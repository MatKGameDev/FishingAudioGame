#pragma once

#include "FishManager.h"
#include "AudioEngine.h"

class Game
{
public:
	enum GameState
	{
		Tutorial,
		WaitingToCast,
		WaitingForBite,
		Reeling
	};

	Game();

	void Init();
	void Update(float dt);
	void ShutDown();

	void CastReel();

	void ChangeToWaitingToCast();
	void ChangeToWaitingForBite();
	void ChangeToReeling();

private:
	//NOTE: ALL TIMERS ARE IN SECONDS

	GameState gameState;

	AudioEngine  audioEngine;
	FishManager* fishManager;

	glm::vec3 bobberPos;

	const int MIN_CAST_DISTANCE = 1;
	const int MAX_CAST_DISTANCE = 18;
	int castDistance;

	float waitBeforeCastingCountdown;

	glm::vec3 castDirection;

	const float TUTORIAL_TIME = 10.0f; //tutorial length
	float tutorialCountdown   = TUTORIAL_TIME;

	bool isCasting;

	const float CAST_TIME = 0.5f; //time it takes to cast
	float castCountdown;

	const float MIN_BITE_WAIT = 1.5f;  //min wait time for a bite after casting
	const float MAX_BITE_WAIT = 12.0f; //max wait time for a bite after casting
	float biteWaitCountdown;

	bool isBite;

	float fishGotAwayTimer;
};