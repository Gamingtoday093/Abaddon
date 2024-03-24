#include "pch.h"
#include "Renderer.h"
#include "Graphics/Bindables/Bindables.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"

#define DegToRadFactor 0.01745329251f

void Renderer::Init()
{
	myCBufferTransform.Init(eBindType::vertexShader);
}

void Renderer::Render(ModelData& aModelData, TextureData& aTextureData, Transform& aTransform, std::shared_ptr<Camera> aCamera)
{
	// Bind model
	aModelData.myVertexBuffer.Bind();
	aModelData.myIndexBuffer.Bind();
	aModelData.myInputLayout.Bind();

	// Bind texture
	aTextureData.mySRV.Bind();
	aTextureData.mySampler.Bind();

	// Set transform
	aTransform.myRotation *= DegToRadFactor;

	myCBufferTransform.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixScaling(aTransform.myScale.x, aTransform.myScale.y, aTransform.myScale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(aTransform.myRotation.x, aTransform.myRotation.y, aTransform.myRotation.z) *
			DirectX::XMMatrixTranslation(aTransform.myPosition.x, aTransform.myPosition.y, aTransform.myPosition.z) *
			aCamera->GetMatrix() *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f)
		);
	myCBufferTransform.ApplyChanges();
	myCBufferTransform.Bind();

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
	myCBufferTransform.myData.myTransformation =
		DirectX::XMMatrixTranspose(
			aCamera->GetMatrix() *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f)
		);
	myCBufferTransform.ApplyChanges();
	myCBufferTransform.Bind();

	// Draw Skybox
	DX11::ourContext->DrawIndexed(36, 0, 0);
}
