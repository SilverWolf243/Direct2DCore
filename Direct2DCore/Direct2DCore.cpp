#include "pch.h"
#include "Direct2DCore.h"
#include "Direct2DBitmap.h"
void Direct2DCore::Init()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2DFactory);
	assert(hr == S_OK && "D2D1CreateFactory Init 에러 발생");

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&_pDWriteFactory));
	assert(hr == S_OK && "DWriteCreateFactory Init 에러 발생");

	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(hr == S_OK && "CoInitializeEx Init 에러 발생");

	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_pWICFactory));
	assert(hr == S_OK && "CoCreateInstance Init 에러 발생");
}

void Direct2DCore::Clear()
{
	if (_pD2DFactory) _pD2DFactory->Release();
	CoUninitialize();
	if (_pWICFactory) _pWICFactory->Release();
}

void Direct2DCore::CreateRenderTarget(HWND hWnd, ID2D1HwndRenderTarget** rt)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	HRESULT hr = _pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		rt);
	assert(hr == S_OK && "CreateRenderTarget 에러 발생");
}

void Direct2DCore::CreateTextFromat(IDWriteTextFormat** pTextFormat, const wchar_t* fontName, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, float fontSize)
{
	HRESULT hr = _pDWriteFactory->CreateTextFormat(fontName, NULL, fontWeight, fontStyle, fontStretch, fontSize, L"en-us", pTextFormat);
	assert(hr == S_OK && "CreateTextFromat 에러 발생");
}

void Direct2DCore::CreateBitmapRenderTarget(HWND hWnd, ID2D1HwndRenderTarget* rt, ID2D1BitmapRenderTarget** brt)
{
	HRESULT hr = rt->CreateCompatibleRenderTarget(brt);
	assert(hr == S_OK && "CreateBitmapRenderTarget 에러 발생");
}

void Direct2DCore::CreateSolidColorBrush(ID2D1HwndRenderTarget* rt, const D2D1_COLOR_F color, ID2D1SolidColorBrush** brush)
{
	HRESULT hr = rt->CreateSolidColorBrush(color, brush);
	assert(hr == S_OK && "CreateSolidColorBrush 에러 발생");
}

Direct2DBitmap* Direct2DCore::Direct2DLoadBitmap(const wchar_t* filePath, ID2D1RenderTarget* rt)
{
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pSource = nullptr;
	IWICStream* pStream = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	IWICBitmapScaler* pScaler = nullptr;

	HRESULT hr = _pWICFactory->CreateDecoderFromFilename(
		filePath,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	assert(hr == S_OK && "_pWICFactory->CreateDecoderFromFilename 에러 발생");
	
	hr = pDecoder->GetFrame(0, &pSource);
	assert(hr == S_OK && "pDecoder->GetFrame(0, &pSource) 에러 발생");

	IWICBitmapSource* bitmapSource = pSource;
	hr = WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, pSource, &bitmapSource);
	assert(hr == S_OK && "WICConvertBitmapSource 에러 발생");

	UINT width = 0;
	UINT height = 0;
	hr = bitmapSource->GetSize(&width, &height);
	assert(hr == S_OK && "bitmapSource->GetSize 에러 발생");

	const int buffSize = width * height * 4;
	BYTE* buffer = new BYTE[width * height * 4];
	hr = bitmapSource->CopyPixels(NULL, width * 4, buffSize, buffer);
	assert(hr == S_OK && "bitmapSource->CopyPixels 에러 발생");

	bitmapSource->Release();
	assert(hr == S_OK && "bitmapSource->Release() 에러 발생");
	hr = _pWICFactory->CreateFormatConverter(&pConverter);
	
	assert(hr == S_OK && "_pWICFactory->CreateFormatConverter 에러 발생");
	hr = pConverter->Initialize(
		pSource
		, GUID_WICPixelFormat32bppPBGRA
		, WICBitmapDitherTypeNone
		, nullptr
		, 0.0f
		, WICBitmapPaletteTypeCustom);
	assert(hr == S_OK && "pConverter->Initialize 에러 발생");

	ID2D1Bitmap* pBitmap = nullptr;

	hr = rt->CreateBitmapFromWicBitmap(pConverter, nullptr, &pBitmap);
	assert(hr == S_OK && "CreateBitmapFromWicBitmap 에러 발생");

	Direct2DBitmap* bitmap = new Direct2DBitmap(width, height, pBitmap, buffer);
	
	if (pDecoder) pDecoder->Release();
	if (pSource) pSource->Release();
	if (pConverter) pConverter->Release();

	return bitmap;
}
