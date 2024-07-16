#pragma once
class __declspec(dllexport) Direct2DCore
{
private:
	ID2D1Factory* _pD2DFactory;
	IWICImagingFactory* _pWICFactory;
	IDWriteFactory* _pDWriteFactory;
public:
	static Direct2DCore* GetInstance() 
	{
		static Direct2DCore s_instance;
		return &s_instance;
	}

public:
	void Init();
	void Clear();

	void  CreateTextFromat(IDWriteTextFormat** pTextFormat, const wchar_t* fontName, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, float fontSize);
	void  CreateSolidColorBrush(ID2D1HwndRenderTarget* rt, const D2D1_COLOR_F color, ID2D1SolidColorBrush** brush);
	void  CreateRenderTarget(HWND hWnd, ID2D1HwndRenderTarget** rt);
	void  CreateBitmapRenderTarget(HWND hWnd, ID2D1HwndRenderTarget* rt, ID2D1BitmapRenderTarget** brt);
	class Direct2DBitmap* Direct2DLoadBitmap(const wchar_t* filePath, ID2D1RenderTarget* rt);
};

