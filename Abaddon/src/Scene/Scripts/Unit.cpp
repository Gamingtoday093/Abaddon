#include "pch.h"
#include "Unit.h"
#include "Scene/ModelAssetHandler.h"
#include "Graphics/Renderer.h"
#include "Graphics/Bindables/Materials/Materials.h"

void Unit::Select(bool aNewSelected)
{
	isSelected = aNewSelected;
	if (aNewSelected) LOG("Selected!");
}

bool Unit::GetIsSelected()
{
	return isSelected;
}

void Unit::MoveTo(math::vector3<float> aTargetPosition)
{
	myTargetPosition = aTargetPosition;
	//myTargetForward = math::vector3<float>::zero();
	myTargetForward = (aTargetPosition - GetComponent<TransformComponent>().myTransform.myPosition).GetNormalized();
}

void Unit::MoveTo(math::vector3<float> aTargetPosition, math::vector3<float> aTargetForward)
{
	myTargetPosition = aTargetPosition;
	myTargetForward = aTargetForward;
}

void Unit::Attack(Unit* aOtherUnit)
{
	LOG("ATTACKING!");

	myTargetUnit = aOtherUnit;

	Transform& myTransform = GetComponent<TransformComponent>().myTransform;
	Transform& otherTransform = myTargetUnit->GetComponent<TransformComponent>().myTransform;

	if ((otherTransform.myPosition - myTransform.myPosition).LengthSqr() < 1) return;

	auto towardsTarget = (otherTransform.myPosition - myTransform.myPosition).GetNormalized();

	if ((otherTransform.myPosition - myTransform.myPosition).LengthSqr() > 625)
		MoveTo(myTransform.myPosition + towardsTarget * ((otherTransform.myPosition - myTransform.myPosition).Length() - 25));
	else
		MoveTo(myTransform.myPosition + towardsTarget);
}

void Unit::ClearTargetUnit()
{
	myTargetUnit = nullptr;
}

void Unit::Damage(int aDamage)
{
	myHealth -= aDamage;
	LOG("THAT HURT! " + std::to_string(myHealth));
}

void Unit::Init(std::shared_ptr<Renderer> aRenderer)
{
	myRenderer = aRenderer;
}

void Unit::Awake()
{
	// Weapons

	// Smol Cannons = 3, 0.9f, 5.35f AND -3, 0.9f, 5.35f
	// Omega Cannons = 2, 0.9f, 6.5f AND -2, 0.9f, 6.5f

	Weapon bigCannon_0(*this);
	bigCannon_0.myDamage = 6;
	bigCannon_0.myFirerate = 1.7f;
	bigCannon_0.myFiringRange = 100;
	bigCannon_0.myFiringAngleX = XM_PIDIV2;
	bigCannon_0.myFiringAngleY = 0.2f;
	bigCannon_0.myBarrel = { 2, 0.9f, 6.5f };
	myWeapons.push_back(bigCannon_0);

	Weapon bigCannon_1(bigCannon_0);
	bigCannon_1.myBarrel = { -2, 0.9f, 6.5f };
	myWeapons.push_back(bigCannon_1);

	Weapon smallBlaster_0(*this);
	smallBlaster_0.myDamage = 1;
	smallBlaster_0.myFirerate = 0.4f;
	smallBlaster_0.myFiringRange = 50;
	smallBlaster_0.myFiringAngleX = XM_PIDIV2;
	smallBlaster_0.myFiringAngleY = 0.5f;
	smallBlaster_0.myBarrel = { 3, 0.9f, 5.35f };
	myWeapons.push_back(smallBlaster_0);

	Weapon smallBlaster_1(smallBlaster_0);
	smallBlaster_1.myBarrel = { -3, 0.9f, 5.35f };
	myWeapons.push_back(smallBlaster_1);

	// Set TargetForward
	const auto myRotationQuaternion = math::vector4<float>::RotateAngleAxis(GetComponent<TransformComponent>().myTransform.myRotation.y, math::vector3<float>::up());
	myTargetForward = myRotationQuaternion.Rotate(math::vector3<float>::forward());
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
		myRenderer->Render(ModelAssetHandler::GetModelData("SelectionCircle.fbx") /*ModelAssetHandler::GetPrimitiveModelData(ePrimitive::Plane)*/, ModelAssetHandler::GetMaterial("SelectionCircle2"), mySelectionTransform, myEntity.GetScene().GetCamera());
	}

	if (myTargetUnit != nullptr && (myTargetUnit->GetComponent<TransformComponent>().myTransform.myPosition - GetComponent<TransformComponent>().myTransform.myPosition).LengthSqr() > 625)
	{
		Transform otherTransform = myTargetUnit->GetComponent<TransformComponent>().myTransform;
		if ((otherTransform.myPosition - GetComponent<TransformComponent>().myTransform.myPosition).LengthSqr() > 625)
			MoveTo(GetComponent<TransformComponent>().myTransform.myPosition + (otherTransform.myPosition - GetComponent<TransformComponent>().myTransform.myPosition).GetNormalized() * ((otherTransform.myPosition - GetComponent<TransformComponent>().myTransform.myPosition).Length() - 25));
	}

	if (myTargetUnit != nullptr)
	{
		const auto myRotationQuaternion = math::vector4<float>::RotateAngleAxis(GetComponent<TransformComponent>().myTransform.myRotation.y, math::vector3<float>::up());
		for (Weapon& weapon : myWeapons)
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			long long lastFired = std::chrono::duration_cast<std::chrono::milliseconds>(now - weapon.mylastFired).count();
			if (lastFired / 1000 > weapon.myFirerate && weapon.Fire(myTargetUnit))
			{
				weapon.mylastFired = now;
			}
		}
	}

	UpdateMovement();
}

