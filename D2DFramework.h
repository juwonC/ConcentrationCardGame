#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>
#include "ComException.h"

class D2DFramework
{
private:
	const LPCWSTR WINDOW_CLASSNAME{ L"D2DWindowClass" };

protected:
	HWND mHwnd{};
	Microsoft::WRL::ComPtr<ID2D1Factory>			mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>	mspRenderTarget{};
	Microsoft::WRL::ComPtr<IDWriteFactory>			mspDWriteFactory{};
	Microsoft::WRL::ComPtr<IDWriteTextFormat>		mspTextFormat{};
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	mspBrush{};

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	virtual HRESULT InitD2D();
	virtual HRESULT CreateDevicResources();

public:
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"D2DFramework", 
		UINT width = 1024, UINT height = 800);
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error = 0, LPCWSTR title = L"Error");

public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	ID2D1HwndRenderTarget* GetRenderTarget() { return mspRenderTarget.Get(); }
	inline HWND GetWindowHandle() const { return mHwnd; }
};

