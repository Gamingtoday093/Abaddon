#pragma once
#include "Script.hpp"

class NavigationAgent : public Script
{
public:
	void Awake() override;
	void OnDestroy() override;

	void Update() override;

	inline float GetRadius() const
	{
		return myRadius;
	}
	inline float GetInfluence() const
	{
		return myMass * myPriority;
	}

	// Velocity taking into account other NavigationAgents
	math::vector3<float> myVelocity;
	// Ideal Velocity
	math::vector3<float> myTargetVelocity;

	float myRadius = 8;

	float myMass = 10;
	float myPriority = 1;
};

