#include "KOF_Iori.h"
#include "Image.h"

HRESULT KOF_Iori::Init()
{
	pos = { 0.0f, 0.0f };
	moveSpeed = 5.0f;
	image = new Image();
	if (FAILED(image->Init(TEXT("Image/iori_walk.bmp"), 612, 104, 9, 1, 
		true, RGB(255,0,255))))
	{
		MessageBox(g_hWnd, TEXT("Image/iori_walk.bmp 파일 로드에 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	elapsedFrame = 0;
	currAnimaionFrame = 0;

	return S_OK;
}

void KOF_Iori::Release()
{
	if (image)
	{
		image->Release();
		delete image;
		image = nullptr;
	}
}

void KOF_Iori::Update()
{
	elapsedFrame++;
	//if (elapsedFrame >= 5)
	if (KeyManager::GetInstance()->IsOnceKeyUp(VK_SPACE))
	{
		currAnimaionFrame++;
		Move();

		if (currAnimaionFrame > 8)
		{
			currAnimaionFrame = 0;
		}
		elapsedFrame = 0;
	}
	//currAnimaionFrame = elapsedFrame / 5;
	//if (currAnimaionFrame > 8)
	//{
	//	currAnimaionFrame = 0;
	//	elapsedFrame = 0;
	//}
}

void KOF_Iori::Render(HDC hdc)
{
	if (image)
		image->Render(hdc, pos.x, pos.y, currAnimaionFrame, true);
}

void KOF_Iori::Move()
{
	pos.x += moveSpeed;
}

KOF_Iori::KOF_Iori()
{
}

KOF_Iori::~KOF_Iori()
{
}
