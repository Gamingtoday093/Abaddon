#include "pch.h"
#include "TopDownCamera.h"

void TopDownCamera::Init(float aMovementSpeed, float aRotationSpeed, float aZoomSpeed, float aStartZoom, float aMinZoom, float aMaxZoom, math::vector2<float> aStartRotation, float aMinRotation, float aMaxRotation)
{
	myMovementSpeed = aMovementSpeed;
	myRotationSpeed = aRotationSpeed;
	myZoomSpeed = aZoomSpeed;
	myMinZoom = aMinZoom;
	myMaxZoom = aMaxZoom;
	myMinRotation = aMinRotation;
	myMaxRotation = aMaxRotation;

	myDefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	myDefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	myCamOrbitTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	myCamPosition = XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	myCamUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	myRot = aStartRotation;
	myZoom = aStartZoom;

	CalculateMatrix();
}

void TopDownCamera::Update()
{
	UpdateInput();
	CalculateMatrix();
}

XMMATRIX TopDownCamera::GetMatrix()
{
	return myCameraMatrix;
}

void TopDownCamera::CalculateMatrix()
{
	// Create new Move Vector from myDir
	XMVECTOR newMove = XMVectorSet(myDir.x, myDir.y, myDir.z, 0);

	// Transform Move Vector Along Rotation Matrix
	newMove = XMVector3Transform(-newMove, XMMatrixRotationRollPitchYaw(0, myRot.y, 0));

	// Set new Orbit Target
	myCamOrbitTarget = XMVectorAdd(myCamOrbitTarget, newMove);

	// Update Position
	myCamPosition = XMVectorAdd(myCamOrbitTarget, XMVector3Transform(XMVectorSet(0, 0, myZoom, 0), XMMatrixRotationRollPitchYaw(myRot.x, myRot.y, 0)));

	// Reset Direction Vector
	myDir = math::vector3<float>::zero();

	// Set Camera Matrix
	myCameraMatrix = XMMatrixLookAtLH(myCamPosition, myCamOrbitTarget, myCamUp);
}

void TopDownCamera::UpdateInput()
{
	// Mouse
	if (myInput.IsMouseButtonDown((int)eKeys::MBUTTON))
	{
		myRot.x += myInput.GetMouseDelta().y * myRotationSpeed;
		if (myRot.x < myMinRotation) myRot.x = myMinRotation;
		else if (myRot.x > myMaxRotation) myRot.x = myMaxRotation;
		myRot.y += myInput.GetMouseDelta().x * myRotationSpeed;
	}

	// Zoom
	myZoom += (myInput.GetScrollDelta() < 0) ? -1 : (myInput.GetScrollDelta() > 0) * myZoomSpeed;
	if (myZoom < myMinZoom) myZoom = myMinZoom;
	else if (myZoom > myMaxZoom) myZoom = myMaxZoom;

	// Keyboard
	if (myInput.IsKeyDown((int)eKeys::W))
	{
		myDir.z += 1;
	}
	if (myInput.IsKeyDown((int)eKeys::A))
	{
		myDir.x -= 1;
	}
	if (myInput.IsKeyDown((int)eKeys::S))
	{
		myDir.z -= 1;
	}
	if (myInput.IsKeyDown((int)eKeys::D))
	{
		myDir.x += 1;
	}
	if (myInput.IsKeyDown((int)eKeys::Q))
	{
		myDir.y += 1;
	}
	if (myInput.IsKeyDown((int)eKeys::E))
	{
		myDir.y -= 1;
	}
	if (myDir.LengthSqr() > 1)
	{
		myDir.Normalize();
	}
	myDir *= myMovementSpeed;
}
