#include <sstream>
#include "D2DFramework.h"
#include "BitmapManager.h"
#include "Game.h"

#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

concentration::Game myGame;

HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HWND hwnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = WINDOW_CLASSNAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = D2DFramework::WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register the window class", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	RECT rc{ 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&rc, WS_EX_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindowEx(
		NULL,
		WINDOW_CLASSNAME,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Failed to create the window class", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	
	mHwnd = hwnd;

	SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	return S_OK;
}

HRESULT D2DFramework::InitD2D()
{
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, mspD2DFactory.GetAddressOf()
	);
	ThrowIfFailed(hr);

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(mspDWriteFactory.GetAddressOf())
	);
	ThrowIfFailed(hr);

	hr = mspDWriteFactory->CreateTextFormat(
		L"Calibri",
		NULL,
		DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-us",
		mspTextFormat.GetAddressOf()
	);
	ThrowIfFailed(hr);

	//hr = mspTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	//ThrowIfFailed(hr);

	//hr = mspTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	//ThrowIfFailed(hr);

	return CreateDevicResources();
}

HRESULT D2DFramework::CreateDevicResources()
{
	RECT rc;

	GetClientRect(mHwnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	HRESULT hr = mspD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(mHwnd, size),
		mspRenderTarget.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);
	
	hr = mspRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		mspBrush.GetAddressOf()
	);
	ThrowIfFailed(hr);

	return hr;
}

HRESULT D2DFramework::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr;
	
	hr = CoInitialize(nullptr);
	ThrowIfFailed(hr);

	hr = InitWindow(hInstance, title, width, height);
	ThrowIfFailed(hr);

	hr = InitD2D();
	ThrowIfFailed(hr);

	hr = BitmapManager::Instance().Initialize(mspRenderTarget.Get());
	ThrowIfFailed(hr, "Failed to Initialize BitmapManager");

	ShowWindow(mHwnd, SW_SHOW);
	UpdateWindow(mHwnd);

	return hr;
}

void D2DFramework::Release()
{
	BitmapManager::Instance().Release();
	
	mspBrush.Reset();
	mspRenderTarget.Reset();
	mspTextFormat.Reset();
	mspDWriteFactory.Reset();
	mspD2DFactory.Reset();

	CoUninitialize();
}

int D2DFramework::GameLoop()
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			Render();
		}
	}

	Release();

	return static_cast<int>(msg.wParam);
}

void D2DFramework::Render()
{
	HRESULT hr;
	
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDevicResources();
	}
}

void D2DFramework::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
{
	std::wostringstream oss;

	oss << L"ERROR : " << error << std::endl;
	oss << msg;

	OutputDebugString(oss.str().c_str());

	MessageBox(nullptr, oss.str().c_str(), title, MB_ICONEXCLAMATION | MB_OK);
}

LRESULT CALLBACK D2DFramework::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	concentration::Game* game = reinterpret_cast<concentration::Game*>((GetWindowLongPtr(hwnd, GWLP_USERDATA)));
	
	switch (message)
	{
		case WM_LBUTTONUP:
			game->OnClick(LOWORD(lParam), HIWORD(lParam));
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

	return 0;
}
