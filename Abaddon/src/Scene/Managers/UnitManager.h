#pragma once
#include <Scene/Scripts/Unit.h>

class UnitManager : public Script
{
public:
	void Awake() override;
	void Update() override;

	static Unit* CreateUnit(Entity& entity);
	static const std::shared_ptr<std::vector<Unit*>> GetUnits();

private:
	static UnitManager& GetInstance();
	static UnitManager* myInstance;

	void HandleInput();
	void InputMoveTo();
	void InputSelect();
	bool dragging = false;
	math::vector2<float> dragSelectStart;

	std::shared_ptr<std::vector<Unit*>> myUnits;
};

