#pragma once

#include <glm/glm.hpp>

class Fish
{
public:
	enum FishType
	{
		Sunfish,
		RockBass,
		YellowPerch,
		SmallmouthBass,
		LargemouthBass,
		Salmon,
		Pike,
		Musky,
		Sturgeon
	};

	static const int NUM_FISH_TYPES = 9;

	Fish(FishType fishType, glm::vec3 startPosition);

	void Update(float dt);

	FishType type;

	float weight;
	float stamina;
	float escapeTime;
	float speed;
	int minDistance;

	glm::vec3 moveDirection;
	glm::vec3 position;
};