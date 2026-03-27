#pragma once
#include "Bindables/LineBuffer.h"
#include "Bindables/CBuffer.hpp"
#include "Bindables/BlendState.h"
#include "Bindables/VertexShader.h"
#include "Bindables/InputLayout.h"
#include "Bindables/PixelShader.h"

struct ModelData;
class Camera;

class GizmoRenderer
{
public:
	GizmoRenderer();

	void RenderLine(const math::vector3<float>& aFromPosition, const math::vector3<float>& aToPosition, const math::vector4<float>& aColor = { 0, 1, 0, 1 });
	void RenderPlane(const math::vector3<float>& aPosition, math::vector2<float> aScale, const math::vector4<float>& aColor = { 0, 1, 0, 1 });
	void RenderCube(const math::vector3<float>& aPosition, const math::vector3<float>& aScale, const math::vector4<float>& aColor = { 0, 1, 0, 1 });

private:
	static constexpr UINT MAX_LINES_PER_RENDER = LineBuffer::MAX_LINES;
	void ExecuteCommands(const Camera& aCamera);

	LineBuffer myLineBuffer;

	CBuffer<ProjectionBuffer> myCBufferTransform;
	InputLayout myInputLayout;
	VertexShader myVertexShader;

	PixelShader myPixelShader;
	BlendState myBlendState;

	friend class Engine;
};

