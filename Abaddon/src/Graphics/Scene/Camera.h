#pragma once
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"
#include "Tools/Input.h"
#include "Math/vectors.h"

using namespace DirectX;

class Camera
{
public:
	Camera(Input& aInput);
	~Camera() = default;

	void Init(float aCameraSpeed, float aRotationSpeed, bool aFirstPerson = false);
	void Update();
	XMMATRIX GetMatrix();

private:
	void CalculateMatrix();
	void UpdateInput();

	bool myFirstPerson;
	float myMovementSpeed;
	float myRotationSpeed;
	Input& myInput;

	XMMATRIX myCameraMatrix;

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

	math::vector3<float> myDir = { 0, 0, 0 };
	math::vector2<float> myRot = { 0, 0 };
};

