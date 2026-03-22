#pragma once
#include <DirectXMath.h>
#include "Scene/Scripts/Script.hpp"
#include "Scene/Components/Components.h"
#include "Models/Weapon.h"

using namespace DirectX;

struct ModelData;
struct TextureData;

class Unit : public Script
{
public:
	void Select(bool aNewSelected);
	bool GetIsSelected() const;

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
	bool isSelected = false;

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

	Transform mySelectionTransform;
	math::vector3<float> mySelectionPositionOffset;

	friend class UnitManager;
};
