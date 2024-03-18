#pragma once
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"
#include "Tools/Input.h"
#include "Math/vectors.h"
#include "Camera.h"

using namespace DirectX;

class TopDownCamera : public virtual Camera
{
public:
	void Init(float aMovementSpeed, float aRotationSpeed, float aZoomSpeed, float aStartZoom, float aMinZoom, float aMaxZoom, math::vector2<float> aStartRotation, float aMinRotation, float aMaxRotation);
	void Update();
	XMMATRIX GetMatrix();

private:
	void CalculateMatrix();
	void UpdateInput();

	float myMovementSpeed;
	float myRotationSpeed;
	float myZoomSpeed;
	float myMinZoom;
	float myMaxZoom;
	float myMinRotation;
	float myMaxRotation;
	Input& myInput;

	XMMATRIX myCameraMatrix;

private:
	// Variables used to calculate Camera Matrix
	XMVECTOR myDefaultForward;
	XMVECTOR myDefaultRight;
	XMMATRIX myCamRotationMatrix;

	XMVECTOR myCamPosition;
	XMVECTOR myCamOrbitTarget;
	XMVECTOR myCamUp;

	math::vector3<float> myDir;
	math::vector2<float> myRot;

	float myZoom;
};

