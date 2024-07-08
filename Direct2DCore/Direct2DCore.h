#pragma once
class __declspec(dllexport) Direct2DCore
{
private:
	class ID2D1Factory* _pD2DFactory;
	class ID2D1HwndRenderTarget* _pRenderTarget;

public:
	static Direct2DCore* GetInstance() 
	{
		static Direct2DCore s_instance;
		return &s_instance;
	}
};

