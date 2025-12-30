#include "pch.h"
#include "RenderPass.h"

void RenderPass::Init(const std::string& aVertexShaderFileName, D3D11_CULL_MODE aCullMode, const std::string& aPixelShaderFileName)
{
	myVertexShader.Init(aVertexShaderFileName);
	myRasterizer.Init(aCullMode);
	myPixelShader.Init(aPixelShaderFileName);
}

void RenderPass::Bind()
{
	myVertexShader.Bind();
	myRasterizer.Bind();
	myPixelShader.Bind();
}
