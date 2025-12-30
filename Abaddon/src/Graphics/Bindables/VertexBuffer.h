#pragma once
#include "Graphics/Vertex.h"
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11Buffer;

class VertexBuffer
{
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

	template<VertexType TVertex>
	void Init(const std::vector<TVertex>& aVertexList);
	void Init(const void* aData, UINT aNumData, UINT aDataSize);
	void Bind();
	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAdressOf() const;

private:
	ComPtr<ID3D11Buffer> myBuffer;
	UINT myDataSize = 0;
};

template<VertexType TVertex>
void VertexBuffer::Init(const std::vector<TVertex>& aVertexList)
{
	Init(std::data(aVertexList), UINT(aVertexList.size()), sizeof(TVertex));
}

