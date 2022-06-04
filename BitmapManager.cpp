#include "BitmapManager.h"
#include "D2DFramework.h"

#pragma comment (lib, "WindowsCodecs.lib")

using namespace std;
using namespace Microsoft::WRL;

HRESULT BitmapManager::Initialize(ID2D1HwndRenderTarget* pRT)
{
	if (pRT == nullptr)
	{
		return E_FAIL;
	}
	
	mpRenderTarget = pRT;

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
		);
	ThrowIfFailed(hr);

	return S_OK;
}

void BitmapManager::Release()
{
	for (auto& bitmap : mBitmapResources)
	{
		bitmap.second.Reset();
	}

	mBitmapResources.clear();
	mspWICFactory.Reset();
	// mpRenderTarget 일반 포인터는 해제하면 안됨
}

HRESULT BitmapManager::LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap)
{
	auto pWICFactory{ mspWICFactory.Get() };
	// 예외 체크
	if (pWICFactory == nullptr)
	{
		return E_FAIL;
	}

	auto pRT{ mpRenderTarget };
	if (pRT == nullptr)
	{
		return E_FAIL;
	}

	// decoder
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr;

	hr = pWICFactory->CreateDecoderFromFilename(
		filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// frame
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	hr = bitmapDecoder->GetFrame(0, frame.GetAddressOf());
	ThrowIfFailed(hr);

	// converter
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	hr = pWICFactory->CreateFormatConverter(converter.GetAddressOf());
	ThrowIfFailed(hr);

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0,
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);

	// bitmap
	hr = pRT->CreateBitmapFromWicBitmap(
		converter.Get(),
		ppBitmap
	);
	ThrowIfFailed(hr);

	return S_OK;
}

ID2D1Bitmap* BitmapManager::LoadBitmap(std::wstring filename)
{
	if (mspWICFactory == nullptr)
	{
		return nullptr;
	}

	// 로딩을 안하고 map에 넣는다
	auto result = mBitmapResources.insert({filename, nullptr});

	if (result.second == true)
	{
		auto spBitmap = ComPtr<ID2D1Bitmap>();
		LoadWICBitmap(filename, spBitmap.GetAddressOf());

		result.first->second = spBitmap;
	}
	
	// 이미 존재하면 기존 포인터 반환
	return result.first->second.Get();
}

// map은 find, insert(중복 체크)를 할 때 - 둘 다 traverse 일어나면서 오버헤드 발생!
// 최적화 필요! -> find를 하지 않고 insert만 한다