#pragma once
#include <wrl.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

class VertexShader
{
public:
	VertexShader() = default;
	~VertexShader() = default;

	void Init(std::string aShaderFileName);
	void Bind();
	ID3D11VertexShader* Get() const;
	ID3D11VertexShader* const* GetAdressOf() const;
private:
	ComPtr<ID3D11VertexShader> myVertexShader;
};

