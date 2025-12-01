#include "pch.h"
#include "ColorMaterial.h"

void ColorMaterial::Init(math::vector3<float> aColor)
{
	myMaterialSettings.myData.Color = DirectX::XMVectorSet(aColor.x, aColor.y, aColor.z, 1);
	myMaterialSettings.ApplyChanges();
}

void ColorMaterial::Bind()
{
	Material::Bind();

	myMaterialSettings.Bind(1);
}
