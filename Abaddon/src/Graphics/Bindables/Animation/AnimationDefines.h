#pragma once

namespace Animations
{
	// Be Careful Changing these Values as VertexShaders and Model Importer needs to be Updated as well

	/// <summary>
	/// Maximum Weights per Vertex
	/// </summary>
	/// <remarks>
	/// Very Annoying to Change as both the VertexShader assumes 4 Weights and the Model Importer Currently only Supports 4 Weights
	/// </remarks>
	constexpr int MAX_WEIGHTS = 4;
	/// <summary>
	/// Maximum Bones in Vertex Shader
	/// </summary>
	/// <remarks>
	/// Not restrictive at all, just means longer initial Load times. Tested and Works with MAX_BONES = 2 097 152 (No need to edit VertexShaders or Model Importer)
	/// </remarks>
	constexpr int MAX_BONES = 256;
}