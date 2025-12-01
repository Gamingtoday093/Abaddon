#include "pch.h"
#include "SelectCircleMaterial.h"

void SelectCircleMaterial::SetColor(math::vector3<float> aColor)
{
	myMaterialSettings.myData.Color = DirectX::XMVectorSet(aColor.x, aColor.y, aColor.z, 1);
	myMaterialSettings.ApplyChanges();
}

void SelectCircleMaterial::AddCirclePosition(math::vector3<float> aCirclePosition)
{
	myMaterialSettings.myData.CirclePositions[myNextCirclePositionIndex] = DirectX::XMVectorSet(aCirclePosition.x, aCirclePosition.y, aCirclePosition.z, 1);
	myNextCirclePositionIndex++;
}

void SelectCircleMaterial::ApplyCirclePositions()
{
	myMaterialSettings.myData.CirclePositionsCount = myNextCirclePositionIndex;
	myNextCirclePositionIndex = 0;
	myMaterialSettings.ApplyChanges();
}

void SelectCircleMaterial::Bind()
{
	Material::Bind();

	myMaterialSettings.Bind(1);
}
