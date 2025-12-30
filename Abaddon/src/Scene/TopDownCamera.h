#pragma once
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"
#include "Tools/Input.h"
#include "Math/vectors.h"
#include "Camera.h"

using namespace DirectX;

class TopDownCamera : public Camera
{
public:
	TopDownCamera() : Camera() { }

	void Init(float aMovementSpeed, float aRotationSpeed, float aZoomSpeed, float aStartZoom, float aMinZoom, float aMaxZoom, math::vector3<float> aStartOrbit, math::vector2<float> aStartRotation, float aMinRotation, float aMaxRotation);
	void Update() override;
	math::vector3<float> GetPosition() const override;
	math::vector4<float> GetRotation() const override;
	void SetTransformation(math::vector3<float> aPosition, math::vector4<float> aRotation) override;
	XMMATRIX GetMatrix() const override;

private:
	void CalculateMatrix() override;
	void UpdateInput() override;

	float myMovementSpeed = 1;
	float myRotationSpeed = 1;
	float myZoomSpeed = 1;
	float myMinZoom = 0;
	float myMaxZoom = 1;
	float myMinRotation = 0;
	float myMaxRotation = 0;
	math::vector2<float> myDefaultRotation;

	XMMATRIX myCameraMatrix;

private:
	// Variables used to calculate Camera Matrix
	XMVECTOR myCamPosition;
	XMVECTOR myCamOrbitTarget;
	XMVECTOR myCamUp;

	math::vector3<float> myDir;
	math::vector2<float> myRot;

	float myZoom = 0;
};

