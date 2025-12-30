#include "pch.h"
#include "Weapon.h"
#include "Scene/Transform.h"
#include "Scene/Components/Components.h"
#include "Scene/Scripts/Unit.h"

Weapon::Weapon(Unit& aUnit) : myUnit(aUnit)
{
	myDamage = 0;
	myFirerate = 0;
	myFiringRange = 0;
	myFiringAngle = 0;
}

bool Weapon::Fire(Unit* aTargetUnit) const
{
	Transform& myTransform = myUnit.GetComponent<TransformComponent>().myTransform;
	Transform& otherTransform = aTargetUnit->GetComponent<TransformComponent>().myTransform;

	XMVECTOR newBarrelXM = XMVector3TransformCoord(XMVectorSet(myBarrel.x, myBarrel.y, myBarrel.z, 0), myTransform.GetModelMatrix());
	math::vector3<float> newBarrel = { XMVectorGetX(newBarrelXM), XMVectorGetY(newBarrelXM), XMVectorGetZ(newBarrelXM) };

	const auto towardsTarget = (otherTransform.myPosition - newBarrel);

	if (towardsTarget.Length() > myFiringRange) return false; // Cant Hit Target, Outside Range

	const auto myRotationQuaternion = math::vector4<float>::RotateAngleAxis(myTransform.myRotation.y, math::vector3<float>::up());
	const auto myForward = myRotationQuaternion.Rotate(math::vector3<float>::forward());

	const float angleBetween = myForward.AngleBetween(towardsTarget);

	if (angleBetween > myFiringAngle) return false; // Cant Hit Target, Outside of Firing Arc

	aTargetUnit->Damage(myDamage);
	return true;
}