#pragma once
#include "Graphics/Vertex.h"

struct D3D11_INPUT_ELEMENT_DESC;

namespace InputLayoutFactory
{
	template<typename TVertex>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription()
	{
		LOG_ERROR("Unsupported Vertex Type");
		Assert(false);
	}

	template<>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<Vertex>();

	template<>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<SkinnedVertex>();

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetVertexDescription();
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetSkinnedVertexDescription();
};

