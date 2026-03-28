#pragma once

namespace Navigation
{
	struct GridCoord
	{
		GridCoord() : x(), y() { }
		GridCoord(int32_t aX, int32_t aY) : x(aX), y(aY) { }

		int32_t x, y;
	};
}
