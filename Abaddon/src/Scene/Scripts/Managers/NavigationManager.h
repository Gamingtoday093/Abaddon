#pragma once
#include "Scene/Scripts/Script.hpp"
#include "../Models/SpatialHash.h"
#include "RVO.h"
#include "../Models/Navigation/NavGrid.h"

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
	Navigation::NavGrid myNavGrid;

	friend class NavigationAgent;
};

