#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "CBuffer.hpp"
#include "SRV.h"
#include "Sampler.h"
#include "Rasterizer.h"
#include "Graphics/AABB.h"
#include "Graphics/Bindables/Animation/Animation.h"

struct Skeleton
{
	typedef unsigned int BoneIndex;

	struct Bone
	{
		Bone(BoneIndex aBoneIndex, DirectX::XMMATRIX aRestMatrix)
		{
			myBoneParentIndex = aBoneIndex;
			myRestMatrix = aRestMatrix;

			DirectX::XMVECTOR position;
			DirectX::XMVECTOR rotation;
			DirectX::XMVECTOR scale;
			DirectX::XMMatrixDecompose(&scale, &rotation, &position, myRestMatrix);

			LOG("REST MATRIX BEGIN");

			LOG("Position: (" + std::to_string(DirectX::XMVectorGetX(position)) + ", " + std::to_string(DirectX::XMVectorGetY(position)) + ", " + std::to_string(DirectX::XMVectorGetZ(position)) + ")");
			math::vector4<float> rot = { DirectX::XMVectorGetX(rotation), DirectX::XMVectorGetY(rotation), DirectX::XMVectorGetZ(rotation), DirectX::XMVectorGetW(rotation) };
			math::vector3<float> rotEuler = rot.ToEuler();
			LOG("Rotation: (" + std::to_string(rotEuler.x) + ", " + std::to_string(rotEuler.y) + ", " + std::to_string(rotEuler.z) + ")");
			LOG("Scale: (" + std::to_string(DirectX::XMVectorGetX(scale)) + ", " + std::to_string(DirectX::XMVectorGetY(scale)) + ", " + std::to_string(DirectX::XMVectorGetZ(scale)) + ")");

			LOG("REST MATRIX END");
		}

		// This Bone has no Parent if the Parent Index is the same as it's Index
		BoneIndex myBoneParentIndex;
		DirectX::XMMATRIX myRestMatrix;
	};

	std::vector<Bone> myBones;
	// Needed for Animations to know what Bone to affect
	std::unordered_map<std::string, BoneIndex> myBoneNameToIndex;

	DirectX::XMMATRIX* GetBones(Animation& aAnimation, double aTime)
	{
		DirectX::XMMATRIX bones[Animations::MAX_BONES];
		for (int i = 0; i < Animations::MAX_BONES; i++)
			bones[i] = DirectX::XMMatrixIdentity();

		for (int i = 0; i < aAnimation.myChannels.size(); i++)
		{
			auto pair = myBoneNameToIndex.find(aAnimation.myChannels[i].myName);
			if (pair == myBoneNameToIndex.end()) continue;
			if (pair->second < 0 && pair->second >= Animations::MAX_BONES) continue;
			
			DirectX::XMMATRIX local = aAnimation.myChannels[i].GetInterpolated(aTime).GetMatrix();
			BoneIndex parentIndex = myBones.at(pair->second).myBoneParentIndex;
			if (parentIndex == pair->second) bones[pair->second] = local; // No Parent
			else bones[pair->second] = bones[parentIndex] * local;
		}

		for (int i = 0; i < myBones.size(); i++)
			bones[i] = DirectX::XMMatrixTranspose(bones[i] * myBones[i].myRestMatrix);

		return bones;
	}
};

struct ModelData
{
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;

	AABB myAABB;
	Skeleton mySkeleton;

	bool HasSkeleton() const
	{
		return mySkeleton.myBones.size() > 0;
	}
};

struct TextureData
{
	SRV mySRV;
	Sampler mySampler;
};
