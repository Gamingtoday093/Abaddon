#pragma once
#include <DirectXMath.h>
#include "Scene/Scripts/Script.h"
#include "Scene/Components/Components.h"
#include "Scene/Transform.h"
#include <chrono>

using namespace DirectX;

struct ModelData;
struct TextureData;

class Unit : public Script
{
public:

	struct Weapon
	{
		Weapon(Unit& aUnit) : myUnit(aUnit) {}

		int myDamage;

		float myFirerate;
		std::chrono::steady_clock::time_point mylastFired;

		float myFiringRange;

		float myFiringAngleX;
		float myFiringAngleY;

		math::vector3<float> myBarrel;

		Unit& myUnit;

		bool Fire(Unit* aTargetUnit)
		{
			Transform& myTransform = myUnit.GetComponent<TransformComponent>().myTransform;
			Transform& otherTransform = aTargetUnit->GetComponent<TransformComponent>().myTransform;

			XMMATRIX worldMatrix = DirectX::XMMatrixRotationRollPitchYaw(myTransform.myRotation.x, myTransform.myRotation.y, myTransform.myRotation.z) *	// ***
								   DirectX::XMMatrixTranslation(myTransform.myPosition.x, myTransform.myPosition.y, myTransform.myPosition.z) *				// World Matrix
								   DirectX::XMMatrixScaling(myTransform.myScale.x, myTransform.myScale.y, myTransform.myScale.z);							// ***

			XMVECTOR newBarrelXM = XMVector3TransformCoord(XMVectorSet(myBarrel.x, myBarrel.y, myBarrel.z, 0), worldMatrix);
			math::vector3<float> newBarrel = { XMVectorGetX(newBarrelXM), XMVectorGetY(newBarrelXM), XMVectorGetZ(newBarrelXM) };

			const auto towardsTarget = (otherTransform.myPosition - newBarrel);

			if (towardsTarget.Length() > myFiringRange) return false; // Cant Hit Target, Outside Range

			const auto myRotationQuaternion = math::vector4<float>::RotateAngleAxis(myTransform.myRotation.y, math::vector3<float>::up());
			const auto myForward = myRotationQuaternion.Rotate(math::vector3<float>::forward());

			const float angleBetween = myForward.AngleBetween(towardsTarget);

			if (angleBetween > myFiringAngleY) return false; // Cant Hit Target, Outside of Firing Arc

			aTargetUnit->Damage(myDamage);

			return true;
		}
	};

	void Select(bool aNewSelected);
	bool GetIsSelected();

	void MoveTo(math::vector3<float> aTargetPosition);
	void MoveTo(math::vector3<float> aTargetPosition, math::vector3<float> aTargetForward);
	
	void Attack(Unit* aOtherUnit);
	void ClearTargetUnit();
	void Damage(int aDamage);
	
	void Init(std::shared_ptr<Renderer> aRenderer);

	void Awake() override;
	void Start() override;
	void Update() override;

protected:
	bool isSelected;

	int myHealth = 500;

	std::vector<Weapon> myWeapons;

	float myAcceleration = 0.005f;
	float myMaxSpeed = 0.2f;
	float mySpeed = 0;
	float myRotationAcceleration = 0.002f;
	float myRotationMaxSpeed = 0.02f;
	float myRotationSpeed = 0;
	float myRotationMaxBankAngle = 0.35f;
	float myRotationBankAngleAccelaration = 0.03f;

private:
	void UpdateMovement();

	math::vector3<float> myTargetPosition;
	math::vector3<float> myTargetForward;

	Unit* myTargetUnit = nullptr;

	std::shared_ptr<Renderer> myRenderer;

	ModelData* mySelectionMesh;
	TextureData* mySelectionTexture;
	Transform mySelectionTransform;
	math::vector3<float> mySelectionPositionOffset;

	friend class UnitManager;
};
