#include "pch.h"
#include "SkyboxCube.h"

SkyboxCube::SkyboxCube()
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

void SkyboxCube::Bind() const
{
	myVertexBuffer.Bind();
	myIndexBuffer.Bind();
	myInputLayout.Bind();
	mySampler.Bind();
}
