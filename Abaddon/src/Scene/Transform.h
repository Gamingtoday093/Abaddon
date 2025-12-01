#pragma once
#include "Math/vector3.hpp"

struct Transform
{
	Transform()
	{
		myPosition = { 0,0,0 };
		myRotation = { 0,0,0 };
		myScale = { 1,1,1 };
	}

	math::vector3<float> myPosition;
	/// <summary>
	/// Rotation in Radians.
	/// </summary>
	math::vector3<float> myRotation;
	math::vector3<float> myScale;

	DirectX::XMMATRIX GetModelMatrix() const
	{
		return DirectX::XMMatrixScaling(myScale.x, myScale.y, myScale.z) *
			   DirectX::XMMatrixRotationRollPitchYaw(myRotation.x, myRotation.y, myRotation.z) *
			   DirectX::XMMatrixTranslation(myPosition.x, myPosition.y, myPosition.z);
	}
};