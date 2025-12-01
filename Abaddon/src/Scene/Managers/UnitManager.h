#pragma once
#include "Scene/Scripts/Unit.h"
#include <unordered_map>

class UnitManager : public Script
{
public:
	void Awake() override;
	void Update() override;

	static Unit* CreateUnit(Entity& entity);
	static const std::vector<Unit*>& GetUnits();

private:
	static UnitManager& GetInstance();
	static UnitManager* myInstance;

	void HandleInput();
	void InputMoveTo(const math::vector3<float> aRayOrigin, const math::vector3<float> aRayDirection, const math::vector3<float> aNewPosition);
	void RemoveMappedUnit(Unit* unit);
	void InputSelectMouse();
	bool multipleSelected = false;
	bool dragging = false;
	math::vector2<float> dragSelectStart;

	std::unordered_map<int, std::vector<Unit*>> myMappedUnits;
	std::vector<Unit*> myUnits;
};

