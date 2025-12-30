#pragma once
#include "Graphics/DX11.h"
#include "Graphics/CBufferStructs.h"
#include "BufferCommon.hpp"

using namespace Microsoft::WRL;

template<typename T>
class CBuffer
{
public:
	CBuffer() = default;
	~CBuffer() = default;

	void Init(eBindType aBindType);
	void Bind(UINT slot = 0);
	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAddressOf() const;

	void ApplyChanges();
	void SetBindType(eBindType aBindType);
	eBindType GetBindType() const;

	T myData{};

private:
	ComPtr<ID3D11Buffer> myBuffer;
	eBindType myBindType = eBindType::VertexAndPixelShader;
};

template<typename T>
inline void CBuffer<T>::Init(eBindType aBindType)
{
	myBindType = aBindType;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(myData);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResData;
	subResData.pSysMem = &myData;
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;

	// Create
	HRESULT hr = DX11::ourDevice->CreateBuffer(&bufferDesc, &subResData, myBuffer.GetAddressOf());
	DX11::HRASSERT(hr, "Creating CBuffer");
}

template<typename T>
inline void CBuffer<T>::Bind(UINT slot)
{
	switch (myBindType)
	{
		case eBindType::VertexShader:
			DX11::ourContext->VSSetConstantBuffers(slot, 1, myBuffer.GetAddressOf());
			break;

		case eBindType::PixelShader:
			DX11::ourContext->PSSetConstantBuffers(slot, 1, myBuffer.GetAddressOf());
			break;

		case eBindType::VertexAndPixelShader:
			DX11::ourContext->VSSetConstantBuffers(slot, 1, myBuffer.GetAddressOf());
			DX11::ourContext->PSSetConstantBuffers(slot, 1, myBuffer.GetAddressOf());
			break;

		default:
			LOG_ERROR("Can't bind CBuffer, invalid Bind Type.");
			Assert(false);
			break;
	}
}

template<typename T>
inline void CBuffer<T>::ApplyChanges()
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = DX11::ourContext->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	DX11::HRASSERT(hr, "Applying changes to CBuffer", false);

	memcpy(bufferData.pData, &myData, sizeof(myData));

	DX11::ourContext->Unmap(myBuffer.Get(), 0);
}

template<typename T>
inline void CBuffer<T>::SetBindType(eBindType aBindType)
{
	myBindType = aBindType;
}

template<typename T>
inline eBindType CBuffer<T>::GetBindType() const
{
	return myBindType;
}

template<typename T>
inline ID3D11Buffer* CBuffer<T>::Get() const
{
	return myBuffer.Get();
}

template<typename T>
inline ID3D11Buffer* const* CBuffer<T>::GetAddressOf() const
{
	return myBuffer.GetAddressOf();
}
