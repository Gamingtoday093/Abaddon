#pragma once

namespace Navigation
{
	struct ValuedNode;

	class NavGrid
	{
	public:
		NavGrid() = default;
		~NavGrid() = default;

		void Resize(uint32_t aWidth, uint32_t aHeight);
		void StampSquare(size_t aNodeIndex, uint8_t aValue, int32_t aRadius);
		void StampCircle(size_t aNodeIndex, uint8_t aValue, int32_t aRadius);
		bool Pathfind(math::vector2<float> aOrigin, math::vector2<float> aTarget, std::vector<math::vector2<float>>& aResultPath) const;
		size_t GetNearest(math::vector2<float> aPosition) const;
		math::vector2<float> GetWorldPosition(int64_t aNodeIndex) const;

		float myNodeSize = 1.f;
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		std::vector<uint8_t> myNodes;
	private:
		void StampCircleLine(int64_t aX, int64_t aY, int64_t aEndX, int64_t aEndY, int64_t aRadius, uint8_t aValue);
		void GetResultPath(ValuedNode& aValuedNode, std::vector<math::vector2<float>>& aResultPath) const;
		bool HasLineOfSight(int64_t aNodeIndexLhs, int64_t aNodeIndexRhs) const;
		std::vector<ValuedNode> GetNeighbours(ValuedNode& aValuedNode, math::vector2<float> aOrigin, math::vector2<float> aTarget) const;
		std::vector<size_t> GetNeighbours(int64_t aNodeIndex) const;
		bool HasSameWidthIndex(int64_t aOriginNodeIndex, int64_t aTargetNodeIndex) const;

		// C++ Modulo does not return correct values for negative values, so use this instead
		inline static int64_t Mod(int64_t aLhs, int64_t aRhs)
		{
			return (aLhs % aRhs + aRhs) % aRhs;
		}

		uint32_t myWidth = 1, myHeight = 1;
	};
}
