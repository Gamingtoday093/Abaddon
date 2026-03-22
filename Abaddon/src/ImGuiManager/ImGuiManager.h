#pragma once
#include "ImGui/ImGui.h"
#include "external/ImGuizmo/ImGuizmo.h"

class Scene;
class Entity;
class ImGuiLogger;

class ImGuiManager
{
public:
	ImGuiManager(HWND aHWND, std::shared_ptr<Scene> aScene);
	~ImGuiManager();

	void SetScene(std::shared_ptr<Scene> aScene);
	void BeginFrame();
	void EndFrame();

private:
	void Update();
	void SceneTab(const std::shared_ptr<Scene>& aScene);
	void GameTab();
	void HierarchyTab(const std::shared_ptr<Scene>& aScene);
	void InspectorTab();
	void AssetsTab();
	void ConsoleTab();

	HWND myHWND;
	std::weak_ptr<Scene> myScene;
	ImGuizmo::OPERATION myGizmoOperation;
	ImGuizmo::MODE myGizmoSpace;
	std::unique_ptr<Entity> mySelectedEntity = nullptr;
	std::unique_ptr<ImGuiLogger> myLogger;
};

