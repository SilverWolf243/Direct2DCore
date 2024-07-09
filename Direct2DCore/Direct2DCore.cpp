#include "pch.h"
#include "Direct2DCore.h"

void Direct2DCore::Init(HWND hWnd)
{
	RECT rc;
	
	GetClientRect(hWnd, &rc);
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2DFactory);
	assert(hr == S_OK && "D2D1CreateFactory Init 에러 발생");

	hr = _pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&_pRenderTarget);
	assert(hr == S_OK && "CreateHwndRenderTarget Init 에러 발생");

	_pRenderTarget->CreateCompatibleRenderTarget(&_pBitmapRenderTarget);
}

void Direct2DCore::Clear()
{
	if (_pBitmapRenderTarget) _pBitmapRenderTarget->Release();
	if (_pRenderTarget) _pRenderTarget->Release();
	if (_pD2DFactory) _pD2DFactory->Release();
}
