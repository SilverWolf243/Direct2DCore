#include "pch.h"
#include "Direct2DCore.h"
#include "Direct2DBitmap.h"
void Direct2DCore::Init(HWND hWnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2DFactory);
	assert(hr == S_OK && "D2D1CreateFactory Init 에러 발생");

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

void Direct2DCore::CreateBitmapRenderTarget(HWND hWnd, ID2D1HwndRenderTarget* rt, ID2D1BitmapRenderTarget** brt)
{
	HRESULT hr = rt->CreateCompatibleRenderTarget(brt);
	assert(hr == S_OK && "CreateBitmapRenderTarget 에러 발생");
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
	assert(hr == S_OK && "pDecoder->GetFrame 에러 발생");
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

	UINT width, height;
	pSource->GetSize(&width, &height);

	UINT stride = width * 4;
	UINT size = stride * height;
	Direct2DBitmap* bitmap = new Direct2DBitmap(width, height, pBitmap);
	bitmap->RefBuffer().resize(size);
	hr = pSource->CopyPixels(nullptr, stride, size, bitmap->RefBuffer().data());
	assert(hr == S_OK && "pSource->CopyPixels 에러 발생");
	
	if (pDecoder) pDecoder->Release();
	if (pSource) pSource->Release();
	if (pConverter) pConverter->Release();

	return bitmap;
}
