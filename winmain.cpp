#include <Windows.h>
#include <gdiplus.h>
#include "Game.h"

#pragma comment (lib, "Gdiplus.lib")

const wchar_t* gClassName = L"MyWindowClass";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain
(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd
)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	HWND hwnd;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to Register a Window Class", L"Error", MB_OK);
		return 0;
	}

	RECT rc{ 0, 0, 1024, 768 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Concentration",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hwnd)
	{
		MessageBox(NULL, L"Failed to Create a Window", L"Error", MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);

	Gdiplus::Graphics graphics(hdc);

	Game game;
	game.Init(hwnd);
	game.Draw(graphics);

	Card card(hwnd, Type::Wolf, 10, 10);
	Card card1(hwnd, Type::Bear, 120, 10);
	card.Draw(graphics);
	card1.Draw(graphics);

	card.Flip(true);

	card.Draw(graphics);
	card1.Draw(graphics);

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			OnPaint(hwnd);
			break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
}