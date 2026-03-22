#pragma once
#include "Scene/Scripts/Script.hpp"
#include "../Models/SpatialHash.h"

class NavigationAgent;

class NavigationManager : public Script
{
public:
	void Awake() override;
	void Update() override;

	static math::vector3<float> CalculateTotalAvoidanceVelocity(NavigationAgent& aAgent, const std::vector<NavigationAgent*>& aAgents);

private:
	static NavigationManager& GetInstance();
	static NavigationManager* myInstance;

	SpatialHash mySpatialHash{12}; // Should be larger than the average Agents Radius but smaller than the average Agents Radius * 2
	std::vector<NavigationAgent*> myAgents;

	friend class NavigationAgent;
};

