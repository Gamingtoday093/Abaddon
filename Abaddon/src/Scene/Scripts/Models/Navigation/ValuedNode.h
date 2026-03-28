#pragma once

namespace Navigation
{
	class NavGrid;
	
	struct ValuedNode
	{
		ValuedNode(int32_t aNodeIndex, const NavGrid& aNodeGrid, math::vector2<float> aOrigin, math::vector2<float> aTarget);

		inline float GetFCost() const
		{
			return myGCost + (GoalBias * myHCost);
		}

		int32_t myNodeIndex;

		static constexpr float GoalBias = 2.f;
		float myGCost;
		float myHCost;
	};

	struct ValuedNodeComparer
	{
		bool operator()(const ValuedNode& _Left, const ValuedNode& _Right) const
		{
			return _Left.GetFCost() > _Right.GetFCost();
		}
	};
}
