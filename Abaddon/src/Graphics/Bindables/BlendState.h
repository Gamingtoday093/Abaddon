#pragma once
#include <wrl.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

enum class eBlendState;

class BlendState
{
public:
	BlendState() = default;
	~BlendState() = default;

	void Init(eBlendState aBlendState);
	void Bind();
	ID3D11BlendState* Get() const;
	ID3D11BlendState* const* GetAdressOf() const;
private:
	ComPtr<ID3D11BlendState> myBlendState;
};

