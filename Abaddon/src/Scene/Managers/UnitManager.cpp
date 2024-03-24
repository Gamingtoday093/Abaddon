#include "pch.h"
#include "UnitManager.h"
#include "Scene/ModelAssetHandler.h"

void UnitManager::Awake()
{
    ModelAssetHandler::LoadModel("SelectionCircle.fbx");
	ModelAssetHandler::LoadTexture("SelectionTexture.png");
    myUnits = std::make_shared<std::vector<Unit*>>();
    myInstance = this;
}

void UnitManager::Update()
{
	HandleInput();
}

Unit* UnitManager::CreateUnit(Entity& entity)
{
    Unit* unit = static_cast<Unit*>(entity.AddComponent<ScriptComponent>().Bind<Unit>(entity));
    GetInstance().myUnits->push_back(unit);
    return unit;
}

const std::shared_ptr<std::vector<Unit*>> UnitManager::GetUnits()
{
    return GetInstance().myUnits;
}

UnitManager* UnitManager::myInstance = nullptr;

UnitManager& UnitManager::GetInstance()
{
    return *myInstance;
}

void UnitManager::HandleInput()
{
	if (Input::GetInstance().IsMouseButtonPressed((int)eKeys::MOUSERBUTTON))
	{
		InputMoveTo();
	}

	if (Input::GetInstance().IsMouseButtonDown((int)eKeys::MOUSELBUTTON) && !dragging)
	{
		dragSelectStart = Camera::MousePositionToCameraSpace(myEntity.GetScene().myHWND);
		dragging = true;
	}

	if (Input::GetInstance().IsMouseButtonUp((int)eKeys::MOUSELBUTTON))
	{
		InputSelect();
	}
}

void UnitManager::InputMoveTo()
{
	const math::vector3<float> rayOrigin = myEntity.GetScene().GetCamera()->CameraSpaceToWorldSpace(Camera::MousePositionToCameraSpace(myEntity.GetScene().myHWND));
	const math::vector3<float> rayDirection = (rayOrigin - myEntity.GetScene().GetCamera()->GetPosition()).GetNormalized();

	const math::vector3<float> normal = math::vector3<float>::up();

	const float normalDotRayDirectionMaybeZero = normal.Dot(rayDirection);

	if (normalDotRayDirectionMaybeZero == 0) return;

	const float t = -(normal.Dot(rayOrigin)) / normalDotRayDirectionMaybeZero;

	if (t <= 0) return;
	
	math::vector3<float> hitPosition = rayOrigin + (rayDirection * t);

	for (const auto unit : *myUnits)
	{
		if (unit->GetIsSelected())
			unit->MoveTo(hitPosition);
	}
}

void UnitManager::InputSelect()
{
	math::vector2<float> dragSelectEnd = Camera::MousePositionToCameraSpace(myEntity.GetScene().myHWND);
	dragging = false;

	if ((dragSelectEnd - dragSelectStart).LengthSqr() > 0.01f) // Drag
	{
		int selected = 0;

		for (const auto unit : *myUnits)
		{
			math::vector2<float> unitPosition = myEntity.GetScene().GetCamera()->WorldSpaceToCameraSpace(unit->GetComponent<TransformComponent>().myTransform.myPosition);

			unit->Select(unitPosition.x > std::min(dragSelectStart.x, dragSelectEnd.x) && unitPosition.y > std::min(dragSelectStart.y, dragSelectEnd.y) &&
				unitPosition.x < std::max(dragSelectStart.x, dragSelectEnd.x) && unitPosition.y < std::max(dragSelectStart.y, dragSelectEnd.y));

			if (!multipleSelected && unit->GetIsSelected())
				selected++;
		}

		multipleSelected = selected > 1;
	}
	else // Click
	{
		math::vector3<float> rayOrigin = myEntity.GetScene().GetCamera()->CameraSpaceToWorldSpace(Camera::MousePositionToCameraSpace(myEntity.GetScene().myHWND));
		math::vector3<float> rayDirection = (rayOrigin - myEntity.GetScene().GetCamera()->GetPosition()).GetNormalized();

		int selected = 0;

		for (const auto unit : *myUnits)
		{
			AABB transformedAABB = unit->GetComponent<TransformComponent>().TransformAABB(ModelAssetHandler::GetModelData(unit->GetComponent<ModelComponent>().myModelName).myAABB);
			if (Input::GetInstance().IsKeyDown((int)eKeys::SHIFT))
				unit->Select(transformedAABB.RayBounds(rayOrigin, rayDirection) != unit->GetIsSelected());
			else
				unit->Select(transformedAABB.RayBounds(rayOrigin, rayDirection) && (!unit->GetIsSelected() || multipleSelected));

			if (!multipleSelected && unit->GetIsSelected())
				selected++;
		}

		multipleSelected = selected > 1;
	}
}
