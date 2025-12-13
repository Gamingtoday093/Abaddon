#pragma once

namespace Animations
{
	// Be Careful Changing these Values as VertexShaders and Model Importer needs to be Updated as well

	/// <summary>
	/// Maximum Weights per Vertex
	/// </summary>
	constexpr int MAX_WEIGHTS = 4;
	/// <summary>
	/// Maximum Bones in Vertex Shader
	/// </summary>
	constexpr int MAX_BONES = 32;
}