#include "pch.h"
#include "Entity.h"

Entity::Entity(entt::entity aHandle, Scene* aScene) : myEntityHandle(aHandle), myScene(aScene)
{
}

Entity::~Entity()
{
	myScene = nullptr;
	delete myScene;
}

Scene& Entity::GetScene()
{
	return *myScene;
}

bool Entity::operator==(entt::entity aEntity)
{
	return myEntityHandle == aEntity;
}

bool Entity::operator==(Entity aEntity)
{
	return myEntityHandle == aEntity.myEntityHandle;
}
