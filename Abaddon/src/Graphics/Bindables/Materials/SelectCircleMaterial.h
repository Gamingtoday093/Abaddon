#pragma once
#include "Material.hpp"
#include "Graphics/CBufferStructs.h"
#include "Graphics/Bindables/Bindables.h"

class SelectCircleMaterial : public Material
{
public:
	SelectCircleMaterial() : Material("SelectCircleShader_ps.cso", eBlendState::Alpha)
	{
		myNextCirclePositionIndex = 0;
		myMaterialSettings.Init(eBindType::pixelShader);
	}

	void SetColor(math::vector3<float> aColor);
	void AddCirclePosition(math::vector3<float> aCirclePosition);
	void ApplyCirclePositions();

	void Bind() override;

private:
	int myNextCirclePositionIndex;
	CBuffer<SelectCircleBuffer> myMaterialSettings;
};

