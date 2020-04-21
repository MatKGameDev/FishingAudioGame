#include "Game.h"
#include <time.h>
#include "TTK/Input.h"

/*
	Sorry for the mess, this was an extremely rushed job.

	NOTE: Play with the console window open to see the rarity of the fish you caught.
*/

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
	audioEngine.LoadEvent("Background", "{b2ec4fbd-7944-4704-bdf9-7991edc77827}");

	//play event
	audioEngine.PlayEvent("Background");

	//play tutorial
	audioEngine.LoadEvent("Tutorial", "{c1f0bcd3-dace-4da2-961a-1b8769a6733e}");	
	audioEngine.PlayEvent("Tutorial");

	//lower background volume
	audioEngine.SetEventParameter("Background", "Cast", 1.0f);

}

void Game::Update(float dt)
{
	fishManager->Update(dt);

	switch (gameState)
	{
		case GameState::Tutorial:
		{
			tutorialCountdown -= dt;
			if (tutorialCountdown < 0.0f)
			{
				audioEngine.SetEventParameter("Background", "Cast", 0.0f);
				ChangeToWaitingToCast();
			}

			break;
		}

		case GameState::WaitingToCast:
		{
			audioEngine.SetEventParameter("Background", "Cast", 0.0f);
			//get random cast distance
			if (isCasting)
			{
				audioEngine.SetEventParameter("Background", "Cast", 1.0f);
				 
				castCountdown -= dt;
				if (castCountdown < 0.0f)
				{
					audioEngine.SetEventParameter("Casting", "isLanded", 1.0f);
					audioEngine.LoadEvent("BobberBloop", "{de350efe-1efa-4769-8aae-7edd49452c6a}");
					audioEngine.SetEventPosition("BobberBloop", bobberPos);
					audioEngine.SetEventParameter("BobberBloop", "Distance", castDistance);
					audioEngine.PlayEvent("BobberBloop");

					ChangeToWaitingForBite();
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

					audioEngine.LoadEvent("FishAway", "{ddc97946-a268-4b10-a950-5237c4225be4}");
					audioEngine.PlayEvent("FishAway");

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

					audioEngine.LoadEvent("FishBite", "{2d2893a7-ba57-4e50-ba84-f228d204c456}");
					audioEngine.SetEventPosition("FishBite", bobberPos);
					audioEngine.PlayEvent("FishBite");

				}
			}

			break;
		}

		case GameState::Reeling:
		{
		
			float tension = 1.0f / (fishManager->activeFish->weight * 1.0f) + tensionTime; //multiply by players pull force

			if (TTK::Input::GetKeyDown(TTK::KeyCode::Space))
			{
				tensionTime += dt;
				audioEngine.SetEventParameter("Reeling", "Reeling", 0.0f);
				audioEngine.SetEventParameter("Reeling", "Tension", tension);

				glm::vec3 directionToPlayer = glm::normalize(fishManager->activeFish->position) * -1.0f;
				fishManager->activeFish->position += directionToPlayer * 8.0f * dt;

				if (fishManager->activeFish->stamina <= 0)
				{
					tension = 0.0f;
				}
				else if (tension >= maxTension)
				{
					audioEngine.LoadEvent("LineBreak", "{3577e4a9-b01b-4702-95bd-f17129fdf21c}");
					audioEngine.PlayEvent("LineBreak");

					audioEngine.LoadEvent("FishAway", "{ddc97946-a268-4b10-a950-5237c4225be4}");
					audioEngine.PlayEvent("FishAway");

					audioEngine.StopEvent("Reeling");

					waitBeforeCastingCountdown = 5.0f;
					ChangeToWaitingToCast();
				}
			}
			else
			{
				audioEngine.SetEventParameter("Reeling", "Swimming", 1.0f);

				if (fishManager->activeFish->stamina > 0)
				{
					fishManager->activeFish->position += fishManager->activeFish->moveDirection * fishManager->activeFish->speed * dt;
					audioEngine.SetEventParameter("Reeling", "Swimming", 0.0f);
				}

				audioEngine.SetEventParameter("Reeling", "Reeling", 1.0f);

				audioEngine.SetEventParameter("Reeling", "Tension", tension);
				tensionTime -= dt;
				if (tensionTime < 0)
					tensionTime = 0;
			}

			//check if fish is very close to the player
			if (glm::length(fishManager->activeFish->position) < 0.2f)
			{
				ChangeToFishCaught();
			}

			break;
		}

		case GameState::FishCaught:
		{
			audioEngine.StopEvent("Reeling");

			audioEngine.LoadEvent("FishCaught", "{82fd2cf2-8ad4-4952-83a0-9606197aff7b}");
			audioEngine.PlayEvent("FishCaught");

			std::cout << "\nCaught a fish! Rarity: ";
			switch (fishManager->activeFish->rarity)
			{
				case Fish::Rarity::Common:
				{
					std::cout << "COMMON";
					break;
				}
				case Fish::Rarity::Uncommon:
				{
					std::cout << "UNCOMMON";
					break;
				}
				case Fish::Rarity::Rare:
				{
					std::cout << "RARE!";
					break;
				}
				case Fish::Rarity::SuperRare:
				{
					std::cout << "SUPER RARE!!";
					break;
				}
				case Fish::Rarity::ExtremelyRare:
				{
					std::cout << "EXTREMELY RARE!!!";
					break;
				}
			}

			waitBeforeCastingCountdown = 5.0f;
			ChangeToWaitingToCast();
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
	isCasting     = true;

	//cast forward, with the x direction being random
	castDirection = glm::vec3(
		rand() % 100,
		0.0f,
		100);

	if (rand() % 2)
		castDirection.x *= -1;

	castDirection = glm::normalize(castDirection);


	//set bobber position
	bobberPos = castDirection * (float)castDistance;
	bobberPos.y = -1.0f; //put the bobber a bit below the player since that's how it would normally be when fishing

	audioEngine.SetEventParameter("Background", "Cast", 1.0f);
	audioEngine.LoadEvent("Casting", "{141eef9f-0ca3-42a1-82da-2a39f444e73c}");
	audioEngine.PlayEvent("Casting");

}

void Game::ChangeToWaitingToCast()
{
	gameState = GameState::WaitingToCast;

	castDistance  = rand() % (MAX_CAST_DISTANCE - MIN_CAST_DISTANCE + 1) + MIN_CAST_DISTANCE;
	castCountdown = (float)castDistance / 4.0f;

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
	gameState = GameState::Reeling;

	audioEngine.LoadEvent("Reeling", "{22cfbe7c-99c6-4a5d-936d-52c58bd9be59}"); //almost everything is handled inside of fmod
	audioEngine.PlayEvent("Reeling");

	tensionTime = 0.0f;
}

void Game::ChangeToFishCaught()
{
	gameState = GameState::FishCaught;
}