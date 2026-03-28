#pragma once
#include <queue>
#include "ValuedNode.h"

namespace Navigation
{
	class OpenNodesHeap : public std::priority_queue<ValuedNode, std::vector<ValuedNode>, ValuedNodeComparer>
	{
	public:
		bool ContainsValuedNode(const ValuedNode& aValuedNode);
		bool ContainsNodeIndex(int32_t aNodeIndex);
	};
}
