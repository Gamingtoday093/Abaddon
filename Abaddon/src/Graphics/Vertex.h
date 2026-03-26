#pragma once
#include "Bindables/Animation/AnimationDefines.h"

struct Vertex
{
	Vertex() 
	{
		myPosition = { 0,0,0 };
		myNormal = { 0,0,0 };
		myTexcoord = { 0.0f,0.0f };
	}

	Vertex(float aX, float aY, float aZ, float aNX, float aNY, float aNZ, float aU, float aV)
	{ 
		myPosition.x = aX;
		myPosition.y = aY;
		myPosition.z = aZ;

		myNormal.x = aNX;
		myNormal.y = aNY;
		myNormal.z = aNZ;

		myTexcoord.x = aU;
		myTexcoord.y = aV;
	}

	Vertex(math::vector3<float>& aPosition, math::vector3<float>& aNormal, math::vector2<float>& aTexcoord)
	{ 
		myPosition.x = aPosition.x;
		myPosition.y = aPosition.y;
		myPosition.z = aPosition.z;

		myNormal.x = aNormal.x;
		myNormal.y = aNormal.y;
		myNormal.z = aNormal.z;

		myTexcoord.x = aTexcoord.x;
		myTexcoord.y = aTexcoord.y;
	}

	math::vector3<float> myPosition;
	math::vector3<float> myNormal;
	math::vector2<float> myTexcoord;
};

struct SkinnedVertex
{
	SkinnedVertex()
	{
		myPosition = { 0,0,0 };
		myNormal = { 0,0,0 };
		myTexcoord = { 0.0f,0.0f };

		for (size_t i = 0; i < Animations::MAX_WEIGHTS; i++)
		{
			myBoneIDs[i] = 0;
			myBoneWeights[i] = 0;
		}
	}

	SkinnedVertex(float aX, float aY, float aZ, float aNX, float aNY, float aNZ, float aU, float aV)
	{
		myPosition.x = aX;
		myPosition.y = aY;
		myPosition.z = aZ;

		myNormal.x = aNX;
		myNormal.y = aNY;
		myNormal.z = aNZ;

		myTexcoord.x = aU;
		myTexcoord.y = aV;

		for (size_t i = 0; i < Animations::MAX_WEIGHTS; i++)
		{
			myBoneIDs[i] = 0;
			myBoneWeights[i] = 0;
		}
	}

	math::vector3<float> myPosition;
	math::vector3<float> myNormal;
	math::vector2<float> myTexcoord;

	uint32_t myBoneIDs[Animations::MAX_WEIGHTS];
	float myBoneWeights[Animations::MAX_WEIGHTS];
};

struct GizmoVertex
{
	math::vector3<float> myPosition;
	math::vector4<float> myColor;
};

struct SkyboxVertex
{
	SkyboxVertex()
	{
		myPosition = {};
	}

	SkyboxVertex(float aX, float aY, float aZ)
	{
		myPosition = { aX, aY, aZ };
	}

	math::vector3<float> myPosition;
};

template<typename TVertex>
concept VertexType =
std::same_as<std::remove_cvref_t<TVertex>, Vertex> ||
std::same_as<std::remove_cvref_t<TVertex>, SkinnedVertex> ||
std::same_as<std::remove_cvref_t<TVertex>, GizmoVertex> ||
std::same_as<std::remove_cvref_t<TVertex>, SkyboxVertex>;