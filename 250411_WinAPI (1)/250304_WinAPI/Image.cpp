#include "Image.h"
#include "ImageManager.h"
//Lagacy
//HRESULT Image::Init(int width, int height)
//{
//    HDC hdc = GetDC(g_hWnd);
//
//    imageInfo = new IMAGE_INFO();
//    imageInfo->resID = 0;
//    imageInfo->hMemDC = CreateCompatibleDC(hdc);
//    imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
//    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
//
//    imageInfo->hTempDC = CreateCompatibleDC(hdc);
//    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
//    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);
//
//    imageInfo->width = width;
//    imageInfo->height = height;
//    imageInfo->loadType = IMAGE_LOAD_TYPE::Empty;
//
//    ReleaseDC(g_hWnd, hdc);
//
//    if (imageInfo->hBitmap == NULL)
//    {
//        Release();
//        return E_FAIL;
//    }
//
//    return S_OK;   // S_OK, E_FAIL
//}
//
//HRESULT Image::Init(const wchar_t* filePath, int width, int height, 
//    bool isTransparent, COLORREF transColor)
//{
//    HDC hdc = GetDC(g_hWnd);
//
//    imageInfo = new IMAGE_INFO();
//    imageInfo->resID = 0;
//    imageInfo->hMemDC = CreateCompatibleDC(hdc);
//    imageInfo->hBitmap = (HBITMAP)LoadImage(
//        g_hInstance, filePath, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
//    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
//
//    imageInfo->hTempDC = CreateCompatibleDC(hdc);
//    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
//    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);
//
//    imageInfo->width = width;
//    imageInfo->height = height;
//    imageInfo->loadType = IMAGE_LOAD_TYPE::File;
//
//    imageInfo->maxFrameX = 0;
//    imageInfo->maxFrameY = 0;
//    imageInfo->frameWidth = 0;
//    imageInfo->frameHeight = 0;
//    imageInfo->currFrameX = imageInfo->currFrameY = 0;
//
//    ReleaseDC(g_hWnd, hdc);
//
//    if (imageInfo->hBitmap == NULL)
//    {
//        Release();
//        return E_FAIL;
//    }
//
//    this->isTransparent = isTransparent;
//    this->transColor = transColor;
//
//    return S_OK;   // S_OK, E_FAIL
//}
//
//HRESULT Image::Init(const wchar_t* filePath, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor)
//{
//    HDC hdc = GetDC(g_hWnd);
//
//    imageInfo = new IMAGE_INFO();
//    imageInfo->resID = 0;
//    imageInfo->hMemDC = CreateCompatibleDC(hdc);
//    imageInfo->hBitmap = (HBITMAP)LoadImage(
//        g_hInstance, filePath, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
//    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
//
//    imageInfo->width = width;
//    imageInfo->height = height;
//    imageInfo->loadType = IMAGE_LOAD_TYPE::File;
//
//    imageInfo->maxFrameX = maxFrameX;
//    imageInfo->maxFrameY = maxFrameY;
//    imageInfo->frameWidth = width / maxFrameX;
//    imageInfo->frameHeight = height / maxFrameY;
//    imageInfo->currFrameX = imageInfo->currFrameY = 0;
//
//    imageInfo->hTempDC = CreateCompatibleDC(hdc);
//    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
//    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);
//
//    ReleaseDC(g_hWnd, hdc);
//
//    if (imageInfo->hBitmap == NULL)
//    {
//        Release();
//        return E_FAIL;
//    }
//
//    this->isTransparent = isTransparent;
//    this->transColor = transColor;
//
//    return S_OK;   // S_OK, E_FAIL
//}
//
//void Image::Render(HDC hdc, int destX, int destY)
//{
//    if (isTransparent)
//    {
//        GdiTransparentBlt(hdc,
//            destX, destY,
//            imageInfo->width, imageInfo->height,
//            imageInfo->hMemDC,
//            0, 0,
//            imageInfo->width, imageInfo->height,
//            transColor);
//    }
//    else
//    {
//        BitBlt(
//            hdc,                // 복사 목적지 DC
//            destX, destY,       // 복사 목적지 위치
//            imageInfo->width,   // 원본에서 복사될 가로크기
//            imageInfo->height,  // 원본에서 복사될 세로크기
//            imageInfo->hMemDC,  // 원본 DC
//            0, 0,               // 원본 복사 시작 위치
//            SRCCOPY             // 복사 옵션
//        );
//    }
//}
//
//void Image::Render(HDC hdc, int destX, int destY, int frameIndex, bool isFlip)
//{
//    imageInfo->currFrameX = frameIndex;
//
//    if (isFlip && isTransparent)
//    {
//        StretchBlt(imageInfo->hTempDC, 0, 0,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//            imageInfo->hMemDC,
//            (imageInfo->frameWidth * imageInfo->currFrameX) + (imageInfo->frameWidth - 1),
//            imageInfo->frameHeight * imageInfo->currFrameY,
//            -imageInfo->frameWidth, imageInfo->frameHeight,
//            SRCCOPY
//        );
//
//        GdiTransparentBlt(hdc,
//            destX, destY,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//
//            imageInfo->hTempDC,
//            0, 0,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//            transColor);
//    }
//    else if (isTransparent)
//    {
//        GdiTransparentBlt(hdc,
//            destX, destY,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//
//            imageInfo->hMemDC,
//            imageInfo->frameWidth * imageInfo->currFrameX,
//            imageInfo->frameHeight * imageInfo->currFrameY,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//            transColor);
//    }
//    else
//    {
//        BitBlt(
//            hdc,
//            destX, destY,
//            imageInfo->width / 9,
//            imageInfo->height,
//            imageInfo->hMemDC,
//            imageInfo->width / 9 * frameIndex, 0,
//            SRCCOPY
//        );
//    }
//}
//
//void Image::FrameRender(HDC hdc, int destX, int destY, 
//    int frameX, int frameY, bool isFlip, bool isCenter)
//{
//    int x = destX;
//	int y = destY;
//	if (isCenter)
//	{
//        x = destX - (imageInfo->frameWidth / 2);
//        y = destY - (imageInfo->frameHeight / 2);
//	}
//
//    imageInfo->currFrameX = frameX;
//    imageInfo->currFrameY = frameY;
//
//    if (isFlip && isTransparent)
//    {
//        StretchBlt(imageInfo->hTempDC, 0, 0,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//            imageInfo->hMemDC,
//            (imageInfo->frameWidth * imageInfo->currFrameX) + (imageInfo->frameWidth - 1),
//            imageInfo->frameHeight * imageInfo->currFrameY,
//            -imageInfo->frameWidth, imageInfo->frameHeight,
//            SRCCOPY
//        );
//
//        GdiTransparentBlt(hdc,
//            x, y,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//
//            imageInfo->hTempDC,
//            0, 0,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//            transColor);
//    }
//    else if (isTransparent)
//    {
//        GdiTransparentBlt(hdc,
//            x, y,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//
//            imageInfo->hMemDC,
//            imageInfo->frameWidth * imageInfo->currFrameX,
//            imageInfo->frameHeight * imageInfo->currFrameY,
//            imageInfo->frameWidth, imageInfo->frameHeight,
//            transColor);
//    }
//    else
//    {
//        BitBlt(
//            hdc,
//            x, y,
//            imageInfo->frameWidth,
//            imageInfo->frameHeight,
//            imageInfo->hMemDC,
//            imageInfo->frameWidth * imageInfo->currFrameX, 
//            imageInfo->frameHeight * imageInfo->currFrameY,
//            SRCCOPY
//        );
//    }
//}
//
//void Image::Release()
//{
//    if (imageInfo)
//    {
//        SelectObject(imageInfo->hTempDC, imageInfo->hOldTemp);
//        DeleteObject(imageInfo->hTempBit);
//        DeleteDC(imageInfo->hTempDC);
//
//        SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);
//        DeleteObject(imageInfo->hBitmap);
//        DeleteDC(imageInfo->hMemDC);
//
//        delete imageInfo;
//        imageInfo = nullptr;
//    }
//}

