#pragma once
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"
#include "Tools/Input.h"
#include "Math/vectors.h"
#include "Camera.h"

using namespace DirectX;

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera() : Camera() { }

	void Init(float aMovementSpeed, float aRotationSpeed, bool aFirstPerson = false);
	void Update() override;
	math::vector3<float> GetPosition() override;
	math::vector4<float> GetRotation() override;
	void SetTransformation(math::vector3<float> aPosition, math::vector4<float> aRotation) override;
	XMMATRIX GetMatrix() override;

private:
	void CalculateMatrix() override;
	void UpdateInput() override;

	bool myFirstPerson;
	float myMovementSpeed;
	float myRotationSpeed;

private:
	// Variables used to calculate Camera Matrix
	XMVECTOR myDefaultForward;
	XMVECTOR myDefaultRight;
	XMMATRIX myCamRotationMatrix;

	XMVECTOR myCamForward;
	XMVECTOR myCamRight;

	XMVECTOR myCamPosition;
	XMVECTOR myCamTarget;
	XMVECTOR myCamUp;

	math::vector3<float> myDir;
	math::vector2<float> myRot;
};

