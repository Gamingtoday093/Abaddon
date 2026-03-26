#pragma once
#include <queue>
#include "ValuedNode.h"

namespace Navigation
{
	class OpenNodesHeap : public std::priority_queue<ValuedNode*, std::vector<ValuedNode*>, ValuedNodePtrComparer>
	{
	public:
		~OpenNodesHeap();

		bool ContainsValuedNode(const ValuedNode& aValuedNode);
	};
}
