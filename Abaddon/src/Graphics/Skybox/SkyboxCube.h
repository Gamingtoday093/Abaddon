#pragma once
#include "pch.h"
#include "Graphics/Bindables/Bindables.h"
#include "Graphics/Vertex.h"
#include "Graphics/Bindables/InputLayoutFactory.h"
#include "Scene/Cameras.h"

class SkyboxCube
{
public:
	SkyboxCube();
	~SkyboxCube() = default;
	void Bind() const;

private:
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;
	Sampler mySampler;
};