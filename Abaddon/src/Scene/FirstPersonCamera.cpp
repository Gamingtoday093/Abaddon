#include "pch.h"
#include "FirstPersonCamera.h"

void FirstPersonCamera::Init(float aMovementSpeed, float aRotationSpeed, bool aFirstPerson)
{
	myFirstPerson = aFirstPerson;

	myMovementSpeed = aMovementSpeed;
	myRotationSpeed = aRotationSpeed;

	myDefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	myDefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	myCamForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	myCamRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	myCamTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	myCamPosition = DirectX::XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	myCamUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	CalculateMatrix();
}

void FirstPersonCamera::Update()
{
	UpdateInput();
	CalculateMatrix();
}

math::vector3<float> FirstPersonCamera::GetPosition()
{
	return { XMVectorGetX(myCamPosition), XMVectorGetY(myCamPosition), XMVectorGetZ(myCamPosition) };
}

math::vector4<float> FirstPersonCamera::GetRotation()
{
	return math::vector4<float>::identity();
}

void FirstPersonCamera::SetTransformation(math::vector3<float> aPosition, math::vector4<float> aRotation)
{
}

XMMATRIX FirstPersonCamera::GetMatrix()
{
	return myCameraMatrix;
}

void FirstPersonCamera::CalculateMatrix()
{
	// Reset values
	myCamForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	myCamRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	myCamRotationMatrix = XMMatrixIdentity();
	
	// Calculates new forward direction based on cameras rotation
	myCamRotationMatrix = XMMatrixRotationRollPitchYaw(myRot.x, myRot.y, 0);
	myCamTarget = XMVector3TransformCoord(myDefaultForward, myCamRotationMatrix);
	myCamTarget = XMVector3Normalize(myCamTarget);
	
	// Calculate new left, right and up direction based on cameras rotation
	if (myFirstPerson) 
	{
		// First Person camera
		XMMATRIX RotateYTempMatrix;
		RotateYTempMatrix = XMMatrixRotationY(myRot.y);
	
		myCamRight = XMVector3TransformCoord(myDefaultRight, RotateYTempMatrix);
		myCamForward = XMVector3TransformCoord(myDefaultForward, RotateYTempMatrix);
		myCamUp = XMVector3TransformCoord(myCamUp, RotateYTempMatrix);
	}
	else
	{
		// Free-Look camera
		myCamRight = XMVector3TransformCoord(myDefaultRight, myCamRotationMatrix);
		myCamForward = XMVector3TransformCoord(myDefaultForward, myCamRotationMatrix);
		myCamUp = XMVector3Cross(myCamForward, myCamRight);
	}
	
	// Update position
	myCamPosition = XMVectorAdd(myCamPosition, XMVectorScale(myCamRight, myDir.x));
	myCamPosition = XMVectorAdd(myCamPosition, XMVectorScale(myCamForward, myDir.z));
	myCamPosition = XMVectorAdd(myCamPosition, XMVECTOR({ 0.0f, myDir.y, 0.0f, 0.0f }));
	
	myDir = math::vector3<float>::zero();
	
	// Calculate final target
	myCamTarget = XMVectorAdd(myCamPosition, myCamTarget);
	
	// Set camera matrix
	myCameraMatrix = XMMatrixLookAtLH(myCamPosition, myCamTarget, myCamUp);
}

void FirstPersonCamera::UpdateInput()
{
	if (Input::GetInstance().IsMouseButtonDown((int)eKeys::MOUSERBUTTON))
	{
		myRot.x += Input::GetInstance().GetMouseDelta().y * myRotationSpeed;
		myRot.y += Input::GetInstance().GetMouseDelta().x * myRotationSpeed;
	}

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
