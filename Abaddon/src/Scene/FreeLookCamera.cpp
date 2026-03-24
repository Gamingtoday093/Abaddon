#include "pch.h"
#include "FreeLookCamera.h"

FreeLookCamera::FreeLookCamera() : Camera()
{
	myCamPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	CalculateMatrix();
}

void FreeLookCamera::Update()
{
	UpdateInput();
	CalculateMatrix();
}

math::vector3<float> FreeLookCamera::GetPosition() const
{
	return { XMVectorGetX(myCamPosition), XMVectorGetY(myCamPosition), XMVectorGetZ(myCamPosition) };
}

math::vector4<float> FreeLookCamera::GetRotation() const
{
	const XMVECTOR quat = XMQuaternionRotationRollPitchYaw(myYawPitch.x, myYawPitch.y, 0);
	return { XMVectorGetX(quat), XMVectorGetY(quat), XMVectorGetZ(quat), XMVectorGetW(quat) };
}

void FreeLookCamera::SetTransformation(math::vector3<float> aPosition, math::vector4<float> aRotation)
{
	myCamPosition = XMVectorSet(aPosition.x, aPosition.y, aPosition.z, 0);
	const math::vector3<float> eulerAngles = aRotation.ToEuler();
	myYawPitch = { -eulerAngles.x, eulerAngles.y + XM_PI };
}

XMMATRIX FreeLookCamera::GetMatrix() const
{
	return myCameraMatrix;
}

void FreeLookCamera::CalculateMatrix()
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(myYawPitch.x, myYawPitch.y, 0);

	XMVECTOR cameraForward = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
	XMVECTOR cameraUp = XMVector3TransformCoord(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationMatrix);

	XMVECTOR newMove = XMVectorSet(myMoveDir.x, myMoveDir.y, myMoveDir.z, 0);
	newMove = XMVector3TransformCoord(newMove, rotationMatrix);

	myCamPosition += newMove;
	cameraForward += myCamPosition;

	myCameraMatrix = XMMatrixLookAtLH(myCamPosition, cameraForward, cameraUp);
}

void FreeLookCamera::UpdateInput()
{
	// Mouse
	if (Input::GetInstance().IsMouseButtonDown((int)eKeys::MOUSERBUTTON))
	{
		myYawPitch.x += Input::GetInstance().GetMouseDelta().y * myRotationSpeed;
		myYawPitch.y += Input::GetInstance().GetMouseDelta().x * myRotationSpeed;
	}

	// Speed
	myMovementSpeed -= ((Input::GetInstance().GetScrollDelta() < 0) ? -1 : (Input::GetInstance().GetScrollDelta() > 0)) * myMovementSpeedMultiplier;
	if (myMovementSpeed < myMinMovementSpeed) myMovementSpeed = myMinMovementSpeed;
	else if (myMovementSpeed > myMaxMovementSpeed) myMovementSpeed = myMaxMovementSpeed;

	// Keyboard
	myMoveDir = math::vector3<float>::zero();
	if (Input::GetInstance().IsKeyDown((int)eKeys::W))
	{
		myMoveDir.z += 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::A))
	{
		myMoveDir.x -= 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::S))
	{
		myMoveDir.z -= 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::D))
	{
		myMoveDir.x += 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::Q))
	{
		myMoveDir.y += 1;
	}
	if (Input::GetInstance().IsKeyDown((int)eKeys::E))
	{
		myMoveDir.y -= 1;
	}
	if (myMoveDir.LengthSqr() > 0)
	{
		myMoveDir.Normalize();
		myMoveDir *= myMovementSpeed;
		if (Input::GetInstance().IsKeyDown((int)eKeys::SHIFT))
		{
			myMoveDir *= 2.f;
		}
	}
}