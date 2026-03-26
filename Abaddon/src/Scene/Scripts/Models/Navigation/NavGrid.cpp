#include "pch.h"
#include "NavGrid.h"
#include "ValuedNode.h"
#include "Tools/Stopwatch.h"
#include "OpenNodesHeap.h"

namespace Navigation
{
	void NavGrid::Resize(uint32_t aWidth, uint32_t aHeight)
	{
		myWidth = aWidth;
		myHeight = aHeight;

		myNodes.clear();
		myNodes.reserve(size_t(myWidth) * size_t(myHeight));
		for (size_t w = 0; w < myWidth; w++)
			for (size_t h = 0; h < myHeight; h++)
				myNodes.push_back(0);

		//for (size_t i = 4; i < 35; i++)
		//{
		//	myNodes[(50 * myHeight) + i] = 0xff;
		//}
	}

	void NavGrid::StampSquare(size_t aNodeIndex, uint8_t aValue, int32_t aRadius)
	{
		for (int64_t x = -aRadius; x <= aRadius; x++)
			for (int64_t y = -aRadius; y <= aRadius; y++)
			{
				int64_t index = int64_t(aNodeIndex) + (x * int64_t(myHeight)) + y;
				if (index < 0 || size_t(index) >= myNodes.size()) continue;
				myNodes[index] = aValue;
			}
	}

	void NavGrid::StampCircle(size_t aNodeIndex, uint8_t aValue, int32_t aRadius)
	{
		int64_t cx = (aNodeIndex - Mod(aNodeIndex, myHeight)) / myHeight;
		int64_t cy = Mod(aNodeIndex, myHeight);

		int64_t x = 0;
		int64_t y = -aRadius;
		int64_t p = -aRadius;

		// Fill Center
		StampSquare(aNodeIndex, aValue, aRadius - (aRadius / 3));
		while (x < -y)
		{
			if (p > 0)
			{
				y++;
				p += 2 * (x + y) + 1;
			}
			else
			{
				p += 2 * x + 1;
			}

			int64_t lineLength = (aRadius / 3) - (aRadius + y);
			StampCircleLine(cx + x, cy + y, cx + x, cy - y, lineLength, aValue);
			StampCircleLine(cx - x, cy + y, cx - x, cy - y, lineLength, aValue);
			
			StampCircleLine(cx + y, cy + x, cx - y, cy + x, lineLength, aValue);
			StampCircleLine(cx + y, cy - x, cx - y, cy - x, lineLength, aValue);

			x++;
		}
	}

	void NavGrid::StampCircleLine(int64_t aX, int64_t aY, int64_t aEndX, int64_t aEndY, int64_t aRadius, uint8_t aValue)
	{
		int64_t stepX = 0;
		int64_t stepY = 0;
		if (aX == aEndX) stepY = aY > aEndY ? -1 : 1;
		else stepX = aX > aEndX ? -1 : 1;

		for (size_t i = 0; i < aRadius; i++)
		{
			int64_t index = ((aX + (stepX * i)) * int64_t(myHeight)) + aY + (stepY * i);
			if (index >= 0 && size_t(index) < myNodes.size()) myNodes[index] = aValue;

			index = ((aEndX + (-stepX * i)) * int64_t(myHeight)) + aEndY + (-stepY * i);
			if (index >= 0 && size_t(index) < myNodes.size()) myNodes[index] = aValue;
		}
	}

