#include "pch.h"
#include "SpatialHash.h"
#include "Scene/Scripts/NavigationAgent.h"
#include "Scene/Components/Components.h"

SpatialHash::SpatialHash(float aCellSize) : myCellSizeMultiplier(1.0f / aCellSize) { }

void SpatialHash::Rebuild(const std::vector<NavigationAgent*>& aAgents)
{
	myCells.clear();
	for (auto agent : aAgents)
	{
		auto position = agent->myTransform->myTransform.myPosition;
		auto x = int32_t(position.x * myCellSizeMultiplier);
		auto z = int32_t(position.z * myCellSizeMultiplier);
		myCells[GetCellKey(x, z)].push_back(agent);
	}
}

std::vector<NavigationAgent*> SpatialHash::Query(math::vector3<float> aPosition, float aRadius) const
{
	std::vector<NavigationAgent*> neighbours;

	auto radiusCells = int32_t(aRadius * myCellSizeMultiplier) + 1;
	auto centerX = int32_t(aPosition.x * myCellSizeMultiplier);
	auto centerZ = int32_t(aPosition.z * myCellSizeMultiplier);

	for (int32_t z = -radiusCells; z <= radiusCells; z++)
		for (int32_t x = -radiusCells; x <= radiusCells; x++)
		{
			auto pair = myCells.find(GetCellKey(centerX + x, centerZ + z));
			if (pair == myCells.end()) continue;

			neighbours.insert(neighbours.end(), pair->second.begin(), pair->second.end());
		}

	return neighbours;
}

uint64_t SpatialHash::GetCellKey(int32_t x, int32_t z) const
{
	return uint64_t(x) | (uint64_t(z) << 32);
}
