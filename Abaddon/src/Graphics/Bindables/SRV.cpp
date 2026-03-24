#include "pch.h"
#include "SRV.h"
#include "Graphics/DX11.h"
#include "DirectXTex/DirectXTex.h"

void SRV::Init(const std::string& aTextureFileName)
{
	// Image
	DirectX::ScratchImage imageData = {};
	HRESULT hr = DirectX::LoadFromWICFile(AddStringsReturnWStr("Textures/", aTextureFileName).c_str(), DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, imageData);
	// ALERT ALERT ALERT
	// I have absolutely no idea what unforseen consequences DirectX::WIC_FLAGS_IGNORE_SRGB has! But it works so Yippiee
	HRASSERT(hr, "Loading Texture Image");

	// Texture
	ComPtr<ID3D11Resource> texture;

	hr = DirectX::CreateTexture(DX11::ourDevice.Get(), imageData.GetImages(), imageData.GetImageCount(), imageData.GetMetadata(), &texture);
	HRASSERT(hr, "Creating Texture");

	// Creating SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = imageData.GetMetadata().format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(imageData.GetMetadata().mipLevels);
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = DX11::ourDevice->CreateShaderResourceView(texture.Get(), &srvDesc, &mySRV);
	HRASSERT(hr, "Creating Shader Resource View");
}

void SRV::Bind(UINT slot) const
{
	DX11::ourContext->PSSetShaderResources(slot, 1, mySRV.GetAddressOf());
}

ID3D11ShaderResourceView* SRV::Get() const
{
	return mySRV.Get();
}

ID3D11ShaderResourceView* const* SRV::GetAddressOf() const
{
	return mySRV.GetAddressOf();
}
