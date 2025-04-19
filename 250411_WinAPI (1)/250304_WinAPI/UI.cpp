#include "pch.h"
#include "UI.h"
#include "Image.h"

HRESULT UI::Init()
{

	return S_OK;
}

void UI::Release()
{
}

void UI::Update(float TimeDelta)
{
}

void UI::Render(ID2D1HwndRenderTarget* renderTarget)
{
}

FPOINT UI::ResolutionRatio()
{
	FPOINT ratio = { 0.0f, 0.0f };
	Image* standardImage = ImageManager::GetInstance()->FindImage("GameOverPage");
	ratio.x = WINSIZE_X / static_cast<float>(standardImage->GetWidth());
	ratio.y = WINSIZE_Y / static_cast<float>(standardImage->GetHeight());

	return ratio;
}
