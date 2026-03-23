#pragma once
#include "Script.hpp"
#include "Graphics/Bindables/Materials/StandardMaterial.h"

class LightingVisualizer : public Script
{
	SCRIPT_NAME(LightingVisualizer)
public:
	void Init(const std::string& aStandardMaterial);

	void Start() override;
	void Update() override;

private:
	std::string myStandardMaterial;
};

