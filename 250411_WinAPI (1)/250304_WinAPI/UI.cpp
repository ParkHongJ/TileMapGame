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
	if (requestOpaqueChange)
	{
		SetOpaque();
		requestOpaqueChange = false;
	}
	if (isOpaque)
	{
		opacityTimer += TimeDelta;
		if (opacityTimer >= opacityDuration)
		{
			DisableOpaque();
		}
	}
}

void UI::Render(ID2D1RenderTarget* renderTarget)
{
}

void UI::SetOpaque()
{
	isOpaque = true;
	defaultOpacity = 1.0f;
	opacityTimer = 0.0f;
}

void UI::DisableOpaque()
{
	isOpaque = false;
	defaultOpacity = 0.5f;
	opacityTimer = 0.0f;
}

FPOINT UI::ResolutionRatio()
{
	FPOINT ratio = { 0.0f, 0.0f };
	Image* standardImage = ImageManager::GetInstance()->FindImage("GameOverPage");
	ratio.x = WINSIZE_X / static_cast<float>(standardImage->GetWidth());
	ratio.y = WINSIZE_Y / static_cast<float>(standardImage->GetHeight());

	return ratio;
}

UI::UI()
{
	x_pos_divide_factor = 25.0f;

	defaultOpacity = 0.5f;		//기본 반투명
	opacityDuration = 3.0f;		//3초간 또렷해짐
	opacityTimer = 0.0f;		//3초 카운트용 변수
	isOpaque = false;			//기본적으로 반투명
	requestOpaqueChange = false;
}
