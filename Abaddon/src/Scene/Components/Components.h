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
		XMVECTOR cornerPos[8];
		cornerPos[0] = XMVectorSet(aAABB.myMin.x, aAABB.myMin.y, aAABB.myMin.z, 0);
		cornerPos[1] = XMVectorSet(aAABB.myMin.x, aAABB.myMin.y, aAABB.myMax.z, 0);
		cornerPos[2] = XMVectorSet(aAABB.myMin.x, aAABB.myMax.y, aAABB.myMin.z, 0);
		cornerPos[3] = XMVectorSet(aAABB.myMax.x, aAABB.myMin.y, aAABB.myMin.z, 0);
		cornerPos[4] = XMVectorSet(aAABB.myMin.x, aAABB.myMax.y, aAABB.myMax.z, 0);
		cornerPos[5] = XMVectorSet(aAABB.myMax.x, aAABB.myMin.y, aAABB.myMax.z, 0);
		cornerPos[6] = XMVectorSet(aAABB.myMax.x, aAABB.myMax.y, aAABB.myMin.z, 0);
		cornerPos[7] = XMVectorSet(aAABB.myMin.x, aAABB.myMin.y, aAABB.myMin.z, 0);
	
		XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(myTransform.myRotation.x, myTransform.myRotation.y, myTransform.myRotation.z) *	// ***
						  DirectX::XMMatrixTranslation(myTransform.myPosition.x, myTransform.myPosition.y, myTransform.myPosition.z) *			// World Matrix
						  DirectX::XMMatrixScaling(myTransform.myScale.x, myTransform.myScale.y, myTransform.myScale.z);						// ***
		
		AABB newAABB(myTransform.myPosition, myTransform.myPosition);

		for (int i = 0; i < 8; i++)
		{
			cornerPos[i] = XMVector3TransformCoord(cornerPos[i], matrix);
			math::vector3<float> transformedPos = { XMVectorGetX(cornerPos[i]), XMVectorGetY(cornerPos[i]), XMVectorGetZ(cornerPos[i]) };
			newAABB.ExpandTo(transformedPos);
		}

		return newAABB;
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

class Material;

struct ModelComponent
{
	ModelComponent(std::string aModelName, std::string aMaterialName)
	{
		myModelName = aModelName;
		myMaterialName = aMaterialName;
	}

	std::string myModelName;
	std::string myMaterialName;

	void SetModelAndTexture(std::string aModelName, std::string aTextureName)
	{
		myModelName = aModelName;
		myMaterialName = aTextureName;
	}
};
