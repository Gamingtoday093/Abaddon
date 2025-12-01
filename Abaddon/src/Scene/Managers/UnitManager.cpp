#include "pch.h"
#include "UnitManager.h"
#include "Scene/ModelAssetHandler.h"
#include "Scene/Components/Components.h"

#include "Graphics/Renderer.h"
#include "Graphics/Bindables/Materials/Materials.h"

void UnitManager::Awake()
{
    ModelAssetHandler::LoadModel("SelectionCircle.fbx");
	ModelAssetHandler::LoadTexture("SelectionTexture.png");
	ColorMaterial& material = ModelAssetHandler::CreateMaterial<ColorMaterial>("SelectionCircle");
	material.Init({ 0.25f,0.75f,0.125f });

	SelectCircleMaterial& material2 = ModelAssetHandler::CreateMaterial<SelectCircleMaterial>("SelectionCircle2");
	material2.SetColor({ 0.25f,0.75f,0.125f });

	myMappedUnits = {};
	myUnits = {};
    myInstance = this;
}

void UnitManager::Update()
{
	HandleInput();

	SelectCircleMaterial& material = ModelAssetHandler::GetMaterial<SelectCircleMaterial>("SelectionCircle2");
	for (const auto unit : myUnits)
	{
		if (unit->GetIsSelected())
			material.AddCirclePosition(unit->GetComponent<TransformComponent>().myTransform.myPosition);
	}
	material.ApplyCirclePositions();
}

Unit* UnitManager::CreateUnit(Entity& entity)
{
    Unit* unit = entity.AddComponent<ScriptComponent>().Bind<Unit>(entity);
    GetInstance().myUnits.push_back(unit);
    return unit;
}

const std::vector<Unit*>& UnitManager::GetUnits()
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
	// Interact
	if (Input::GetInstance().IsMouseButtonPressed((int)eKeys::MOUSERBUTTON))
	{
		const math::vector3<float> rayOrigin = myEntity.GetScene().GetCamera()->CameraSpaceToWorldSpace(Camera::MousePositionToCameraSpace(myEntity.GetScene().myHWND));
		const math::vector3<float> rayDirection = (rayOrigin - myEntity.GetScene().GetCamera()->GetPosition()).GetNormalized();

		const math::vector3<float> normal = math::vector3<float>::up();

		const float normalDotRayDirectionMaybeZero = normal.Dot(rayDirection);

		if (normalDotRayDirectionMaybeZero == 0) return;

		const float t = -(normal.Dot(rayOrigin)) / normalDotRayDirectionMaybeZero;

		if (t <= 0) return;

		const math::vector3<float> hitPosition = rayOrigin + (rayDirection * t);

		InputMoveTo(rayOrigin, rayDirection, hitPosition);
	}

	// Mouse Selection
	if (Input::GetInstance().IsMouseButtonDown((int)eKeys::MOUSELBUTTON) && !dragging)
	{
		dragSelectStart = Camera::MousePositionToCameraSpace(myEntity.GetScene().myHWND);
		dragging = true;
	}

	if (Input::GetInstance().IsMouseButtonUp((int)eKeys::MOUSELBUTTON))
	{
		InputSelectMouse();
	}

	// Keyboard Selection
	if (Input::GetInstance().IsKeyDown((int)eKeys::CONTROL))
	{
		for (size_t key = 0; key < 10; key++)
		{
			int keyCode = (int)eKeys::Alpha0 + key;
			if (!Input::GetInstance().IsKeyDown(keyCode)) continue;

			myMappedUnits.try_emplace(keyCode, std::vector<Unit*>{});
			myMappedUnits.at(keyCode).clear();

			// Reassign all Selected Units to new Mapped Units Group
			for (const auto unit : myUnits)
			{
				if (!unit->GetIsSelected()) continue;

				RemoveMappedUnit(unit);
				myMappedUnits.at(keyCode).push_back(unit);
			}
			break;
		}
	}
	else
	{
		for (const auto& pair : myMappedUnits)
		{
			if (!Input::GetInstance().IsKeyPressed(pair.first)) continue;
			if (pair.second.size() == 0) continue;

			for (const auto unit : myUnits)
			{
				unit->Select(false);
			}

			for (const auto unit : pair.second)
			{
				unit->Select(true);
			}

			multipleSelected = pair.second.size() > 1;
			break;
		}
	}
}

void UnitManager::RemoveMappedUnit(Unit* unit)
{
	for (auto& pair : myMappedUnits)
	{
		for (int u = pair.second.size() - 1; u >= 0; u--)
		{
			if (pair.second.at(u) != unit) continue;

			pair.second.erase(std::next(pair.second.begin(), u));
			return;
		}
	}
}

void UnitManager::InputMoveTo(const math::vector3<float> aRayOrigin, const math::vector3<float> aRayDirection, const math::vector3<float> aNewPosition)
{
	Unit* hitUnit = nullptr;

	for (const auto unit : myUnits)
	{
		AABB transformedAABB = unit->GetComponent<TransformComponent>().TransformAABB(ModelAssetHandler::GetModelData(unit->GetComponent<ModelComponent>().myModelName).myAABB);
	
		if (transformedAABB.RayBounds(aRayOrigin, aRayDirection))
			hitUnit = unit;
	}

	float offset = 0;
	math::vector3<float> forwardTo;
	math::vector3<float> right;
	for (const auto unit : myUnits)
	{
		if (!unit->GetIsSelected())	continue;

		if (hitUnit && hitUnit != unit)
			unit->Attack(hitUnit);
		else
		{
			unit->ClearTargetUnit();
			if (offset == 0)
			{
				auto towardsTarget = (aNewPosition - unit->GetComponent<TransformComponent>().myTransform.myPosition);
				if (towardsTarget.LengthSqr() == 0)
				{
					const auto myRotationQuaternion = math::vector4<float>::RotateAngleAxis(unit->GetComponent<TransformComponent>().myTransform.myRotation.y, math::vector3<float>::up());
					const auto myForward = myRotationQuaternion.Rotate(math::vector3<float>::forward());

					towardsTarget = myForward;
				}
				else
				{
					towardsTarget.Normalize();
				}

				forwardTo = towardsTarget;

				right = towardsTarget.Cross(math::vector3<float>::up());
			}

			unit->MoveTo((right * offset * 20.f * (fmodf(offset, 2.f) == 0 ? -1.f : 1.f)) + aNewPosition, forwardTo);
			offset++;
		}
	}
}

void UnitManager::InputSelectMouse()
{
	math::vector2<float> dragSelectEnd = Camera::MousePositionToCameraSpace(myEntity.GetScene().myHWND);
	dragging = false;

	if ((dragSelectEnd - dragSelectStart).LengthSqr() > 0.01f) // Drag
	{
		int selected = 0;

		for (const auto unit : myUnits)
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

		for (const auto unit : myUnits)
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
