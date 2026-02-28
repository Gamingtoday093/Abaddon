#pragma once
#include "pch.h"
#include "Graphics/DX11.h"
#include "BufferCommon.hpp"

using namespace Microsoft::WRL;

template<typename T>
class StructBuffer
{
public:
	StructBuffer() = default;
	~StructBuffer() = default;

	void Init(eBindType aBindType, UINT aMaxLength);
	void Resize(UINT aNewMaxLength);
	void ApplyChanges();
	void Bind(UINT slot = 0);

	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAddressOf() const;

	void SetBindType(eBindType aBindType);
	eBindType GetBindType() const;

	uint32_t GetMaxLength() const;

	std::vector<T> myData;

private:
	ComPtr<ID3D11Buffer> myBuffer;
	ComPtr<ID3D11ShaderResourceView> mySRV;
	eBindType myBindType = eBindType::VertexAndPixelShader;
	uint32_t myBufferLength = 0;
};

template<typename T>
inline void StructBuffer<T>::Init(eBindType aBindType, UINT aMaxLength)
{
	myBindType = aBindType;
	Resize(aMaxLength);
}

template<typename T>
inline void StructBuffer<T>::Resize(UINT aNewMaxLength)
{
	myBuffer = nullptr;
	mySRV = nullptr;
	myData.reserve(aNewMaxLength);

	myBufferLength = aNewMaxLength;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(T) * myBufferLength;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(T);

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData.pSysMem = std::data(myData);
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;

	HRESULT hr = DX11::ourDevice->CreateBuffer(&bufferDesc, &subResData, myBuffer.GetAddressOf());
	HRASSERT(hr, "Creating StructBuffer");

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = myBufferLength;

	hr = DX11::ourDevice->CreateShaderResourceView(myBuffer.Get(), &srvDesc, mySRV.GetAddressOf());
	HRASSERT(hr, "Creating SRV for StructBuffer");
}

template<typename T>
inline void StructBuffer<T>::Bind(UINT slot)
{
	switch (myBindType)
	{
	case eBindType::VertexShader:
		DX11::ourContext->VSSetShaderResources(slot, 1, mySRV.GetAddressOf());
		break;

	case eBindType::PixelShader:
		DX11::ourContext->PSSetShaderResources(slot, 1, mySRV.GetAddressOf());
		break;

	case eBindType::VertexAndPixelShader:
		DX11::ourContext->VSSetShaderResources(slot, 1, mySRV.GetAddressOf());
		DX11::ourContext->PSSetShaderResources(slot, 1, mySRV.GetAddressOf());
		break;

	default:
		LOG_ERROR("Can't bind StructBuffer, invalid Bind Type.");
		Assert(false);
		break;
	}
}

template<typename T>
inline void StructBuffer<T>::ApplyChanges()
{
	Assert(myData.size() <= myBufferLength);

	D3D11_MAPPED_SUBRESOURCE bufferData = {};

	HRESULT hr = DX11::ourContext->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	HRASSERT(hr, "Locking StructBuffer for Write");

	memcpy_s(bufferData.pData, sizeof(T) * myBufferLength, std::data(myData), sizeof(T) * myData.size());

	DX11::ourContext->Unmap(myBuffer.Get(), 0);
}

template<typename T>
inline ID3D11Buffer* StructBuffer<T>::Get() const
{
	return myBuffer.Get();
}

template<typename T>
inline ID3D11Buffer* const* StructBuffer<T>::GetAddressOf() const
{
	return myBuffer.GetAddressOf();
}

template<typename T>
inline void StructBuffer<T>::SetBindType(eBindType aBindType)
{
	myBindType = aBindType;
}

template<typename T>
inline eBindType StructBuffer<T>::GetBindType() const
{
	return myBindType;
}

template<typename T>
inline uint32_t StructBuffer<T>::GetMaxLength() const
{
	return myBufferLength;
}
