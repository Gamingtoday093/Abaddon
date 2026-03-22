#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "RenderStates.hpp"

using namespace Microsoft::WRL;

class BlendState
{
public:
	BlendState() = default;
	~BlendState() = default;

	void Init(eBlendState aBlendState);
	void Bind() const;
	ID3D11BlendState* Get() const;
	ID3D11BlendState* const* GetAddressOf() const;
private:
	ComPtr<ID3D11BlendState> myBlendState;
};

