#include "pch.h"
#include "ModelAssetHandler.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include "Graphics/Bindables/InputLayoutFactory.h"

std::unordered_map<ePrimitive, ModelData> ModelAssetHandler::myPrimitiveModels;
std::unordered_map<std::string, ModelData> ModelAssetHandler::myLoadedModels;
std::unordered_map<std::string, Animation> ModelAssetHandler::myLoadedAnimations;
std::unordered_map<std::string, TextureData> ModelAssetHandler::myLoadedTextures;
std::unordered_map<std::string, Material*> ModelAssetHandler::myCreatedMaterials;

void ModelAssetHandler::LoadModel(const std::string& aModelFileName)
{
	// Check if it's already loaded
	if (myLoadedModels.find(aModelFileName) != myLoadedModels.end())
	{
		LOG_WARNING("Model has been already loaded.");
		Assert(false);
		return;
	}
	
	// Load file
	Assimp::Importer importer;
	auto data = importer.ReadFile("Models/" + aModelFileName,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenBoundingBoxes |
		aiProcess_GenSmoothNormals |
		aiProcess_PopulateArmatureData |
		aiProcess_LimitBoneWeights);
	if (!data)
	{
		LOG_ERROR("Failed to load model '" + aModelFileName + "'");
		Assert(false);
	}

	// Vertex Buffer & Index Buffer list & Bounding Box
	ModelData meshData;
	std::vector<Vertex> vertexList;
	std::vector<SkinnedVertex> vertexSkinnedList;
	std::vector<unsigned short> indexList;
	AABB boundingBox;
	for (int mIndex = 0; mIndex < data->mNumMeshes; mIndex++)
	{
		auto mesh = data->mMeshes[mIndex];
		int indexOffset = vertexList.size();

		for (int vIndex = 0; vIndex < mesh->mNumVertices; vIndex++)
		{
			auto vertex = mesh->mVertices[vIndex];
			aiVector3D normal;
			if (mesh->HasNormals())
				normal = mesh->mNormals[vIndex];

			float vertexU = 0;
			float vertexV = 0;
			if (mesh->mTextureCoords[0])
			{
				vertexU = mesh->mTextureCoords[0][vIndex].x;
				vertexV = 1.0f - mesh->mTextureCoords[0][vIndex].y;
			}

			if (mesh->HasBones() && mesh->mNumBones <= Animations::MAX_BONES)
				vertexSkinnedList.emplace_back(vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z, vertexU, vertexV);
			else
				vertexList.emplace_back(vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z, vertexU, vertexV);
		}

		for (int fIndex = 0; fIndex < mesh->mNumFaces; fIndex++)
		{
			auto face = mesh->mFaces[fIndex];

			indexList.push_back(face.mIndices[0] + indexOffset);
			indexList.push_back(face.mIndices[1] + indexOffset);
			indexList.push_back(face.mIndices[2] + indexOffset);
		}
		
		boundingBox.ExpandTo(reinterpret_cast<math::vector3<float>&>(mesh->mAABB.mMin));
		boundingBox.ExpandTo(reinterpret_cast<math::vector3<float>&>(mesh->mAABB.mMax));

		// Skeleton
		if (mesh->HasBones() && mesh->mNumBones <= Animations::MAX_BONES)
		{
			meshData.mySkeleton.myBones.reserve(mesh->mNumBones);
			meshData.mySkeleton.myBoneNameToIndex.reserve(mesh->mNumBones);

			for (int b = 0; b < mesh->mNumBones; b++)
			{
				auto bone = mesh->mBones[b];
				auto& om = bone->mOffsetMatrix;
				meshData.mySkeleton.myBones.emplace_back(b,
					DirectX::XMFLOAT4X4(
					om.a1, om.a2, om.a3, om.a4,
					om.b1, om.b2, om.b3, om.b4,
					om.c1, om.c2, om.c3, om.c4,
					om.d1, om.d2, om.d3, om.d4)
				);

				meshData.mySkeleton.myBoneNameToIndex.insert({ bone->mName.C_Str(), b });

				// Assign Weights to Vertices
				for (int w = 0; w < bone->mNumWeights; w++)
				{
					auto weight = bone->mWeights[w];

					SkinnedVertex& vertex = vertexSkinnedList[weight.mVertexId];
					for (int i = 0; i < Animations::MAX_WEIGHTS; i++)
					{
						if (vertex.myBoneWeights[i] != 0) continue;

						vertex.myBoneIDs[i] = b;
						vertex.myBoneWeights[i] = weight.mWeight;
						break;
					}
				}
			}

			LoadBoneHierarchyRecursive(meshData.mySkeleton.myBoneNameToIndex, meshData.mySkeleton, data->mRootNode);
		}
	}

	// Init
	if (meshData.HasSkeleton()) 
		meshData.myVertexBuffer.Init(vertexSkinnedList);
	else meshData.myVertexBuffer.Init(vertexList);
	meshData.myIndexBuffer.Init(indexList);
	meshData.myAABB = boundingBox;

	// Add to loaded model list
	myLoadedModels.insert({ aModelFileName, meshData });
}

