#pragma once
#include "Material.hpp"
#include "Graphics/CBufferStructs.h"
#include "Graphics/Bindables/Bindables.h"

class ColorMaterial : public Material
{
public:
	ColorMaterial() : Material("ColorShader_ps.cso")
	{
		myMaterialSettings.Init(eBindType::pixelShader);
	}

	void Init(math::vector3<float> aColor);
	void Bind() override;

private:
	CBuffer<ColorBuffer> myMaterialSettings;
};

