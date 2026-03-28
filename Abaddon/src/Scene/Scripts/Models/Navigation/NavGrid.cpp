#include "pch.h"
#include "NavGrid.h"
#include "ValuedNode.h"
#include "Tools/Stopwatch.h"
#include "OpenNodesHeap.h"

namespace Navigation
{
	void NavGrid::Resize(uint16_t aWidth, uint16_t aHeight)
	{
		myWidth = aWidth;
		myHeight = aHeight;

		myNodes.assign(size_t(myWidth) * size_t(myHeight), 0);
	}

	void NavGrid::StampSquare(GridCoord aNodeCoord, uint8_t aValue, int32_t aRadius)
	{
		int32_t xMin = std::max(-aRadius, -aNodeCoord.x);
		int32_t xMax = std::min(aRadius, myWidth - 1 - aNodeCoord.x);
		int32_t yMin = std::max(-aRadius, -aNodeCoord.y);
		int32_t yMax = std::min(aRadius, myHeight - 1 - aNodeCoord.y);

		for (int32_t x = xMin; x <= xMax; x++)
			for (int32_t y = yMin; y <= yMax; y++)
				myNodes[size_t(((aNodeCoord.x + x) * int32_t(myHeight)) + (aNodeCoord.y + y))] = aValue;
	}

