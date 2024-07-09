#pragma once
class __declspec(dllexport) Direct2DCore
{
private:
	ID2D1Factory* _pD2DFactory;
	ID2D1HwndRenderTarget* _pRenderTarget;
	ID2D1BitmapRenderTarget* _pBitmapRenderTarget;

public:
	static Direct2DCore* GetInstance() 
	{
		static Direct2DCore s_instance;
		return &s_instance;
	}

	ID2D1HwndRenderTarget* GetHWNDRT() 
	{
		return _pRenderTarget;
	}

	ID2D1BitmapRenderTarget* GetBitmapRT() 
	{
		return _pBitmapRenderTarget;
	}

public:
	void Init(HWND hWnd);
	void Clear();
};

