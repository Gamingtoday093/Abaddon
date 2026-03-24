#pragma once
#include "Material.hpp"
#include "Graphics/CBufferStructs.h"
#include "Graphics/Bindables/Bindables.h"

class SelectCircleMaterial : public Material
{
public:
	SelectCircleMaterial();

	void SetColor(math::vector3<float> aColor);
	void AddCirclePosition(math::vector3<float> aCirclePosition);
	void ApplyCirclePositions();

	void Bind() const override;

private:
	int myNextCirclePositionIndex;
	CBuffer<SelectCircleBuffer> myMaterialSettings;
};

