#include "pch.h"
#include "RenderPass.h"

void RenderPass::Init(std::string aVertexShaderFileName, D3D11_CULL_MODE aCullMode, std::string aPixelShaderFileName)
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
