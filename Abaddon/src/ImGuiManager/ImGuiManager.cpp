#include "pch.h"
#include "ImGuiManager.h"
#include "Graphics/DX11.h"
#include "Scene/Scene.h"
#include "Scene/Components/Components.h"
#include "Scene/ModelAssetHandler.h"

ImGuiManager::ImGuiManager(HWND& aHWND, std::shared_ptr<Scene> aScene) : myHWND(aHWND), myScene(aScene) {}

void ImGuiManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	ImGui_ImplWin32_Init(myHWND);
	ImGui_ImplDX11_Init(DX11::ourDevice.Get(), DX11::ourContext.Get());

	// Override Output Buffer with a Mirrored Buffer (Writes to Both)
	myLogger = std::make_unique<ImGuiLogger>();
	myLogger->Bind();
}

void ImGuiManager::Update()
{
	//ImGui::ShowDemoWindow();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	SceneTab();
	GameTab();
	HierarchyTab();
	InspectorTab();
	AssetsTab();
	ConsoleTab();
}

void ImGuiManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	Update();
	ImGui::Render();
}

void ImGuiManager::EndFrame()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ImGuiManager::SceneTab()
{
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse);

	RECT rect;
	GetClientRect(myHWND, &rect);

	ImVec2 textureSizeCorrected;
	float aspectRatio = (float)(rect.bottom - rect.top) / (float)(rect.right - rect.left);
	if (ImGui::GetWindowHeight() / ImGui::GetWindowWidth() > aspectRatio)
	{
		textureSizeCorrected = ImVec2{ ImGui::GetWindowSize().x, ImGui::GetWindowSize().x * aspectRatio };
	}
	else
	{
		aspectRatio = (float)(rect.right - rect.left) / (float)(rect.bottom - rect.top);
		textureSizeCorrected = ImVec2{ ImGui::GetWindowSize().y * aspectRatio, ImGui::GetWindowSize().y };
	}

	ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - textureSizeCorrected.x) * 0.5f, (ImGui::GetWindowSize().y - textureSizeCorrected.y) * 0.5f + 10 });
	ImGui::Image((void*)DX11::ourTextureSRV.Get(), textureSizeCorrected);

	if (mySelectedEntity && mySelectedEntity->HasComponent<TransformComponent>())
	{
		ImVec2 viewportMin = ImGui::GetItemRectMin();
		ImVec2 viewportMax = ImGui::GetItemRectMax();

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(viewportMin.x, viewportMin.y, viewportMax.x - viewportMin.x, viewportMax.y - viewportMin.y);

		DirectX::XMFLOAT4X4 storedViewMatrix;
		DirectX::XMStoreFloat4x4(&storedViewMatrix, myScene->GetCamera()->GetMatrix());
		float* viewMatrix = &storedViewMatrix.m[0][0];

		DirectX::XMFLOAT4X4 storedProjectionMatrix;
		DirectX::XMStoreFloat4x4(&storedProjectionMatrix, DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
		float* projectionMatrix = &storedProjectionMatrix.m[0][0];

		TransformComponent& transform = mySelectedEntity->GetComponent<TransformComponent>();

		DirectX::XMFLOAT4X4 storedModelMatrix;
		DirectX::XMStoreFloat4x4(&storedModelMatrix, transform.myTransform.GetModelMatrix());
		float* modelMatrix = &storedModelMatrix.m[0][0];

		ImGuizmo::Manipulate(viewMatrix, projectionMatrix, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD, modelMatrix);

		DirectX::XMVECTOR position;
		DirectX::XMVECTOR rotation;
		DirectX::XMVECTOR scale;
		DirectX::XMMatrixDecompose(&scale, &rotation, &position, DirectX::XMMATRIX(modelMatrix));

		transform.myTransform.myPosition = { DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position) };
		transform.myTransform.myRotation = (math::vector4<float> { DirectX::XMVectorGetX(rotation), DirectX::XMVectorGetY(rotation), DirectX::XMVectorGetZ(rotation), DirectX::XMVectorGetW(rotation) }).ToEuler();
		transform.myTransform.myScale = { DirectX::XMVectorGetX(scale), DirectX::XMVectorGetY(scale), DirectX::XMVectorGetZ(scale) };
	}

	ImGui::End();
}

void ImGuiManager::GameTab()
{
	ImGui::Begin("Game");
	ImGui::End();
}

void ImGuiManager::HierarchyTab()
{
	ImGui::Begin("Hierarchy");

	for (const auto& entity : myScene->GetAllEntities())
	{
		bool selected = mySelectedEntity && entity == *mySelectedEntity;
		if (entity.HasComponent<TagComponent>() && ImGui::Selectable(entity.GetComponent<TagComponent>().myTag.c_str(), selected))
		{
			mySelectedEntity = std::make_unique<Entity>(entity);
		}
	}

	ImGui::End();
}

