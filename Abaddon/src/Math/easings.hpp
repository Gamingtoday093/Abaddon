#pragma once

namespace math
{
	static float pow3(float t)
	{
		return t * t * t;
	}

	float easeInOutCubic(float t)
	{
		return t < 0.5 ? 4 * t * t * t : 1 - pow3(-2 * t + 2) / 2;
	}
}