#include "pch.h"
#include "playerRope_UI.h"
#include "Image.h"
#include "PlayerStatus.h"

HRESULT playerRope_UI::Init()
{
	playerRopeImage = ImageManager::GetInstance()->FindImage("playerRopeImage");
	
	Pos = { WINSIZE_X * (5.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	playerStat = new PlayerStatus();
	//playerRope_value = playerStat->GetRopeCount();

	return S_OK;
}

void playerRope_UI::Release()
{
}

void playerRope_UI::Update(float TimeDelta)
{
	//playerRope_value = playerStat->GetRopeCount();
}

void playerRope_UI::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (playerRopeImage)
		playerRopeImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
}