void ImGuiManager::InspectorTab()
{
	ImGui::Begin("Inspector");

	if (mySelectedEntity)
	{
		Entity& entity = *mySelectedEntity;
		if (entity.HasComponent<TagComponent>())
		{
			TagComponent& tag = entity.GetComponent<TagComponent>();
			ImGui::SeparatorText("Tag");
			
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ImGui::GetFrameHeight() - ImGui::GetTextLineHeight()) * 0.5f);
			ImGui::Text("Tag");
			ImGui::SameLine();

			char tagBuffer[128];
			std::strncpy(tagBuffer, tag.myTag.c_str(), sizeof(tagBuffer));
			tagBuffer[sizeof(tagBuffer) - 1] = '\0';

			ImGui::PushItemWidth(-1);
			if (ImGui::InputText("##Tag", tagBuffer, sizeof(tagBuffer)))
			{
				if (tagBuffer[0] != '\0') tag.myTag = tagBuffer;
			}
		}
		if (entity.HasComponent<TransformComponent>())
		{
			ImGui::SeparatorText("Transform");
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			if (ImGui::BeginTable("Transform", 2))
			{
				ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_::ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_::ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ImGui::GetFrameHeight() - ImGui::GetTextLineHeight()) * 0.5f);
				ImGui::Text("Position");
				ImGui::TableNextColumn();
				float position[]
				{
					transform.myTransform.myPosition.x,
					transform.myTransform.myPosition.y,
					transform.myTransform.myPosition.z
				};
				ImGui::PushItemWidth(-1);
				if (ImGui::InputFloat3("##Position", position))
				{
					transform.myTransform.myPosition.x = position[0];
					transform.myTransform.myPosition.y = position[1];
					transform.myTransform.myPosition.z = position[2];
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ImGui::GetFrameHeight() - ImGui::GetTextLineHeight()) * 0.5f);
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				constexpr float PI = 3.14159265358979323846f;
				constexpr float RadToDegFactor = 180 / PI;
				constexpr float DegToRadFactor = PI / 180;
				float rotation[]
				{
					transform.myTransform.myRotation.x * RadToDegFactor,
					transform.myTransform.myRotation.y * RadToDegFactor,
					transform.myTransform.myRotation.z * RadToDegFactor
				};
				ImGui::PushItemWidth(-1);
				if (ImGui::InputFloat3("##Rotation", rotation))
				{
					transform.myTransform.myRotation.x = rotation[0] * DegToRadFactor;
					transform.myTransform.myRotation.y = rotation[1] * DegToRadFactor;
					transform.myTransform.myRotation.z = rotation[2] * DegToRadFactor;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ImGui::GetFrameHeight() - ImGui::GetTextLineHeight()) * 0.5f);
				ImGui::Text("Scale");
				ImGui::TableNextColumn();
				float scale[]
				{
					transform.myTransform.myScale.x,
					transform.myTransform.myScale.y,
					transform.myTransform.myScale.z
				};
				ImGui::PushItemWidth(-1);
				if (ImGui::InputFloat3("##Scale", scale))
				{
					transform.myTransform.myScale.x = scale[0];
					transform.myTransform.myScale.y = scale[1];
					transform.myTransform.myScale.z = scale[2];
				}

				ImGui::EndTable();
			}
		}
		if (entity.HasComponent<ModelComponent>())
		{
			ImGui::SeparatorText("Model");
			ModelComponent& model = entity.GetComponent<ModelComponent>();
			ImGui::Text(model.myModelName.c_str());
			ImGui::Text(model.myMaterialName.c_str());
		}
		if (entity.HasComponent<ScriptComponent>())
		{
			ImGui::SeparatorText("Script");
		}
	}

	ImGui::End();
}

void ImGuiManager::AssetsTab()
{
	ImGui::Begin("Assets");

	for (auto& model : ModelAssetHandler::myLoadedModels)
	{
		ImGui::Text(model.first.c_str());
	}

	ImGui::Separator();

	for (auto& texture : ModelAssetHandler::myLoadedTextures)
	{
		ImGui::Text(texture.first.c_str());
	}

	ImGui::Separator();

	for (auto& material : ModelAssetHandler::myCreatedMaterials)
	{
		ImGui::Text(material.first.c_str());
	}

	ImGui::End();
}

void ImGuiManager::ConsoleTab()
{
	ImGui::Begin("Console");

	if (ImGui::BeginPopupContextItem("ConsoleContextMenu"))
	{
		if (ImGui::MenuItem("Clear", "", false))
			myLogger->ClearOutput();

		ImGui::EndPopup();
	}

	for (auto& logMessage : myLogger->GetOutput())
		ImGui::TextColored(logMessage.myColor, logMessage.myMessage.c_str());
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);
	
	ImGui::End();
}
