#include "pch.h"
#include "ModelAssetHandler.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

std::unordered_map<ePrimitive, ModelData> ModelAssetHandler::myPrimitiveModels;
std::unordered_map<std::string, ModelData> ModelAssetHandler::myLoadedModels;
std::unordered_map<std::string, TextureData> ModelAssetHandler::myLoadedTextures;

void ModelAssetHandler::LoadModel(std::string aModelFileName, std::string aVertexShaderFileName)
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
	auto data = importer.ReadFile("Models/" + aModelFileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes);
	if (!data)
	{
		LOG_ERROR("Failed to load model '" + aModelFileName + "'");
		Assert(false);
	}
	
	// Vertex Buffer & Index Buffer list & Bounding Box
	std::vector<Vertex> vertexList;
	std::vector<unsigned short> indexList;
	AABB boundingBox;
	for (int mIndex = 0; mIndex < data->mNumMeshes; mIndex++)
	{
		auto mesh = data->mMeshes[mIndex];
		int indexOffset = vertexList.size();
		
		for (int vIndex = 0; vIndex < mesh->mNumVertices; vIndex++)
		{
			auto vertex = mesh->mVertices[vIndex];

			aiVector3D* textureCoords = mesh->mTextureCoords[0];
			if (textureCoords)
			{
				vertexList.push_back({ vertex.x, vertex.y, vertex.z, mesh->mTextureCoords[0][vIndex].x, 1.0f - mesh->mTextureCoords[0][vIndex].y });
			}
			else
			{
				vertexList.push_back({ vertex.x, vertex.y, vertex.z, 0, 0 });
			}
		}

		for (int fIndex = 0; fIndex < mesh->mNumFaces; fIndex++)
		{
			auto face = mesh->mFaces[fIndex];

			indexList.push_back(face.mIndices[0] + indexOffset);
			indexList.push_back(face.mIndices[1] + indexOffset);
			indexList.push_back(face.mIndices[2] + indexOffset);
		}
		
		boundingBox.ExpandTo({ mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z });
		boundingBox.ExpandTo({ mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z });
	}

	// Create Input Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

	// Init
	ModelData meshData;
	meshData.myVertexBuffer.Init(vertexList);
	meshData.myIndexBuffer.Init(indexList);
	meshData.myInputLayout.Init(inputLayout, aVertexShaderFileName);
	meshData.myAABB = boundingBox;

	// Add to loaded model list
	myLoadedModels.insert({ aModelFileName, meshData });
}

void ModelAssetHandler::LoadTexture(std::string aTextureFileName)
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
	vertexList.push_back({ -side, 0, side, 0, 0 });
	vertexList.push_back({ side, 0, side, 1, 0 });
	vertexList.push_back({ -side, 0, -side, 0, 1 });
	vertexList.push_back({ side, 0, -side, 1, 1 });

	// Create Input Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	// Init
	ModelData planeMeshData;
	planeMeshData.myVertexBuffer.Init(vertexList);
	planeMeshData.myIndexBuffer.Init({ 1,2,0, 1,3,2 });
	planeMeshData.myInputLayout.Init(inputLayout, "VertexShader_vs.cso");

	// Add to primitive model list
	myPrimitiveModels.insert({ ePrimitive::Plane, planeMeshData});
#pragma endregion

#pragma region Cube
	// Vertex Buffer list
	vertexList.clear();
	vertexList.push_back({ -side, -side, -side, 0.375f, 0.25f });
	vertexList.push_back({ side, -side, -side, 0.375f, 0.50f });
	vertexList.push_back({ -side, side, -side, 0.625f, 0.25f });
	vertexList.push_back({ side, side, -side, 0.625f, 0.50f });
	vertexList.push_back({ -side, -side, side, 0.375f, 1 });
	vertexList.push_back({ side, -side, side, 0.375f, 0.75f });
	vertexList.push_back({ -side, side, side, 0.625f, 0 });
	vertexList.push_back({ side, side, side, 0.625f, 0.75f });

	ModelData cubeMeshData;
	cubeMeshData.myVertexBuffer.Init(vertexList);
	cubeMeshData.myIndexBuffer.Init({ 0,2,1, 2,3,1,
									  1,3,5, 3,7,5,
									  2,6,3, 3,6,7,
									  4,5,7, 4,7,6,
									  0,4,2, 2,4,6,
									  0,1,4, 1,5,4 });
	cubeMeshData.myInputLayout.Init(inputLayout, "VertexShader_vs.cso");

	// Add to primitive model list
	myPrimitiveModels.insert({ ePrimitive::Cube, cubeMeshData });
#pragma endregion
}

ModelData& ModelAssetHandler::GetModelData(std::string aModelFileName)
{
	if (myLoadedModels.find(aModelFileName) == myLoadedModels.end())
	{
		LOG_ERROR("Model not found.");
		Assert(false);
	}
	
	return myLoadedModels.at(aModelFileName);
}

TextureData& ModelAssetHandler::GetTextureData(std::string aTextureFileName)
{
	if (myLoadedTextures.find(aTextureFileName) == myLoadedTextures.end())
	{
		LOG_ERROR("Texture not found.");
		Assert(false);
	}

	return myLoadedTextures.at(aTextureFileName);
}
