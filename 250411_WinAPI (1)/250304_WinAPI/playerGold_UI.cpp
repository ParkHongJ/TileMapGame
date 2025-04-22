#include "pch.h"
#include "playerGold_UI.h"
#include "Image.h"
#include "PlayerStatus.h"

HRESULT playerGold_UI::Init()
{
	playerGoldImage = ImageManager::GetInstance()->FindImage("playerGoldImage");

	Pos = { WINSIZE_X * (18.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	playerStat = new PlayerStatus();
	playerGold_value = playerStat->GetGold();

	return S_OK;
}

void playerGold_UI::Release()
{
}

void playerGold_UI::Update(float TimeDelta)
{
	UI::Update(TimeDelta);
	playerGold_value = playerStat->GetGold();
	if (KeyManager::GetInstance()->IsOnceKeyDown('G'))
		RequestOpaqueChange();
}

void playerGold_UI::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (playerGoldImage)
		playerGoldImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
}
