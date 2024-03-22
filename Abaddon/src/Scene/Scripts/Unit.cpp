#include "pch.h"
#include "Unit.h"
#include "Scene/ModelAssetHandler.h"
#include "Graphics/Renderer.h"

void Unit::Select(bool aNewSelected)
{
	isSelected = aNewSelected;
	if (aNewSelected) LOG("Selected!");
	else LOG("Unselected!");
}

bool Unit::GetIsSelected()
{
	return isSelected;
}

void Unit::MoveTo(math::vector3<float> aTargetPosition)
{
	myTargetPosition = aTargetPosition;
	mySpeed = 0;
}

void Unit::Init(Renderer& aRenderer)
{
	myRenderer = &aRenderer;
}

void Unit::Start()
{
	myTargetPosition = GetComponent<TransformComponent>().myTransform.myPosition;
	const AABB myAABB = ModelAssetHandler::GetModelData(GetComponent<ModelComponent>().myModelName).myAABB;
	const math::vector3<float> AABBSize = myAABB.GetSize();
	const float size = std::max(AABBSize.x, AABBSize.z) * 1.5f;
	mySelectionTransform.myScale = { size, 1, size };
	mySelectionPositionOffset = myAABB.GetCenter();
	mySelectionPositionOffset.y = 0;
}

void Unit::Update()
{
	if (GetIsSelected())
	{
		mySelectionTransform.myPosition = GetComponent<TransformComponent>().myTransform.myPosition;
		myRenderer->Render(ModelAssetHandler::GetModelData("SelectionCircle.fbx"), ModelAssetHandler::GetTextureData("sand.jpg"), mySelectionTransform, myEntity.GetScene().GetCamera());
	}
	UpdateMovement();
}

void Unit::UpdateMovement()
{
	Transform& myTransform = GetComponent<TransformComponent>().myTransform;

	if ((myTargetPosition - myTransform.myPosition).LengthSqr() > 25)
	{
		auto moveDirection = (myTargetPosition - myTransform.myPosition).GetNormalized();
		auto quatlook = math::vector4<float>::LookRotation(moveDirection, math::vector3<float>::up());

		myTargetRotation = XMVectorSet(quatlook.x, quatlook.y, quatlook.z, quatlook.w);

		auto myQuaternionRotation = XMQuaternionRotationRollPitchYaw(myTransform.myRotation.x, myTransform.myRotation.y, myTransform.myRotation.z);

		math::vector4<float> quat = { XMVectorGetX(myQuaternionRotation), XMVectorGetY(myQuaternionRotation), XMVectorGetZ(myQuaternionRotation), XMVectorGetW(myQuaternionRotation) };

		math::vector3<float> myForward = quat.Rotate({ 0, 0, 1 }).GetNormalized();

		const float angleBetween = myForward.AngleBetween(moveDirection);

		const float angleMultiplier = 0.1f / (myForward.AngleBetween(moveDirection) / XM_PI);

		if (angleBetween < XM_PIDIV2 && (myTargetPosition - myTransform.myPosition).LengthSqr() > 250 || angleBetween < 0.4)
		{
			mySpeed += myAcceleration;
			if (mySpeed > myMaxSpeed) mySpeed = myMaxSpeed;

			if ((myTargetPosition - myTransform.myPosition).LengthSqr() > 100)
				myTransform.myPosition += myForward * mySpeed;
			else
				myTransform.myPosition += moveDirection * mySpeed;
		}

		myQuaternionRotation = XMQuaternionSlerp(myQuaternionRotation, myTargetRotation, std::min(std::max(0.035f, 0.02f), 0.05f));

		quat = { XMVectorGetX(myQuaternionRotation), XMVectorGetY(myQuaternionRotation), XMVectorGetZ(myQuaternionRotation), XMVectorGetW(myQuaternionRotation) };
		myTransform.myRotation = quat.ToEuler();
	}
	else if (mySpeed > 0)
	{
		mySpeed -= myAcceleration;
		if (mySpeed < 0) mySpeed = 0;

		auto moveDirection = (myTargetPosition - myTransform.myPosition).GetNormalized();
		myTransform.myPosition += moveDirection * mySpeed;
	}
}
