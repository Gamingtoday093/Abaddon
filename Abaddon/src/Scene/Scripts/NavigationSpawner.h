#pragma once
#include "Script.hpp"

class NavigationSpawner : public Script
{
	SCRIPT_NAME(NavigationSpawner)
public:
	void Start() override;

	size_t myAgentRowCount = 5;
};

