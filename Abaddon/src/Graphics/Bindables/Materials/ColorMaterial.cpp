#include "pch.h"
#include "ColorMaterial.h"

ColorMaterial::ColorMaterial() : Material("ColorShader_ps.cso")
{
	myMaterialSettings.Init(eBindType::PixelShader);
}

void ColorMaterial::Init(math::vector3<float> aColor)
{
	myMaterialSettings.myData.Color = DirectX::XMVectorSet(aColor.x, aColor.y, aColor.z, 1);
	myMaterialSettings.ApplyChanges();
}

void ColorMaterial::Bind() const
{
	Material::Bind();

	myMaterialSettings.Bind(1);
}
