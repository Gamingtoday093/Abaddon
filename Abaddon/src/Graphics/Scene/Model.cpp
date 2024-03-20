#include "pch.h"
#include "Model.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

void Model::LoadModel(std::string aFilePath, std::string aTextureName, std::shared_ptr<Camera> aCamera)
{
	Assimp::Importer importer;
	if (!aFilePath.starts_with("C:")) aFilePath = "Models/" + aFilePath;
	auto modelData = importer.ReadFile(aFilePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes);

	if (!modelData)
	{
		LOG_ERROR("Failed to load model '" + aFilePath + "'");
		Assert(false);
	}

	// Vertex Buffer & Index Buffer
	std::vector<Vertex> vertexList;
	std::vector<unsigned short> indexList;
	for (int mIndex = 0; mIndex < modelData->mNumMeshes; mIndex++)
	{
		auto mesh = modelData->mMeshes[mIndex];
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

		myModelData.myAABB.ExpandTo({ mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z });
		myModelData.myAABB.ExpandTo({ mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z });
	}
	myModelData.myVertexBuffer.Init(vertexList);
	myModelData.myIndexBuffer.Init(indexList);

	// Input Layout
	myModelData.myInputLayout.Init({
		{ "POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		}, "VertexShader_vs.cso");

	// SRV
	myModelData.mySRV.Init(aTextureName);

	// Sampler
	myModelData.mySampler.Init();

	// Transform buffer
	myModelData.myTransformCBuffer.Init(eBindType::vertexShader);
	myModelData.myTransformCBuffer.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationQuaternion(myQuaternionRotation) *
			DirectX::XMMatrixTranslation(myPosition.x, myPosition.y, myPosition.z) *
			DirectX::XMMatrixScaling(myScale.x, myScale.y, myScale.z) *
			aCamera->GetMatrix() *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f)
		);
	myModelData.myTransformCBuffer.ApplyChanges();
	myModelData.myTransformCBuffer.Bind();
}

void Model::UpdateRender(std::shared_ptr<Camera> aCamera)
{
	// Transform buffer
	myModelData.myTransformCBuffer.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationQuaternion(myQuaternionRotation) *
			DirectX::XMMatrixTranslation(myPosition.x, myPosition.y, myPosition.z) *
			DirectX::XMMatrixScaling(myScale.x, myScale.y, myScale.z) *
			aCamera->GetMatrix() *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f)
		);
	myModelData.myTransformCBuffer.ApplyChanges();
	myModelData.myTransformCBuffer.Bind();
}

const math::vector3<float>& Model::GetPosition()
{
	return myPosition;
}

const math::vector3<float>& Model::GetRotation()
{
	return { 0, 0, 0 };
}

void Model::Transform(math::vector3<float> aPosition, math::vector3<float> aRotation)
{
	myPosition += aPosition;
	//myRotation += aRotation;
}

void Model::SetScale(math::vector3<float> aScale)
{
	myScale = aScale;
}

void Model::SetPosition(math::vector3<float> aPosition)
{
	myPosition = aPosition;
}

void Model::SetRotation(math::vector3<float> aRotation)
{
	myQuaternionRotation = XMQuaternionRotationRollPitchYaw(aRotation.x, aRotation.y, aRotation.z);
}

ModelData& Model::GetModelData()
{
	return myModelData;
}

AABB Model::GetTransformedAABB()
{
	XMVECTOR min = XMVectorSet(myModelData.myAABB.myMin.x, myModelData.myAABB.myMin.y, myModelData.myAABB.myMin.z, 0);
	XMVECTOR max = XMVectorSet(myModelData.myAABB.myMax.x, myModelData.myAABB.myMax.y, myModelData.myAABB.myMax.z, 0);

	XMMATRIX matrix = DirectX::XMMatrixRotationQuaternion(myQuaternionRotation) *				// ***
					  DirectX::XMMatrixTranslation(myPosition.x, myPosition.y, myPosition.z) *	// World Matrix
					  DirectX::XMMatrixScaling(myScale.x, myScale.y, myScale.z);				// ***

	min = XMVector3TransformCoord(min, matrix);
	max = XMVector3TransformCoord(max, matrix);

	math::vector3<float> minPos = { XMVectorGetX(min), XMVectorGetY(min), XMVectorGetZ(min) };
	math::vector3<float> maxPos = { XMVectorGetX(max), XMVectorGetY(max), XMVectorGetZ(max) };

	return AABB(minPos, maxPos);
}
