#include "pch.h"
#include "UI.h"
#include "Image.h"

HRESULT UI::Init()
{
	//imageRatio = ResolutionRatio();
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

FPOINT UI::ResolutionRatio(Image* image)
{
	FPOINT ratio = { 0.0f, 0.0f };
	ratio.x = WINSIZE_X / static_cast<float>(image->GetWidth());
	ratio.y = WINSIZE_Y / static_cast<float>(image->GetHeight());

	return ratio;
}
