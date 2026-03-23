#pragma once
#include "Scene/Components/Components.h"
#include "Log.h"

class PlayerMovement : public Script
{
	SCRIPT_NAME(PlayerMovement)
public:
	PlayerMovement() = default;

	void Start() override;
	void OnDestroy() override;
	void Update() override;

private:
	float mySpeed;
};