void ModelAssetHandler::LoadBoneHierarchyRecursive(const std::unordered_map<std::string, unsigned int>& aBoneNameToIndex, Skeleton& aSkeleton, const aiNode* aNode)
{
	if (aNode->mParent)
	{
		auto pair = aBoneNameToIndex.find(aNode->mName.C_Str());
		if (pair != aBoneNameToIndex.end())
		{
			auto parent = aBoneNameToIndex.find(aNode->mParent->mName.C_Str());
			if (parent != aBoneNameToIndex.end())
				aSkeleton.myBones.at(pair->second).myBoneParentIndex = parent->second;
		}
	}

	for (int c = 0; c < aNode->mNumChildren; c++)
		LoadBoneHierarchyRecursive(aBoneNameToIndex, aSkeleton, aNode->mChildren[c]);
}

void ModelAssetHandler::LoadAnimations(const std::string& aAnimationFileName)
{
	Assimp::Importer importer;
	auto data = importer.ReadFile("Models/" + aAnimationFileName, 0);

	for (int a = 0; a < data->mNumAnimations; a++)
	{
		auto animationData = data->mAnimations[a];

		if (animationData->mNumChannels == 0) continue;

		if (myLoadedAnimations.find(animationData->mName.C_Str()) != myLoadedAnimations.end())
		{
			LOG_WARNING("Duplicate Animation Name.");
			Assert(false);
		}

		Animation animation;
		animation.myFramerate = animationData->mTicksPerSecond == 0 ? 30. : animationData->mTicksPerSecond;
		animation.myDurationSeconds = animationData->mDuration / animation.myFramerate;

		for (int c = 0; c < animationData->mNumChannels; c++)
		{
			auto channelData = animationData->mChannels[c];

			Animation::Channel& channel = animation.myChannels.emplace_back(channelData->mNodeName.C_Str());

			for (int i = 0; i < channelData->mNumPositionKeys; i++)
			{
				auto& positionKey = channelData->mPositionKeys[i];

				channel.myPositionKeyframes.emplace_back(positionKey.mTime, positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z);
			}

			for (int i = 0; i < channelData->mNumRotationKeys; i++)
			{
				auto& rotationKey = channelData->mRotationKeys[i];

				channel.myRotationKeyframes.emplace_back(rotationKey.mTime, rotationKey.mValue.x, rotationKey.mValue.y, rotationKey.mValue.z, rotationKey.mValue.w);
			}

			for (int i = 0; i < channelData->mNumScalingKeys; i++)
			{
				auto& scaleKey = channelData->mScalingKeys[i];

				channel.myScaleKeyframes.emplace_back(scaleKey.mTime, scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z);
			}
		}

		myLoadedAnimations.insert({ animationData->mName.C_Str(), animation });
	}
}

void ModelAssetHandler::LoadTexture(const std::string& aTextureFileName)
{
	if (myLoadedTextures.find(aTextureFileName) != myLoadedTextures.end())
	{
		LOG_WARNING("Texture has been already loaded.");
		Assert(false);
	}

	TextureData textureData;

	// Init SRV and Sampler
	textureData.mySRV.Init(aTextureFileName);
	textureData.mySampler.Init();

	// Add to loaded texture list
	myLoadedTextures.insert({ aTextureFileName, textureData });
}

