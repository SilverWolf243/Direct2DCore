#pragma once
class Direct2DBitmap
{
private:
	int			 _width;
	int			 _height;
	vector<BYTE> _buffer;
	ID2D1Bitmap* _d2d1Bitmap;
public:
	Direct2DBitmap();
	~Direct2DBitmap();

	Direct2DBitmap(UINT width, UINT height, ID2D1Bitmap* d2d1Bitmap) : _width(width), _height(height), _d2d1Bitmap(d2d1Bitmap)
	{
			
	}
	vector<BYTE>& RefBuffer() { return _buffer; }
	ID2D1Bitmap* GetD2D1Bitmap() { return _d2d1Bitmap; }
};

