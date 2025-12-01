#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "CBuffer.hpp"
#include "SRV.h"
#include "Sampler.h"
#include "Rasterizer.h"
#include "Graphics/AABB.h"

struct ModelData
{
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;

	AABB myAABB;
};

struct TextureData
{
	SRV mySRV;
	Sampler mySampler;
};
