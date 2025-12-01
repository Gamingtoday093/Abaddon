#pragma once
#include <d3d11.h>
#include "RenderStates.hpp"

namespace BlendStateFactory
{
	D3D11_BLEND_DESC GetBlendDesc(eBlendState aBlendState);

	D3D11_BLEND_DESC OpaqueBlendDesc();
	D3D11_BLEND_DESC AlphaBlendDesc();
	D3D11_BLEND_DESC AdditiveBlendDesc();
	D3D11_BLEND_DESC MultiplyBlendDesc();
	D3D11_BLEND_DESC PremultipliedBlendDesc();
}

