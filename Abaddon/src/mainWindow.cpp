#include "pch.h"
#include <windows.h>
#include <iostream>
#include "../Resources/resource.h"
#include <filesystem>
#include "Graphics/DX11.h"
#include "Graphics/Renderer.h"
#include "Graphics/Scene/Scene.h"

#include "Tools/Input.h"

#pragma warning(disable : 28251)

static Input myInput;

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

	float clearColor[4] = { 0.3f,0.4f,0.6f,1.0f };

	// Create renderer
	Renderer renderer;

	// Create scene
	Scene scene;
	scene.Init(myInput);

	// Init
	std::shared_ptr<Model> rose = std::make_shared<Model>();
	rose->LoadModel("Rose.obj", scene.GetCamera());
	scene.AddModel(rose);

	std::shared_ptr<Model> landscape = std::make_shared<Model>();
	landscape->LoadModel("Landscape.fbx", scene.GetCamera());
	scene.AddModel(landscape);

	std::shared_ptr<Model> house = std::make_shared<Model>();
	house->LoadModel("House.obj", scene.GetCamera());
	scene.AddModel(house);

	math::vector3<float> movementDirection = { 0, 0, 0 };
	float movementSpeed = 1.f;

	//math::vector3<float>::forward()

	auto testVector = math::vector3<float>::forward();

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
		myInput.Update();

		// Game loop --------------
		movementDirection = { 0, 0, 0 };
		if (myInput.IsKeyDown((int)eKeys::W))
		{
			movementDirection.z += 1;
		}
		if (myInput.IsKeyDown((int)eKeys::A))
		{
			movementDirection.x -= 1;
		}
		if (myInput.IsKeyDown((int)eKeys::S))
		{
			movementDirection.z -= 1;
		}
		if (myInput.IsKeyDown((int)eKeys::D))
		{
			movementDirection.x += 1;
		}
		if (movementDirection.LengthSqr() > 0)
		{
			movementDirection.Normalize();
		}

		house->Transform(movementDirection * movementSpeed, house->GetRotation());

		scene.Update();
		// ------------------------

		renderer.Render(scene.GetModels());
		framework.EndFrame();
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	myInput.UpdateEvents(uMsg, wParam, lParam);

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