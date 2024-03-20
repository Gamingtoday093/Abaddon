#include "pch.h"
#include "Unit.h"

void Unit::Select(bool aNewSelected)
{
	isSelected = aNewSelected;
	if (aNewSelected) LOG("Selected!");
	else LOG("Unselected!");
}

bool Unit::GetIsSelected()
{
	return isSelected;
}

void Unit::MoveTo(math::vector3<float> aTargetPosition)
{
	myTargetPosition = aTargetPosition;
	mySpeed = 0;

	//if ((myTargetPosition - myPosition).LengthSqr() > 1)
	//{
	//	auto moveDirection = (myTargetPosition - myPosition).GetNormalized();

	//	//math::vector3<float> modelForward = { cos(myRotation.y) * sin(myRotation.z), -sin(myRotation.y), cos(myRotation.y) * cos(myRotation.z) };
	//	//LOG(std::to_string(modelForward.x));
	//	//LOG(std::to_string(modelForward.y));
	//	//LOG(std::to_string(modelForward.z));
	//	
	//	//float angle = acos(modelForward.Dot(moveDirection));

	//	//LOG(std::to_string(asin(moveDirection.y)));

	//	//auto axisVector = modelForward.Cross(moveDirection);

	//	//float axis = 0;

	//	//if (axisVector.x < 0.1f && axisVector.x > 0.1f) axis = axisVector.x;
	//	//else if (axisVector.y < 0.1f && axisVector.y > 0.1f) axis = axisVector.y;
	//	//else if (axisVector.z < 0.1f && axisVector.z > 0.1f) axis = axisVector.z;

	//	//LOG("X: " + std::to_string(axisVector.x));
	//	//LOG("Y: " + std::to_string(axisVector.y));
	//	//LOG("Z: " + std::to_string(axisVector.z));
	//	
	//	//myRotation.y += angle;

	//	//auto quatlook = math::vector4<float>::LookRotation(moveDirection, math::vector3<float>::up());

	//	//myTargetRotation = XMVectorSet(quatlook.x, quatlook.y, quatlook.z, quatlook.w);
	//	
	//	//quatlook = quatlook.Multiply({ modelForward.x, modelForward.y, modelForward.z, 0 });
	//	
	//	//float outAngle;
	//	//math::vector4<float> quat = math::vector4<float>::FromToRotation(modelForward, moveDirection);
	//	//XMVECTOR sillyaxis;
	//	//XMQuaternionToAxisAngle(&sillyaxis, &outAngle, XMVectorSet(quat.x, quat.y, quat.z, quat.w));
	//	//myRotation.y = asin(quat.y) * 2;
	//}
}

void Unit::Awake()
{
	myTargetPosition = GetPosition();
}

void Unit::Update()
{
	if ((myTargetPosition - myPosition).LengthSqr() > 1)
	{
		auto moveDirection = (myTargetPosition - myPosition).GetNormalized();
		auto quatlook = math::vector4<float>::LookRotation(moveDirection, math::vector3<float>::up());

		myTargetRotation = XMVectorSet(quatlook.x, quatlook.y, quatlook.z, quatlook.w);

		math::vector4<float> quat = { XMVectorGetX(myQuaternionRotation), XMVectorGetY(myQuaternionRotation), XMVectorGetZ(myQuaternionRotation), XMVectorGetW(myQuaternionRotation) };

		math::vector3<float> myForward = quat.Rotate({ 0, 0, 1 }).GetNormalized();

		const float angleBetween = myForward.AngleBetween(moveDirection);

		const float angleMultiplier = 0.1f / (myForward.AngleBetween(moveDirection) / XM_PI);

		if (angleBetween < XM_PIDIV2 && (myTargetPosition - myPosition).LengthSqr() > 250 || angleBetween < 0.4)
		{
			mySpeed += myAcceleration;
			if (mySpeed > myMaxSpeed) mySpeed = myMaxSpeed;

			if ((myTargetPosition - myPosition).LengthSqr() > 100)
				myPosition += myForward * mySpeed;
			else
				myPosition += moveDirection * mySpeed;
		}

		//myQuaternionRotation = myTargetRotation;
		myQuaternionRotation = XMQuaternionSlerp(myQuaternionRotation, myTargetRotation, std::min(std::max(0.035f, 0.02f), 0.05f));
	}
	else
	{
		mySpeed = 0;
	}
}
