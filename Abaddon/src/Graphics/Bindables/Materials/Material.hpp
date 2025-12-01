#pragma once
#include "pch.h"
#include "Graphics/Bindables/PixelShader.h"
#include "Graphics/Bindables/RenderStates.hpp"

class Material abstract
{
public:
	Material() = delete;
	Material(const std::string aPixelShader, eBlendState aBlendState = eBlendState::Opaque);
	~Material() = default;

	eBlendState GetBlendState() const;
	virtual void Bind();

private:
	eBlendState myBlendState;
	PixelShader myPixelShader;
};

inline Material::Material(const std::string aPixelShader, eBlendState aBlendState)
{
	myBlendState = aBlendState;
	myPixelShader.Init(aPixelShader);
}

inline eBlendState Material::GetBlendState() const
{
	return myBlendState;
}

inline void Material::Bind()
{
	myPixelShader.Bind();
}