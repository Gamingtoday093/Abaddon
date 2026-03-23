#pragma once
#include "Scene/Entity.h"
#include <typeinfo>

class Script
{
public:
	Script() = default;
	virtual ~Script() = default;

	template<typename T>
	T& GetComponent()
	{
		return myEntity.GetComponent<T>();
	}

#define SCRIPT_NAME(ScriptName) const char* GetScriptName() const override { return #ScriptName; };
	virtual const char* GetScriptName() const = 0;

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

