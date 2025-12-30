#pragma once
#include "Scene/Transform.h"
#include "Scene/Scripts/Script.h"
#include "Graphics/AABB.h"
#include <functional>
#include <string>

struct TransformComponent
{
	Transform myTransform;

	AABB TransformAABB(AABB aAABB)
	{
		XMVECTOR cornerPos[8]
		{
			XMVectorSet(aAABB.myMin.x, aAABB.myMin.y, aAABB.myMin.z, 0),
			XMVectorSet(aAABB.myMin.x, aAABB.myMin.y, aAABB.myMax.z, 0),
			XMVectorSet(aAABB.myMin.x, aAABB.myMax.y, aAABB.myMin.z, 0),
			XMVectorSet(aAABB.myMax.x, aAABB.myMin.y, aAABB.myMin.z, 0),
			XMVectorSet(aAABB.myMin.x, aAABB.myMax.y, aAABB.myMax.z, 0),
			XMVectorSet(aAABB.myMax.x, aAABB.myMin.y, aAABB.myMax.z, 0),
			XMVectorSet(aAABB.myMax.x, aAABB.myMax.y, aAABB.myMin.z, 0),
			XMVectorSet(aAABB.myMin.x, aAABB.myMin.y, aAABB.myMin.z, 0)
		};
			
		AABB newAABB(myTransform.myPosition, myTransform.myPosition);

		DirectX::XMMATRIX modelMatrix = myTransform.GetModelMatrix();

		for (int i = 0; i < 8; i++)
		{
			cornerPos[i] = XMVector3TransformCoord(cornerPos[i], modelMatrix);
			math::vector3<float> transformedPos = { XMVectorGetX(cornerPos[i]), XMVectorGetY(cornerPos[i]), XMVectorGetZ(cornerPos[i]) };
			newAABB.ExpandTo(transformedPos);
		}

		return newAABB;
	}
};

struct TagComponent
{
	TagComponent(const std::string& aTag)
	{
		myTag = aTag;
	}

	std::string myTag;
};

struct ScriptComponent
{
	Script* myInstance = nullptr;
	bool myHasStarted = false;

	Script*(*InitFunction)() = nullptr;
	void(*DestroyFunction)(ScriptComponent*) = nullptr;

	template<typename T>
	T* Bind(Entity& aEntity)
	{
		InitFunction = []() { return static_cast<Script*>(new T()); };
		DestroyFunction = [](ScriptComponent* aScriptComponent) { delete aScriptComponent->myInstance; aScriptComponent->myInstance = nullptr; };

		myInstance = InitFunction();
		myInstance->myEntity = aEntity;

		myInstance->Awake();

		return static_cast<T*>(myInstance);
	}
};

struct ModelComponent
{
	ModelComponent(const std::string& aModelName, const std::string& aMaterialName)
	{
		myModelName = aModelName;
		myMaterialName = aMaterialName;
	}

	std::string myModelName;
	std::string myMaterialName;

	void SetModelAndTexture(const std::string& aModelName, const std::string& aTextureName)
	{
		myModelName = aModelName;
		myMaterialName = aTextureName;
	}
};

struct AnimatorComponent
{
	AnimatorComponent(const std::string& aAnimationName, double aAnimationDurationSeconds)
	{
		SetAnimation(aAnimationName, aAnimationDurationSeconds);
	}

	std::string myAnimationName;
	double myAnimationDurationSeconds;
	double myTimeSeconds;

	void SetAnimation(const std::string& aAnimationName, double aAnimationDurationSeconds)
	{
		myAnimationName = aAnimationName;
		myAnimationDurationSeconds = aAnimationDurationSeconds;
		myTimeSeconds = 0;
	}

	void Update()
	{
		myTimeSeconds += 0.02; // TODO: Delta Time ig? Change this to some proper solution
		if (myTimeSeconds > myAnimationDurationSeconds)
		{
			myTimeSeconds = 0;
		}
	}
};
