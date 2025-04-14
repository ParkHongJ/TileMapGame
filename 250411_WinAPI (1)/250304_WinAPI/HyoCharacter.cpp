#include "HyoCharacter.h"
#include "Image.h"
#include "ImageManager.h"
#include "CommonFunction.h"
#include "config.h"

HRESULT HyoCharacter::Init()
{
	ImageManager::GetInstance()->AddImage("Character_Iris", TEXT("Image/char_iris.bmp"), 2048, 2048, 16, 15, true, RGB(255, 0, 255));
	playerIris = new Image();

	if (FAILED(playerIris->Init(TEXT("Image/char_iris.bmp"), WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/char_iris.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	return S_OK;

	pos = { (WINSIZE_X / 2), (WINSIZE_Y / 2) };
	dir = -1;
	state = PlayerState::IDLE;
}

void HyoCharacter::Release()
{
	
}

void HyoCharacter::Update(float TimeDelta)
{
	
}

void HyoCharacter::Render(HDC hdc)
{
	switch (state)
	{
	case PlayerState::IDLE:
		playerIris->FrameRender(hdc, pos.x, pos.y, 0, 0);
		break;
	case PlayerState::WALK:
		break;
	case PlayerState::JUMP:
		break;
	}
	//playerIris->Render(hdc);
}