	bool NavGrid::Pathfind(math::vector2<float> aOrigin, math::vector2<float> aTarget, std::vector<math::vector2<float>>& aResultPath) const
	{
		aResultPath.clear();

		size_t startNode = GetNearest(aOrigin);
		size_t endNode = GetNearest(aTarget);

		if (startNode == endNode)
		{
			aResultPath.push_back(GetWorldPosition(startNode));
			return true;
		}

		OpenNodesHeap OpenNodes;
		std::unordered_map<size_t, ValuedNode*> ClosedNodes;

		OpenNodes.push(new ValuedNode(startNode, *this, nullptr, aOrigin, aTarget));

		Stopwatch sw = Stopwatch::StartNew();
		while (OpenNodes.size() > 0)
		{
			ValuedNode& lowestNode = *OpenNodes.top();
			ClosedNodes.try_emplace(lowestNode.myNodeIndex, &lowestNode);
			OpenNodes.pop();

			if (lowestNode.myNodeIndex == endNode)
			{
				GetResultPath(lowestNode, aResultPath);
				break;
			}

			for (const ValuedNode& valuedNode : GetNeighbours(lowestNode, aOrigin, aTarget))
			{
				if (ClosedNodes.contains(valuedNode.myNodeIndex)) continue;
				else if (!OpenNodes.ContainsValuedNode(valuedNode)) OpenNodes.push(new ValuedNode(valuedNode));
			}
		}
		sw.Stop();
		LOG("Pathfinding Internal took: " + std::to_string(sw.GetElapsedMilliseconds()) + "ms (OpenNodes: " + std::to_string(OpenNodes.size()) + ") (ClosedNodes: " + std::to_string(ClosedNodes.size()) + ")");
		
		for (auto& pair : ClosedNodes)
			delete pair.second;
		
		return aResultPath.size() > 0;
	}

	size_t NavGrid::GetNearest(math::vector2<float> aPosition) const
	{
		int64_t worldWidth = int64_t(aPosition.x / myNodeSize);
		int64_t worldHeight = int64_t(aPosition.y / myNodeSize);

		if (worldWidth < 0) worldWidth = 0;
		else if (worldWidth >= myWidth) worldWidth = int64_t(myWidth) - 1;

		if (worldHeight < 0) worldHeight = 0;
		else if (worldHeight >= myHeight) worldHeight = int64_t(myHeight) - 1;

		return (worldWidth * myHeight) + worldHeight;
	}

	math::vector2<float> NavGrid::GetWorldPosition(int64_t aNodeIndex) const
	{
		int64_t width = (aNodeIndex - Mod(aNodeIndex, myHeight)) / myHeight;
		int64_t height = Mod(aNodeIndex, myHeight);
		return { (width * myNodeSize) + (myNodeSize * 0.5f), (height * myNodeSize) + (myNodeSize * 0.5f) };
	}

	void NavGrid::GetResultPath(ValuedNode& aValuedNode, std::vector<math::vector2<float>>& aResultPath) const
	{
		ValuedNode* currentNode = &aValuedNode;

		while (currentNode != nullptr)
		{
			aResultPath.push_back(GetWorldPosition(currentNode->myNodeIndex));

			ValuedNode* lineOfSightTarget = currentNode;
			currentNode = currentNode->myParent;
			while (lineOfSightTarget->myParent != nullptr)
			{
				if (HasLineOfSight(currentNode->myNodeIndex, lineOfSightTarget->myParent->myNodeIndex))
				{
					lineOfSightTarget = lineOfSightTarget->myParent;
					if (lineOfSightTarget->myParent == nullptr)
					{
						currentNode = lineOfSightTarget;
						break;
					}
				}
				else
				{
					currentNode = lineOfSightTarget;
					break;
				}
			}
		}

		std::reverse(aResultPath.begin(), aResultPath.end());
	}

