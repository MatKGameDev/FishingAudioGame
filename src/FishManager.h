#pragma once

#include <vector>
#include "Fish.h"

class FishManager
{
public:
	FishManager();
	~FishManager();

	void Update(float dt);

	void SpawnRandomFish(glm::vec3 position, int castDistance);

	Fish* activeFish;
};