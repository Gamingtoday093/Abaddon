#include "pch.h"
#include <windows.h>
#include <iostream>
#include "../Resources/resource.h"
#include <filesystem>
#include "Graphics/DX11.h"
#include "Scene/Scene.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderPass.h"
#include "Graphics/Scene/Scene.h"
#include "Exo-Star Encounter/Unit.h"
#include "Graphics/Skybox/CubeTexture.h"
#include "Graphics/Skybox/Cube.h"

#include "Tools/Input.h"
#include <Exo-Star Encounter/UnitManager.h>

#pragma warning(disable : 28251)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

FILE* CreateConsole()
{
	FILE* consoleStream;
	AllocConsole();
	freopen_s(&consoleStream, "CONIN$", "r", stdin);
	freopen_s(&consoleStream, "CONOUT$", "w", stdout);
	freopen_s(&consoleStream, "CONOUT$", "w", stderr);

	LOG_SUCCESS("Console successfully initiated.");

	return consoleStream;
}

void CreateAndRegisterWindowClass(HINSTANCE& hInstance, LPCWSTR className)
{
	LPCWSTR CLASS_NAME = className;

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));

	RegisterClass(&wc);
}

static float rotation = 0;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
#pragma region SetWorkDir
	// Set working directory
	std::filesystem::path currentPath = std::filesystem::current_path();

	if (IsDebuggerPresent())
	{
		currentPath.append("../Assets");
	}
	else
	{
		currentPath.append("../../Assets");
	}

	std::filesystem::current_path(currentPath);
#pragma endregion

	// Create console
	FILE* consoleStream = CreateConsole();

	// Register the window class.
	LPCWSTR className = L"Abaddon";
	CreateAndRegisterWindowClass(hInstance, className);

	// Create the window.
#pragma region CreateWindow
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		className,                     // Window class
		L"Abaddon",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		LOG_ERROR("Failed to create window");
		Assert(false);
		return 0;
	}
#pragma endregion

	// Set window position and size
	int width = 1366;
	int height = 768;

	SetWindowPos(hwnd, nullptr, 200, 200, width, height, 0);
	ShowWindow(hwnd, nCmdShow);
	// ---------------------------------------------------------------

	// Create framework
	DX11 framework(hwnd);
	framework.Initialize(true);

	//float clearColor[4] = { 0.3f,0.4f,0.6f,1.0f };
	float clearColor[4] = { 0.16f, 0.16f, 0.16f, 0.16f };

	// Create Render Passes
	std::shared_ptr<RenderPass> DefaultPass = std::make_shared<RenderPass>();
	DefaultPass->Init("VertexShader_vs.cso", D3D11_CULL_BACK, "PixelShader_ps.cso");

	std::shared_ptr<RenderPass> SkyboxPass = std::make_shared<RenderPass>();
	SkyboxPass->Init("Skybox_vs.cso", D3D11_CULL_NONE, "Skybox_ps.cso");

	// Create renderer
	Renderer renderer;
	renderer.Init();

	// Create scene
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(renderer);
	scene->Init();

	// Skybox
	std::shared_ptr<CubeTexture> SkyboxTexture = std::make_shared<CubeTexture>();
	SkyboxTexture->Init("Skybox");

	std::shared_ptr<Cube> SkyboxMesh = std::make_shared<Cube>();
	SkyboxMesh->Init();

	// Init
	std::shared_ptr<Unit> ship = UnitManager::CreateUnit();
	ship->LoadModel("Ship.fbx", "ShipTexture.png", scene.GetCamera());
	scene.AddModel(ship);

	std::shared_ptr<Unit> ship2 = UnitManager::CreateUnit();
	ship2->LoadModel("Ship.fbx", "ShipTexture.png", scene.GetCamera());
	scene.AddModel(ship2);

	ship2->SetPosition({ -30, 0, 10 });

	std::shared_ptr<Unit> ship3 = UnitManager::CreateUnit();
	ship3->LoadModel("Ship.fbx", "ShipTexture.png", scene.GetCamera());
	scene.AddModel(ship3);

	ship3->SetPosition({ 3, 0, 20 });

	scene.Awake();

	bool dragging = false;
	math::vector2<float> dragSelectStart;

	math::vector3<float> movementDirection = { 0, 0, 0 };
	float movementSpeed = 1.f;

	// Loop
	bool running = true;
	MSG msg = { };
	while (running)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				// End program
				consoleStream = nullptr;
				delete consoleStream;

				running = false;
			}
		}

		framework.BeginFrame(clearColor);
		Input::GetInstance().Update();

		// Game loop --------------
		if (Input::GetInstance().IsMouseButtonPressed((int)eKeys::MOUSERBUTTON))
		{
			math::vector3<float> rayOrigin = scene.GetCamera()->CameraSpaceToWorldSpace(Camera::MousePositionToCameraSpace(hwnd));
			math::vector3<float> rayDirection = (rayOrigin - scene.GetCamera()->GetPosition()).GetNormalized();

			math::vector3<float> normal = math::vector3<float>::up();

			float t = -(normal.Dot(rayOrigin)) / normal.Dot(rayDirection);
			
			math::vector3<float> hitPosition = rayOrigin + (rayDirection * t);

			for (std::shared_ptr<Unit> unit : UnitManager::GetUnits())
			{
				if (unit->GetIsSelected())
					unit->MoveTo(hitPosition);
			}
		}

		if (Input::GetInstance().IsMouseButtonDown((int)eKeys::MOUSELBUTTON) && !dragging)
		{
			dragSelectStart = Camera::MousePositionToCameraSpace(hwnd);
			dragging = true;
		}

		if (Input::GetInstance().IsMouseButtonUp((int)eKeys::MOUSELBUTTON))
		{
			math::vector2<float> dragSelectEnd = Camera::MousePositionToCameraSpace(hwnd);
			dragging = false;

			if ((dragSelectEnd - dragSelectStart).Length() > 0.1f) // Drag
			{
				for (std::shared_ptr<Unit> unit : UnitManager::GetUnits())
				{
					math::vector2<float> unitPosition = scene.GetCamera()->WorldSpaceToCameraSpace(unit->GetPosition());

					unit->Select(unitPosition.x > std::min(dragSelectStart.x, dragSelectEnd.x) && unitPosition.y > std::min(dragSelectStart.y, dragSelectEnd.y) &&
						unitPosition.x < std::max(dragSelectStart.x, dragSelectEnd.x) && unitPosition.y < std::max(dragSelectStart.y, dragSelectEnd.y));
				}
			}
			else // Click
			{
				math::vector3<float> rayOrigin = scene.GetCamera()->CameraSpaceToWorldSpace(Camera::MousePositionToCameraSpace(hwnd));
				math::vector3<float> rayDirection = (rayOrigin - scene.GetCamera()->GetPosition()).GetNormalized();

				for (std::shared_ptr<Unit> unit : UnitManager::GetUnits())
				{
					unit->Select(unit->GetTransformedAABB().RayBounds(rayOrigin, rayDirection) && !unit->GetIsSelected());
				}
			}
		}

		scene.Update();
		// ------------------------

		SkyboxPass->Bind();
		SkyboxMesh->UpdateViewProjection(scene.GetCamera());
		renderer.RenderSkybox(SkyboxMesh, SkyboxTexture);

		DefaultPass->Bind();
		renderer.Render(scene.GetModels());
		framework.EndFrame();
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Input::GetInstance().UpdateEvents(uMsg, wParam, lParam);

	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));

			EndPaint(hwnd, &ps);
		}
		return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}