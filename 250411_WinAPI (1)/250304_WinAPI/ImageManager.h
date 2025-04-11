#pragma once
#include "Singleton.h"
#include "config.h"

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

public:
	void Init();
	void Release();

	Image* AddImage(string key, const wchar_t* filePath,
		int width, int height,
		bool isTransparent = FALSE,
		COLORREF transColor = FALSE);

	Image* AddImage(string key, const wchar_t* filePath, 
		int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTransparent = FALSE, 
		COLORREF transColor = FALSE);

	void DeleteImage(string key);
	Image* FindImage(string key);
};

