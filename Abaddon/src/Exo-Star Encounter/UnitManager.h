#pragma once
#include "Unit.h"

class UnitManager
{
public:
	UnitManager() = default;
	~UnitManager() = default;

	static std::shared_ptr<Unit> CreateUnit();
	static std::vector<std::shared_ptr<Unit>> GetUnits();

	UnitManager(UnitManager& other) = delete;
	void operator=(const UnitManager&) = delete;

private:
	static UnitManager& GetInstance();
	std::vector<std::shared_ptr<Unit>> myUnits;
};

