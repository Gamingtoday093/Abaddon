#include "pch.h"
#include "LightingVisualizer.h"
#include "Scene/Components/Components.h"
#include "Scene/ModelAssetHandler.h"

void LightingVisualizer::Init(const std::string& aStandardMaterial)
{
	myStandardMaterial = aStandardMaterial;
}

void LightingVisualizer::Start()
{
	GetComponent<TransformComponent>().myTransform.myScale = { 0.4f, 2, 0.4f };
}

void LightingVisualizer::Update()
{
	StandardMaterial& material = ModelAssetHandler::GetMaterial<StandardMaterial>(myStandardMaterial);
	math::vector3<float> rotation = math::vector4<float>().LookRotation(material.GetLightingDirection(), math::vector3<float>::up()).ToEuler();
	TransformComponent& transform = GetComponent<TransformComponent>();
	transform.myTransform.myRotation = rotation;
}
