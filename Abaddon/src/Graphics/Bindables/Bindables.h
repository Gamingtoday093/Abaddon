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
	using BoneIndex = uint32_t;

	struct Bone
	{
		Bone(BoneIndex aBoneIndex, const DirectX::XMFLOAT4X4& aRestMatrix)
		{
			myBoneParentIndex = aBoneIndex;
			myRestMatrix = aRestMatrix;
		}

		// This Bone has no Parent if the Parent Index is the same as it's Index
		BoneIndex myBoneParentIndex;
		// Needs to be Stored as a Float4x4 instead of a Matrix because Bone isn't 16 byte aligned (Apparently Undefined Behaviour)
		DirectX::XMFLOAT4X4 myRestMatrix;
	};

	std::vector<Bone> myBones;
	// Needed for Animations to know what Bone to affect
	std::unordered_map<std::string, BoneIndex> myBoneNameToIndex;

	void GetBones(const Animation& aAnimation, double aTime, std::vector<DirectX::XMMATRIX>& aBones) const
	{
		aBones.assign(myBones.size(), DirectX::XMMatrixIdentity());

		for (int i = 0; i < aAnimation.myChannels.size(); i++)
		{
			auto pair = myBoneNameToIndex.find(aAnimation.myChannels[i].myName);
			if (pair == myBoneNameToIndex.end()) continue;
			if (pair->second >= Animations::MAX_BONES) continue; // BoneIndex is unsigned and can't be less than 0

			// This is based on the assumption that Animation::myChannels is ordered from RootBone and Down
			DirectX::XMMATRIX local = aAnimation.myChannels[i].GetInterpolated(aTime).GetMatrix();
			BoneIndex parentIndex = myBones.at(pair->second).myBoneParentIndex;
			if (parentIndex == pair->second) aBones[pair->second] = local; // No Parent
			else aBones[pair->second] = aBones[parentIndex] * local;
		}

		for (int i = 0; i < aAnimation.myChannels.size(); i++)
		{
			auto pair = myBoneNameToIndex.find(aAnimation.myChannels[i].myName);
			if (pair == myBoneNameToIndex.end()) continue;
			if (pair->second < 0 && pair->second >= Animations::MAX_BONES) continue;

			aBones[pair->second] = DirectX::XMMatrixTranspose(aBones[pair->second] * DirectX::XMLoadFloat4x4(&myBones[pair->second].myRestMatrix));
		}
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
