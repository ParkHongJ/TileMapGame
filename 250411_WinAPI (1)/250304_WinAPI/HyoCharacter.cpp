#include "HyoCharacter.h"
#include "Image.h"
#include "ImageManager.h"
#include "CommonFunction.h"

HRESULT HyoCharacter::Init()
{
	playerIris = ImageManager::GetInstance()->FindImage("Character_Iris");
	/*playerIris = new Image();

	if (FAILED(playerIris->Init(TEXT("Image/char_iris.bmp"), WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/char_iris.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}*/

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

void HyoCharacter::Render(ID2D1HwndRenderTarget* renderTarget)
{
	switch (state)
	{
	case PlayerState::IDLE:
		playerIris->Render(renderTarget, pos.x, pos.y);
		//playerIris->FrameRender(hdc, pos.x, pos.y, 0, 0);
		break;
	case PlayerState::MOVE:
		break;
	//case PlayerState::JUMP:
		break;
	}
	//playerIris->Render(hdc);
}
