#pragma once

namespace Navigation
{
	class NavGrid;
	
	struct ValuedNode
	{
		ValuedNode(size_t aNodeIndex, const NavGrid& aNodeGrid, ValuedNode* aParent, math::vector2<float> aOrigin, math::vector2<float> aTarget);

		inline float GetFCost() const
		{
			return myGCost + (GoalBias * myHCost);
		}

		ValuedNode* myParent;
		size_t myNodeIndex;

		static constexpr float GoalBias = 2.f;
		float myGCost;
		float myHCost;

		bool operator==(const ValuedNode& other) const
		{
			return myNodeIndex == other.myNodeIndex;
		}
	};

	struct ValuedNodePtrComparer
	{
		bool operator()(const ValuedNode* _Left, const ValuedNode* _Right) const
		{
			return _Left->GetFCost() > _Right->GetFCost();
		}
	};

	struct ValuedNodeComparer
	{
		bool operator()(const ValuedNode& _Left, const ValuedNode& _Right) const
		{
			return _Left.GetFCost() > _Right.GetFCost();
		}
	};
}