HRESULT Image::Init(ID2D1RenderTarget* renderTarget, const wchar_t* filePath)
{
	IWICImagingFactory* wicFactory = ImageManager::GetInstance()->GetWICFactory();

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory, nullptr,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
	if (FAILED(hr)) return hr;

	ComPtr<IWICBitmapDecoder> decoder;
	hr = wicFactory->CreateDecoderFromFilename(
		filePath, nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	ComPtr<IWICFormatConverter> converter;
	hr = wicFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	hr = converter->Initialize(
		frame.Get(), GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, nullptr, 0.0f,
		WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr)) return hr;

	ComPtr<ID2D1Bitmap> bitmap;
	hr = renderTarget->CreateBitmapFromWicBitmap(converter.Get(), &bitmap);
	if (FAILED(hr)) return hr;

	imageInfo = new IMAGE_INFO;
	imageInfo->bitmap = bitmap;
	imageInfo->width = bitmap->GetSize().width;
	imageInfo->height = bitmap->GetSize().height;
	return S_OK;
}

HRESULT Image::Init(ID2D1RenderTarget* renderTarget, const wchar_t* filePath, int maxFrameX, int maxFrameY)
{
	IWICImagingFactory* wicFactory = ImageManager::GetInstance()->GetWICFactory();

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory, nullptr,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
	if (FAILED(hr)) return hr;

	ComPtr<IWICBitmapDecoder> decoder;
	hr = wicFactory->CreateDecoderFromFilename(
		filePath, nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	ComPtr<IWICFormatConverter> converter;
	hr = wicFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	hr = converter->Initialize(
		frame.Get(), GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, nullptr, 0.0f,
		WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr)) return hr;

	ComPtr<ID2D1Bitmap> bitmap;
	hr = renderTarget->CreateBitmapFromWicBitmap(converter.Get(), &bitmap);
	if (FAILED(hr)) return hr;

	imageInfo = new IMAGE_INFO;
	imageInfo->bitmap = bitmap;
	imageInfo->width = bitmap->GetSize().width;
	imageInfo->height = bitmap->GetSize().height;
	
	imageInfo->maxFrameX = maxFrameX;
    imageInfo->maxFrameY = maxFrameY;
    imageInfo->frameWidth = imageInfo->width / maxFrameX;
    imageInfo->frameHeight = imageInfo->height / maxFrameY;
    imageInfo->currFrameX = imageInfo->currFrameY = 0;
	return S_OK;
}

