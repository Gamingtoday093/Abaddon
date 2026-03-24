#pragma once
#include "Material.hpp"
#include "Graphics/CBufferStructs.h"
#include "Graphics/Bindables/Bindables.h"

class StandardMaterial : public Material
{
public:
	StandardMaterial();

	void Init(math::vector4<float> aColor, float aRoughness, TextureData aMainTexture, TextureData aEmissionTexture);
	void Bind() const override;

	void UpdateLighting(math::vector4<float> aColor, float aStrength, math::vector3<float> aDirection);
	math::vector3<float> GetLightingDirection() const;
	void UpdateLightingDirection(math::vector3<float> aNewDirection);

private:
	CBuffer<MaterialBuffer> myMaterialSettings;
	CBuffer<LightingBuffer> myLighting;
	TextureData myMainTexture;
	TextureData myEmissionTexture;
};

