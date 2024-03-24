#include "pch.h"
#include "CubeTexture.h"
#include "DirectXTex/DirectXTex.h"
#include "Graphics/DX11.h"

void CubeTexture::Init(const std::string& aFolderPath)
{
	std::vector<DirectX::ScratchImage> images;
	for (int i = 0; i < 6; i++)
	{
		images.push_back(DirectX::ScratchImage{});
		HRESULT hr = DirectX::LoadFromWICFile(AddStringsReturnWStr("Textures/" + aFolderPath + "/" + std::to_string(i), ".png").c_str(), DirectX::WIC_FLAGS_NONE, nullptr, images[i]);
		DX11::HRASSERT(hr, "Loading Texture Image");
	}

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = images[0].GetMetadata().width;
	textureDesc.Height = images[0].GetMetadata().height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 6;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	// subresource data
	D3D11_SUBRESOURCE_DATA data[6];
	for (int i = 0; i < 6; i++)
	{
		data[i].pSysMem = images[i].GetPixels();
		data[i].SysMemPitch = (unsigned int)images[i].GetImage(0, 0, 0)->rowPitch;
		data[i].SysMemSlicePitch = 0;
	}

	ComPtr<ID3D11Texture2D> pTexture;
	HRESULT hr = DX11::ourDevice->CreateTexture2D(&textureDesc, data, &pTexture);
	DX11::HRASSERT(hr, "Creating Texture");

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	hr = DX11::ourDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView);
	DX11::HRASSERT(hr, "Creating Shader Resource");
}

void CubeTexture::Bind()
{
	DX11::ourContext->PSSetShaderResources(0, 1, pTextureView.GetAddressOf());
}
