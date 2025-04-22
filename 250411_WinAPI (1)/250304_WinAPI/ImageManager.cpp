#include "pch.h"
#include "ImageManager.h"
#include "Image.h"

IWICImagingFactory* ImageManager::GetWICFactory()
{
	return wicFactory.Get();
}

void ImageManager::Init()
{
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory, nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&wicFactory)
	);
	
	//문제가있따
	if (FAILED(hr))
	{
		int a = 10;
	}	
}

void ImageManager::Release()
{
	map<string, Image*>::iterator iter;
	for (iter = mapImages.begin(); iter != mapImages.end(); iter++)
	{
		if (iter->second)
		{
			(iter->second)->Release();
			delete (iter->second);
			(iter->second) = nullptr;
		}
	}
	mapImages.clear();

	ReleaseInstance();
}

Image* ImageManager::AddImage(string key, const wchar_t* filePath, ID2D1RenderTarget* renderTarget)
{
	Image* image = nullptr;
	image = FindImage(key);
	if (image)	return image;

	image = new Image();
	if (FAILED(image->Init(renderTarget, filePath)))
	{
		image->Release();
		delete image;

		return nullptr;
	}

	mapImages.insert(make_pair(key, image)); 

	return image;
}

//Image* ImageManager::AddImage(string key,
//	const wchar_t* filePath, int width, int height, 
//	bool isTransparent, COLORREF transColor)
//{
//	/*Image* image = nullptr;
//	image = FindImage(key);
//	if (image)	return image;
//
//	image = new Image();
//	if (FAILED(image->Init(filePath, width, height,
//		isTransparent, transColor)))
//	{
//		image->Release();
//		delete image;
//
//		return nullptr;
//	}
//
//	mapImages.insert(make_pair(key, image));*/
//
//	Image* image = new Image;
//
//	//image->Init();
//	return image;
//}

//Image* ImageManager::AddImage(string key, 
//	const wchar_t* filePath, int width, int height, 
//	int maxFrameX, int maxFrameY, 
//	bool isTransparent, COLORREF transColor)
//{
//	Image* image = nullptr;
//	image = FindImage(key);
//	if (image)	return image;
//
//	image = new Image();
//	if (FAILED(image->Init(filePath, width, height,
//		maxFrameX, maxFrameY,
//		isTransparent, transColor)))
//	{
//		image->Release();
//		delete image;
//
//		return nullptr;
//	}
//
//	mapImages.insert(make_pair(key, image));
//	return image;
//}

Image* ImageManager::AddImage(string key, const wchar_t* filePath, int maxFrameX, int maxFrameY, ID2D1RenderTarget* renderTarget)
{
	Image* image = nullptr;
	image = FindImage(key);
	if (image)	return image;

	image = new Image();
	if (FAILED(image->Init(renderTarget, filePath, maxFrameX, maxFrameY)))
	{
		image->Release();
		delete image;

		return nullptr;
	}

	mapImages.insert(make_pair(key, image));

	return image;
}

void ImageManager::DeleteImage(string key)
{
	map<string, Image*>::iterator iter;
	iter = mapImages.find(key);

	if (iter == mapImages.end()) return;

	(iter->second)->Release();
	delete (iter->second); 
	(iter->second) = nullptr;

	mapImages.erase(iter);
}

Image* ImageManager::FindImage(string key)
{
	map<string, Image*>::iterator iter;
	iter = mapImages.find(key);

	if (iter == mapImages.end()) return nullptr;

	return iter->second;

}
