#pragma once
#include "ImGui/ImGui.h"
#include "ImGuiLogger.h"
#include "Scene/Entity.h"

class Scene;

class ImGuiManager
{
public:
	ImGuiManager(HWND& aHWND, std::shared_ptr<Scene> aScene);
	~ImGuiManager() = default;

	void Init();
	void Update();
	void BeginFrame();
	void EndFrame();

private:
	void SceneTab();
	void GameTab();
	void HierarchyTab();
	void InspectorTab();
	void AssetsTab();
	void ConsoleTab();

	HWND& myHWND;
	std::shared_ptr<Scene> myScene;
	std::unique_ptr<Entity> mySelectedEntity = nullptr;
	std::unique_ptr<ImGuiLogger> myLogger;
};

