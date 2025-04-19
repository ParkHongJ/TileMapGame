#pragma once
#include "config.h"

class Image
{
public:
	enum IMAGE_LOAD_TYPE
	{
		Resource,	// ������Ʈ ��ü�� ���� ��ų �̹���
		File,		// �ܺο��� �ε��� �̹���
		Empty,		// ��ü ������ �̹���
		End
	};

	// Lagacy
	//typedef struct tagImageInfo
	//{
	//	DWORD resID;		// ���ҽ��� ���� ID
	//	HDC hMemDC;			// �׸��⸦ �ְ��ϴ� ��ü �ڵ�
	//	HBITMAP hBitmap;	// �̹��� ����
	//	HBITMAP hOldBit;	// ���� �̹��� ����
	//	HDC hTempDC;
	//	HBITMAP hTempBit;
	//	HBITMAP hOldTemp;
	//	int width;			// �̹��� ���� ũ��
	//	int height;			// �̹��� ���� ũ��
	//	BYTE loadType;		// �ε� Ÿ��

	//	// �ִϸ��̼� ����
	//	int maxFrameX;
	//	int maxFrameY;
	//	int frameWidth;
	//	int frameHeight;
	//	int currFrameX;
	//	int currFrameY;

	//	tagImageInfo()
	//	{
	//		resID = 0;
	//		hMemDC = NULL;
	//		hBitmap = NULL;
	//		hOldBit = NULL;
	//		width = 0;
	//		height = 0;
	//		loadType = IMAGE_LOAD_TYPE::Empty;

	//		maxFrameX = 0;
	//		maxFrameY = 0;
	//		frameWidth = 0;
	//		frameHeight = 0;
	//		currFrameX = 0;
	//		currFrameY = 0;
	//	}
	//} IMAGE_INFO, * LPIMAGE_INFO;

	struct IMAGE_INFO
	{
		ComPtr<ID2D1Bitmap> bitmap;
		int width = 0;
		int height = 0;

		// �ִϸ��̼� ������
		int maxFrameX = 0;
		int maxFrameY = 0;
		int frameWidth = 0;
		int frameHeight = 0;
		int currFrameX = 0;
		int currFrameY = 0;
		IMAGE_LOAD_TYPE loadType = Empty;
	};
private:
	IMAGE_INFO* imageInfo;

	//Lagacy
	//bool isTransparent;
	//COLORREF transColor;

	D2D1_COLOR_F transColor = D2D1::ColorF(0, 0); // ����Ʈ �����
public:
	HRESULT Init(ID2D1RenderTarget* renderTarget, const wchar_t* filePath);
	HRESULT Init(ID2D1RenderTarget* renderTarget, const wchar_t* filePath, int maxFrameX, int maxFrameY);
	void Render(ID2D1RenderTarget* renderTarget, 
		float x = 0.f, float y = 0.f, 
		float scaleX = 1.0f, float scaleY = 1.0f);

	void Render(ID2D1RenderTarget* renderTarget,
		float x, float y,
		float scaleX, float scaleY,
		float atalasX, float atalasY,
		float srcW, float srcH, float alpha = 1.f, float angle = 0.f);

	void FrameRender(ID2D1RenderTarget* renderTarget, float x, float y, int frameX, int frameY, bool isFlip = false);
	void FrameRender(ID2D1RenderTarget* renderTarget, float x, float y, int frameX, int frameY, float scaleX, float scaleY, bool isFlip = false);
	void Release();

	inline ID2D1Bitmap* GetBitmap() const { return imageInfo->bitmap.Get(); }
	inline int GetWidth() { return imageInfo->width; }
	inline int GetHeight() { return imageInfo->height; }

	// Lagacy
	//// �� ��Ʈ�� �̹����� ����� �Լ�
	//HRESULT Init(int width, int height);

	//// ���Ϸκ��� �̹����� �ε��ϴ� �Լ�
	//HRESULT Init(const wchar_t* filePath, int width, int height, 
	//	bool isTransparent = FALSE, COLORREF transColor = FALSE);

	//// ���Ϸκ��� �̹����� �ε��ϴ� �Լ�
	//HRESULT Init(const wchar_t* filePath, int width, int height,
	//	int maxFrameX, int maxFrameY,
	//	bool isTransparent = FALSE, COLORREF transColor = FALSE);

	//// ȭ�鿡 ���
	//void Render(HDC hdc, int destX = 0, int destY = 0);
	//void Render(HDC hdc, int destX, int destY, int frameIndex, bool isFlip = false);
	//void FrameRender(HDC hdc, int destX, int destY,
	//	int frameX, int frameY, bool isFlip = false, bool isCenter = true);

	//// �޸� ����
	//void Release();

	//inline HDC GetMemDC() {
	//	if (imageInfo)
	//	{
	//		return imageInfo->hMemDC;
	//	}
	//	return NULL;
	//}

	//inline int GetMaxFrameX() { return imageInfo->maxFrameX; }
	//inline int GetMaxFrameY() { return imageInfo->maxFrameY; }
	//inline int GetWidth() { return imageInfo->width; }
	//inline int GetHeight() { return imageInfo->height; }
	//inline int GetFrameWidth() { return imageInfo->frameWidth; }
	//inline int GetFrameHeight() { return imageInfo->frameHeight; }
};

