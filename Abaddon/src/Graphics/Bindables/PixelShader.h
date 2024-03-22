#pragma once
#include <wrl.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

class PixelShader
{
public:
	PixelShader() = default;
	~PixelShader() = default;

	void Init(std::string aShaderFileName);
	void Bind();
	ID3D11PixelShader* Get() const;
	ID3D11PixelShader* const* GetAdressOf() const;
private:
	ComPtr<ID3D11PixelShader> myPixelShader;
};
