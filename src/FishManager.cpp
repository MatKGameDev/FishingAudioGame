#include "FishManager.h"

FishManager::FishManager()
{
	activeFish = nullptr;
}

FishManager::~FishManager()
{
	delete activeFish;
}

void FishManager::Update(float dt)
{
	if (activeFish != nullptr)
		activeFish->Update(dt);
}

void FishManager::SpawnRandomFish(glm::vec3 position, int castDistance)
{
	int minFishDistance = 9001;

	while (minFishDistance > castDistance)
	{
		Fish::FishType newFishType = Fish::FishType(rand() % Fish::NUM_FISH_TYPES);

		activeFish = new Fish(newFishType, position);

		minFishDistance = activeFish->minDistance;
	}
}