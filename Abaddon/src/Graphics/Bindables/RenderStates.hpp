#pragma once

enum class eBlendState : uint8_t
{
	Opaque,
	Alpha,
	Additive,
	Multiply,
	Premultiplied,
	Count
};

enum class eDepthMode : uint8_t // TODO
{
	ReadWrite,
	ReadOnly,
	Disabled
};

enum class eCullMode : uint8_t // TODO
{
	Back,
	Front,
	None
};