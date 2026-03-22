#pragma once

class NavigationAgent;

class SpatialHash
{
public:
	SpatialHash(float aCellSize);
	~SpatialHash() = default;

	void Rebuild(const std::vector<NavigationAgent*>& aAgents);
	std::vector<NavigationAgent*> Query(math::vector3<float> aPosition, float aRadius) const;

private:
	std::unordered_map<uint64_t, std::vector<NavigationAgent*>> myCells;
	float myCellSizeMultiplier;

	uint64_t GetCellKey(int32_t x, int32_t z) const;
};

