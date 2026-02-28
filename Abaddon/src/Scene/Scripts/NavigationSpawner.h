#pragma once
#include "Script.hpp"

class NavigationSpawner : public Script
{
public:
	void Start() override;

	size_t myAgentCount = 10;
};

