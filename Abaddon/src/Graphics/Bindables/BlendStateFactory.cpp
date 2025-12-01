#include "pch.h"
#include "BlendStateFactory.h"

namespace BlendStateFactory
{
	D3D11_BLEND_DESC GetBlendDesc(eBlendState aBlendState)
	{
		switch (aBlendState)
		{
			case eBlendState::Opaque:
				return OpaqueBlendDesc();
			case eBlendState::Alpha:
				return AlphaBlendDesc();
			case eBlendState::Additive:
				return AdditiveBlendDesc();
			case eBlendState::Multiply:
				return MultiplyBlendDesc();
			case eBlendState::Premultiplied:
				return PremultipliedBlendDesc();
		}

		LOG_ERROR("Unsupported BlendState");
		Assert(false);
	}

	static D3D11_BLEND_DESC BaseBlendDesc()
	{
		D3D11_BLEND_DESC blendStateDesc;
		ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
		auto& renderTarget = blendStateDesc.RenderTarget[0];

		blendStateDesc.AlphaToCoverageEnable = false;
		blendStateDesc.IndependentBlendEnable = false;
		renderTarget.BlendEnable = false;
		renderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		return blendStateDesc;
	}

	D3D11_BLEND_DESC OpaqueBlendDesc()
	{
		D3D11_BLEND_DESC blendStateDesc = BaseBlendDesc();
		auto& renderTarget = blendStateDesc.RenderTarget[0];

		renderTarget.BlendEnable = false;
		// final = src * 1 + dest * 0
		renderTarget.SrcBlend = D3D11_BLEND_ONE;
		renderTarget.DestBlend = D3D11_BLEND_ZERO;
		renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

		renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
		renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;

		return blendStateDesc;
	}

	D3D11_BLEND_DESC AlphaBlendDesc()
	{
		D3D11_BLEND_DESC blendStateDesc = BaseBlendDesc();
		auto& renderTarget = blendStateDesc.RenderTarget[0];

		renderTarget.BlendEnable = true;
		// final = src * src.a + dest * (1 - src.a)
		renderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

		renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTarget.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;

		return blendStateDesc;
	}

	D3D11_BLEND_DESC AdditiveBlendDesc()
	{
		D3D11_BLEND_DESC blendStateDesc = BaseBlendDesc();
		auto& renderTarget = blendStateDesc.RenderTarget[0];

		renderTarget.BlendEnable = true;
		// final = src * src.a + dst * 1;
		renderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderTarget.DestBlend = D3D11_BLEND_ONE;
		renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

		renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTarget.DestBlendAlpha = D3D11_BLEND_ONE;
		renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;

		return blendStateDesc;
	}
	
	D3D11_BLEND_DESC MultiplyBlendDesc()
	{
		D3D11_BLEND_DESC blendStateDesc = BaseBlendDesc();
		auto& renderTarget = blendStateDesc.RenderTarget[0];

		renderTarget.BlendEnable = true;
		// final = src * 0 + dst * src
		renderTarget.SrcBlend = D3D11_BLEND_ZERO;
		renderTarget.DestBlend = D3D11_BLEND_SRC_COLOR;
		renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

		renderTarget.SrcBlendAlpha = D3D11_BLEND_ZERO;
		renderTarget.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;

		return blendStateDesc;
	}

	D3D11_BLEND_DESC PremultipliedBlendDesc()
	{
		D3D11_BLEND_DESC blendStateDesc = BaseBlendDesc();
		auto& renderTarget = blendStateDesc.RenderTarget[0];

		renderTarget.BlendEnable = true;
		// final = src * 1 + dst * (1 - src.a)
		renderTarget.SrcBlend = D3D11_BLEND_ONE;
		renderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

		renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTarget.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;

		return blendStateDesc;
	}
}
