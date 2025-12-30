#pragma once
#include <chrono>

class Unit;

class Weapon
{
public:
	Weapon(Unit& aUnit);

	int myDamage;

	float myFirerate;
	std::chrono::steady_clock::time_point mylastFired;

	float myFiringRange;

	float myFiringAngle;

	math::vector3<float> myBarrel;
	
	bool Fire(Unit* aTargetUnit) const;

private:
	Unit& myUnit;
};