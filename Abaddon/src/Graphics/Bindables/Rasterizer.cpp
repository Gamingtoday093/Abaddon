#include "pch.h"
#include "Rasterizer.h"
#include "Graphics/DX11.h"

void Rasterizer::Init(D3D11_CULL_MODE aCullMode)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = aCullMode;

	DX11::ourDevice->CreateRasterizerState(&rasterizerDesc, myRasterizerState.GetAddressOf());

	DX11::ourContext->RSSetState(myRasterizerState.Get());
}

void Rasterizer::Bind()
{
	DX11::ourContext->RSSetState(myRasterizerState.Get());
}

ID3D11RasterizerState* Rasterizer::Get() const
{
	return myRasterizerState.Get();
}

ID3D11RasterizerState* const* Rasterizer::GetAdressOf() const
{
	return myRasterizerState.GetAddressOf();
}
