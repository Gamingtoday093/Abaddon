#pragma once
#include "Graphics/Bindables/Bindables.h"
#include <unordered_map>

enum class ePrimitive : int
{
	Plane,
	Cube
};

class ModelAssetHandler
{
public:
	static void LoadModel(std::string aModelFileName, std::string aVertexShaderFileName = "VertexShader_vs.cso");
	static void LoadTexture(std::string aTextureFileName);

	static ModelData& GetPrimitiveModelData(ePrimitive aPrimitive);
	static ModelData& GetModelData(std::string aModelFileName);
	static TextureData& GetTextureData(std::string aTextureFileName);

private:
	static void LoadPrimitiveModels();
	static std::unordered_map<ePrimitive, ModelData> myPrimitiveModels;
	static std::unordered_map<std::string, ModelData> myLoadedModels;
	static std::unordered_map<std::string, TextureData> myLoadedTextures;
};

