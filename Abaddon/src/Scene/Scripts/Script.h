#pragma once
#include "Scene/Entity.h"
#include <typeinfo>

class Script
{
public:
	Script() = default;
	virtual ~Script() {};

	template<typename T>
	T& GetComponent()
	{
		return myEntity.GetComponent<T>();
	}

protected:
	virtual void Awake() {};
	virtual void Start() {};
	virtual void OnDestroy() {};
	virtual void Update() {};

	Entity myEntity;

private:
	friend class Scene;
	friend struct ScriptComponent;
};

