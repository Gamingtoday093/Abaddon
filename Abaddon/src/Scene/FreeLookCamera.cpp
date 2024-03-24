#include "pch.h"
#include "FreeLookCamera.h"

void FreeLookCamera::Init(float aMovementSpeedMultiplier, float aStartMovementSpeed, float aMinMovementSpeed, float aMaxMovementSpeed, float aRotationSpeed)
{
	myMovementSpeed = aStartMovementSpeed;
	myMovementSpeedMultiplier = aMovementSpeedMultiplier;
	myMinMovementSpeed = aMinMovementSpeed;
	myMaxMovementSpeed = aMaxMovementSpeed;
	myRotationSpeed = aRotationSpeed;

	Forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	myCamTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	myCamPosition = DirectX::XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	myCamUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	CalculateMatrix();
}

void FreeLookCamera::Update()
{
	UpdateInput();
	CalculateMatrix();
}

math::vector3<float> FreeLookCamera::GetPosition()
{
	return { XMVectorGetX(myCamPosition), XMVectorGetY(myCamPosition), XMVectorGetZ(myCamPosition) };
}

math::vector4<float> FreeLookCamera::GetRotation()
{
	const XMVECTOR quat = XMQuaternionRotationRollPitchYaw(myRot.x, myRot.y, 0);
	return { XMVectorGetX(quat), XMVectorGetY(quat), XMVectorGetZ(quat), XMVectorGetW(quat) };
}

void FreeLookCamera::SetTransformation(math::vector3<float> aPosition, math::vector4<float> aRotation)
{
	myCamPosition = XMVectorSet(aPosition.x, aPosition.y, aPosition.z, 0);
	const math::vector3<float> eulerAngles = aRotation.ToEuler();
	myRot = { -eulerAngles.x, eulerAngles.y + XM_PI };
}

XMMATRIX FreeLookCamera::GetMatrix()
{
	return myCameraMatrix;
}

void FreeLookCamera::CalculateMatrix()
{
	// Calculate Rotation Matrix
	myCamRotationMatrix = XMMatrixRotationRollPitchYaw(myRot.x, myRot.y, 0);

	// Calculates Forward direction
	myCamTarget = XMVector3TransformCoord(Forward, myCamRotationMatrix);

	// Calculate Up direction
	myCamUp = XMVector3TransformCoord(Up, myCamRotationMatrix);

	// Create new Move Vector from myDir
	XMVECTOR newMove = XMVectorSet(myDir.x, myDir.y, myDir.z, 0);
	newMove = XMVector3TransformCoord(newMove, myCamRotationMatrix);

	// Update position
	myCamPosition += newMove;

	myDir = math::vector3<float>::zero();

	// Calculate final target
	myCamTarget = XMVectorAdd(myCamPosition, myCamTarget);

	// Set camera matrix
	myCameraMatrix = XMMatrixLookAtLH(myCamPosition, myCamTarget, myCamUp);
}

void FreeLookCamera::UpdateInput()
{
	// Mouse
	if (Input::GetInstance().IsMouseButtonDown((int)eKeys::MOUSERBUTTON))
	{
		myRot.x += Input::GetInstance().GetMouseDelta().y * myRotationSpeed;
		myRot.y += Input::GetInstance().GetMouseDelta().x * myRotationSpeed;
	}

	// Speed
	myMovementSpeed -= ((Input::GetInstance().GetScrollDelta() < 0) ? -1 : (Input::GetInstance().GetScrollDelta() > 0)) * myMovementSpeedMultiplier;
	if (myMovementSpeed < myMinMovementSpeed) myMovementSpeed = myMinMovementSpeed;
	else if (myMovementSpeed > myMaxMovementSpeed) myMovementSpeed = myMaxMovementSpeed;

	// Keyboard
	if (Input::GetInstance().IsKeyDown((int)eKeys::W))
	{
		myDir.z += 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::A))
	{
		myDir.x -= 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::S))
	{
		myDir.z -= 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::D))
	{
		myDir.x += 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::Q))
	{
		myDir.y += 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::E))
	{
		myDir.y -= 1;
	}
	if (myDir.LengthSqr() > 1)
	{
		myDir.Normalize();
	}
	myDir *= myMovementSpeed;
}