#pragma once
#include "Graphics/Bindables/Bindables.h"
#include "Graphics/Bindables/Animation/Animation.h"
#include "Graphics/Bindables/Materials/Material.hpp"
#include <concepts>

struct aiNode;

enum class ePrimitive : uint8_t
{
	Plane,
	Cube
};

template<typename T>
concept DerivedFromMaterial = std::derived_from<T, Material>;

class ModelAssetHandler
{
public:
	static void LoadModel(const std::string& aModelFileName);
	static void LoadAnimations(const std::string& aAnimationFileName);
	static void LoadTexture(const std::string& aTextureFileName);
	template<DerivedFromMaterial T>
	static T& CreateMaterial(const std::string& aMaterialName);

	static ModelData& GetPrimitiveModelData(ePrimitive aPrimitive);
	static ModelData& GetModelData(const std::string& aModelFileName);
	static Animation& GetAnimation(const std::string& aAnimationName);
	static TextureData& GetTextureData(const std::string& aTextureFileName);
	static Material& GetMaterial(const std::string& aMaterialName);
	template<DerivedFromMaterial T>
	static T& GetMaterial(const std::string& aMaterialName);

private:
	friend class ImGuiManager;

	static void LoadPrimitiveModels();
	static void LoadBoneHierarchyRecursive(const std::unordered_map<std::string, unsigned int>& aBoneNameToIndex, Skeleton& aSkeleton, const aiNode* aNode);
	static std::unordered_map<ePrimitive, ModelData> myPrimitiveModels;
	static std::unordered_map<std::string, ModelData> myLoadedModels;
	static std::unordered_map<std::string, Animation> myLoadedAnimations;
	static std::unordered_map<std::string, TextureData> myLoadedTextures;
	static std::unordered_map<std::string, Material*> myCreatedMaterials; // Materials need to be pointers as they use inheritence
};

template<DerivedFromMaterial T>
T& ModelAssetHandler::CreateMaterial(const std::string& aMaterialName)
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
T& ModelAssetHandler::GetMaterial(const std::string& aMaterialName)
{
	return static_cast<T&>(GetMaterial(aMaterialName));
}
