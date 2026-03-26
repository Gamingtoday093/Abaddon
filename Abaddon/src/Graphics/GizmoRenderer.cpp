#include "pch.h"
#include "GizmoRenderer.h"
#include "Bindables/InputLayoutFactory.h"
#include "Scene/Camera.h"

GizmoRenderer::GizmoRenderer()
{
	myLineBuffer.Init(LineBuffer::MAX_LINES);

	myCBufferTransform.Init(eBindType::VertexShader);

	myInputLayout.Init(InputLayoutFactory::GetDescription<GizmoVertex>(), "GizmoShader_vs.cso");
	myVertexShader.Init("GizmoShader_vs.cso");

	myPixelShader.Init("GizmoShader_ps.cso");
	myBlendState.Init(eBlendState::Alpha);
}

void GizmoRenderer::RenderLine(const math::vector3<float>& aFromPosition, const math::vector3<float>& aToPosition, const math::vector4<float>& aColor)
{
	myLineBuffer.myLines.emplace_back(aFromPosition, aToPosition, aColor);
}

void GizmoRenderer::RenderPlane(const math::vector3<float>& aPosition, math::vector2<float> aScale, const math::vector4<float>& aColor)
{
	RenderLine({ aPosition.x + aScale.x, aPosition.y, aPosition.z + aScale.y }, { aPosition.x - aScale.x, aPosition.y, aPosition.z + aScale.y }, aColor);
	RenderLine({ aPosition.x + aScale.x, aPosition.y, aPosition.z + aScale.y }, { aPosition.x + aScale.x, aPosition.y, aPosition.z - aScale.y }, aColor);;
	RenderLine({ aPosition.x - aScale.x, aPosition.y, aPosition.z - aScale.y }, { aPosition.x - aScale.x, aPosition.y, aPosition.z + aScale.y }, aColor);;
	RenderLine({ aPosition.x - aScale.x, aPosition.y, aPosition.z - aScale.y }, { aPosition.x + aScale.x, aPosition.y, aPosition.z - aScale.y }, aColor);;
}

void GizmoRenderer::RenderCube(const math::vector3<float>& aPosition, const math::vector3<float>& aScale, const math::vector4<float>& aColor)
{
	RenderPlane(aPosition - math::vector3<float>{ 0, -aScale.y, 0 }, { aScale.x, aScale.z }, aColor);
	RenderPlane(aPosition - math::vector3<float>{ 0, aScale.y, 0 }, { aScale.x, aScale.z }, aColor);
	RenderLine({ aPosition.x + aScale.x, aPosition.y - aScale.y, aPosition.z + aScale.z }, { aPosition.x + aScale.x, aPosition.y + aScale.y, aPosition.z + aScale.z }, aColor);
	RenderLine({ aPosition.x + aScale.x, aPosition.y - aScale.y, aPosition.z - aScale.z }, { aPosition.x + aScale.x, aPosition.y + aScale.y, aPosition.z - aScale.z }, aColor);
	RenderLine({ aPosition.x - aScale.x, aPosition.y - aScale.y, aPosition.z + aScale.z }, { aPosition.x - aScale.x, aPosition.y + aScale.y, aPosition.z + aScale.z }, aColor);
	RenderLine({ aPosition.x - aScale.x, aPosition.y - aScale.y, aPosition.z - aScale.z }, { aPosition.x - aScale.x, aPosition.y + aScale.y, aPosition.z - aScale.z }, aColor);
}

void GizmoRenderer::ExecuteCommands(const Camera& aCamera)
{
	if (myLineBuffer.myLines.size() == 0) return;
	if (myLineBuffer.myLines.size() > LineBuffer::MAX_LINES)
	{
		LOG_ERROR("Too Many Lines! Removing Extra");
		myLineBuffer.myLines.resize(LineBuffer::MAX_LINES);
	}

	DX11::ourContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	myCBufferTransform.myData.myProjectionViewMatrix =
		aCamera.GetMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

	myCBufferTransform.ApplyChanges();
	myCBufferTransform.Bind();
	myInputLayout.Bind();
	myVertexShader.Bind();

	myPixelShader.Bind();
	myBlendState.Bind();

	myLineBuffer.ApplyChanges();
	myLineBuffer.Bind();

	DX11::ourContext->DrawIndexed(UINT(myLineBuffer.myLines.size() * 2), 0, 0);
	myLineBuffer.myLines.clear();
	DX11::ourContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
