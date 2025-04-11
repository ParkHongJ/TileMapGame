#include "Button.h"
#include "Image.h"

HRESULT Button::Init(int posX, int posY)
{
	state = ButtonState::None;
	image = ImageManager::GetInstance()->
		AddImage("버튼", L"Image/button.bmp", 122, 62, 1, 2);
	if (image == nullptr)
	{
		return E_FAIL;
	}

	pos.x = posX;
	pos.y = posY;

	rc.left = pos.x - image->GetFrameWidth() / 2;
	rc.top = pos.y - image->GetFrameHeight() / 2;
	rc.right = rc.left + image->GetFrameWidth();
	rc.bottom = rc.top + image->GetFrameHeight();

	buttonFunc = nullptr;

    return S_OK;
}

void Button::Release()
{
}

void Button::Update()
{
	if (PtInRect(&rc, g_ptMouse))
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
		{
			state = ButtonState::Down;
		}
		else if (state == ButtonState::Down &&
			KeyManager::GetInstance()->IsOnceKeyUp(VK_LBUTTON))
		{
			state = ButtonState::Up;

			// 기능 수행
			// TilemapTool::Save()
			//if (buttonFunc && obj)	obj->buttonFunc();
			if (buttonFunc) buttonFunc();
		}
	}
	else
	{
		state = ButtonState::None;
	}
}

void Button::Render(HDC hdc)
{
	switch (state)
	{
	case ButtonState::None:
	case ButtonState::Up:
		image->FrameRender(hdc, pos.x, pos.y, 0, 0);
		break;
	case ButtonState::Down:
		image->FrameRender(hdc, pos.x, pos.y, 0, 1);
		break;
	}
}
