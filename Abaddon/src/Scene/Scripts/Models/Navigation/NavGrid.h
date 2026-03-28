#pragma once
#include "GridCoord.h"

namespace Navigation
{
	struct ValuedNode;

	class NavGrid
	{
	public:
		NavGrid() = default;
		~NavGrid() = default;

		void Resize(uint16_t aWidth, uint16_t aHeight);
		void StampSquare(GridCoord aNodeCoord, uint8_t aValue, int32_t aRadius);
		void StampCircle(GridCoord aNodeCoord, uint8_t aValue, int32_t aRadius);
		bool Pathfind(math::vector2<float> aOrigin, math::vector2<float> aTarget, std::vector<math::vector2<float>>& aResultPath) const;
		int32_t GetNearestIndex(math::vector2<float> aPosition) const;
		GridCoord GetNearest(math::vector2<float> aPosition) const;
		math::vector2<float> GetWorldPosition(int32_t aNodeIndex) const;
		math::vector2<float> GetWorldPosition(GridCoord aNodeCoord) const;

		float myNodeSize = 1.f;
		uint16_t GetWidth() const;
		uint16_t GetHeight() const;
		const std::vector<uint8_t>& GetNodes() const;

	private:
		void StampCircleLine(GridCoord aStartNodeCoord, GridCoord aEndNodeCoord, int32_t aRadius, uint8_t aValue);
		void GetResultPath(int32_t aFinalNodeIndex, int32_t aStartNodeIndex, const std::unordered_map<int32_t, int32_t>& aParents, std::vector<math::vector2<float>>& aResultPath) const;
		bool HasLineOfSight(int32_t aNodeIndexLhs, int32_t aNodeIndexRhs) const;
		std::vector<int32_t> GetNeighbours(int32_t aNodeIndex) const;

		// C++ Modulo does not return correct values for negative values, so use this instead
		inline static int32_t Mod(int32_t aLhs, int32_t aRhs)
		{
			return (aLhs % aRhs + aRhs) % aRhs;
		}

		uint16_t myWidth = 1, myHeight = 1;
		std::vector<uint8_t> myNodes;
	};
}