	void NavGrid::StampCircle(GridCoord aNodeCoord, uint8_t aValue, int32_t aRadius)
	{
		int32_t x = 0;
		int32_t y = -aRadius;
		int32_t p = -aRadius;

		// Fill Center
		StampSquare(aNodeCoord, aValue, aRadius - (aRadius / 3));
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

			int32_t lineLength = (aRadius / 3) - (aRadius + y);
			StampCircleLine({ aNodeCoord.x + x, aNodeCoord.y + y }, { aNodeCoord.x + x, aNodeCoord.y - y }, lineLength, aValue);
			StampCircleLine({ aNodeCoord.x - x, aNodeCoord.y + y }, { aNodeCoord.x - x, aNodeCoord.y - y }, lineLength, aValue);
			
			StampCircleLine({ aNodeCoord.x + y, aNodeCoord.y + x }, { aNodeCoord.x - y, aNodeCoord.y + x }, lineLength, aValue);
			StampCircleLine({ aNodeCoord.x + y, aNodeCoord.y - x }, { aNodeCoord.x - y, aNodeCoord.y - x }, lineLength, aValue);

			x++;
		}
	}

	void NavGrid::StampCircleLine(GridCoord aStartNodeCoord, GridCoord aEndNodeCoord, int32_t aRadius, uint8_t aValue)
	{
		int32_t stepX = 0, stepY = 0;
		if (aStartNodeCoord.x == aEndNodeCoord.x) stepY = aStartNodeCoord.y > aEndNodeCoord.y ? -1 : 1;
		else stepX = aStartNodeCoord.x > aEndNodeCoord.x ? -1 : 1;

		for (int32_t i = 0; i < aRadius; i++)
		{
			GridCoord targetCoord = { aStartNodeCoord.x + (stepX * i), aStartNodeCoord.y + (stepY * i) };
			if (targetCoord.x >= 0 && targetCoord.x < myWidth &&
				targetCoord.y >= 0 && targetCoord.y < myHeight)
			{
				myNodes[size_t((targetCoord.x * myHeight) + targetCoord.y)] = aValue;
			}

			targetCoord = { aEndNodeCoord.x + (-stepX * i), aEndNodeCoord.y + (-stepY * i) };
			if (targetCoord.x >= 0 && targetCoord.x < myWidth && targetCoord.y >= 0 &&
				targetCoord.y < myHeight)
			{
				myNodes[size_t((targetCoord.x * myHeight) + targetCoord.y)] = aValue;
			}
		}
	}

	bool NavGrid::Pathfind(math::vector2<float> aOrigin, math::vector2<float> aTarget, std::vector<math::vector2<float>>& aResultPath) const
	{
		aResultPath.clear();

		int32_t startNode = GetNearestIndex(aOrigin);
		int32_t endNode = GetNearestIndex(aTarget);

		if (startNode == endNode)
		{
			aResultPath.push_back(GetWorldPosition(startNode));
			return true;
		}

		if (HasLineOfSight(startNode, endNode))
		{
			aResultPath.push_back(GetWorldPosition(startNode));
			aResultPath.push_back(GetWorldPosition(endNode));
			return true;
		}

		std::unordered_map<int32_t, int32_t> Parents;
		OpenNodesHeap OpenNodes;
		std::unordered_set<int32_t> ClosedNodes;

		Parents[startNode] = startNode;
		OpenNodes.emplace(startNode, *this, aOrigin, aTarget);

		while (!OpenNodes.empty())
		{
			ValuedNode lowestNode = OpenNodes.top();
			OpenNodes.pop();
			ClosedNodes.insert(lowestNode.myNodeIndex);

			if (lowestNode.myNodeIndex == endNode)
			{
			EndNodeFound:
				GetResultPath(endNode, startNode, Parents, aResultPath);
				break;
			}

			if (lowestNode.myNodeIndex != startNode && HasLineOfSight(lowestNode.myNodeIndex, endNode))
			{
				Parents.insert_or_assign(endNode, lowestNode.myNodeIndex);
				goto EndNodeFound;
			}

			for (int32_t neighbourIndex : GetNeighbours(lowestNode.myNodeIndex))
			{
				if (myNodes[neighbourIndex] == 0xff) continue; // Should be moved into GetNeighbours
				if (neighbourIndex == endNode)
				{
					Parents.insert_or_assign(neighbourIndex, lowestNode.myNodeIndex);
					goto EndNodeFound;
				}
				if (ClosedNodes.contains(neighbourIndex)) continue;
				else if (!OpenNodes.ContainsNodeIndex(neighbourIndex))
				{
					OpenNodes.emplace(neighbourIndex, *this, aOrigin, aTarget);
					Parents.insert_or_assign(neighbourIndex, lowestNode.myNodeIndex);
				}
			}
		}

		return !aResultPath.empty();
	}

	int32_t NavGrid::GetNearestIndex(math::vector2<float> aPosition) const
	{
		int32_t worldWidth = int32_t(aPosition.x / myNodeSize);
		int32_t worldHeight = int32_t(aPosition.y / myNodeSize);

		if (worldWidth < 0) worldWidth = 0;
		else if (worldWidth >= myWidth) worldWidth = int32_t(myWidth) - 1;

		if (worldHeight < 0) worldHeight = 0;
		else if (worldHeight >= myHeight) worldHeight = int32_t(myHeight) - 1;

		return (worldWidth * myHeight) + worldHeight;
	}

	GridCoord NavGrid::GetNearest(math::vector2<float> aPosition) const
	{
		int32_t worldWidth = int32_t(aPosition.x / myNodeSize);
		int32_t worldHeight = int32_t(aPosition.y / myNodeSize);

		if (worldWidth < 0) worldWidth = 0;
		else if (worldWidth >= myWidth) worldWidth = int32_t(myWidth) - 1;

		if (worldHeight < 0) worldHeight = 0;
		else if (worldHeight >= myHeight) worldHeight = int32_t(myHeight) - 1;

		return { worldWidth, worldHeight };
	}

	math::vector2<float> NavGrid::GetWorldPosition(int32_t aNodeIndex) const
	{
		int32_t width = (aNodeIndex - Mod(aNodeIndex, myHeight)) / int32_t(myHeight);
		int32_t height = Mod(aNodeIndex, myHeight);
		return { (width * myNodeSize) + (myNodeSize * 0.5f), (height * myNodeSize) + (myNodeSize * 0.5f) };
	}

	math::vector2<float> NavGrid::GetWorldPosition(GridCoord aNodeCoord) const
	{
		return { (aNodeCoord.x * myNodeSize) + (myNodeSize * 0.5f), (aNodeCoord.y * myNodeSize) + (myNodeSize * 0.5f) };
	}

	uint16_t NavGrid::GetWidth() const
	{
		return myWidth;
	}

	uint16_t NavGrid::GetHeight() const
	{
		return myHeight;
	}

	const std::vector<uint8_t>& NavGrid::GetNodes() const
	{
		return myNodes;
	}

	void NavGrid::GetResultPath(int32_t aFinalNodeIndex, int32_t aStartNodeIndex, const std::unordered_map<int32_t, int32_t>& aParents, std::vector<math::vector2<float>>& aResultPath) const
	{
		int32_t currentNodeIndex = aFinalNodeIndex;
		while (true)
		{
			aResultPath.push_back(GetWorldPosition(currentNodeIndex));

			if (aParents.at(currentNodeIndex) == currentNodeIndex) break;

			int32_t lineOfSightTarget = currentNodeIndex;
			currentNodeIndex = aParents.at(currentNodeIndex);

			// Pathfind already performs a LineOfSight check to try and reach it's final node
			if (lineOfSightTarget == aFinalNodeIndex) continue;
			int32_t LOSChild = lineOfSightTarget;
			while (aParents.at(lineOfSightTarget) != lineOfSightTarget)
			{
				if (HasLineOfSight(currentNodeIndex, lineOfSightTarget))
				{
					LOSChild = lineOfSightTarget;
					lineOfSightTarget = aParents.at(lineOfSightTarget);
					if (aParents.at(lineOfSightTarget) == lineOfSightTarget)
					{
						currentNodeIndex = lineOfSightTarget;
						//aResultPath.push_back(GetWorldPosition(lineOfSightTarget));
						break;
					}
				}
				else if (HasLineOfSight(currentNodeIndex, aStartNodeIndex))
				{
					LOSChild = lineOfSightTarget;
					lineOfSightTarget = aParents.at(lineOfSightTarget);
				}
				else
				{
					currentNodeIndex = LOSChild;
					break;
				}
			}
		}

		std::reverse(aResultPath.begin(), aResultPath.end());
	}

	bool NavGrid::HasLineOfSight(int32_t aOriginNodeIndex, int32_t aTargetNodeIndex) const
	{
		if (aOriginNodeIndex == aTargetNodeIndex && myNodes[aOriginNodeIndex] == 0) return true;

		int32_t originX = (aOriginNodeIndex - (aOriginNodeIndex % myHeight)) / myHeight;
		int32_t originY = aOriginNodeIndex % myHeight;

		int32_t targetX = (aTargetNodeIndex - (aTargetNodeIndex % myHeight)) / myHeight;
		int32_t targetY = aTargetNodeIndex % myHeight;

		int32_t deltaX = abs(originX - targetX);
		int32_t deltaY = abs(originY - targetY);

		int32_t stepX = originX < targetX ? 1 : -1;
		int32_t stepY = originY < targetY ? 1 : -1;

		int32_t xx, xy, yx, yy;
		if (deltaX > deltaY)
		{
			xx = stepX;
			xy = 0;
			yx = 0;
			yy = stepY;
		}
		else
		{
			std::swap(deltaX, deltaY);

			xx = 0;
			xy = stepY;
			yx = stepX;
			yy = 0;
		}

		int32_t D = (2 * deltaY) - deltaX;
		int32_t currentY = 0;

		for (int32_t currentX = 0; currentX < deltaX + 1; currentX++)
		{
			int32_t indexWidth = originX + currentX * xx + currentY * yx;
			int32_t indexHeight = originY + currentX * xy + currentY * yy;
			if (myNodes[size_t((indexWidth * int32_t(myHeight)) + indexHeight)] != 0) return false;

			if (D >= 0)
			{
				currentY++;
				D -= 2 * deltaX;
			}
			D += 2 * deltaY;
		}

		return true;
	}
	
	std::vector<int32_t> NavGrid::GetNeighbours(int32_t aNodeIndex) const
	{
		std::vector<int32_t> neighbours;
		neighbours.reserve(8);

		GridCoord nodeCoord = { (aNodeIndex - (aNodeIndex % myHeight)) / myHeight, aNodeIndex % myHeight };

		if (nodeCoord.y - 1 >= 0) neighbours.push_back(aNodeIndex - 1);
		if (nodeCoord.y + 1 < myHeight) neighbours.push_back(aNodeIndex + 1);

		if (nodeCoord.x - 1 >= 0)
		{
			if (nodeCoord.y - 1 >= 0) neighbours.push_back(aNodeIndex - 1 - myHeight);
			neighbours.push_back(aNodeIndex - myHeight);
			if (nodeCoord.y + 1 < myHeight) neighbours.push_back(aNodeIndex + 1 - myHeight);
		}

		if (nodeCoord.x + 1 < myWidth)
		{
			if (nodeCoord.y - 1 >= 0) neighbours.push_back(aNodeIndex - 1 + myHeight);
			neighbours.push_back(aNodeIndex + myHeight);
			if (nodeCoord.y + 1 < myHeight) neighbours.push_back(aNodeIndex + 1 + myHeight);
		}

		return neighbours;
	}
}