	bool NavGrid::HasLineOfSight(int64_t aOriginNodeIndex, int64_t aTargetNodeIndex) const
	{
		if (aOriginNodeIndex == aTargetNodeIndex && myNodes[aOriginNodeIndex] == 0) return true;

		int64_t originX = (aOriginNodeIndex - Mod(aOriginNodeIndex, myHeight)) / myHeight;
		int64_t originY = Mod(aOriginNodeIndex, myHeight);

		int64_t targetX = (aTargetNodeIndex - Mod(aTargetNodeIndex, myHeight)) / myHeight;
		int64_t targetY = Mod(aTargetNodeIndex, myHeight);

		int64_t deltaX = abs(originX - targetX);
		int64_t deltaY = abs(originY - targetY);

		int64_t stepX = originX < targetX ? 1 : -1;
		int64_t stepY = originY < targetY ? 1 : -1;

		int64_t xx, xy, yx, yy;
		if (deltaX > deltaY)
		{
			xx = stepX;
			xy = 0;
			yx = 0;
			yy = stepY;
		}
		else
		{
			// Swap values
			deltaX = deltaY ^ deltaX;
			deltaY = deltaY ^ deltaX;
			deltaX = deltaY ^ deltaX;

			xx = 0;
			xy = stepY;
			yx = stepX;
			yy = 0;
		}

		int64_t D = (2 * deltaY) - deltaX;
		int64_t currentY = 0;

		for (int64_t currentX = 0; currentX < deltaX + 1; currentX++)
		{
			int64_t indexWidth = originX + currentX * xx + currentY * yx;
			int64_t indexHeight = originY + currentX * xy + currentY * yy;
			if (myNodes[(indexWidth * int64_t(myHeight)) + indexHeight] != 0) return false;

			if (D >= 0)
			{
				currentY++;
				D -= 2 * deltaX;
			}
			D += 2 * deltaY;
		}

		return true;
	}
	
	std::vector<ValuedNode> NavGrid::GetNeighbours(ValuedNode& aValuedNode, math::vector2<float> aOrigin, math::vector2<float> aTarget) const
	{
		std::vector<ValuedNode> neighbours;

		for (size_t nodeIndex : GetNeighbours(aValuedNode.myNodeIndex))
		{
			if (myNodes[nodeIndex] == 0xff) continue;
			neighbours.emplace_back(nodeIndex, *this, &aValuedNode, aOrigin, aTarget);
		}

		return neighbours;
	}
	
	std::vector<size_t> NavGrid::GetNeighbours(int64_t aNodeIndex) const
	{
		std::vector<size_t> neighbours;
		neighbours.reserve(8);

		if (HasSameWidthIndex(aNodeIndex, aNodeIndex - 1)) neighbours.push_back(aNodeIndex - 1);
		if (HasSameWidthIndex(aNodeIndex, aNodeIndex + 1)) neighbours.push_back(aNodeIndex + 1);

		if (aNodeIndex - myHeight >= 0)
		{
			if (HasSameWidthIndex(aNodeIndex - myHeight, aNodeIndex - 1 - myHeight)) neighbours.push_back(aNodeIndex - 1 - myHeight);
			neighbours.push_back(aNodeIndex - myHeight);
			if (HasSameWidthIndex(aNodeIndex - myHeight, aNodeIndex + 1 - myHeight)) neighbours.push_back(aNodeIndex + 1 - myHeight);
		}
		
		if (aNodeIndex + myHeight < int64_t(myNodes.size()))
		{
			if (HasSameWidthIndex(aNodeIndex + myHeight, aNodeIndex - 1 + myHeight)) neighbours.push_back(aNodeIndex - 1 + myHeight);
			neighbours.push_back(aNodeIndex + myHeight);
			if (HasSameWidthIndex(aNodeIndex + myHeight, aNodeIndex + 1 + myHeight)) neighbours.push_back(aNodeIndex + 1 + myHeight);
		}

		return neighbours;
	}

	bool NavGrid::HasSameWidthIndex(int64_t aNodeIndexLhs, int64_t aNodeIndexRhs) const
	{
		int64_t widthLhs = (aNodeIndexLhs - Mod(aNodeIndexLhs, myHeight)) / myHeight;
		int64_t widthRhs = (aNodeIndexRhs - Mod(aNodeIndexRhs, myHeight)) / myHeight;

		return widthRhs == widthLhs;
	}
}
