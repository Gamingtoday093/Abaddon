#pragma once

enum class eBlendState
{
	Opaque,
	Alpha,
	Additive,
	Multiply,
	Premultiplied,
	Count
};

enum class eDepthMode // TODO
{
	ReadWrite,
	ReadOnly,
	Disabled
};

enum class eCullMode // TODO
{
	Back,
	Front,
	None
};