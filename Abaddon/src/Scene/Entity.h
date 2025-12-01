#pragma once
#include "Scene.h"
#include "EnTT/entt.hpp"

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity aHandle, Scene* aScene);
	~Entity();
	
	template<typename T, typename... Args>
	T& AddComponent(Args&&... aArgs)
	{
		if (HasComponent<T>())
		{
			LOG_ERROR("Entity already has component");
			Assert(false);
		}

		return myScene->myRegistry.emplace<T>(myEntityHandle, std::forward<Args>(aArgs)...);
	}

	template<typename T>
	T& GetComponent() const
	{
		if (!HasComponent<T>())
		{
			LOG_ERROR("Entity does not have component");
			Assert(false);
		}

		return myScene->myRegistry.get<T>(myEntityHandle);
	}

	template <typename T>
	bool HasComponent() const
	{
		return myScene->myRegistry.any_of<T>(myEntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		if (!HasComponent<T>())
		{
			LOG_ERROR("Entity does not have component");
			Assert(false);
		}

		myScene->myRegistry.remove<T>(myEntityHandle);
	}

	Scene& GetScene();

	bool operator==(entt::entity aEntity);
	bool operator==(Entity aEntity);

private:
	entt::entity myEntityHandle{ 0 };
	Scene* myScene = nullptr;
};
