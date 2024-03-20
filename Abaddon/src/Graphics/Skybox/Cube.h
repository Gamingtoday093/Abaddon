#pragma once
#include "pch.h"
#include "Graphics/Bindables/Bindables.h"
#include "Graphics/Vertex.h"
#include "Graphics/Scene/Cameras.h"

class Cube
{
public:
	void Init()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<Vertex> vertices;

		vertices.push_back({ -side, -side, -side, 0, 0 });
		vertices.push_back({ side, -side, -side, 0, 0 });
		vertices.push_back({ -side, side, -side, 0, 0 });
		vertices.push_back({ side, side, -side, 0, 0 });
		vertices.push_back({ -side, -side, side, 0, 0 });
		vertices.push_back({ side, -side, side, 0, 0 });
		vertices.push_back({ -side, side, side, 0, 0 });
		vertices.push_back({ side, side, side, 0, 0 });

		myVertexBuffer.Init(vertices);

		myIndexBuffer.Init({ 0,2,1, 2,3,1,
						   1,3,5, 3,7,5,
						   2,6,3, 3,6,7,
						   4,5,7, 4,7,6,
						   0,4,2, 2,4,6,
						   0,1,4, 1,5,4 });

		myInputLayout.Init({
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			}, "Skybox_vs.cso");

		mySampler.Init();

		myViewProjectionBuffer.Init(eBindType::vertexShader);
	}

	void UpdateViewProjection(const std::shared_ptr<Camera>& aCamera)
	{
		myViewProjectionBuffer.myData.myTransformation =
			XMMatrixTranspose(
			XMMatrixScaling(10, 10, 10) *
			aCamera->GetMatrix() *
			DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
		myViewProjectionBuffer.ApplyChanges();
	}

	void Bind()
	{
		myVertexBuffer.Bind();
		myIndexBuffer.Bind();
		myInputLayout.Bind();
		mySampler.Bind();
		myViewProjectionBuffer.Bind();
	}
private:
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;
	Sampler mySampler;
	CBuffer<TransformBuffer> myViewProjectionBuffer;
};