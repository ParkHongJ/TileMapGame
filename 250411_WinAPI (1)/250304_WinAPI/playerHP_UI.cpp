#include "pch.h"
#include "playerHP_UI.h"
#include "Image.h"
#include "PlayerStatus.h"

HRESULT playerHP_UI::Init()
{
	playerLifeImage = ImageManager::GetInstance()->FindImage("playerLife");
	playerLifeRunsOutImage = ImageManager::GetInstance()->FindImage("playerLifeRunsOut");

	playerStat = new PlayerStatus();
	playerHP_value = playerStat->GetPlayerMaxHP();
	//float x_pos_divide_factor = 25.0f;
	Pos = { WINSIZE_X * (1.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	isAlive = true;

	return S_OK;
}

void playerHP_UI::Update(float TimeDelta)
{
	playerHP_value = playerStat->GetPlayerHP();
	if (playerHP_value == 0)
		isAlive = false;
}

void playerHP_UI::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (playerLifeImage)
		playerLifeImage->Render(renderTarget, Pos.x, Pos.y);
	if (playerLifeRunsOutImage && !isAlive)
		playerLifeRunsOutImage->Render(renderTarget, Pos.x, Pos.y);
}

void playerHP_UI::Release()
{
}
