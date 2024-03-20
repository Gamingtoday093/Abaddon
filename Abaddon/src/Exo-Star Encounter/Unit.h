#pragma once
#include "Graphics/Scene/Model.h"

class Unit : public Model
{
public:
	void Select(bool aNewSelected);
	bool GetIsSelected();
	void MoveTo(math::vector3<float> aTargetPosition);
	void Awake() override;
	void Update() override;

protected:
	int myHealth = 100;
	bool isSelected;
	float myAcceleration = 0.005f;
	float myMaxSpeed = 0.15f;
	float mySpeed = 0;

private:
	math::vector3<float> myTargetPosition;
	XMVECTOR myTargetRotation;
};
