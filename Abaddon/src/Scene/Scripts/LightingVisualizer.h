#pragma once
#include "Script.h"
#include "Graphics/Bindables/Materials/StandardMaterial.h"

class LightingVisualizer : public Script
{
public:
	void Init(std::string aStandardMaterial);

	void Start() override;
	void Update() override;

private:
	std::string myStandardMaterial;
};

