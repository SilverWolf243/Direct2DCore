#pragma once
class __declspec(dllexport) Direct2DCore
{
private:
	ID2D1Factory* _pD2DFactory;
	IWICImagingFactory* _pWICFactory;

public:
	static Direct2DCore* GetInstance() 
	{
		static Direct2DCore s_instance;
		return &s_instance;
	}

public:
	void Init(HWND hWnd);
	void Clear();

	void  CreateRenderTarget(HWND hWnd, ID2D1HwndRenderTarget** rt);
	void  CreateBitmapRenderTarget(HWND hWnd, ID2D1HwndRenderTarget* rt, ID2D1BitmapRenderTarget** brt);
	class Direct2DBitmap* Direct2DLoadBitmap(const wchar_t* filePath, ID2D1RenderTarget* rt);
};

