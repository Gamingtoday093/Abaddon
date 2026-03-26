#pragma once

struct Line
{
	Line()
	{
		myFromPosition = {};
		myToPosition = {};
		myColor = {};
	}

	Line(const math::vector3<float>& aFromPosition, const math::vector3<float>& aToPosition, const math::vector4<float>& aColor)
	{
		myFromPosition = aFromPosition;
		myToPosition = aToPosition;
		myColor = aColor;
	}

	math::vector3<float> myFromPosition;
	math::vector3<float> myToPosition;
	math::vector4<float> myColor = { 0, 1, 0, 1 };
};
