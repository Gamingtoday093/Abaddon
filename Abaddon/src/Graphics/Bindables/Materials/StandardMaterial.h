#pragma once
#include "Material.hpp"
#include "Graphics/CBufferStructs.h"
#include "Graphics/Bindables/Bindables.h"

class StandardMaterial : public Material
{
public:
	StandardMaterial() : Material("PixelShader_ps.cso")
	{
		myMaterialSettings.Init(eBindType::pixelShader);
		myLighting.Init(eBindType::pixelShader);
	}

	void Init(math::vector4<float> aColor, float aRoughness, TextureData aMainTexture, TextureData aEmissionTexture);
	void Bind() override;

	void UpdateLighting(math::vector4<float> aColor, float aStrength, math::vector3<float> aDirection);
	math::vector3<float> GetLightingDirection();
	void UpdateLightingDirection(math::vector3<float> aNewDirection);

private:
	CBuffer<MaterialBuffer> myMaterialSettings;
	CBuffer<LightingBuffer> myLighting;
	TextureData myMainTexture;
	TextureData myEmissionTexture;
};

