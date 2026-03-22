#include "pch.h"
#include "BlendState.h"
#include "Graphics/DX11.h"
#include "BlendStateFactory.h"

void BlendState::Init(eBlendState aBlendState)
{
	D3D11_BLEND_DESC blendDesc = BlendStateFactory::GetBlendDesc(aBlendState);

	HRESULT hr = DX11::ourDevice->CreateBlendState(&blendDesc, &myBlendState);
	HRASSERT(hr, "Creating BlendState");
}

void BlendState::Bind() const
{
	const float blendFactor[4] = { 0, 0, 0, 0 };
	DX11::ourContext->OMSetBlendState(myBlendState.Get(), blendFactor, 0xffffffff);
}

ID3D11BlendState* BlendState::Get() const
{
	return myBlendState.Get();
}

ID3D11BlendState* const* BlendState::GetAddressOf() const
{
	return myBlendState.GetAddressOf();
}
