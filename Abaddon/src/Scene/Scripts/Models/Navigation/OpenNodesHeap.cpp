#include "pch.h"
#include "OpenNodesHeap.h"

namespace Navigation
{
	OpenNodesHeap::~OpenNodesHeap()
	{
		for (ValuedNode* valuedNode : c)
			delete valuedNode;

		c.clear();
	}

	bool OpenNodesHeap::ContainsValuedNode(const ValuedNode& aValuedNode)
	{
		for (const ValuedNode* valuedNode : c)
			if (valuedNode->myNodeIndex == aValuedNode.myNodeIndex) return true;

		return false;
	}
}
