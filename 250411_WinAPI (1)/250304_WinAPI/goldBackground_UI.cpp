#include "pch.h"
#include "goldBackground_UI.h"
#include "Image.h"

HRESULT goldBackground_UI::Init()
{
	image = ImageManager::GetInstance()->FindImage("goldBackgroundImage");
	Pos = { WINSIZE_X * (20.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	return S_OK;
}

void goldBackground_UI::Release()
{
}

void goldBackground_UI::Update(float TimeDelta)
{
}

void goldBackground_UI::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (image)
		image->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
}
