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
	FreeLookCamera();

	void Update() override;
	math::vector3<float> GetPosition() const override;
	math::vector4<float> GetRotation() const override;
	void SetTransformation(const math::vector3<float>& aPosition, const math::vector4<float>& aRotation) override;
	void Focus(const math::vector3<float>& aPosition) override;
	XMMATRIX GetMatrix() const override;

	float myMovementSpeed = 1.2f;
	float myMinMovementSpeed = 0.1f;
	float myMaxMovementSpeed = 3.0f;
	float myMovementSpeedMultiplier = 0.125f;
	float myRotationSpeed = 0.005f;

protected:
	void CalculateMatrix() override;
	void UpdateInput() override;

private:
	XMMATRIX myCameraMatrix;

	XMVECTOR myCamPosition;
	math::vector3<float> myMoveDir;
	math::vector2<float> myYawPitch;
};

