#include "pch.h"
#include "StandardMaterial.h"

void StandardMaterial::Init(math::vector4<float> aColor, float aRoughness, TextureData aMainTexture, TextureData aEmissionTexture)
{
	myMaterialSettings.myData.Color = DirectX::XMVectorSet(aColor.x, aColor.y, aColor.z, aColor.w);
	myMaterialSettings.myData.Roughness = aRoughness;
	myMaterialSettings.ApplyChanges();

	myMainTexture = aMainTexture;
	myEmissionTexture = aEmissionTexture;
}

void StandardMaterial::Bind()
{
	Material::Bind();

	myMaterialSettings.Bind(1);
	myLighting.Bind(2);

	myMainTexture.mySRV.Bind(0);
	myMainTexture.mySampler.Bind(0);
	myEmissionTexture.mySRV.Bind(1);
	myEmissionTexture.mySampler.Bind(1);
}

void StandardMaterial::UpdateLighting(math::vector4<float> aColor, float aStrength, math::vector3<float> aDirection)
{
	aDirection.Normalize();

	myLighting.myData.LightColor = DirectX::XMVectorSet(aColor.x, aColor.y, aColor.z, aColor.w);
	myLighting.myData.LightStrength = aStrength;
	myLighting.myData.LightDirection = DirectX::XMVectorSet(aDirection.x, aDirection.y, aDirection.z, 0);
	myLighting.ApplyChanges();
}

math::vector3<float> StandardMaterial::GetLightingDirection()
{
	return { DirectX::XMVectorGetX(myLighting.myData.LightDirection), DirectX::XMVectorGetY(myLighting.myData.LightDirection), DirectX::XMVectorGetZ(myLighting.myData.LightDirection) };
}

void StandardMaterial::UpdateLightingDirection(math::vector3<float> aNewDirection)
{
	aNewDirection.Normalize();
	myLighting.myData.LightDirection = DirectX::XMVectorSet(aNewDirection.x, aNewDirection.y, aNewDirection.z, 0);
	myLighting.ApplyChanges();
}
