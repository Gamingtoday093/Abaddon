#pragma once
#include <DirectXMath.h>
#include "Scene/Components/Components.h"

using namespace DirectX;

class Unit : public Script
{
public:
	void Select(bool aNewSelected);
	bool GetIsSelected();
	void MoveTo(math::vector3<float> aTargetPosition);
	void Init(Renderer& aRenderer);
	void Start() override;
	void Update() override;

protected:
	int myHealth = 100;
	bool isSelected;
	float myAcceleration = 0.005f;
	float myMaxSpeed = 0.15f;
	float mySpeed = 0;

private:
	void UpdateMovement();

	math::vector3<float> myTargetPosition;
	XMVECTOR myTargetRotation;

	Renderer* myRenderer = nullptr;

	ModelData* mySelectionMesh;
	TextureData* mySelectionTexture;
	Transform mySelectionTransform;
	math::vector3<float> mySelectionPositionOffset;

	friend class UnitManager;
};
