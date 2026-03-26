#pragma once
#include "Graphics/DX11.h"
#include "Graphics/Line.h"
#include "Graphics/Vertex.h"

using namespace Microsoft::WRL;

class LineBuffer
{
public:
	static constexpr UINT MAX_LINES = 0xffff / 2;

	void Init(UINT aMaxLines);
	void Resize(UINT aNewMaxLines);

	void Bind() const;

	void ApplyChanges();

	std::vector<Line> myLines;

private:
	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;

	UINT myMaxLines = 0;
};

