#include "Game.h"
#include <time.h>
#include "TTK/Input.h"

Game::Game()
{

}

void Game::Init()
{
	srand(time(NULL));

	gameState = GameState::Tutorial;

	fishManager = new FishManager();

	waitBeforeCastingCountdown = 0.0f;

	audioEngine.Init();

	//load bank
	audioEngine.LoadBank("Master", FMOD_STUDIO_LOAD_BANK_NORMAL);

	//load event
	audioEngine.LoadEvent("Music", "{13f73348-5181-4c6b-838a-4ef9f8ad1b56}");

	//play event
	audioEngine.PlayEvent("Music");

	//set position
	//audioEngine.SetEventPosition("Music", startPosition);

	playerPos = glm::vec3(0.0f, 1.0f, 0.0f); //put player slightly above the water

	//TODO: play tutorial sound clip

}

void Game::Update(float dt)
{
	switch (gameState)
	{
		case GameState::Tutorial:
		{
			tutorialCountdown -= dt;
			if (tutorialCountdown < 0.0f)
				ChangeToWaitingToCast();

			break;
		}

		case GameState::WaitingToCast:
		{
			if (isCasting)
			{
				castCountdown -= dt;
				if (castCountdown < 0.0f)
				{
					ChangeToWaitingForBite();

					//TODO: play bloop sound from the bobber hitting the water at bobberPos

				}
			}
			else //not casting
			{
				waitBeforeCastingCountdown -= dt;
				if (waitBeforeCastingCountdown < 0.0f)
				{
					if (TTK::Input::GetKeyPressed(TTK::KeyCode::Space))
						CastReel();
				}
			}

			break;
		}

		case GameState::WaitingForBite:
		{
			if (isBite)
			{
				if (TTK::Input::GetKeyPressed(TTK::KeyCode::Space))
				{
					ChangeToReeling();

					break; //they got the fish on the line, no need to continue in this case statement
				}

				fishGotAwayTimer -= dt;
				if (fishGotAwayTimer < 0.0f)
				{
					waitBeforeCastingCountdown = 5.0f; //add a brief delay before the player can cast again to let the voice line play
					ChangeToWaitingToCast();

					//TODO: player fish got away voice line

				}
			}
			else //there was no bite yet
			{
				biteWaitCountdown -= dt;
				if (biteWaitCountdown < 0.0f)
				{
					isBite = true;
					fishManager->SpawnRandomFish(bobberPos, castDistance);
					fishGotAwayTimer = fishManager->activeFish->escapeTime;

					//TODO: play bite sound effect at bobberPos

				}
			}

			break;
		}

		case GameState::Reeling:
		{

			break;
		}
	}

	audioEngine.Update();
}

void Game::ShutDown()
{
	audioEngine.Shutdown();
	delete fishManager;
}

void Game::CastReel()
{
	castCountdown = CAST_TIME;
	isCasting     = true;

	//cast forward, with the x direction being random
	castDirection = glm::vec3(
		rand() % 100,
		0.0f,
		100);

	if (rand() % 2)
		castDirection.x *= -1;

	castDirection = glm::normalize(castDirection);

	//get random cast distance
	castDistance = rand() % (MAX_CAST_DISTANCE - MIN_CAST_DISTANCE + 1) + MIN_CAST_DISTANCE;

	//set bobber position
	bobberPos = castDirection * (float)castDistance;
	bobberPos.y = 0.0f; //should already be 0 but just to be safe

	//TODO: play casting sound

}

void Game::ChangeToWaitingToCast()
{
	gameState = GameState::WaitingToCast;

	isCasting = false;
}

void Game::ChangeToWaitingForBite()
{
	gameState = GameState::WaitingForBite;

	//generate random float between min and max wait times
	float randNum = ((float)rand()) / (float)RAND_MAX;
	float diff = MAX_BITE_WAIT - MIN_BITE_WAIT;
	float r = randNum * diff;
	biteWaitCountdown = MIN_BITE_WAIT + r;

	isBite = false;
}

void Game::ChangeToReeling()
{
	gameState = GameState::WaitingForBite;


}