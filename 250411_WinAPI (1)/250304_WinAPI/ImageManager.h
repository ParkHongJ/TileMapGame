#pragma once
#include "Singleton.h"
#include "config.h"
#include <wincodec.h>

///*
//	�迭 : �ε��� -> ������
//	stl::vector : �ε��� -> ������
//	std::map : Ű -> ������
//
//	����-�� Ʈ�� ���� : ����, ����, �˻� O(logN) �ð����⵵
//*/

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<string, Image*> mapImages;

	ComPtr<IWICImagingFactory> wicFactory;

public:
	IWICImagingFactory* GetWICFactory();
	void Init();
	void Release();

	Image* AddImage(string key, const wchar_t* filePath,
		ID2D1RenderTarget* renderTarget);

	//Legacy
	/*Image* AddImage(string key, const wchar_t* filePath,
		int width, int height,
		bool isTransparent = FALSE,
		COLORREF transColor = FALSE);*/

	Image* AddImage(string key, const wchar_t* filePath, 
		int maxFrameX, int maxFrameY, ID2D1RenderTarget* renderTarget);

	void DeleteImage(string key);
	Image* FindImage(string key);
};

