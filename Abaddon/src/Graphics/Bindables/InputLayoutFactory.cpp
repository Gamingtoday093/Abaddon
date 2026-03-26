#include "pch.h"
#include "InputLayoutFactory.h"
#include <d3d11.h>

namespace InputLayoutFactory
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> VertexDescription =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> SkinnedVertexDescription =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> GizmoVertexDescription =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> SkyboxVertexDescription =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetVertexDescription()
	{
		return VertexDescription;
	}

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetSkinnedVertexDescription()
	{
		return SkinnedVertexDescription;
	}

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetGizmoVertexDescription()
	{
		return GizmoVertexDescription;
	}

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetSkyboxVertexDescription()
	{
		return SkyboxVertexDescription;
	}

	template<>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<Vertex>()
	{
		return GetVertexDescription();
	}

	template<>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<SkinnedVertex>()
	{
		return GetSkinnedVertexDescription();
	}

	template<>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<GizmoVertex>()
	{
		return GetGizmoVertexDescription();
	}

	template<>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<SkyboxVertex>()
	{
		return GetSkyboxVertexDescription();
	}
}
