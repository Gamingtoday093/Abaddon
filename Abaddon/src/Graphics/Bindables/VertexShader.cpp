#include "pch.h"
#include "VertexShader.h"
#include "Graphics/DX11.h"
#include <d3dcompiler.h>

void VertexShader::Init(std::string aShaderFileName)
{
	ComPtr<ID3DBlob> blob;
	HRESULT hr = D3DReadFileToBlob(AddStringsReturnWStr("Shaders/", aShaderFileName).c_str(), &blob);
	DX11::HRASSERT(hr, "Creating Vertex Shader Blob");

	hr = DX11::ourDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &myVertexShader);
	DX11::HRASSERT(hr, "Creating Vertex Shader");
}

void VertexShader::Bind()
{
	DX11::ourContext->VSSetShader(myVertexShader.Get(), nullptr, 0);
}

ID3D11VertexShader* VertexShader::Get() const
{
	return myVertexShader.Get();
}

ID3D11VertexShader* const* VertexShader::GetAdressOf() const
{
	return myVertexShader.GetAddressOf();
}
