#include "pch.h"
#include "sandGlass_UI.h"
#include "Image.h"

HRESULT sandGlass_UI::Init()
{
	image = ImageManager::GetInstance()->FindImage("sandGlassImage");
	Pos = { WINSIZE_X * (22.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	return S_OK;
}

void sandGlass_UI::Release()
{
}

void sandGlass_UI::Update(float TimeDelta)
{
}

void sandGlass_UI::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (image)
		image->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
}
