#pragma once
#include <wrl.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

class Rasterizer
{
public:
	Rasterizer() = default;
	~Rasterizer() = default;

	void Init(D3D11_CULL_MODE aCullMode);
	void Bind();
	ID3D11RasterizerState* Get() const;
	ID3D11RasterizerState* const* GetAdressOf() const;
private:
	ComPtr<ID3D11RasterizerState> myRasterizerState;
};

