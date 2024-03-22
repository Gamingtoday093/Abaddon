#pragma once
#include "Scene/Transform.h"
#include "Scene/Scripts/Script.h"
#include <functional>
#include <string>

struct TransformComponent
{
	Transform myTransform;

	AABB TransformAABB(AABB aAABB)
	{
		XMVECTOR min = XMVectorSet(aAABB.myMin.x, aAABB.myMin.y, aAABB.myMin.z, 0);
		XMVECTOR max = XMVectorSet(aAABB.myMax.x, aAABB.myMax.y, aAABB.myMax.z, 0);
	
		XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(myTransform.myRotation.x, myTransform.myRotation.y, myTransform.myRotation.z) *	// ***
						  DirectX::XMMatrixTranslation(myTransform.myPosition.x, myTransform.myPosition.y, myTransform.myPosition.z) *			// World Matrix
						  DirectX::XMMatrixScaling(myTransform.myScale.x, myTransform.myScale.y, myTransform.myScale.z);						// ***
	
		min = XMVector3TransformCoord(min, matrix);
		max = XMVector3TransformCoord(max, matrix);
	
		math::vector3<float> minPos = { XMVectorGetX(min), XMVectorGetY(min), XMVectorGetZ(min) };
		math::vector3<float> maxPos = { XMVectorGetX(max), XMVectorGetY(max), XMVectorGetZ(max) };
	
		return AABB(minPos, maxPos);
	}
};

struct TagComponent
{
	TagComponent(std::string aTag)
	{
		myTag = aTag;
	}

	std::string myTag;
};

struct ScriptComponent
{
	Script* myInstance = nullptr;
	bool myHasStarted = false;

	Script*(*InitFunction)();
	void(*DestroyFunction)(ScriptComponent*);

	template<typename T>
	Script* Bind(Entity& aEntity)
	{
		InitFunction = []() { return static_cast<Script*>(new T()); };
		DestroyFunction = [](ScriptComponent* aScriptComponent) { delete aScriptComponent->myInstance; aScriptComponent->myInstance = nullptr; };

		myInstance = InitFunction();
		myInstance->myEntity = aEntity;

		myInstance->Awake();

		return myInstance;
	}
};

struct ModelComponent
{
	ModelComponent(std::string aModelName, std::string aTextureName)
	{
		myModelName = aModelName;
		myTextureName = aTextureName;
	}

	std::string myModelName;
	std::string myTextureName;

	void SetModelAndTexture(std::string aModelName, std::string aTextureName)
	{
		myModelName = aModelName;
		myTextureName = aTextureName;
	}
};

