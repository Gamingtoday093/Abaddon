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
			
			// This is based on the assumption that Animation::myChannels is ordered from RootBone and Down
			DirectX::XMMATRIX local = aAnimation.myChannels[i].GetInterpolated(aTime).GetMatrix();
			BoneIndex parentIndex = myBones.at(pair->second).myBoneParentIndex;
			if (parentIndex == pair->second) bones[pair->second] = local; // No Parent
			else bones[pair->second] = bones[parentIndex] * local;
		}

		// TODO: This doesnt check the bounds compared to bones!!
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
