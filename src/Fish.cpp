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

	speed = 5.0f;

	switch (fishType)
	{
		case FishType::Sunfish:
		{
			weight      = 0.53f;
			stamina     = 2.0f;
			escapeTime  = 1.5f;
			minDistance = 0;

			rarity = Rarity::Common;

			break;
		}

		case FishType::RockBass:
		{
			weight      = 0.82f;
			stamina     = 5.0f;
			escapeTime  = 1.5f;
			minDistance = 0;

			rarity = Rarity::Common;

			break;
		}

		case FishType::YellowPerch:
		{
			weight      = 1.1f;
			stamina     = 5.0f;
			escapeTime  = 1.3f;
			minDistance = 0;

			rarity = Rarity::Common;

			break;
		}

		case FishType::SmallmouthBass:
		{
			weight      = 2.0f;
			stamina     = 7.0f;
			escapeTime  = 1.3f;
			minDistance = 3;

			rarity = Rarity::Uncommon;

			break;
		}

		case FishType::LargemouthBass:
		{
			weight      = 3.5f;
			stamina     = 9.0f;
			escapeTime  = 1.1f;
			minDistance = 6;

			rarity = Rarity::Rare;

			break;
		}

		case FishType::Salmon:
		{
			weight      = 6.0f;
			stamina     = 10.0f;
			escapeTime  = 0.8f;
			minDistance = 8;

			rarity = Rarity::SuperRare;

			break;
		}

		case FishType::Pike:
		{
			weight      = 7.0f;
			stamina     = 12.0f;
			escapeTime  = 0.8f;
			minDistance = 8;

			rarity = Rarity::SuperRare;

			break;
		}

		case FishType::Musky:
		{
			weight      = 8.0f;
			stamina     = 12.0f;
			escapeTime  = 0.5f;
			minDistance = 10;

			rarity = Rarity::ExtremelyRare;

			break;
		}

		case FishType::Sturgeon:
		{
			weight      = 12.0f;
			stamina     = 15.0f;
			escapeTime  = 0.3f;
			minDistance = 13;

			rarity = Rarity::ExtremelyRare;

			break;
		}
	}
}

void Fish::Update(float dt)
{
	stamina -= dt;

	position += speed * moveDirection * dt;
}