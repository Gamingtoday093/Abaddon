#pragma once
#include "Graphics/Bindables/Bindables.h"
#include "Bindables/VertexShader.h"
#include "Bindables/PixelShader.h"

class RenderPass
{
public:
	void Init(const std::string& aVertexShaderFileName, D3D11_CULL_MODE aCullMode, const std::string& aPixelShaderFileName);
	void Bind();

private:
	VertexShader myVertexShader;
	Rasterizer myRasterizer;
	PixelShader myPixelShader;
};

