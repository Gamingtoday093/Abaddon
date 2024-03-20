#include "pch.h"
#include "UnitManager.h"

std::shared_ptr<Unit> UnitManager::CreateUnit()
{
    std::shared_ptr<Unit> newUnit = std::make_shared<Unit>();
    UnitManager::GetInstance().myUnits.push_back(newUnit);
    return newUnit;
}

std::vector<std::shared_ptr<Unit>> UnitManager::GetUnits()
{
    return UnitManager::GetInstance().myUnits;
}

UnitManager& UnitManager::GetInstance()
{
    static UnitManager instance;

    return instance;
}
