#pragma once
#include "Scene/Scripts/Script.hpp"
#include "../Models/SpatialHash.h"
#include "RVO.h"

class NavigationAgent;

class NavigationManager : public Script
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	static NavigationManager& GetInstance();
	static NavigationManager* myInstance;

	void NewUpdate();
	void OldUpdate();
	static math::vector3<float> CalculateTotalAvoidanceVelocity(NavigationAgent& aAgent, const std::vector<NavigationAgent*>& aAgents);

	SpatialHash mySpatialHash{12}; // Should be larger than the average Agents Radius but smaller than the average Agents Radius * 2
	std::vector<NavigationAgent*> myAgents;

	std::unique_ptr<RVO::RVOSimulator> myRVOSimulator;

	friend class NavigationAgent;
};

