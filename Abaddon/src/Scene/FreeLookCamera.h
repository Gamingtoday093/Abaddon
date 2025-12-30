#pragma once
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"
#include "Tools/Input.h"
#include "Math/vectors.h"
#include "Camera.h"

using namespace DirectX;

class FreeLookCamera : public Camera
{
public:
	FreeLookCamera() : Camera() {}

	void Init(float aMovementSpeedMultiplier, float aStartMovementSpeed, float aMinMovementSpeed, float aMaxMovementSpeed, float aRotationSpeed);
	void Update() override;
	math::vector3<float> GetPosition() const override;
	math::vector4<float> GetRotation() const override;
	void SetTransformation(math::vector3<float> aPosition, math::vector4<float> aRotation) override;
	XMMATRIX GetMatrix() const override;

private:
	void CalculateMatrix() override;
	void UpdateInput() override;

	float myMovementSpeed;

	float myMovementSpeedMultiplier;
	float myMinMovementSpeed;
	float myMaxMovementSpeed;
	float myRotationSpeed;

private:
	XMMATRIX myCameraMatrix;

	// Variables used to calculate Camera Matrix
	XMVECTOR Forward;
	XMVECTOR Up;
	XMMATRIX myCamRotationMatrix;

	XMVECTOR myCamPosition;
	XMVECTOR myCamTarget;
	XMVECTOR myCamUp;

	math::vector3<float> myDir;
	math::vector2<float> myRot;
};

