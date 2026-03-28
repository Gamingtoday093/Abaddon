#include "pch.h"
#include "ValuedNode.h"
#include "NavGrid.h"

namespace Navigation
{
	ValuedNode::ValuedNode(int32_t aNodeIndex, const NavGrid& aNodeGrid, math::vector2<float> aOrigin, math::vector2<float> aTarget)
		: myNodeIndex(aNodeIndex)
	{
		math::vector2<float> worldPosition = aNodeGrid.GetWorldPosition(aNodeIndex);
		myGCost = (worldPosition - aOrigin).Length();
		myHCost = (aTarget - worldPosition).Length();
	}
}
