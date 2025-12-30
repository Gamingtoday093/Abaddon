#pragma once
#include "Script.hpp"
#include "Graphics/Bindables/Materials/StandardMaterial.h"

class LightingVisualizer : public Script
{
public:
	void Init(const std::string& aStandardMaterial);

	void Start() override;
	void Update() override;

private:
	std::string myStandardMaterial;
};

