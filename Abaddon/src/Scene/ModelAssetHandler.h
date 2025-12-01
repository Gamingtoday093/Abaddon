#pragma once
#include "Graphics/Bindables/Bindables.h"
#include "Graphics/Bindables/Materials/Material.hpp"
#include <unordered_map>
#include <concepts>

enum class ePrimitive : int
{
	Plane,
	Cube
};

template<typename T>
concept DerivedFromMaterial = std::derived_from<T, Material>;

class ModelAssetHandler
{
public:
	static void LoadModel(std::string aModelFileName, std::string aVertexShaderFileName = "VertexShader_vs.cso");
	static void LoadTexture(std::string aTextureFileName);
	template<DerivedFromMaterial T>
	static T& CreateMaterial(std::string aMaterialName);

	static ModelData& GetPrimitiveModelData(ePrimitive aPrimitive);
	static ModelData& GetModelData(std::string aModelFileName);
	static TextureData& GetTextureData(std::string aTextureFileName);
	static Material& GetMaterial(std::string aMaterialName);
	template<DerivedFromMaterial T>
	static T& GetMaterial(std::string aMaterialName);

private:
	friend class ImGuiManager;

	static void LoadPrimitiveModels();
	static std::unordered_map<ePrimitive, ModelData> myPrimitiveModels;
	static std::unordered_map<std::string, ModelData> myLoadedModels;
	static std::unordered_map<std::string, TextureData> myLoadedTextures;
	static std::unordered_map<std::string, Material*> myCreatedMaterials; // Raw Pointer because idgaf
};

template<DerivedFromMaterial T>
T& ModelAssetHandler::CreateMaterial(std::string aMaterialName)
{
	if (myCreatedMaterials.find(aMaterialName) != myCreatedMaterials.end())
	{
		LOG_WARNING("Material already exists.");
		Assert(false);
	}

	Material* material = myCreatedMaterials.emplace(aMaterialName, new T()).first->second;
	return static_cast<T&>(*material);
}

template<DerivedFromMaterial T>
T& ModelAssetHandler::GetMaterial(std::string aMaterialName)
{
	return static_cast<T&>(GetMaterial(aMaterialName));
}
