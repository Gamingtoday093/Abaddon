#pragma once
#include "Scene/Scripts/Script.hpp"

class NavigationAgent;

class NavigationManager : public Script
{
public:
	void Awake() override;
	void Update() override;

	math::vector3<float> CalculateTotalAvoidanceVelocity(NavigationAgent& aAgent);

private:
	static NavigationManager& GetInstance();
	static NavigationManager* myInstance;

	std::vector<NavigationAgent*> myAgents;

	friend class NavigationAgent;
};