ModelData& ModelAssetHandler::GetPrimitiveModelData(ePrimitive aPrimitive)
{
	if (myPrimitiveModels.size() == 0)
		LoadPrimitiveModels();

	return myPrimitiveModels.at(aPrimitive);
}

void ModelAssetHandler::LoadPrimitiveModels()
{
	constexpr float side = 1.0f / 2.0f;

#pragma region Plane
	// Vertex Buffer list
	std::vector<Vertex> vertexList;
	vertexList.push_back({ -side, 0, side, 0, 1, 0, 0, 0 });
	vertexList.push_back({ side, 0, side, 0, 1, 0, 1, 0 });
	vertexList.push_back({ -side, 0, -side, 0, 1, 0, 0, 1 });
	vertexList.push_back({ side, 0, -side, 0, 1, 0, 1, 1 });

	// Init
	ModelData planeMeshData;
	planeMeshData.myVertexBuffer.Init(vertexList);
	planeMeshData.myIndexBuffer.Init({ 1,2,0, 1,3,2 });

	for (auto& vertex : vertexList)
		planeMeshData.myAABB.ExpandTo(vertex.myPosition);

	// Add to primitive model list
	myPrimitiveModels.insert({ ePrimitive::Plane, planeMeshData});
#pragma endregion

#pragma region Cube
	// Vertex Buffer list
	vertexList.clear();
	vertexList.push_back({ -side, -side, -side, 0, 1, 0, 0.375f, 0.25f });
	vertexList.push_back({ side, -side, -side, 0, 1, 0, 0.375f, 0.50f });
	vertexList.push_back({ -side, side, -side, 0, 1, 0, 0.625f, 0.25f });
	vertexList.push_back({ side, side, -side, 0, 1, 0, 0.625f, 0.50f });
	vertexList.push_back({ -side, -side, side, 0, 1, 0, 0.375f, 1 });
	vertexList.push_back({ side, -side, side, 0, 1, 0, 0.375f, 0.75f });
	vertexList.push_back({ -side, side, side, 0, 1, 0, 0.625f, 0 });
	vertexList.push_back({ side, side, side, 0, 1, 0, 0.625f, 0.75f });

	ModelData cubeMeshData;
	cubeMeshData.myVertexBuffer.Init(vertexList);
	cubeMeshData.myIndexBuffer.Init({ 0,2,1, 2,3,1,
									  1,3,5, 3,7,5,
									  2,6,3, 3,6,7,
									  4,5,7, 4,7,6,
									  0,4,2, 2,4,6,
									  0,1,4, 1,5,4 });

	for (auto& vertex : vertexList)
		cubeMeshData.myAABB.ExpandTo(vertex.myPosition);

	// Add to primitive model list
	myPrimitiveModels.insert({ ePrimitive::Cube, cubeMeshData });
#pragma endregion
}

ModelData& ModelAssetHandler::GetModelData(const std::string& aModelFileName)
{
	if (myLoadedModels.find(aModelFileName) == myLoadedModels.end())
	{
		LOG_ERROR("Model not found.");
		Assert(false);
	}
	
	return myLoadedModels.at(aModelFileName);
}

Animation& ModelAssetHandler::GetAnimation(const std::string& aAnimationName)
{
	if (myLoadedAnimations.find(aAnimationName) == myLoadedAnimations.end())
	{
		LOG_ERROR("Animation not found.");
		Assert(false);
	}

	return myLoadedAnimations.at(aAnimationName);
}

TextureData& ModelAssetHandler::GetTextureData(const std::string& aTextureFileName)
{
	if (myLoadedTextures.find(aTextureFileName) == myLoadedTextures.end())
	{
		LOG_ERROR("Texture not found.");
		Assert(false);
	}

	return myLoadedTextures.at(aTextureFileName);
}

Material& ModelAssetHandler::GetMaterial(const std::string& aMaterialName)
{
	if (myCreatedMaterials.find(aMaterialName) == myCreatedMaterials.end())
	{
		LOG_ERROR("Material not found.");
		Assert(false);
	}

	return *myCreatedMaterials.at(aMaterialName);
}
