#pragma once
#include "Math/vectors.h"
#include <utility>

struct AABB
{
	math::vector3<float> myMin;
	math::vector3<float> myMax;

	AABB()
	{
		myMin = {};
		myMax = {};
	}

	AABB(math::vector3<float> aMin, math::vector3<float> aMax)
	{
		myMin = aMin;
		myMax = aMax;
	}

	bool InsideBounds(const math::vector3<float>& aPosition) const
	{
		return aPosition.x >= myMin.x && aPosition.y >= myMin.y && aPosition.z >= myMin.z &&
			   aPosition.x <= myMax.x && aPosition.y <= myMax.y && aPosition.z <= myMax.z;
	}

	bool RayBounds(const math::vector3<float>& aOrigin, const math::vector3<float>& aDirection) const
	{
		const math::vector3<float> invDir = { 1.0f / aDirection.x, 1.0f / aDirection.y, 1.0f / aDirection.z };
		const math::vector3<float> tMin = { (myMin.x - aOrigin.x) * invDir.x, (myMin.y - aOrigin.y) * invDir.y, (myMin.z - aOrigin.z) * invDir.z };
		const math::vector3<float> tMax = { (myMax.x - aOrigin.x) * invDir.x, (myMax.y - aOrigin.y) * invDir.y, (myMax.z - aOrigin.z) * invDir.z };
		const math::vector3<float> t1 = { std::min(tMin.x, tMax.x), std::min(tMin.y, tMax.y), std::min(tMin.z, tMax.z) };
		const math::vector3<float> t2 = { std::max(tMin.x, tMax.x), std::max(tMin.y, tMax.y), std::max(tMin.z, tMax.z) };
		const float tNear = std::max(std::max(t1.x, t1.y), t1.z);
		const float tFar = std::min(std::min(t2.x, t2.y), t2.z);
		return tNear <= tFar;
	}

	void ExpandTo(const math::vector3<float>& aPosition)
	{
		if (aPosition.x < myMin.x)
			myMin.x = aPosition.x;

		if (aPosition.y < myMin.y)
			myMin.y = aPosition.y;

		if (aPosition.z < myMin.z)
			myMin.z = aPosition.z;

		if (aPosition.x > myMax.x)
			myMax.x = aPosition.x;

		if (aPosition.y > myMax.y)
			myMax.y = aPosition.y;

		if (aPosition.z > myMax.z)
			myMax.z = aPosition.z;
	}

	void MergeWith(const AABB& aAABB)
	{
		ExpandTo(aAABB.myMax);
		ExpandTo(aAABB.myMin);
	}
};