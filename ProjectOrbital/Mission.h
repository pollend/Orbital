#pragma once

class Mission
{
private:
	int* resourceCount;					// amount of resources currently obtained from player of each type (10 is arbitrary)
	int* resourcesNeeded;				// current goal resource count of each type

public:
	Mission();
	~Mission();

	void Load();
	void AddToResourceCount(int index, int amount);
	void SetResourcesNeeded(int index, int count);
	bool GetIsMissionComplete();
	
	int GetResourceCount(int index)		  { return resourceCount[index]; }
	int GetResourcesNeeded(int index)	  { return resourcesNeeded[index]; }	
	int GetResourcesCurrNeeded(int index) { return (resourcesNeeded[index] - resourceCount[index]); }
};