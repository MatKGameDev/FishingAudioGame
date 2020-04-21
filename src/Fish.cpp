#include "Fish.h"

Fish::Fish(FishType fishType, glm::vec3 startPosition) : type(fishType), position(startPosition)
{
	moveDirection = glm::vec3(
		rand() % 9 + 1, 
		0.0f, 
		-rand() % 9 + 1);

	moveDirection = glm::normalize(moveDirection);

	if (rand() % 2)
		moveDirection.x *= -1;

	switch (fishType)
	{
		case FishType::Sunfish:
		{
			weight      = 0.53f;
			stamina     = 2.0f;
			escapeTime  = 1.5f;
			minDistance = 0;

			break;
		}

		case FishType::RockBass:
		{
			weight      = 0.82f;
			stamina     = 5.0f;
			escapeTime  = 1.5f;
			minDistance = 0;

			break;
		}

		case FishType::YellowPerch:
		{
			weight      = 1.1f;
			stamina     = 5.0f;
			escapeTime  = 1.3f;
			minDistance = 0;

			break;
		}

		case FishType::SmallmouthBass:
		{
			weight      = 2.0f;
			stamina     = 7.0f;
			escapeTime  = 1.3f;
			minDistance = 3;

			break;
		}

		case FishType::LargemouthBass:
		{
			weight      = 3.5f;
			stamina     = 9.0f;
			escapeTime  = 1.1f;
			minDistance = 6;

			break;
		}

		case FishType::Salmon:
		{
			weight      = 6.0f;
			stamina     = 10.0f;
			escapeTime  = 0.8f;
			minDistance = 8;

			break;
		}

		case FishType::Pike:
		{
			weight      = 7.0f;
			stamina     = 12.0f;
			escapeTime  = 0.8f;
			minDistance = 8;

			break;
		}

		case FishType::Musky:
		{
			weight      = 8.0f;
			stamina     = 12.0f;
			escapeTime  = 0.5f;
			minDistance = 10;

			break;
		}

		case FishType::Sturgeon:
		{
			weight      = 12.0f;
			stamina     = 15.0f;
			escapeTime  = 0.3f;
			minDistance = 13;

			break;
		}
	}
}

void Fish::Update(float dt)
{

}