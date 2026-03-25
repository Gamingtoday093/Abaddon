#pragma once
#include <queue>
#include "ValuedNode.h"

namespace Navigation
{
	class OpenNodesHeap : public std::priority_queue<ValuedNode*, std::vector<ValuedNode*>, ValuedNodePtrComparer>
	{
	public:
		~OpenNodesHeap()
		{
			for (ValuedNode* valuedNode : c)
				delete valuedNode;

			c.clear();
		}

		bool ContainsValuedNode(const ValuedNode& aValuedNode)
		{
			for (ValuedNode* valuedNode : c)
				if (valuedNode->myNodeIndex == aValuedNode.myNodeIndex) return true;

			return false;
		}
	};
}
