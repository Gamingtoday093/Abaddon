#pragma once
#include "Material.hpp"
#include "Graphics/CBufferStructs.h"
#include "Graphics/Bindables/Bindables.h"

class ColorMaterial : public Material
{
public:
	ColorMaterial();

	void Init(math::vector3<float> aColor);
	void Bind() const override;

private:
	CBuffer<ColorBuffer> myMaterialSettings;
};

