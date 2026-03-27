#include "pch.h"
#include "OpenNodesHeap.h"

namespace Navigation
{
	bool OpenNodesHeap::ContainsValuedNode(const ValuedNode& aValuedNode)
	{
		for (const ValuedNode& valuedNode : c)
			if (valuedNode.myNodeIndex == aValuedNode.myNodeIndex) return true;

		return false;
	}

	bool OpenNodesHeap::ContainsNodeIndex(size_t aNodeIndex)
	{
		for (const ValuedNode& valuedNode : c)
			if (valuedNode.myNodeIndex == aNodeIndex) return true;

		return false;
	}
}
