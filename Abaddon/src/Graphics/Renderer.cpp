#include "pch.h"
#include "Renderer.h"
#include "Bindables/Bindables.h"
#include "Bindables/Animation/Animation.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"
#include "Bindables/Materials/Material.hpp"
#include <Scene/ModelAssetHandler.h>
#include "Bindables/InputLayoutFactory.h"

void Renderer::Init()
{
	myCBufferTransform.Init(eBindType::vertexShader);
	myCBufferAnimation.Init(eBindType::vertexShader);
	myCBufferCamera.Init(eBindType::pixelShader);

	myInputLayout.Init(InputLayoutFactory::GetDescription<Vertex>(), "VertexShader_vs.cso");
	mySkinnedInputLayout.Init(InputLayoutFactory::GetDescription<SkinnedVertex>(), "SkinnedShader_vs.cso");

	myVertexShader.Init("VertexShader_vs.cso");
	mySkinnedShader.Init("SkinnedShader_vs.cso");

	for (size_t i = 0; i < myBlendStates.size(); i++)
	{
		BlendState blendState;
		blendState.Init(static_cast<eBlendState>(i));
		myBlendStates.at(i) = blendState;
	}
}

inline constexpr BlendState& Renderer::GetBlendState(eBlendState aBlendState)
{
	return myBlendStates.at(static_cast<size_t>(aBlendState));
}

void Renderer::Render(ModelData& aModelData, Material& aMaterial, Transform& aTransform, Animation& aAnimation, double aTimeSeconds, std::shared_ptr<Camera> aCamera)
{
	Assert(aModelData.HasSkeleton());

	constexpr rsize_t bonesArraySize = sizeof(DirectX::XMMATRIX) * Animations::MAX_BONES;
	memcpy_s(myCBufferAnimation.myData.myBones, bonesArraySize, aModelData.mySkeleton.GetBones(aAnimation, aAnimation.myFramerate * aTimeSeconds), bonesArraySize);
	myCBufferAnimation.ApplyChanges();
	myCBufferAnimation.Bind(1);

	Render(aModelData, aMaterial, aTransform, aCamera);
}

void Renderer::Render(ModelData& aModelData, Material& aMaterial, Transform& aTransform, std::shared_ptr<Camera> aCamera)
{
	// Bind model
	aModelData.myVertexBuffer.Bind();
	aModelData.myIndexBuffer.Bind();

	// Bind material
	aMaterial.Bind();

	// TODO: Should sort Renders into RenderCommands so BlendState and Others only has to be set when Switching Material BlendState Context
	if (aModelData.HasSkeleton())
	{
		mySkinnedInputLayout.Bind();
		mySkinnedShader.Bind(); // TODO: This should only Bind if bones are actually set! Otherwise bind regular VertexShader
	}
	else
	{
		myInputLayout.Bind();
		myVertexShader.Bind();
	}

	GetBlendState(aMaterial.GetBlendState()).Bind();

	// Set Vertex Constant Buffer
	myCBufferTransform.myData.myProjectionViewMatrix =
		aCamera->GetMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

	myCBufferTransform.myData.myModelMatrix = 
		aTransform.GetModelMatrix();

	myCBufferTransform.ApplyChanges();
	myCBufferTransform.Bind();

	// Set camera
	math::vector3<float> cameraPosition = aCamera.get()->GetPosition();
	myCBufferCamera.myData.CameraPosition = DirectX::XMVectorSet(cameraPosition.x, cameraPosition.y, cameraPosition.z, 0);
	myCBufferCamera.ApplyChanges();
	myCBufferCamera.Bind();
	
	// Draw textured model
	DX11::ourContext->DrawIndexed(aModelData.myIndexBuffer.GetIndexAmount(), 0, 0);
}

void Renderer::RenderSkybox(std::shared_ptr<Cube> aCube, std::shared_ptr<CubeTexture> aCubeTexture, std::shared_ptr<Camera> aCamera)
{
	// Bind Mesh
	aCube->Bind();

	// Bind Texture
	aCubeTexture->Bind();

	// Set View Projection Matrix
	myCBufferTransform.myData.myProjectionViewMatrix =
		aCamera->GetMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	myCBufferTransform.ApplyChanges();
	myCBufferTransform.Bind();

	// Draw Skybox
	DX11::ourContext->DrawIndexed(36, 0, 0);
}
