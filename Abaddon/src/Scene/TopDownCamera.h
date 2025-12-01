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
	math::vector3<float> GetPosition() override;
	math::vector4<float> GetRotation() override;
	void SetTransformation(math::vector3<float> aPosition, math::vector4<float> aRotation) override;
	XMMATRIX GetMatrix() override;

private:
	void CalculateMatrix() override;
	void UpdateInput() override;

	float myMovementSpeed;
	float myRotationSpeed;
	float myZoomSpeed;
	float myMinZoom;
	float myMaxZoom;
	float myMinRotation;
	float myMaxRotation;
	math::vector2<float> myDefaultRotation;

	XMMATRIX myCameraMatrix;

private:
	// Variables used to calculate Camera Matrix
	XMVECTOR myCamPosition;
	XMVECTOR myCamOrbitTarget;
	XMVECTOR myCamUp;

	math::vector3<float> myDir;
	math::vector2<float> myRot;

	float myZoom;
};

