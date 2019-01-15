#include "Mission.h"
#include "../source/game/PlayerShip.h"
#include "OrbitsMath.h"
#include <iostream>

#define MAX_RESOURCE_NEEDED 50
#define MIN_RESOURCE_NEEDED 5

Mission::Mission() {
	resourceCount	= new int[NUM_RESOURCE_TYPES];
	resourcesNeeded = new int[NUM_RESOURCE_TYPES];
}

Mission::~Mission() {
	delete resourceCount;
	delete resourcesNeeded;

	resourceCount = 0;
	resourcesNeeded = 0;
}

void Mission::Load() {	
	for(int i = 0; i < NUM_RESOURCE_TYPES; ++i)
	{
		resourceCount[i] = 0;

		float random = randFloat();

		resourcesNeeded[i] = MAX_RESOURCE_NEEDED * random * RESOURCE_RATIO[i] + MIN_RESOURCE_NEEDED;

		std::cout << "Resource " << i << ": Needs " << resourcesNeeded[i] << std::endl;
	}
}

bool Mission::GetIsMissionComplete() {
	for(int i = 0; i < NUM_RESOURCE_TYPES; ++i)
	{
		if(resourceCount[i] < resourcesNeeded[i])
			return false;
	}

	return true;
}

void Mission::AddToResourceCount(int index, int amount) {
	resourceCount[index] += amount;
}

void Mission::SetResourcesNeeded(int index, int count) {
	resourcesNeeded[index] = count;
}