void Image::Render(ID2D1RenderTarget* renderTarget, float x, float y, float scaleX, float scaleY)
{
	if (!imageInfo || !imageInfo->bitmap)
		return;

	D2D1_SIZE_F size = imageInfo->bitmap->GetSize();

	float anchorX = 0.5f;
	float anchorY = 0.5f;

	float drawWidth = size.width * scaleX;
	float drawHeight = size.height * scaleY;

	float left = x - drawWidth * anchorX;
	float top = y - drawHeight * anchorY;
	float right = left + drawWidth;
	float bottom = top + drawHeight;

	D2D1_RECT_F destRect = D2D1::RectF(left, top, right, bottom);

	renderTarget->DrawBitmap(
		imageInfo->bitmap.Get(),
		destRect,
		1.0f, // 불투명도
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);
}

void Image::Render(ID2D1RenderTarget* renderTarget, float x, float y, float scaleX, float scaleY, float atlasX, float atlasY, float srcW, float srcH)
{
	if (!imageInfo->bitmap) return;

	D2D1_SIZE_F bmpSize = imageInfo->bitmap->GetSize();
	
	float anchorX = 0.5f;
	float anchorY = 0.5f;
	// 전체 이미지 크기 사용 시 기본 설정
	if (srcW <= 0.f) srcW = bmpSize.width;
	if (srcH <= 0.f) srcH = bmpSize.height;
	
	bool flipX = scaleX < 0.f;

	float srcLeft = atlasX * srcW;
	float srcTop = atlasY * srcH;
	float srcRight = srcLeft + srcW;
	float srcBottom = srcTop + srcH;

	D2D1_RECT_F srcRect = D2D1::RectF(
		srcLeft,
		srcTop,
		srcRight,
		srcBottom
	);

	float destW = srcW * scaleX;
	float destH = srcH * scaleY;

	D2D1_RECT_F destRect = D2D1::RectF(
		x - destW * anchorX,
		y - destH * anchorY,
		x - destW * anchorX + destW,
		y - destH * anchorY + destH
	);

	// Transform 적용
	D2D1::Matrix3x2F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	if (flipX)
	{
		// 중심 기준 좌우반전 행렬
		D2D1::Matrix3x2F flipMat =
			D2D1::Matrix3x2F::Translation(-x, -y) *
			D2D1::Matrix3x2F::Scale(-1.0f, 1.0f) *
			D2D1::Matrix3x2F::Translation(x, y);

		renderTarget->SetTransform(flipMat * originalTransform);
	}

	renderTarget->DrawBitmap(imageInfo->bitmap.Get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);

	// Transform 복원
	if (flipX)
	{
		renderTarget->SetTransform(originalTransform);
	}
}

void Image::FrameRender(ID2D1RenderTarget* renderTarget, float x, float y, int frameX, int frameY, bool isFlip)
{
	if (!imageInfo->bitmap) return;

	float fw = static_cast<float>(imageInfo->frameWidth);
	float fh = static_cast<float>(imageInfo->frameHeight);

	D2D1_RECT_F destRect = D2D1::RectF(
		x - fw / 2.0f,
		y - fh / 2.0f,
		x + fw / 2.0f,
		y + fh / 2.0f
	);

	D2D1_RECT_F srcRect = D2D1::RectF(
		frameX * fw,
		frameY * fh,
		(frameX + 1) * fw,
		(frameY + 1) * fh
	);

	// Transform 저장
	D2D1::Matrix3x2F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	if (isFlip)
	{
		// 중심 기준 좌우반전 행렬
		D2D1::Matrix3x2F flip =
			D2D1::Matrix3x2F::Translation(-x, -y) *
			D2D1::Matrix3x2F::Scale(-1.0f, 1.0f) *
			D2D1::Matrix3x2F::Translation(x, y);

		renderTarget->SetTransform(flip * originalTransform);
	}

	// 실제 그리기
	renderTarget->DrawBitmap(
		imageInfo->bitmap.Get(),
		&destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		&srcRect
	);

	// Transform 복원
	if (isFlip)
	{
		renderTarget->SetTransform(originalTransform);
	}
}

void Image::Release()
{
	if (imageInfo)
	{
		imageInfo->bitmap.Reset();
		delete imageInfo;
		imageInfo = nullptr;
	}
}