void Unit::UpdateMovement()
{
	Transform& myTransform = GetComponent<TransformComponent>().myTransform;

	const float sqrDstToTarget = (myTargetPosition - myTransform.myPosition).LengthSqr();
	bool rotateTowardsTargetDirection = sqrDstToTarget < 1 || (mySpeed < 0.1f && sqrDstToTarget < 40);

	math::vector3<float> moveDirection;
	if (rotateTowardsTargetDirection)
	{
		moveDirection = myTargetForward;
	}
	else
	{
		moveDirection = (myTargetPosition - myTransform.myPosition).GetNormalized();
	}

	const auto myRotationQuaternion = math::vector4<float>::RotateAngleAxis(myTransform.myRotation.y, math::vector3<float>::up());

	const auto myForward = myRotationQuaternion.Rotate(math::vector3<float>::forward());

	float angleBetween = myForward.AngleBetween(moveDirection);

	// Move

	if (angleBetween < 2 && sqrDstToTarget > 1225 || angleBetween < 0.2)
	{
		if (sqrDstToTarget > 40)
		{
			mySpeed += myAcceleration;
			if (mySpeed > myMaxSpeed) mySpeed = myMaxSpeed;
		}
		else
		{
			mySpeed -= myAcceleration;
			if (mySpeed < 0) mySpeed = 0;
		}

		if (sqrDstToTarget > 40)
			myTransform.myPosition += myForward * mySpeed;
		else
			myTransform.myPosition += moveDirection * mySpeed;
	}
	else
	{
		mySpeed -= myAcceleration;
		if (mySpeed < 0) mySpeed = 0;
	}
	
	// Rotate

	if (angleBetween > 0.1)
	{
		myRotationSpeed += myRotationAcceleration;
		if (myRotationSpeed > myRotationMaxSpeed) myRotationSpeed = myRotationMaxSpeed;
	}
	else
	{
		myRotationSpeed -= myRotationAcceleration;
		if (myRotationSpeed < 0) myRotationSpeed = 0;
	}

	auto cross = myForward.Cross(moveDirection);

	if (cross.LengthSqr() != 0) cross.Normalize();

	bool useRotationSpeed = angleBetween > myRotationSpeed;
	float rotateBy = myRotationSpeed;
	float maxBankAngle = -myRotationMaxBankAngle;

	if (math::vector3<float>::up().Dot(cross) < 0)
	{
		angleBetween = -angleBetween;
		rotateBy = -rotateBy;
		maxBankAngle = -maxBankAngle;
	}

	if (useRotationSpeed)
	{
		myTransform.myRotation.y += rotateBy;
		myTransform.myRotation.z = std::lerp(myTransform.myRotation.z, maxBankAngle * (myRotationSpeed / myRotationMaxSpeed) * std::max(mySpeed / myMaxSpeed, 0.2f), myRotationBankAngleAccelaration);
	}
	else
	{
		myTransform.myRotation.y += angleBetween;
		myTransform.myRotation.z = std::lerp(myTransform.myRotation.z, 0, myRotationBankAngleAccelaration);
	}
}
