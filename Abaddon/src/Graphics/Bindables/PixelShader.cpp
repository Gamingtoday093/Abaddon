#include "pch.h"
#include "PixelShader.h"
#include "Graphics/DX11.h"
#include <d3dcompiler.h>

void PixelShader::Init(std::string aShaderFileName)
{
	ComPtr<ID3DBlob> blob;
	HRESULT hr = D3DReadFileToBlob(AddStringsReturnWStr("Shaders/", aShaderFileName).c_str(), &blob);
	HRASSERT(hr, "Creating Pixel Shader Blob");

	hr = DX11::ourDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &myPixelShader);
	HRASSERT(hr, "Creating Pixel Shader");
}

void PixelShader::Bind() const
{
	DX11::ourContext->PSSetShader(myPixelShader.Get(), nullptr, 0);
}

ID3D11PixelShader* PixelShader::Get() const
{
	return myPixelShader.Get();
}

ID3D11PixelShader* const* PixelShader::GetAddressOf() const
{
	return myPixelShader.GetAddressOf();
}
