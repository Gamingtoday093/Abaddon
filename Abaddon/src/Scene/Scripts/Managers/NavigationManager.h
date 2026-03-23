#pragma once
#include "Scene/Scripts/Script.hpp"
#include "../Models/SpatialHash.h"
#include "RVO.h"

class NavigationAgent;

class NavigationManager : public Script
{
	SCRIPT_NAME(NavigationManager)
public:
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	static NavigationManager& GetInstance();
	static NavigationManager* myInstance;

	std::vector<NavigationAgent*> myAgents;
	std::unique_ptr<RVO::RVOSimulator> myRVOSimulator;

	friend class NavigationAgent;
};

