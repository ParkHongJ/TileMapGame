#pragma once
#include "Singleton.h"
#include "config.h"
#include <wincodec.h>

///*
//	배열 : 인덱스 -> 데이터
//	stl::vector : 인덱스 -> 데이터
//	std::map : 키 -> 데이터
//
//	레드-블랙 트리 구현 : 삽입, 삭제, 검색 O(logN) 시간복잡도
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

