#include "pch.h"
#include <windows.h>
#include <iostream>
#include "../Resources/resource.h"
#include "Graphics/Graphics.h"
#include <filesystem>

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

	// Init graphics class
	Graphics graphics(hwnd);
	float clearColor[4] = { 0.3f,0.4f,0.6f,1.0f };
	graphics.Init(clearColor);

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

		// Game loop
		graphics.Update(rotation);
		rotation += 0.02f;

		graphics.EndFrame();
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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