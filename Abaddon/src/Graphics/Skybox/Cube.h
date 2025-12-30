#pragma once
#include "pch.h"
#include "Graphics/Bindables/Bindables.h"
#include "Graphics/Vertex.h"
#include "Graphics/Bindables/InputLayoutFactory.h"
#include "Scene/Cameras.h"

class Cube
{
public:
	void Init()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<SkyboxVertex> vertices;

		vertices.push_back({ -side, -side, -side });
		vertices.push_back({ side, -side, -side });
		vertices.push_back({ -side, side, -side });
		vertices.push_back({ side, side, -side });
		vertices.push_back({ -side, -side, side });
		vertices.push_back({ side, -side, side });
		vertices.push_back({ -side, side, side });
		vertices.push_back({ side, side, side });

		myVertexBuffer.Init(vertices);

		myIndexBuffer.Init({ 0,2,1, 2,3,1,
						   1,3,5, 3,7,5,
						   2,6,3, 3,6,7,
						   4,5,7, 4,7,6,
						   0,4,2, 2,4,6,
						   0,1,4, 1,5,4 });

		myInputLayout.Init(InputLayoutFactory::GetDescription<SkyboxVertex>(), "Skybox_vs.cso");

		mySampler.Init();
	}

	void Bind()
	{
		myVertexBuffer.Bind();
		myIndexBuffer.Bind();
		myInputLayout.Bind();
		mySampler.Bind();
	}
private:
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;
	Sampler mySampler;
};