#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11ShaderResourceView;

class CubeTexture
{
public:
	CubeTexture() = default;
	~CubeTexture() = default;

	void Init(const std::string& aFolderPath);
	void Bind();

private:
	ComPtr<ID3D11ShaderResourceView> pTextureView;
};

