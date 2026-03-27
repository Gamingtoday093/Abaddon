#include "pch.h"
#include "LineBuffer.h"
#include "Graphics/Vertex.h"

void LineBuffer::Init(UINT aMaxLines)
{
	Resize(aMaxLines);
}

void LineBuffer::Resize(UINT aNewMaxLines)
{
	Assert(aNewMaxLines <= MAX_LINES);

	myVertexBuffer = nullptr;
	myIndexBuffer = nullptr;
	myMaxLines = aNewMaxLines;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(GizmoVertex) * myMaxLines * 2;
	bufferDesc.StructureByteStride = 0;

	HRASSERT(DX11::ourDevice->CreateBuffer(&bufferDesc, nullptr, &myVertexBuffer), "Create LineBuffer VertexBuffer");

	bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(USHORT) * myMaxLines * 2;
	bufferDesc.StructureByteStride = 0;

	HRASSERT(DX11::ourDevice->CreateBuffer(&bufferDesc, nullptr, &myIndexBuffer), "Create LineBuffer IndexBuffer");
}

void LineBuffer::Bind() const
{
	const UINT stride = sizeof(GizmoVertex);
	const UINT offset = 0;
	DX11::ourContext->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &stride, &offset);
	DX11::ourContext->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

void LineBuffer::ApplyChanges()
{
	Assert(myLines.size() <= myMaxLines);

	D3D11_MAPPED_SUBRESOURCE vertexBufferData = {}, indexBufferData = {};

	HRASSERT(DX11::ourContext->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexBufferData), "Map Line VertexBuffer");
	HRASSERT(DX11::ourContext->Map(myIndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &indexBufferData), "Map Line IndexBuffer");

	GizmoVertex* vertexBuffer = static_cast<GizmoVertex*>(vertexBufferData.pData);
	USHORT* indexBuffer = static_cast<USHORT*>(indexBufferData.pData);
	for (size_t i = 0; i < myLines.size(); i++)
	{
		const Line& line = myLines[i];

		GizmoVertex& fromVertex = vertexBuffer[i * 2 + 0];
		fromVertex.myPosition = line.myFromPosition;
		fromVertex.myColor = line.myColor;

		GizmoVertex& toVertex = vertexBuffer[i * 2 + 1];
		toVertex.myPosition = line.myToPosition;
		toVertex.myColor = line.myColor;
		
		indexBuffer[i * 2 + 0] = USHORT(i * 2 + 0);
		indexBuffer[i * 2 + 1] = USHORT(i * 2 + 1);
	}

	DX11::ourContext->Unmap(myVertexBuffer.Get(), 0);
	DX11::ourContext->Unmap(myIndexBuffer.Get(), 0);
}
